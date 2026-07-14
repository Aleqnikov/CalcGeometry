#ifndef DELAUNAYTREE_H
#define DELAUNAYTREE_H

#include <vector>
#include <array>
#include <cstddef>
#include "DCEL.h"
#include "LinAl.h"

struct DelaunayTreeNode {
    size_t v0, v1, v2;           // вершины треугольника
    size_t face;                  // грань DCEL
    std::vector<size_t> children; // дочерние узлы
};

class DelaunayTree {
public:
    explicit DelaunayTree(DCEL& dcel) : dcel_(dcel) {}

    size_t make_root(size_t v0, size_t v1, size_t v2, size_t face);

    size_t insert(size_t parent,
                  size_t v0, size_t v1, size_t v2, size_t face);

    size_t insert(size_t parent0, size_t parent1,
                  size_t v0, size_t v1, size_t v2, size_t face);

    size_t locate(Point2D p) const;

    const DelaunayTreeNode& node(size_t idx) const { return nodes_[idx]; }
    DelaunayTreeNode&       node(size_t idx)       { return nodes_[idx]; }

    size_t node_count() const { return nodes_.size(); }

private:
    DCEL& dcel_;
    std::vector<DelaunayTreeNode> nodes_;
    std::vector<size_t>           roots_;

    bool   point_in_node(Point2D p, size_t node_idx) const;
    size_t locate_from(Point2D p, size_t node_idx)   const;
};

#endif // DELAUNAYTREE_H