#include "DCEL.h"
#include <cassert>
#include <stdexcept>


size_t DCEL::new_vertex(Point2D pos, bool infinite) {
    size_t idx = vertices_.size();
    vertices_.push_back({pos, INVALID, infinite});
    return idx;
}

size_t DCEL::new_edge() {
    size_t idx = edges_.size();
    edges_.push_back({}); // e
    edges_.push_back({}); // twin
    edges_[idx].twin     = idx + 1;
    edges_[idx + 1].twin = idx;
    return idx;
}

size_t DCEL::new_face(bool infinite) {
    size_t idx = faces_.size();
    faces_.push_back({INVALID, INVALID, infinite});
    return idx;
}


bool DCEL::is_infinite_vertex(size_t v) const {
    return v == infinite_vertex_;
}

bool DCEL::is_infinite_edge(size_t e) const {
    size_t o = edges_[e].origin;
    size_t t = edges_[edges_[e].twin].origin;
    return is_infinite_vertex(o) || is_infinite_vertex(t);
}

bool DCEL::is_infinite_face(size_t f) const {
    return faces_[f].infinite;
}

void DCEL::face_vertices(size_t f, size_t& v0, size_t& v1, size_t& v2) const {
    size_t e0 = faces_[f].edge;
    size_t e1 = edges_[e0].next;
    size_t e2 = edges_[e1].next;
    v0 = edges_[e0].origin;
    v1 = edges_[e1].origin;
    v2 = edges_[e2].origin;
}


void DCEL::link_face(size_t f, size_t ea, size_t eb, size_t ec) {
    edges_[ea].face = f;  edges_[eb].face = f;  edges_[ec].face = f;
    faces_[f].edge  = ea;
    edges_[ea].next = eb; edges_[eb].next = ec; edges_[ec].next = ea;
    edges_[ea].prev = ec; edges_[eb].prev = ea; edges_[ec].prev = eb;
}


size_t DCEL::split_face(size_t f, Point2D p) {
    size_t e0 = faces_[f].edge;
    size_t e1 = edges_[e0].next;
    size_t e2 = edges_[e1].next;

    size_t v0 = edges_[e0].origin;
    size_t v1 = edges_[e1].origin;
    size_t v2 = edges_[e2].origin;

    // Новая вершина
    size_t vp = new_vertex(p);

    // ea: v1->vp,  ea_t: vp->v1
    // eb: v2->vp,  eb_t: vp->v2
    // ec: v0->vp,  ec_t: vp->v0
    size_t ea = new_edge(); size_t ea_t = ea + 1;
    size_t eb = new_edge(); size_t eb_t = eb + 1;
    size_t ec = new_edge(); size_t ec_t = ec + 1;

    edges_[ea].origin   = v1; edges_[ea_t].origin = vp;
    edges_[eb].origin   = v2; edges_[eb_t].origin = vp;
    edges_[ec].origin   = v0; edges_[ec_t].origin = vp;

    vertices_[vp].edge = ea_t;

    //   edges: e0(v0->v1), ea(v1->vp), ec_t(vp->v0)
    link_face(f, e0, ea, ec_t);

    //   edges: e1(v1->v2), eb(v2->vp), ea_t(vp->v1)
    size_t f1 = new_face(is_infinite_face(f));
    link_face(f1, e1, eb, ea_t);

    //   edges: e2(v2->v0), ec(v0->vp), eb_t(vp->v2)
    size_t f2 = new_face(is_infinite_face(f));
    link_face(f2, e2, ec, eb_t);

    return vp;
}

