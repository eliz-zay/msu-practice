#pragma once

#include <set>
#include <unordered_map>
#include <vector>
#include <functional>
#include <algorithm>

#include "i_edge.hpp"
#include "i_vertex.hpp"

namespace uni_course_cpp {
using GraphDepth = int;

using ForEachVertexCallback = std::function<void(const IVertex& vertex, int index)>;
using ForEachEdgeCallback = std::function<void(const IEdge& edge, int index)>;
using ForEachVertexCallbackPar = std::function<void(const IVertex& vertex)>;

class IGraph {
 public:
  virtual ~IGraph() {}

  virtual VertexId add_vertex() = 0;
  virtual EdgeId add_edge(VertexId from_vertex_id, VertexId to_vertex_id) = 0;

  virtual int get_vertices_count() const = 0;
  virtual int get_edges_count() const = 0;

  virtual void for_each_vertex(const ForEachVertexCallback& callback) const = 0;
  virtual void for_each_edge(const ForEachEdgeCallback& callback) const = 0;

  virtual void for_each_vertex_par(const ForEachVertexCallbackPar& callback) const = 0;

  virtual const std::vector<EdgeId>& get_vertex_edge_ids(
      VertexId vertex_id) const = 0;
  virtual const std::set<VertexId>& get_vertex_ids_at_depth(
      GraphDepth depth) const = 0;
  virtual const std::set<EdgeId>& get_edge_ids_by_color(
      EdgeColor color) const = 0;

  virtual GraphDepth get_vertex_depth(VertexId vertex_id) const = 0;
  virtual GraphDepth get_depth() const = 0;

  virtual bool has_edge(VertexId from_vertex_id,
                        VertexId to_vertex_id) const = 0;
};
}  // namespace uni_course_cpp
