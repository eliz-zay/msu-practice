#include <algorithm>
#include <cassert>
#include <stdexcept>

#include "graph.hpp"

namespace uni_course_cpp {
VertexId Graph::add_vertex() {
  const VertexId vertex_id = get_new_vertex_id();
  vertices_.emplace(vertex_id, vertex_id);
  adjacency_list_.emplace(vertex_id, std::vector<EdgeId>());

  init_vertex_depth(vertex_id);

  return vertex_id;
}

EdgeId Graph::add_edge(VertexId from_vertex_id, VertexId to_vertex_id) {
  assert(has_vertex(from_vertex_id) && "Vertex from_vertex is not found");
  assert(has_vertex(to_vertex_id) && "Vertex to_vertex is not found");

  const EdgeId edge_id = get_new_edge_id();
  const auto color = calculate_edge_color(from_vertex_id, to_vertex_id);
  edges_.emplace(edge_id, Edge(edge_id, from_vertex_id, to_vertex_id, color));

  colored_edges_[color].emplace(edge_id);

  adjacency_list_[from_vertex_id].emplace_back(edge_id);
  if (from_vertex_id != to_vertex_id) {
    adjacency_list_[to_vertex_id].emplace_back(edge_id);
  }

  if (color == EdgeColor::Grey) {
    update_vertex_depth(from_vertex_id, to_vertex_id);
  }

  return edge_id;
}

int Graph::get_vertices_count() const {
  return vertices_.size();
}

int Graph::get_edges_count() const {
  return edges_.size();
}

void Graph::for_each_vertex(const ForEachVertexCallback& callback) const {
  int index = 0;
  for (const auto& [vertex_id, vertex] : vertices_) {
    callback(vertex, index++);
  }
}

void Graph::for_each_edge(const ForEachEdgeCallback& callback) const {
  int index = 0;
  for (const auto& [edge_id, edge] : edges_) {
    callback(edge, index++);
  }
}

void Graph::for_each_vertex_par(
    const ForEachVertexCallbackPar& callback) const {
  std::for_each(vertices_.cbegin(), vertices_.cend(),
                [&callback](const auto& vertex_it) {
                  callback(vertex_it.second);
                });
}

const std::vector<EdgeId>& Graph::get_vertex_edge_ids(
    VertexId vertex_id) const {
  return adjacency_list_.at(vertex_id);
}

const std::set<VertexId>& Graph::get_vertex_ids_at_depth(
    GraphDepth depth) const {
  if (depth_vertices_.find(depth) == depth_vertices_.end()) {
    static const std::set<VertexId> empty_set;
    return empty_set;
  }
  return depth_vertices_.at(depth);
}

const std::set<EdgeId>& Graph::get_edge_ids_by_color(EdgeColor color) const {
  if (colored_edges_.find(color) == colored_edges_.end()) {
    static const std::set<VertexId> empty_set;
    return empty_set;
  }
  return colored_edges_.at(color);
}

GraphDepth Graph::get_vertex_depth(VertexId vertex_id) const {
  return vertex_depths_.at(vertex_id);
}

GraphDepth Graph::get_depth() const {
  return depth_vertices_.size();
}

VertexId Graph::get_new_vertex_id() {
  return vertex_id_counter_++;
}

EdgeId Graph::get_new_edge_id() {
  return edge_id_counter_++;
}

void Graph::init_vertex_depth(VertexId vertex_id) {
  depth_vertices_[kInitialDepth].emplace(vertex_id);
  vertex_depths_[vertex_id] = kInitialDepth;
}

void Graph::update_vertex_depth(VertexId previous_vertex_id,
                                VertexId vertex_id) {
  const auto vertex_depth = vertex_depths_.at(previous_vertex_id) + 1;

  depth_vertices_[kInitialDepth].erase(vertex_id);
  depth_vertices_[vertex_depth].emplace(vertex_id);
  vertex_depths_[vertex_id] = vertex_depth;
}

EdgeColor Graph::calculate_edge_color(VertexId from_vertex_id,
                                      VertexId to_vertex_id) const {
  const auto from_vertex_depth = vertex_depths_.at(from_vertex_id);
  const auto to_vertex_depth = vertex_depths_.at(to_vertex_id);

  if (from_vertex_id == to_vertex_id) {
    return EdgeColor::Green;
  }
  if (adjacency_list_.at(to_vertex_id).size() == 0) {
    return EdgeColor::Grey;
  }
  if (to_vertex_depth - from_vertex_depth == 1 &&
      !has_edge(from_vertex_id, to_vertex_id)) {
    return EdgeColor::Yellow;
  }
  if (to_vertex_depth - from_vertex_depth == 2) {
    return EdgeColor::Red;
  }
  throw std::runtime_error("Failed to determine color");
}

bool Graph::has_vertex(VertexId vertex_id) const {
  return vertices_.find(vertex_id) != vertices_.end();
}

bool Graph::has_edge(VertexId from_vertex_id, VertexId to_vertex_id) const {
  if (from_vertex_id != to_vertex_id) {
    for (const auto from_edge_id : adjacency_list_.at(from_vertex_id)) {
      for (const auto to_edge_id : adjacency_list_.at(to_vertex_id)) {
        if (from_edge_id == to_edge_id) {
          return true;
        }
      }
    }
  } else {
    for (const auto edge_id : adjacency_list_.at(from_vertex_id)) {
      const auto& edge = edges_.at(edge_id);
      if (edge.from_vertex_id() == edge.to_vertex_id()) {
        return true;
      }
    }
  }

  return false;
}
};  // namespace uni_course_cpp
