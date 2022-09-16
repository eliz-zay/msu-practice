#pragma once

#include <set>
#include <unordered_map>
#include <vector>

#include "interfaces/i_edge.hpp"
#include "interfaces/i_graph.hpp"
#include "interfaces/i_vertex.hpp"

namespace uni_course_cpp {
class Graph : public IGraph {
 public:
  static constexpr GraphDepth kInitialDepth = 1;

  VertexId add_vertex() override;
  EdgeId add_edge(VertexId from_vertex_id, VertexId to_vertex_id) override;

  int get_vertices_count() const override;
  int get_edges_count() const override;

  void for_each_vertex(const ForEachVertexCallback& callback) const override;
  void for_each_edge(const ForEachEdgeCallback& callback) const override;

  void for_each_vertex_par(
      const ForEachVertexCallbackPar& callback) const override;

  const std::vector<EdgeId>& get_vertex_edge_ids(
      VertexId vertex_id) const override;
  const std::set<VertexId>& get_vertex_ids_at_depth(
      GraphDepth depth) const override;
  const std::set<EdgeId>& get_edge_ids_by_color(EdgeColor color) const override;

  GraphDepth get_vertex_depth(VertexId vertex_id) const override;
  GraphDepth get_depth() const override;

  bool has_edge(VertexId from_vertex_id, VertexId to_vertex_id) const override;

 private:
  class Vertex : public IVertex {
   public:
    explicit Vertex(VertexId init_id) : id_(init_id) {}
    VertexId id() const override { return id_; }

   private:
    VertexId id_ = 0;
  };

  class Edge : public IEdge {
   public:
    Edge(EdgeId init_id,
         VertexId init_from_vertex_id,
         VertexId init_to_vertex_id,
         EdgeColor color)
        : id_(init_id),
          from_vertex_id_(init_from_vertex_id),
          to_vertex_id_(init_to_vertex_id),
          color_(color) {}

    EdgeId id() const override { return id_; }
    VertexId from_vertex_id() const override { return from_vertex_id_; }
    VertexId to_vertex_id() const override { return to_vertex_id_; }
    EdgeColor color() const override { return color_; }

   private:
    EdgeId id_ = 0;
    VertexId from_vertex_id_ = 0;
    VertexId to_vertex_id_ = 0;
    EdgeColor color_;
  };

  std::unordered_map<VertexId, Vertex> vertices_;
  std::unordered_map<EdgeId, Edge> edges_;

  std::unordered_map<VertexId, std::vector<EdgeId>> adjacency_list_;
  std::unordered_map<VertexId, GraphDepth> vertex_depths_;
  std::unordered_map<GraphDepth, std::set<VertexId>> depth_vertices_;
  std::unordered_map<EdgeColor, std::set<EdgeId>> colored_edges_;

  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;

  VertexId get_new_vertex_id();
  EdgeId get_new_edge_id();

  void update_vertex_depth(VertexId previous_vertex_id, VertexId vertex_id);
  void init_vertex_depth(VertexId vertex_id);

  EdgeColor calculate_edge_color(VertexId from_vertex_id,
                                 VertexId to_vertex_id) const;

  bool has_vertex(VertexId vertex_id) const;
};
};  // namespace uni_course_cpp
