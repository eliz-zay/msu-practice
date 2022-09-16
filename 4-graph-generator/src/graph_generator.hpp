#pragma once

#include <mutex>
#include <string>

#include "graph.hpp"

namespace uni_course_cpp {
class GraphGenerator {
 public:
  struct Params {
   public:
    explicit Params(GraphDepth depth, int new_vertices_count)
        : depth_(depth), new_vertices_count_(new_vertices_count) {}

    GraphDepth depth() const { return depth_; }
    int new_vertices_count() const { return new_vertices_count_; }

   private:
    GraphDepth depth_ = 0;
    int new_vertices_count_ = 0;
  };

  explicit GraphGenerator(Params&& params);

  std::unique_ptr<IGraph> generate() const;

 private:
  Params params_;

  void generate_grey_edges(Graph& graph,
                           std::mutex& graph_mutex,
                           VertexId root_vertex_id) const;
  void generate_green_edges(Graph& graph, std::mutex& graph_mutex) const;
  void generate_yellow_edges(Graph& graph, std::mutex& graph_mutex) const;
  void generate_red_edges(Graph& graph, std::mutex& graph_mutex) const;

  void generate_grey_branch(std::mutex& graph_mutex,
                            Graph& graph,
                            VertexId from_vertex_id,
                            GraphDepth from_vertex_depth) const;
};
};  // namespace uni_course_cpp