// flip_edge: переворачиваем ребро по Делоне
//
// Было:
//   f0: a->b, b->c, c->a
//   f1: b->a, a->d, d->b
//
// Стало (диагональ ab -> cd):
//   f0: a->d, d->c, c->a
//   f1: b->c, c->d, d->b
//
// не забываем обновить исходящие рёбра вершин a и b!
size_t DCEL::flip_edge(size_t e) {
    size_t et      = edges_[e].twin;
    size_t e_next  = edges_[e].next;   // b->c
    size_t e_prev  = edges_[e].prev;   // c->a
    size_t et_next = edges_[et].next;  // a->d
    size_t et_prev = edges_[et].prev;  // d->b

    size_t a = edges_[e].origin;
    size_t b = edges_[et].origin;
    size_t c = edges_[e_prev].origin;
    size_t d = edges_[et_prev].origin;

    size_t f0 = edges_[e].face;
    size_t f1 = edges_[et].face;

    edges_[e].origin  = c;
    edges_[et].origin = d;

    vertices_[a].edge = et_next;
    vertices_[b].edge = e_next;

    // f0 = {a, d, c}
    link_face(f0, et_next, et, e_prev);

    // f1 = {b, c, d}
    link_face(f1, e_next, e, et_prev);

    return e;
}

// init_super_triangle: один конечный треугольник abc + бесконечная вершина + 3 бесконечных грани.
size_t DCEL::init_super_triangle(Point2D a, Point2D b, Point2D c) {
    size_t v_inf = new_vertex({0, 0}, true);
    infinite_vertex_ = v_inf;
    size_t va = new_vertex(a);
    size_t vb = new_vertex(b);
    size_t vc = new_vertex(c);

    // Внутренние полурёбра треугольника (против часовой: a->b->c->a)
    size_t e0 = new_edge();
    size_t e1 = new_edge();
    size_t e2 = new_edge();

    edges_[e0].origin   = va; edges_[e0+1].origin = vb;
    edges_[e1].origin   = vb; edges_[e1+1].origin = vc;
    edges_[e2].origin   = vc; edges_[e2+1].origin = va;

    size_t f0 = new_face(false);
    link_face(f0, e0, e1, e2);

    vertices_[va].edge = e0;
    vertices_[vb].edge = e1;
    vertices_[vc].edge = e2;

    // Три бесконечных грани, каждая смотрит на одно внешнее полуребро.
    // fi_ab: inf->b, b->a, a->inf
    // fi_bc: inf->c, c->b, b->inf
    // fi_ca: inf->a, a->c, c->inf

    size_t ab0 = new_edge();
    size_t ab1 = new_edge();
    edges_[ab0].origin   = v_inf; edges_[ab0+1].origin = vb;
    edges_[ab1].origin   = va;    edges_[ab1+1].origin = v_inf;

    size_t fi_ab = new_face(true);
    link_face(fi_ab, ab0, e0+1, ab1);


    size_t bc0 = new_edge();
    size_t bc1 = new_edge();
    edges_[bc0].origin   = v_inf; edges_[bc0+1].origin = vc;
    edges_[bc1].origin   = vb;    edges_[bc1+1].origin = v_inf;

    size_t fi_bc = new_face(true);
    link_face(fi_bc, bc0, e1+1, bc1);


    size_t ca0 = new_edge();
    size_t ca1 = new_edge();
    edges_[ca0].origin   = v_inf; edges_[ca0+1].origin = va;
    edges_[ca1].origin   = vc;    edges_[ca1+1].origin = v_inf;

    size_t fi_ca = new_face(true);
    link_face(fi_ca, ca0, e2+1, ca1);

    // Близнецы между бесконечными гранями (лучи до/от v_inf):
    //   inf<->b: ab0 <-> bc1, bc1+1 <-> ab0+1
    //   inf<->c: bc0 <-> ca1, ca1+1 <-> bc0+1
    //   inf<->a: ca0 <-> ab1, ab1+1 <-> ca0+1

    edges_[ab0].twin   = bc1;   edges_[bc1].twin   = ab0;
    edges_[ab0+1].twin = bc1+1; edges_[bc1+1].twin = ab0+1;
    edges_[bc0].twin   = ca1;   edges_[ca1].twin   = bc0;
    edges_[bc0+1].twin = ca1+1; edges_[ca1+1].twin = bc0+1;
    edges_[ca0].twin   = ab1;   edges_[ab1].twin   = ca0;
    edges_[ca0+1].twin = ab1+1; edges_[ab1+1].twin = ca0+1;

    vertices_[v_inf].edge = ab0;

    return f0;
}