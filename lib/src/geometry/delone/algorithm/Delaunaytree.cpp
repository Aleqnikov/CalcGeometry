#include "DelaunayTree.h"
#include <stdexcept>
#include <limits>
#include <cmath>


static double signed_area2(Point2D a, Point2D b, Point2D p) {
    return (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x);
}


// Предполагается что бесконечная вершина, если есть, хранится в v0.
// Это обеспечивается при вставке узлов в дерево.
bool DelaunayTree::point_in_node(Point2D p, size_t node_idx) const {
    const auto& nd = nodes_[node_idx];

    static constexpr double EPS = -1e-9;

    if (dcel_.is_infinite_vertex(nd.v0)) {
        Point2D a = dcel_.vertex_pos(nd.v1);
        Point2D b = dcel_.vertex_pos(nd.v2);
        return signed_area2(a, b, p) >= EPS;
    }

    Point2D a = dcel_.vertex_pos(nd.v0);
    Point2D b = dcel_.vertex_pos(nd.v1);
    Point2D c = dcel_.vertex_pos(nd.v2);

    return signed_area2(a, b, p) >= EPS &&
           signed_area2(b, c, p) >= EPS &&
           signed_area2(c, a, p) >= EPS;
}


size_t DelaunayTree::locate_from(Point2D p, size_t node_idx) const {
    const auto& nd = nodes_[node_idx];

    if (nd.children.empty()) {
        return nd.face;
    }

    for (size_t child : nd.children) {
        if (point_in_node(p, child)) {
            size_t result = locate_from(p, child);
            if (result != INVALID) return result;
        }
    }

    size_t best_child = INVALID;
    double best_score = -std::numeric_limits<double>::infinity();

    for (size_t child : nd.children) {
        const auto& cn = nodes_[child];

        double score;
        if (dcel_.is_infinite_vertex(cn.v0)) {
            Point2D a = dcel_.vertex_pos(cn.v1);
            Point2D b = dcel_.vertex_pos(cn.v2);
            score = signed_area2(a, b, p);
        } else {
            Point2D a = dcel_.vertex_pos(cn.v0);
            Point2D b = dcel_.vertex_pos(cn.v1);
            Point2D c = dcel_.vertex_pos(cn.v2);
            score = std::min({signed_area2(a, b, p),
                              signed_area2(b, c, p),
                              signed_area2(c, a, p)});
        }

        if (score > best_score) {
            best_score = score;
            best_child = child;
        }
    }

    if (best_child != INVALID)
        return locate_from(p, best_child);

    return INVALID;
}

size_t DelaunayTree::make_root(size_t v0, size_t v1, size_t v2, size_t face) {
    size_t idx = nodes_.size();
    nodes_.push_back({v0, v1, v2, face, {}});
    roots_.push_back(idx);
    return idx;
}

size_t DelaunayTree::insert(size_t parent,
                            size_t v0, size_t v1, size_t v2, size_t face) {
    size_t idx = nodes_.size();
    nodes_.push_back({v0, v1, v2, face, {}});
    nodes_[parent].children.push_back(idx);
    nodes_[parent].face = INVALID;
    return idx;
}

size_t DelaunayTree::insert(size_t parent0, size_t parent1,
                            size_t v0, size_t v1, size_t v2, size_t face) {
    size_t idx = nodes_.size();
    nodes_.push_back({v0, v1, v2, face, {}});
    nodes_[parent0].children.push_back(idx);
    nodes_[parent1].children.push_back(idx);
    nodes_[parent0].face = INVALID;
    nodes_[parent1].face = INVALID;
    return idx;
}

size_t DelaunayTree::locate(Point2D p) const {
    if (roots_.empty()) throw std::runtime_error("DelaunayTree is empty");

    for (size_t root : roots_) {
        if (point_in_node(p, root)) {
            size_t f = locate_from(p, root);
            if (f != INVALID) return f;
        }
    }

    // Линейный поиск по листьям
    size_t best_leaf = INVALID;
    double best_score = -std::numeric_limits<double>::infinity();

    for (size_t i = 0; i < nodes_.size(); ++i) {
        const auto& nd = nodes_[i];
        if (!nd.children.empty() || nd.face == INVALID) continue;

        double score;
        if (dcel_.is_infinite_vertex(nd.v0)) {
            Point2D a = dcel_.vertex_pos(nd.v1);
            Point2D b = dcel_.vertex_pos(nd.v2);
            score = signed_area2(a, b, p);
        } else {
            Point2D a = dcel_.vertex_pos(nd.v0);
            Point2D b = dcel_.vertex_pos(nd.v1);
            Point2D c = dcel_.vertex_pos(nd.v2);
            score = std::min({signed_area2(a, b, p),
                              signed_area2(b, c, p),
                              signed_area2(c, a, p)});
        }

        if (score > best_score) {
            best_score = score;
            best_leaf = nd.face;
        }
    }

    if (best_leaf != INVALID) return best_leaf;

    throw std::runtime_error("DelaunayTree::locate: failed to find face");
}