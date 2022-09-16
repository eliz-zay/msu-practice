#include <algorithm>
#include <atomic>
#include <iterator>
#include <list>
#include <optional>
#include <random>
#include <thread>
#include <vector>

#include "graph_generator.hpp"

namespace uni_course_cpp {
namespace random_util {
bool get_random_bool(double probability) {
  std::random_device rd{};
  std::mt19937 rng{rd()};
  std::bernoulli_distribution d(probability);
  return d(rng);
}

int get_random_int(int limit) {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<int> uni(0, limit);

  return uni(rng);
}
};  // namespace random_util

namespace {
static constexpr double kGreenEdgeProbability = 0.1;
static constexpr double kRedEdgeProbability = 0.33;
static constexpr GraphDepth kGreyDepthOffset = Graph::kInitialDepth;
static constexpr GraphDepth kYellowDepthOffset = Graph::kInitialDepth + 1;
static constexpr GraphDepth kYellowDepthDifference = 1;
static constexpr GraphDepth kRedDepthDifference = 2;

static const int kMaxThreadsCount = std::thread::hardware_concurrency();

VertexId get_random_vertex_id(const std::set<VertexId>& choose_from_set) {
  const auto random_int =
      random_util::get_random_int(choose_from_set.size() - 1);
  auto random_iterator = choose_from_set.begin();
  std::advance(random_iterator, random_int);
  return *random_iterator;
}

std::set<VertexId> get_unconnected_vertex_ids(const Graph& graph,
                                              std::mutex& graph_mutex,
                                              VertexId vertex_id) {
  const auto vertex_depth = graph.get_vertex_depth(vertex_id);
  const auto& next_level_vertex_ids =
      graph.get_vertex_ids_at_depth(vertex_depth + 1);

  std::set<VertexId> unconnected_vertex_ids;
  for (const auto next_level_vertex_id : next_level_vertex_ids) {
    const auto edgeExists = [&graph_mutex, &graph, vertex_id,
                             next_level_vertex_id]() {
      const std::lock_guard lock(graph_mutex);
      return graph.has_edge(vertex_id, next_level_vertex_id);
    }();

    if (!edgeExists) {
      unconnected_vertex_ids.insert(next_level_vertex_id);
    }
  }

  return unconnected_vertex_ids;
}
};  // namespace

GraphGenerator::GraphGenerator(Params&& params) : params_(std::move(params)) {}

std::unique_ptr<IGraph> GraphGenerator::generate() const {
  auto graph = Graph();
  if (params_.depth() == 0) {
    return std::make_unique<Graph>(std::move(graph));
  }

  const auto root_vertex_id = graph.add_vertex();
  if (params_.new_vertices_count() == 0) {
    return std::make_unique<Graph>(std::move(graph));
  }

  std::mutex graph_mutex;
  generate_grey_edges(graph, graph_mutex, root_vertex_id);

  std::thread green_thread([this, &graph, &graph_mutex]() {
    generate_green_edges(graph, graph_mutex);
  });
  std::thread yellow_thread([this, &graph, &graph_mutex]() {
    generate_yellow_edges(graph, graph_mutex);
  });
  std::thread red_thread([this, &graph, &graph_mutex]() {
    generate_red_edges(graph, graph_mutex);
  });

  green_thread.join();
  yellow_thread.join();
  red_thread.join();

  return std::make_unique<Graph>(std::move(graph));
}

void GraphGenerator::generate_grey_edges(Graph& graph,
                                         std::mutex& graph_mutex,
                                         VertexId root_vertex_id) const {
  using JobCallback = std::function<void()>;
  auto jobs = std::list<JobCallback>();
  std::mutex jobs_mutex;

  const auto new_vertices_count = params_.new_vertices_count();
  std::atomic<bool> should_terminate = false;

  const auto root_depth = graph.get_vertex_depth(root_vertex_id);
  for (int i = 0; i < new_vertices_count; i++) {
    jobs.push_back([this, &graph_mutex, &graph, root_vertex_id, root_depth]() {
      generate_grey_branch(graph_mutex, graph, root_vertex_id, root_depth);
    });
  }

  const auto worker = [&should_terminate, &jobs_mutex, &jobs]() {
    while (true) {
      if (should_terminate) {
        return;
      }

      const auto job_optional = [&jobs_mutex,
                                 &jobs]() -> std::optional<JobCallback> {
        const std::lock_guard lock(jobs_mutex);
        if (jobs.size()) {
          auto job = jobs.front();
          jobs.pop_front();
          return job;
        }
        return std::nullopt;
      }();

      if (job_optional.has_value()) {
        const auto& job = job_optional.value();
        job();
      }
    }
  };

  const auto threads_count = std::min(kMaxThreadsCount, new_vertices_count);
  auto threads = std::vector<std::thread>();
  threads.reserve(threads_count);

  for (int i = 0; i < threads_count; i++) {
    threads.emplace_back(std::thread(worker));
  }

  while (jobs.size()) {
  }

  should_terminate = true;
  for (auto& thread : threads) {
    thread.join();
  }
}

void GraphGenerator::generate_grey_branch(std::mutex& graph_mutex,
                                          Graph& graph,
                                          VertexId from_vertex_id,
                                          GraphDepth from_vertex_depth) const {
  const auto vertices_count = params_.new_vertices_count();
  const double step_probability = 1.0 / (params_.depth() - kGreyDepthOffset);

  const bool success = random_util::get_random_bool(
      1 - step_probability * (from_vertex_depth - Graph::kInitialDepth));
  if (!success) {
    return;
  }

  const auto new_vertex_id = [&graph, &graph_mutex, from_vertex_id]() {
    const std::lock_guard lock(graph_mutex);
    const auto new_vertex_id = graph.add_vertex();
    graph.add_edge(from_vertex_id, new_vertex_id);
    return new_vertex_id;
  }();

  for (int i = 0; i < vertices_count; i++) {
    generate_grey_branch(graph_mutex, graph, new_vertex_id,
                         from_vertex_depth + 1);
  }
}

void GraphGenerator::generate_green_edges(Graph& graph,
                                          std::mutex& graph_mutex) const {
  graph.for_each_vertex_par([&graph, &graph_mutex](const IVertex& vertex) {
    if (random_util::get_random_bool(kGreenEdgeProbability)) {
      const std::lock_guard lock(graph_mutex);
      graph.add_edge(vertex.id(), vertex.id());
    }
  });
}

void GraphGenerator::generate_yellow_edges(Graph& graph,
                                           std::mutex& graph_mutex) const {
  const auto depth = graph.get_depth();
  const auto vertices_count = params_.new_vertices_count();
  if (depth < 3) {
    return;
  }

  const double step_probability = 1.0 / (depth - kYellowDepthOffset);
  for (GraphDepth current_depth = Graph::kInitialDepth;
       current_depth <= depth - kYellowDepthDifference; current_depth++) {
    const auto& current_level_vertices =
        graph.get_vertex_ids_at_depth(current_depth);

    std::for_each(
        current_level_vertices.cbegin(), current_level_vertices.cend(),
        [this, &graph, &graph_mutex, step_probability,
         current_depth](const auto vertex_id) {
          const bool success = random_util::get_random_bool(
              step_probability * (current_depth - Graph::kInitialDepth));
          if (!success) {
            return;
          }

          const auto potential_vertex_ids =
              get_unconnected_vertex_ids(graph, graph_mutex, vertex_id);
          if (potential_vertex_ids.size()) {
            const std::lock_guard lock(graph_mutex);
            graph.add_edge(vertex_id,
                           get_random_vertex_id(potential_vertex_ids));
          }
        });
  }
}

void GraphGenerator::generate_red_edges(Graph& graph,
                                        std::mutex& graph_mutex) const {
  const auto depth = graph.get_depth();
  const auto vertices_count = params_.new_vertices_count();
  if (depth < 3) {
    return;
  }

  for (GraphDepth current_depth = Graph::kInitialDepth;
       current_depth <= depth - kRedDepthDifference; current_depth++) {
    const auto& current_level_vertices =
        graph.get_vertex_ids_at_depth(current_depth);

    std::for_each(
        current_level_vertices.cbegin(), current_level_vertices.cend(),
        [this, &graph, &graph_mutex, current_depth](const auto vertex_id) {
          const bool success =
              random_util::get_random_bool(kRedEdgeProbability);
          if (!success) {
            return;
          }

          const auto& potential_vertex_ids = graph.get_vertex_ids_at_depth(
              current_depth + kRedDepthDifference);
          if (potential_vertex_ids.size()) {
            const std::lock_guard lock(graph_mutex);
            graph.add_edge(vertex_id,
                           get_random_vertex_id(potential_vertex_ids));
          }
        });
  }
}
};  // namespace uni_course_cpp
