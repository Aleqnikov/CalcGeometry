#include "DelaunayTriangulation.h"
#include "LinAl.h"

#include <algorithm>
#include <random>
#include <cmath>
#include <stdexcept>


static const double LOC_EPS = -1e-9;

static double area2(Point2D a, Point2D b, Point2D c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}


void DelaunayTriangulation::face_vertices_inf_first(
        size_t f, size_t& v0, size_t& v1, size_t& v2) const
{
    dcel_.face_vertices(f, v0, v1, v2);

    if (dcel_.is_infinite_vertex(v1)) {
        size_t t = v0; v0 = v1; v1 = v2; v2 = t;
    } else if (dcel_.is_infinite_vertex(v2)) {
        size_t t = v0; v0 = v2; v2 = v1; v1 = t;
    }
}


bool DelaunayTriangulation::face_contains(size_t f, Point2D p) const {
    if (f == INVALID || dcel_.face_edge(f) == INVALID) return false;

    size_t v0, v1, v2;
    face_vertices_inf_first(f, v0, v1, v2);

    if (dcel_.is_infinite_vertex(v0)) {

        Point2D a = dcel_.vertex_pos(v1), b = dcel_.vertex_pos(v2);
        double scale = std::max({std::fabs(a.x), std::fabs(a.y),
                                 std::fabs(b.x), std::fabs(b.y),
                                 std::fabs(p.x), std::fabs(p.y), 1.0});
        return area2(a, b, p) > 1e-12 * scale * scale;
    }

    Point2D a = dcel_.vertex_pos(v0);
    Point2D b = dcel_.vertex_pos(v1);
    Point2D c = dcel_.vertex_pos(v2);
    return area2(a, b, p) >= LOC_EPS &&
           area2(b, c, p) >= LOC_EPS &&
           area2(c, a, p) >= LOC_EPS;
}



size_t DelaunayTriangulation::walk_locate(Point2D p, size_t start) const {
    size_t cur = start;

    if (cur == INVALID || dcel_.face_edge(cur) == INVALID) {
        for (size_t fi = 0; fi < dcel_.face_count(); ++fi)
            if (dcel_.face_edge(fi) != INVALID && !dcel_.is_infinite_face(fi)) { cur = fi; break; }
    }

    const size_t max_steps = 4 * dcel_.face_count() + 16;

    for (size_t step = 0; step < max_steps; ++step) {
        if (dcel_.face_edge(cur) == INVALID) break;

        if (dcel_.is_infinite_face(cur)) {
            if (face_contains(cur, p)) return cur;
            size_t e = dcel_.face_edge(cur), fin = INVALID;
            for (int i = 0; i < 3; ++i) {
                if (!dcel_.is_infinite_vertex(dcel_.edge_origin(e)) &&
                    !dcel_.is_infinite_vertex(dcel_.edge_origin(dcel_.edge_twin(e)))) { fin = e; break; }
                e = dcel_.edge_next(e);
            }
            if (fin == INVALID) break;
            cur = dcel_.edge_face(dcel_.edge_twin(fin));
            continue;
        }

        size_t e = dcel_.face_edge(cur), cross = INVALID;
        for (int i = 0; i < 3; ++i) {
            Point2D a = dcel_.vertex_pos(dcel_.edge_origin(e));
            Point2D b = dcel_.vertex_pos(dcel_.edge_origin(dcel_.edge_next(e)));
            if (area2(a, b, p) < LOC_EPS) { cross = e; break; }
            e = dcel_.edge_next(e);
        }
        if (cross == INVALID) return cur;

        size_t nb = dcel_.edge_face(dcel_.edge_twin(cross));
        if (nb == INVALID) break;
        cur = nb;
    }

    for (size_t fi = 0; fi < dcel_.face_count(); ++fi)
        if (!dcel_.is_infinite_face(fi) && face_contains(fi, p)) return fi;
    for (size_t fi = 0; fi < dcel_.face_count(); ++fi)
        if (dcel_.is_infinite_face(fi) && face_contains(fi, p)) return fi;

    return INVALID;
}


size_t DelaunayTriangulation::edge_hit(size_t f, Point2D p) const {
    if (f == INVALID || dcel_.face_edge(f) == INVALID) return INVALID;

    size_t e = dcel_.face_edge(f);
    for (int i = 0; i < 3; ++i, e = dcel_.edge_next(e)) {
        size_t vo = dcel_.edge_origin(e);
        size_t vd = dcel_.edge_origin(dcel_.edge_twin(e));
        if (dcel_.is_infinite_vertex(vo) || dcel_.is_infinite_vertex(vd)) continue;

        Point2D a = dcel_.vertex_pos(vo), b = dcel_.vertex_pos(vd);
        double scale = std::max({std::fabs(a.x), std::fabs(a.y),
                                 std::fabs(b.x), std::fabs(b.y),
                                 std::fabs(p.x), std::fabs(p.y), 1.0});
        if (std::fabs(area2(a, b, p)) > 1e-12 * scale * scale) continue;

        double dx = b.x - a.x, dy = b.y - a.y;
        double t  = (p.x - a.x) * dx + (p.y - a.y) * dy;
        double L2 = dx * dx + dy * dy;
        if (t > 0.0 && t < L2) return e;
    }
    return INVALID;
}


size_t DelaunayTriangulation::locate_face(Point2D p, size_t& on_edge) const {
    on_edge = INVALID;

    size_t f = INVALID;
    try { f = tree_.locate(p); } catch (const std::exception&) { f = INVALID; }

    if (f == INVALID || !face_contains(f, p))
        f = walk_locate(p, f);

    if (f == INVALID) return INVALID;

    on_edge = edge_hit(f, p);
    return f;
}


DelaunayTriangulation::DelaunayTriangulation()
    : tree_(dcel_)
{

}


