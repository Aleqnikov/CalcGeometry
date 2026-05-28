#include "DelaunayTriangulation.h"
#include "LinAl.h"

#include <algorithm>
#include <random>
#include <cmath>
#include <stdexcept>

static constexpr double SUPER = 1e7;

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
DelaunayTriangulation::DelaunayTriangulation()
    : tree_(dcel_)
{
    Point2D a(-SUPER, -SUPER);
    Point2D b( SUPER, -SUPER);
    Point2D c(0,       SUPER);

    size_t f0 = dcel_.init_super_triangle(a, b, c);

    for (size_t fi = 0; fi < dcel_.faces.size(); ++fi) {
        if (dcel_.faces[fi].edge == INVALID) continue;
        size_t v0, v1, v2;
        dcel_.face_vertices(fi, v0, v1, v2);
        // Гарантируем: бесконечная вершина всегда идёт первой (v0),
        // чтобы point_in_node мог проверять только nd.v0 (по лекции).
        if (dcel_.is_infinite_vertex(v1) || dcel_.is_infinite_vertex(v2)) {
            if (dcel_.is_infinite_vertex(v1)) std::swap(v0, v1);
            else                               std::swap(v0, v2);
        }
        size_t ni = tree_.make_root(v0, v1, v2, fi);
        dcel_.faces[fi].node = ni;
    }
    (void)f0;
}

// ---------------------------------------------------------------------------
// point_in_circumcircle
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
// is_invalid_edge
//
// Строго по лекции:
//
//   if e.infinite:
//       return orientation(
//           e.twin_edge.next_finite_edge.source,
//           e.twin_edge.next_finite_edge.target,
//           e.prev_edge.source) == left
//   return point_in_circle(
//       e.twin_edge.source,
//       e.twin_edge.target,
//       e.twin_edge.next_edge.target,
//       e.prev_edge.source)
// ---------------------------------------------------------------------------
bool DelaunayTriangulation::is_invalid_edge(size_t e) const {
    if (e == INVALID) return false;

    size_t et = dcel_.edges[e].twin;
    size_t f0 = dcel_.edges[e].face;
    size_t f1 = dcel_.edges[et].face;

    if (f0 == INVALID || f1 == INVALID) return false;

    // e.prev_edge.source = вершина напротив ребра e в грани f0
    size_t vp_idx = dcel_.edges[dcel_.edges[e].prev].origin;
    if (dcel_.is_infinite_vertex(vp_idx)) return false;
    Point2D p = dcel_.vertices[vp_idx].pos;

    if (dcel_.is_infinite_edge(e)) {
        // Бесконечное ребро:
        // orientation(twin.next_finite_edge.source,
        //             twin.next_finite_edge.target, p) == left
        //
        // next_finite_edge: идём по twin-грани пока не найдём ребро,
        // оба конца которого конечны.
        size_t he = dcel_.edges[et].next;
        for (int i = 0; i < 3; ++i) {
            size_t orig = dcel_.edges[he].origin;
            size_t dest = dcel_.edges[dcel_.edges[he].twin].origin;
            if (!dcel_.is_infinite_vertex(orig) &&
                !dcel_.is_infinite_vertex(dest)) break;
            he = dcel_.edges[he].next;
        }

        Point2D fa = dcel_.vertices[dcel_.edges[he].origin].pos;
        Point2D fb = dcel_.vertices[dcel_.edges[dcel_.edges[he].twin].origin].pos;

        return LinAl::orientation(fa, fb, p) == LinAl::Orient::left;
    }

    // Конечное ребро:
    // point_in_circle(twin.source, twin.target, twin.next.target, p)
    // = point_in_circumcircle вершин грани f1
    size_t va = dcel_.edges[et].origin;
    size_t vb = dcel_.edges[dcel_.edges[et].next].origin;
    size_t vc = dcel_.edges[dcel_.edges[et].next].origin;
    // twin.next.target = origin of twin.next.next
    vc = dcel_.edges[dcel_.edges[dcel_.edges[et].next].next].origin;

    if (dcel_.is_infinite_vertex(va) ||
        dcel_.is_infinite_vertex(vb) ||
        dcel_.is_infinite_vertex(vc)) return false;

    return point_in_circumcircle(
        dcel_.vertices[va].pos,
        dcel_.vertices[vb].pos,
        dcel_.vertices[vc].pos, p);
}

