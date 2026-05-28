#include "VoronoiFromDelaunay.h"
#include <cmath>
#include <unordered_set>

static std::optional<Point2D> circumcenter(Point2D a, Point2D b, Point2D c) {
    double ax = b.x - a.x, ay = b.y - a.y;
    double bx = c.x - a.x, by = c.y - a.y;
    double D = 2.0 * (ax * by - ay * bx);
    if (std::abs(D) < 1e-10) return std::nullopt;
    double ux = (by*(ax*ax+ay*ay) - ay*(bx*bx+by*by)) / D;
    double uy = (ax*(bx*bx+by*by) - bx*(ax*ax+ay*ay)) / D;
    return Point2D{a.x + ux, a.y + uy};
}

static bool is_real_face(const DCEL& dcel, size_t fi) {
    if (fi == INVALID || dcel.faces[fi].infinite) return false;
    if (dcel.faces[fi].edge == INVALID) return false;
    size_t v0, v1, v2;
    dcel.face_vertices(fi, v0, v1, v2);
    auto super = [&](size_t v) {
        if (dcel.is_infinite_vertex(v)) return true;
        const Point2D& p = dcel.vertices[v].pos;
        return std::abs(p.x) >= 1e6*0.9 || std::abs(p.y) >= 1e6*0.9;
    };
    return !super(v0) && !super(v1) && !super(v2);
}

static Point2D outward_direction(const DCEL& dcel, size_t e) {
    size_t orig = dcel.edges[e].origin;
    size_t dest = dcel.edges[dcel.edges[e].twin].origin;
    Point2D a = dcel.vertices[orig].pos;
    Point2D b = dcel.vertices[dest].pos;
    double dx = b.x - a.x, dy = b.y - a.y;
    double mx = (a.x+b.x)*0.5, my = (a.y+b.y)*0.5;

    size_t fi = dcel.edges[e].face;
    size_t v0, v1, v2;
    dcel.face_vertices(fi, v0, v1, v2);
    double cx = (dcel.vertices[v0].pos.x +
                 dcel.vertices[v1].pos.x +
                 dcel.vertices[v2].pos.x) / 3.0;
    double cy = (dcel.vertices[v0].pos.y +
                 dcel.vertices[v1].pos.y +
                 dcel.vertices[v2].pos.y) / 3.0;

    double nx = dy, ny = -dx;
    if ((mx+nx-cx)*nx + (my+ny-cy)*ny < 0) { nx=-nx; ny=-ny; }
    double len = std::sqrt(nx*nx + ny*ny);
    if (len < 1e-12) return {1.0, 0.0};
    return {nx/len, ny/len};
}

VoronoiDiagram voronoi_from_delaunay(const DCEL& dcel) {
    VoronoiDiagram vor;
    std::unordered_set<size_t> visited;

    for (size_t ei = 0; ei < dcel.edges.size(); ++ei) {
        size_t ti = dcel.edges[ei].twin;
        if (ti == INVALID) continue;
        if (!visited.insert(std::min(ei, ti)).second) continue;

        size_t fi = dcel.edges[ei].face;
        size_t fj = dcel.edges[ti].face;
        if (fi == INVALID || fj == INVALID) continue;

        bool ri = is_real_face(dcel, fi);
        bool rj = is_real_face(dcel, fj);
        if (!ri && !rj) continue;

        if (ri && rj) {
            size_t v0,v1,v2; dcel.face_vertices(fi,v0,v1,v2);
            auto ci = circumcenter(dcel.vertices[v0].pos,
                                   dcel.vertices[v1].pos,
                                   dcel.vertices[v2].pos);
            dcel.face_vertices(fj,v0,v1,v2);
            auto cj = circumcenter(dcel.vertices[v0].pos,
                                   dcel.vertices[v1].pos,
                                   dcel.vertices[v2].pos);
            if (!ci || !cj) continue;
            vor.edges.push_back({*ci, *cj, std::nullopt});
        } else {
            size_t real_e  = ri ? ei : ti;
            size_t real_fi = ri ? fi : fj;
            size_t v0,v1,v2; dcel.face_vertices(real_fi,v0,v1,v2);
            auto ci = circumcenter(dcel.vertices[v0].pos,
                                   dcel.vertices[v1].pos,
                                   dcel.vertices[v2].pos);
            if (!ci) continue;
            vor.edges.push_back({*ci, std::nullopt, outward_direction(dcel, real_e)});
        }
    }
    return vor;
}