void DelaunayTriangulation::try_bootstrap() {
    if (pending_.size() < 3) return;

    size_t i0 = 0, i1 = INVALID, i2 = INVALID;
    for (size_t t = 1; t < pending_.size(); ++t) {
        if (pending_[t].x != pending_[i0].x || pending_[t].y != pending_[i0].y) {
            i1 = t; break;
        }
    }
    if (i1 == INVALID) return;

    for (size_t t = 0; t < pending_.size(); ++t) {
        if (t == i0 || t == i1) continue;
        if (area2(pending_[i0], pending_[i1], pending_[t]) != 0.0) { i2 = t; break; }
    }
    if (i2 == INVALID) return;

    Point2D a = pending_[i0], b = pending_[i1], c = pending_[i2];
    if (area2(a, b, c) < 0.0) std::swap(b, c);

    std::vector<Point2D> rest;
    for (size_t t = 0; t < pending_.size(); ++t)
        if (t != i0 && t != i1 && t != i2) rest.push_back(pending_[t]);
    pending_.clear();

    dcel_.init_super_triangle(a, b, c);

    for (size_t fi = 0; fi < dcel_.face_count(); ++fi) {
        if (dcel_.face_edge(fi) == INVALID) continue;
        size_t v0, v1, v2;
        face_vertices_inf_first(fi, v0, v1, v2);
        dcel_.set_face_node(fi, tree_.make_root(v0, v1, v2, fi));
    }

    initialized_ = true;

    for (const auto& q : rest) insert(q);
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
            if (!dcel_.is_infinite_vertex(orig) && !dcel_.is_infinite_vertex(dest)) break;
            he = dcel_.edge_next(he);
        }

        Point2D fa = dcel_.vertex_pos(dcel_.edge_origin(he));
        Point2D fb = dcel_.vertex_pos(dcel_.edge_origin(dcel_.edge_twin(he)));
        return LinAl::orientation(fa, fb, p) == LinAl::Orient::left;
    }

    size_t va = dcel_.edge_origin(et);
    size_t vb = dcel_.edge_origin(dcel_.edge_next(et));
    size_t vc = dcel_.edge_origin(dcel_.edge_next(dcel_.edge_next(et)));

    if (dcel_.is_infinite_vertex(va) ||
        dcel_.is_infinite_vertex(vb) ||
        dcel_.is_infinite_vertex(vc)) return false;

    return point_in_circumcircle(
        dcel_.vertex_pos(va), dcel_.vertex_pos(vb), dcel_.vertex_pos(vc), p);
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

    {
        size_t v0, v1, v2;
        face_vertices_inf_first(f0, v0, v1, v2);
        dcel_.set_face_node(f0, tree_.insert(n0, n1, v0, v1, v2, f0));
    }
    {
        size_t v0, v1, v2;
        face_vertices_inf_first(f1, v0, v1, v2);
        dcel_.set_face_node(f1, tree_.insert(n0, n1, v0, v1, v2, f1));
    }

    validate_edge(dcel_.edge_next(e));
    validate_edge(dcel_.edge_prev(et));
}


void DelaunayTriangulation::insert(Point2D p) {
    if (!initialized_) {
        pending_.push_back(p);
        try_bootstrap();
        return;
    }

    size_t on_edge = INVALID;
    size_t f = locate_face(p, on_edge);
    if (f == INVALID) return;

    {
        size_t w0, w1, w2;
        dcel_.face_vertices(f, w0, w1, w2);
        for (size_t w : {w0, w1, w2}) {
            if (dcel_.is_infinite_vertex(w)) continue;
            Point2D q = dcel_.vertex_pos(w);
            double scale = std::max({std::fabs(q.x), std::fabs(q.y),
                                     std::fabs(p.x), std::fabs(p.y), 1.0});
            if (std::fabs(q.x - p.x) <= 1e-12 * scale &&
                std::fabs(q.y - p.y) <= 1e-12 * scale) return;
        }
    }

    if (on_edge != INVALID) {
        size_t et = dcel_.edge_twin(on_edge);
        size_t f0 = dcel_.edge_face(on_edge);
        size_t f1 = dcel_.edge_face(et);
        if (f0 != INVALID && f1 != INVALID) {
            size_t n0 = dcel_.face_node(f0);
            size_t n1 = dcel_.face_node(f1);

            size_t o1 = dcel_.edge_next(on_edge);
            size_t o2 = dcel_.edge_prev(on_edge);
            size_t o3 = dcel_.edge_next(et);
            size_t o4 = dcel_.edge_prev(et);

            size_t nf = dcel_.face_count();
            dcel_.split_edge(on_edge, p);

            size_t nfs[4] = {f0, f1, nf, nf + 1};
            for (size_t fi : nfs) {
                size_t v0, v1, v2;
                face_vertices_inf_first(fi, v0, v1, v2);
                dcel_.set_face_node(fi, tree_.insert(n0, n1, v0, v1, v2, fi));
            }

            validate_edge(o1);
            validate_edge(o2);
            validate_edge(o3);
            validate_edge(o4);
            return;
        }
    }

    size_t parent_node = dcel_.face_node(f);
    size_t nf_before   = dcel_.face_count();

    dcel_.split_face(f, p);

    auto register_face = [&](size_t fi) {
        size_t v0, v1, v2;
        face_vertices_inf_first(fi, v0, v1, v2);
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
        if (dcel_.face_edge(fi) == INVALID) continue;
        if (dcel_.is_infinite_face(fi))     continue;

        size_t v0, v1, v2;
        dcel_.face_vertices(fi, v0, v1, v2);
        result.push_back({dcel_.vertex_pos(v0),
                          dcel_.vertex_pos(v1),
                          dcel_.vertex_pos(v2)});
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