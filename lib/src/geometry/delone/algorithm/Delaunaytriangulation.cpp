#include "DelaunayTriangulation.h"
#include "LinAl.h"

#include <algorithm>
#include <random>
#include <cmath>
#include <stdexcept>

static const double SUPER = 1e7;


DelaunayTriangulation::DelaunayTriangulation()
    : tree_(dcel_)
{
    Point2D a(-SUPER, -SUPER);
    Point2D b( SUPER, -SUPER);
    Point2D c(0,       SUPER);

    size_t f0 = dcel_.init_super_triangle(a, b, c);

    for (size_t fi = 0; fi < dcel_.face_count(); ++fi) {
        if (dcel_.face_edge(fi) == INVALID) continue;

        size_t v0, v1, v2;
        dcel_.face_vertices(fi, v0, v1, v2);

        if (dcel_.is_infinite_vertex(v1) || dcel_.is_infinite_vertex(v2)) {
            if (dcel_.is_infinite_vertex(v1)) std::swap(v0, v1);
            else                               std::swap(v0, v2);
        }
        size_t ni = tree_.make_root(v0, v1, v2, fi);
        dcel_.set_face_node(fi, ni);
    }
    (void)f0;
}


bool DelaunayTriangulation::point_in_circumcircle(
        Point2D a, Point2D b, Point2D c, Point2D p)
{
    double ax = a.x - p.x, ay = a.y - p.y;
    double bx = b.x - p.x, by = b.y - p.y;
    double cx = c.x - p.x, cy = c.y - p.y;

    double det =
        ax * (by * (cx*cx + cy*cy) - cy * (bx*bx + by*by)) -
        ay * (bx * (cx*cx + cy*cy) - cx * (bx*bx + by*by)) +
        (ax*ax + ay*ay) * (bx * cy - by * cx);

    return det > 0;
}


bool DelaunayTriangulation::is_invalid_edge(size_t e) const {
    if (e == INVALID) return false;

    size_t et = dcel_.edge_twin(e);
    size_t f0 = dcel_.edge_face(e);
    size_t f1 = dcel_.edge_face(et);

    if (f0 == INVALID || f1 == INVALID) return false;

    size_t vp_idx = dcel_.edge_origin(dcel_.edge_prev(e));
    if (dcel_.is_infinite_vertex(vp_idx)) return false;
    Point2D p = dcel_.vertex_pos(vp_idx);

    if (dcel_.is_infinite_edge(e)) {
        size_t he = dcel_.edge_next(et);
        for (int i = 0; i < 3; ++i) {
            size_t orig = dcel_.edge_origin(he);
            size_t dest = dcel_.edge_origin(dcel_.edge_twin(he));
            if (!dcel_.is_infinite_vertex(orig) &&
                !dcel_.is_infinite_vertex(dest)) break;
            he = dcel_.edge_next(he);
        }

        Point2D fa = dcel_.vertex_pos(dcel_.edge_origin(he));
        Point2D fb = dcel_.vertex_pos(dcel_.edge_origin(dcel_.edge_twin(he)));

        return LinAl::orientation(fa, fb, p) == LinAl::Orient::left;
    }

    size_t va = dcel_.edge_origin(et);
    size_t vb = dcel_.edge_origin(dcel_.edge_next(et));
    size_t vc = dcel_.edge_origin(dcel_.edge_next(et));
    vc = dcel_.edge_origin(dcel_.edge_next(dcel_.edge_next(et)));

    if (dcel_.is_infinite_vertex(va) ||
        dcel_.is_infinite_vertex(vb) ||
        dcel_.is_infinite_vertex(vc)) return false;

    return point_in_circumcircle(
        dcel_.vertex_pos(va),
        dcel_.vertex_pos(vb),
        dcel_.vertex_pos(vc), p);
}


