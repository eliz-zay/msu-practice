#pragma once

#include <string>

#include "interfaces/i_graph.hpp"

namespace uni_course_cpp {
namespace printing {
namespace json {
std::string graph_to_json(const IGraph& graph);
std::string vertex_to_json(const IVertex& vertex, const IGraph& graph);
std::string edge_to_json(const IEdge& edge);
};  // namespace json
};  // namespace printing
};  // namespace uni_course_cpp
