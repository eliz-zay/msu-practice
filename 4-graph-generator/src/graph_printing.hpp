#pragma once

#include <string>

#include "interfaces/i_graph.hpp"

namespace uni_course_cpp {
namespace printing {
std::string graph_info_to_string(const IGraph& graph);
std::string vertices_info_to_string(const IGraph& graph);
std::string edges_info_to_string(const IGraph& graph);

std::string edge_color_to_string(const EdgeColor& color);
};  // namespace printing
};  // namespace uni_course_cpp