// ---------------------------------------------------------------------------
// validate_edge
//
// Строго по лекции:
//   if invalid_edge(e):
//       n0 = e.face.node
//       n1 = e.twin_edge.face.node
//       e = dcel.flip_edge(e)
//       e.face.node      = tree.insert(n0, n1, e.face)
//       e.twin_edge.node = tree.insert(n0, n1, e.twin_edge.face)
//       validate_edge(e.next_edge)
//       validate_edge(e.twin_edge.prev_edge)
// ---------------------------------------------------------------------------
void DelaunayTriangulation::validate_edge(size_t e) {
    if (e == INVALID) return;
    if (!is_invalid_edge(e)) return;

    size_t et = dcel_.edges[e].twin;
    size_t f0 = dcel_.edges[e].face;
    size_t f1 = dcel_.edges[et].face;
    if (f0 == INVALID || f1 == INVALID) return;

    size_t n0 = dcel_.faces[f0].node;
    size_t n1 = dcel_.faces[f1].node;

    dcel_.flip_edge(e);

    // После flip обновляем узлы дерева для обеих граней
    {
        size_t v0, v1, v2;
        dcel_.face_vertices(f0, v0, v1, v2);
        if (dcel_.is_infinite_vertex(v1)) std::swap(v0, v1);
        else if (dcel_.is_infinite_vertex(v2)) std::swap(v0, v2);
        dcel_.faces[f0].node = tree_.insert(n0, n1, v0, v1, v2, f0);
    }
    {
        size_t v0, v1, v2;
        dcel_.face_vertices(f1, v0, v1, v2);
        if (dcel_.is_infinite_vertex(v1)) std::swap(v0, v1);
        else if (dcel_.is_infinite_vertex(v2)) std::swap(v0, v2);
        dcel_.faces[f1].node = tree_.insert(n0, n1, v0, v1, v2, f1);
    }

    // Строго по лекции: два рекурсивных вызова
    size_t e_next   = dcel_.edges[e].next;
    size_t et_prev  = dcel_.edges[et].prev;

    validate_edge(e_next);
    validate_edge(et_prev);
}

// ---------------------------------------------------------------------------
// insert
//
// Строго по лекции:
//   l = tree.locate(p)
//   v = dcel.split_face(l.f, l.e, p)
//   for f in dcel.incident_faces(v):
//       f.node = tree.insert(f.node, f)
//   for e in dcel.outgoing_edges(v):
//       validate_edge(e.next_edge)
// ---------------------------------------------------------------------------
void DelaunayTriangulation::insert(Point2D p) {
    size_t f = tree_.locate(p);
    if (f == INVALID) return;

    size_t parent_node = dcel_.faces[f].node;
    size_t nf_before   = dcel_.faces.size();

    dcel_.split_face(f, p);

    // Три грани после split: f (reused), nf_before, nf_before+1
    // Регистрируем все три в дереве (incident_faces(v))
    auto register_face = [&](size_t fi) {
        size_t v0, v1, v2;
        dcel_.face_vertices(fi, v0, v1, v2);
        // Бесконечная вершина — в v0
        if (dcel_.is_infinite_vertex(v1)) std::swap(v0, v1);
        else if (dcel_.is_infinite_vertex(v2)) std::swap(v0, v2);
        dcel_.faces[fi].node = tree_.insert(parent_node, v0, v1, v2, fi);
    };

    register_face(f);
    register_face(nf_before);
    register_face(nf_before + 1);

    // Новая вершина v — последняя добавленная
    size_t v = dcel_.vertices.size() - 1;

    // for e in dcel.outgoing_edges(v): validate_edge(e.next_edge)
    // Обходим все исходящие рёбра из v по структуре DCEL:
    // начинаем с vertices[v].edge, идём через twin->next
    size_t e_start = dcel_.vertices[v].edge;
    size_t e_cur   = e_start;
    do {
        validate_edge(dcel_.edges[e_cur].next);
        // следующее исходящее ребро из v
        e_cur = dcel_.edges[dcel_.edges[e_cur].twin].next;
    } while (e_cur != e_start);
}

// ---------------------------------------------------------------------------
// get_triangles
// ---------------------------------------------------------------------------
std::vector<std::array<Point2D, 3>> DelaunayTriangulation::get_triangles() const {
    std::vector<std::array<Point2D, 3>> result;

    for (size_t fi = 0; fi < dcel_.faces.size(); ++fi) {
        const auto& face = dcel_.faces[fi];
        if (face.infinite || face.edge == INVALID) continue;

        size_t v0, v1, v2;
        dcel_.face_vertices(fi, v0, v1, v2);

        if (dcel_.is_infinite_vertex(v0) ||
            dcel_.is_infinite_vertex(v1) ||
            dcel_.is_infinite_vertex(v2)) continue;

        auto is_super = [](const Point2D& q) {
            return std::abs(q.x) >= SUPER * 0.9 || std::abs(q.y) >= SUPER * 0.9;
        };

        Point2D a = dcel_.vertices[v0].pos;
        Point2D b = dcel_.vertices[v1].pos;
        Point2D c = dcel_.vertices[v2].pos;

        if (is_super(a) || is_super(b) || is_super(c)) continue;

        result.push_back({a, b, c});
    }

    return result;
}

// ---------------------------------------------------------------------------
// triangulate
// ---------------------------------------------------------------------------
std::vector<std::array<Point2D, 3>>
DelaunayTriangulation::triangulate(std::vector<Point2D> points) {
    std::mt19937 rng(42);
    std::shuffle(points.begin(), points.end(), rng);

    DelaunayTriangulation dt;
    for (const auto& p : points) dt.insert(p);
    return dt.get_triangles();
}