void DelaunayTriangulation::validate_edge(size_t e) {
    if (e == INVALID) return;
    if (!is_invalid_edge(e)) return;

    size_t et = dcel_.edge_twin(e);
    size_t f0 = dcel_.edge_face(e);
    size_t f1 = dcel_.edge_face(et);
    if (f0 == INVALID || f1 == INVALID) return;

    size_t n0 = dcel_.face_node(f0);
    size_t n1 = dcel_.face_node(f1);

    dcel_.flip_edge(e);

    // Специально ограничил область видимости.
    {
        size_t v0, v1, v2;
        dcel_.face_vertices(f0, v0, v1, v2);
        if (dcel_.is_infinite_vertex(v1)) std::swap(v0, v1);
        else if (dcel_.is_infinite_vertex(v2)) std::swap(v0, v2);
        dcel_.set_face_node(f0, tree_.insert(n0, n1, v0, v1, v2, f0));
    }
    {
        size_t v0, v1, v2;
        dcel_.face_vertices(f1, v0, v1, v2);
        if (dcel_.is_infinite_vertex(v1)) std::swap(v0, v1);
        else if (dcel_.is_infinite_vertex(v2)) std::swap(v0, v2);
        dcel_.set_face_node(f1, tree_.insert(n0, n1, v0, v1, v2, f1));
    }

    size_t e_next  = dcel_.edge_next(e);
    size_t et_prev = dcel_.edge_prev(et);

    validate_edge(e_next);
    validate_edge(et_prev);
}


void DelaunayTriangulation::insert(Point2D p) {
    size_t f = tree_.locate(p);
    if (f == INVALID) return;

    size_t parent_node = dcel_.face_node(f);
    size_t nf_before   = dcel_.face_count();

    dcel_.split_face(f, p);

    auto register_face = [&](size_t fi) {
        size_t v0, v1, v2;
        dcel_.face_vertices(fi, v0, v1, v2);
        // Бесконечная вершина — в v0
        if (dcel_.is_infinite_vertex(v1)) std::swap(v0, v1);
        else if (dcel_.is_infinite_vertex(v2)) std::swap(v0, v2);
        dcel_.set_face_node(fi, tree_.insert(parent_node, v0, v1, v2, fi));
    };

    register_face(f);
    register_face(nf_before);
    register_face(nf_before + 1);

    size_t v = dcel_.vertex_count() - 1;

    size_t e_start = dcel_.vertex_edge(v);
    size_t e_cur   = e_start;
    do {
        validate_edge(dcel_.edge_next(e_cur));
        e_cur = dcel_.edge_next(dcel_.edge_twin(e_cur));
    } while (e_cur != e_start);
}


std::vector<std::array<Point2D, 3>> DelaunayTriangulation::get_triangles() const {
    std::vector<std::array<Point2D, 3>> result;

    for (size_t fi = 0; fi < dcel_.face_count(); ++fi) {
        if (dcel_.is_infinite_face(fi) || dcel_.face_edge(fi) == INVALID) continue;

        size_t v0, v1, v2;
        dcel_.face_vertices(fi, v0, v1, v2);

        if (dcel_.is_infinite_vertex(v0) ||
            dcel_.is_infinite_vertex(v1) ||
            dcel_.is_infinite_vertex(v2)) continue;

        auto is_super = [](const Point2D& q) {
            return std::abs(q.x) >= SUPER * 0.9 || std::abs(q.y) >= SUPER * 0.9;
        };

        Point2D a = dcel_.vertex_pos(v0);
        Point2D b = dcel_.vertex_pos(v1);
        Point2D c = dcel_.vertex_pos(v2);

        if (is_super(a) || is_super(b) || is_super(c)) continue;

        result.push_back({a, b, c});
    }

    return result;
}


std::vector<std::array<Point2D, 3>>
DelaunayTriangulation::triangulate(std::vector<Point2D> points) {
    std::mt19937 rng(42);
    std::shuffle(points.begin(), points.end(), rng);

    DelaunayTriangulation dt;
    for (const auto& p : points)
        dt.insert(p);
    return dt.get_triangles();
}