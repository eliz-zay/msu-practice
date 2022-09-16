#include <iostream>
#include <sstream>

#include "graph_json_printing.hpp"
#include "graph_printing.hpp"

namespace uni_course_cpp {
namespace printing {
namespace json {
std::string graph_to_json(const IGraph& graph) {
  std::stringstream result_string;

  result_string << "{\n\t\"depth\": " << graph.get_depth()
                << ",\n\t\"vertices\": [\n";

  graph.for_each_vertex(
      [&result_string, &graph](const IVertex& vertex, int index) {
        if (index) {
          result_string << ",\n";
        }

        result_string << "\t\t" << vertex_to_json(vertex, graph);
      });

  result_string << "\n\t],\n\t\"edges\": [\n";

  graph.for_each_edge([&result_string, &graph](const IEdge& edge, int index) {
    if (index) {
      result_string << ",\n";
    }

    result_string << "\t\t" << edge_to_json(edge);
  });

  result_string << "\n\t]\n}";
  return result_string.str();
}

std::string vertex_to_json(const IVertex& vertex, const IGraph& graph) {
  std::stringstream result_string;

  const std::vector<EdgeId>& edge_ids = graph.get_vertex_edge_ids(vertex.id());

  result_string << "{ \"id\": " << vertex.id() << ", \"edge_ids\": [";

  for (auto edge_id_it = edge_ids.cbegin(); edge_id_it != edge_ids.cend();
       edge_id_it++) {
    if (edge_id_it != edge_ids.cbegin()) {
      result_string << ", ";
    }

    result_string << *edge_id_it;
  }
  result_string << "]";

  result_string << ", \"depth\": " << graph.get_vertex_depth(vertex.id())
                << " }";

  return result_string.str();
}

std::string edge_to_json(const IEdge& edge) {
  std::stringstream result_string;
  result_string << "{ \"id\": " << edge.id() << ", \"vertex_ids\": ["
                << edge.from_vertex_id() << ", " << edge.to_vertex_id() << "]"
                << ", \"color\": \"" << edge_color_to_string(edge.color())
                << "\" }";
  return result_string.str();
}
};  // namespace json
};  // namespace printing
};  // namespace uni_course_cpp
