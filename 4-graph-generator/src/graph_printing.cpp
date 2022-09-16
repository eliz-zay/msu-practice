#include <array>
#include <iostream>
#include <sstream>
#include <string>

#include "graph_printing.hpp"

namespace uni_course_cpp {
namespace printing {
std::string graph_info_to_string(const IGraph& graph) {
  std::stringstream result_string;
  result_string << "{\n";
  result_string << "\tdepth: " << graph.get_depth() << ",\n";
  result_string << "\tvertices: " << vertices_info_to_string(graph) << ",\n";
  result_string << "\tedges: " << edges_info_to_string(graph) << ",\n";
  result_string << "}\n";
  return result_string.str();
}

std::string vertices_info_to_string(const IGraph& graph) {
  std::stringstream result_string;

  result_string << "{ amount: " << graph.get_vertices_count()
                << ", distribution: [";

  const auto depth = graph.get_depth();
  for (GraphDepth current_depth = 1; current_depth <= depth; current_depth++) {
    if (current_depth != 1) {
      result_string << ", ";
    }
    result_string << graph.get_vertex_ids_at_depth(current_depth).size();
  }

  result_string << "] }";
  return result_string.str();
}

std::string edges_info_to_string(const IGraph& graph) {
  std::stringstream result_string;

  result_string << "{ amount: " << graph.get_edges_count()
                << ", distribution: [";

  const std::array<EdgeColor, 4> edge_colors = {
      EdgeColor::Grey, EdgeColor::Green, EdgeColor::Yellow, EdgeColor::Red};

  for (auto color = edge_colors.cbegin(); color != edge_colors.cend();
       color++) {
    if (color != edge_colors.cbegin()) {
      result_string << ", ";
    }
    result_string << edge_color_to_string(*color) << ": "
                  << graph.get_edge_ids_by_color(*color).size();
  }

  result_string << "] }";
  return result_string.str();
}

std::string edge_color_to_string(const EdgeColor& color) {
  switch (color) {
    case EdgeColor::Grey:
      return "grey";
    case EdgeColor::Green:
      return "green";
    case EdgeColor::Yellow:
      return "yellow";
    case EdgeColor::Red:
      return "red";
  }

  throw std::runtime_error("Failed to determine color to print");
}
};  // namespace printing
};  // namespace uni_course_cpp
