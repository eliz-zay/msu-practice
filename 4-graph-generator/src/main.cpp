#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <sstream>

#include "config.hpp"

#include "graph_generation_controller.hpp"
#include "graph_generator.hpp"
#include "graph_json_printing.hpp"
#include "graph_printing.hpp"
#include "interfaces/i_graph.hpp"
#include "logger.hpp"

using uni_course_cpp::GraphGenerationController;
using uni_course_cpp::GraphGenerator;
using uni_course_cpp::IGraph;
using uni_course_cpp::Logger;
namespace config = uni_course_cpp::config;
namespace printing = uni_course_cpp::printing;

static constexpr int kMinDepth = 0;
static constexpr int kMinVertexCount = 0;
static constexpr int kMinGraphsCount = 0;
static constexpr int kMinThreadsCount = 1;

int handle_depth_input() {
  int depth;
  while (true) {
    std::cout << "Enter depth: ";
    std::cin >> depth;
    if (std::cin.fail() || depth < kMinDepth) {
      std::cout << "Depth must be a nonnegative integer" << std::endl;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else {
      return depth;
    }
  }
}

int handle_new_vertices_count_input() {
  int new_vertices_count;
  while (true) {
    std::cout << "Enter new vertices number: ";
    std::cin >> new_vertices_count;
    if (std::cin.fail() || new_vertices_count < kMinVertexCount) {
      std::cout << "Number must be a nonnegative integer" << std::endl;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else {
      return new_vertices_count;
    }
  }
}

int handle_graphs_count_input() {
  int graphs_count;
  while (true) {
    std::cout << "Enter number of graphs: ";
    std::cin >> graphs_count;
    if (std::cin.fail() || graphs_count < kMinGraphsCount) {
      std::cout << "The number must be a nonnegative integer" << std::endl;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else {
      return graphs_count;
    }
  }
}

int handle_threads_count_input() {
  int threads_count;
  while (true) {
    std::cout << "Enter number of threads: ";
    std::cin >> threads_count;
    if (std::cin.fail() || threads_count < kMinThreadsCount) {
      std::cout << "The number must be a positive integer" << std::endl;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else {
      return threads_count;
    }
  }
}

void init_temp_directory() {
  std::filesystem::remove_all(config::kTempDirectoryPath);
  std::filesystem::create_directory(config::kTempDirectoryPath);
}

std::string get_start_processing_string(int graph_count) {
  std::stringstream result_string;
  result_string << "Graph " << graph_count << ", Generation started";
  return result_string.str();
}

std::string get_finish_processing_string(int graph_count,
                                         const std::string& graph_info) {
  std::stringstream result_string;
  result_string << "Graph " << graph_count << ", Generation finished "
                << graph_info << "\n";
  return result_string.str();
}

void write_to_file(const std::string& graph_json,
                   const std::string& file_name) {
  std::ofstream file(file_name);
  file << graph_json << std::endl;
  file.close();
}

std::vector<std::unique_ptr<IGraph>> generate_graphs(
    GraphGenerator::Params&& params,
    int graphs_count,
    int threads_count) {
  auto generation_controller =
      GraphGenerationController(threads_count, graphs_count, std::move(params));

  auto& logger = Logger::get_logger();

  auto graphs = std::vector<std::unique_ptr<IGraph>>();
  graphs.reserve(graphs_count);

  generation_controller.generate(
      [&logger](int index) { logger.log(get_start_processing_string(index)); },
      [&logger, &graphs](int index, std::unique_ptr<IGraph> graph) {
        const auto graph_info = printing::graph_info_to_string(*graph);
        logger.log(get_finish_processing_string(index, graph_info));

        const auto graph_json = printing::json::graph_to_json(*graph);
        const auto file_name = config::kTempDirectoryPath + "graph_" +
                               std::to_string(index) + ".json";
        write_to_file(graph_json, file_name);

        graphs.push_back(std::move(graph));
      });

  return graphs;
}

int main() {
  const int graphs_count = handle_graphs_count_input();
  const int threads_count = handle_threads_count_input();
  const int depth = handle_depth_input();
  const int new_vertices_count = handle_new_vertices_count_input();

  init_temp_directory();

  auto params = GraphGenerator::Params(depth, new_vertices_count);
  const auto graphs =
      generate_graphs(std::move(params), graphs_count, threads_count);

  return 0;
}
