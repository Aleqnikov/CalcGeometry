#include "DCEL.h"
#include <cassert>
#include <stdexcept>

size_t DCEL::new_vertex(Point2D pos, bool infinite) {
    size_t idx = vertices.size();
    vertices.push_back({pos, INVALID, infinite});
    return idx;
}

// Creates a twin pair; returns index of first (even) half-edge.
size_t DCEL::new_edge() {
    size_t idx = edges.size();
    edges.push_back({}); // e
    edges.push_back({}); // twin
    edges[idx].twin     = idx + 1;
    edges[idx + 1].twin = idx;
    return idx;
}

size_t DCEL::new_face(bool infinite) {
    size_t idx = faces.size();
    faces.push_back({INVALID, INVALID, infinite});
    return idx;
}

bool DCEL::is_infinite_vertex(size_t v) const {
    return v == infinite_vertex;
}

bool DCEL::is_infinite_edge(size_t e) const {
    size_t o = edges[e].origin;
    size_t t = edges[edges[e].twin].origin;
    return is_infinite_vertex(o) || is_infinite_vertex(t);
}

bool DCEL::is_infinite_face(size_t f) const {
    return faces[f].infinite;
}

void DCEL::face_vertices(size_t f, size_t& v0, size_t& v1, size_t& v2) const {
    size_t e0 = faces[f].edge;
    size_t e1 = edges[e0].next;
    size_t e2 = edges[e1].next;
    v0 = edges[e0].origin;
    v1 = edges[e1].origin;
    v2 = edges[e2].origin;
}

// Helper to link three half-edges into a CCW face loop
static void link_face(DCEL& d, size_t f,
                      size_t ea, size_t eb, size_t ec)
{
    d.edges[ea].face = f;  d.edges[eb].face = f;  d.edges[ec].face = f;
    d.faces[f].edge  = ea;
    d.edges[ea].next = eb; d.edges[eb].next = ec; d.edges[ec].next = ea;
    d.edges[ea].prev = ec; d.edges[eb].prev = ea; d.edges[ec].prev = eb;
}

// split_face: split triangle f into three by inserting point p.
// Layout of triangle f:  e0(v0->v1), e1(v1->v2), e2(v2->v0)
// New triangles:
//   f (reused): v0, v1, vp
//   f1:         v1, v2, vp
//   f2:         v2, v0, vp
size_t DCEL::split_face(size_t f, Point2D p) {
    size_t e0 = faces[f].edge;
    size_t e1 = edges[e0].next;
    size_t e2 = edges[e1].next;

    size_t v0 = edges[e0].origin;
    size_t v1 = edges[e1].origin;
    size_t v2 = edges[e2].origin;

    // New vertex
    size_t vp = new_vertex(p);

    // New edges (each new_edge() creates a CCW/twin pair)
    // ea: v1->vp,  ea_t: vp->v1
    // eb: v2->vp,  eb_t: vp->v2
    // ec: v0->vp,  ec_t: vp->v0
    size_t ea = new_edge(); size_t ea_t = ea + 1;
    size_t eb = new_edge(); size_t eb_t = eb + 1;
    size_t ec = new_edge(); size_t ec_t = ec + 1;

    edges[ea].origin   = v1; edges[ea_t].origin = vp;
    edges[eb].origin   = v2; edges[eb_t].origin = vp;
    edges[ec].origin   = v0; edges[ec_t].origin = vp;

    vertices[vp].edge = ea_t; // one outgoing edge from vp

    // Face f (reused): v0 -> v1 -> vp -> v0
    //   edges: e0(v0->v1), ea(v1->vp), ec_t(vp->v0)
    link_face(*this, f, e0, ea, ec_t);

    // Face f1: v1 -> v2 -> vp -> v1
    //   edges: e1(v1->v2), eb(v2->vp), ea_t(vp->v1)
    size_t f1 = new_face(is_infinite_face(f));
    link_face(*this, f1, e1, eb, ea_t);

    // Face f2: v2 -> v0 -> vp -> v2
    //   edges: e2(v2->v0), ec(v0->vp), eb_t(vp->v2)
    size_t f2 = new_face(is_infinite_face(f));
    link_face(*this, f2, e2, ec, eb_t);

    return vp;
}

// flip_edge: standard Delaunay edge flip.
//
// Before:
//   face f0: e(a->b),  e_next(b->c),  e_prev(c->a)
//   face f1: et(b->a), et_next(a->d), et_prev(d->b)
//
// After flip, diagonal ab becomes cd:
//   face f0: et_next(a->d), et(d->c), e_prev(c->a)   — triangle {a,d,c}
//   face f1: e_next(b->c),  e(c->d),  et_prev(d->b)  — triangle {b,c,d}
//
//   e  gets new origin = c  (now c->d)
//   et gets new origin = d  (now d->c)
//
// FIX: also update vertex->edge pointers for a and b, since
//      their outgoing edges e and et changed meaning.

size_t DCEL::flip_edge(size_t e) {
    size_t et      = edges[e].twin;
    size_t e_next  = edges[e].next;   // b->c
    size_t e_prev  = edges[e].prev;   // c->a
    size_t et_next = edges[et].next;  // a->d
    size_t et_prev = edges[et].prev;  // d->b

    size_t a = edges[e].origin;
    size_t b = edges[et].origin;
    size_t c = edges[e_prev].origin;
    size_t d = edges[et_prev].origin;

    size_t f0 = edges[e].face;
    size_t f1 = edges[et].face;

    // After flip: e goes c->d, et goes d->c
    edges[e].origin  = c;
    edges[et].origin = d;

    // Fix vertex->edge pointers for a and b:
    // their old outgoing edges (e, et) now belong to c and d.
    // Point them to edges that actually start at a and b.
    vertices[a].edge = et_next;  // et_next: a->d  (starts at a) ✓
    vertices[b].edge = e_next;   // e_next:  b->c  (starts at b) ✓

    // face f0 = {a, d, c}: et_next(a->d), et(d->c), e_prev(c->a)
    link_face(*this, f0, et_next, et, e_prev);

    // face f1 = {b, c, d}: e_next(b->c), e(c->d), et_prev(d->b)
    link_face(*this, f1, e_next, e, et_prev);

    return e;
}

// init_super_triangle: one finite triangle abc + infinite vertex + 3 infinite faces.
size_t DCEL::init_super_triangle(Point2D a, Point2D b, Point2D c) {
    size_t v_inf = new_vertex({0, 0}, true);
    infinite_vertex = v_inf;
    size_t va = new_vertex(a);
    size_t vb = new_vertex(b);
    size_t vc = new_vertex(c);

    // Inner half-edges of finite triangle (CCW: a->b->c->a)
    size_t e0 = new_edge(); // e0: a->b,  e0+1: b->a
    size_t e1 = new_edge(); // e1: b->c,  e1+1: c->b
    size_t e2 = new_edge(); // e2: c->a,  e2+1: a->c

    edges[e0].origin   = va; edges[e0+1].origin = vb;
    edges[e1].origin   = vb; edges[e1+1].origin = vc;
    edges[e2].origin   = vc; edges[e2+1].origin = va;

    size_t f0 = new_face(false);
    link_face(*this, f0, e0, e1, e2);

    vertices[va].edge = e0;
    vertices[vb].edge = e1;
    vertices[vc].edge = e2;

    // Three infinite faces, each sharing one outer half-edge with the finite triangle.
    // fi_ab: loop: ab0(inf->b), e0+1(b->a), ab1(a->inf)
    // fi_bc: loop: bc0(inf->c), e1+1(c->b), bc1(b->inf)
    // fi_ca: loop: ca0(inf->a), e2+1(a->c), ca1(c->inf)

    // Edges for fi_ab
    size_t ab0 = new_edge(); // ab0: v_inf->b,  ab0+1: b->v_inf
    size_t ab1 = new_edge(); // ab1: a->v_inf,  ab1+1: v_inf->a
    edges[ab0].origin   = v_inf; edges[ab0+1].origin = vb;
    edges[ab1].origin   = va;    edges[ab1+1].origin = v_inf;

    size_t fi_ab = new_face(true);
    link_face(*this, fi_ab, ab0, e0+1, ab1);

    // Edges for fi_bc
    size_t bc0 = new_edge(); // bc0: v_inf->c,  bc0+1: c->v_inf
    size_t bc1 = new_edge(); // bc1: b->v_inf,  bc1+1: v_inf->b
    edges[bc0].origin   = v_inf; edges[bc0+1].origin = vc;
    edges[bc1].origin   = vb;    edges[bc1+1].origin = v_inf;

    size_t fi_bc = new_face(true);
    link_face(*this, fi_bc, bc0, e1+1, bc1);

    // Edges for fi_ca
    size_t ca0 = new_edge(); // ca0: v_inf->a,  ca0+1: a->v_inf
    size_t ca1 = new_edge(); // ca1: c->v_inf,  ca1+1: v_inf->c
    edges[ca0].origin   = v_inf; edges[ca0+1].origin = va;
    edges[ca1].origin   = vc;    edges[ca1+1].origin = v_inf;

    size_t fi_ca = new_face(true);
    link_face(*this, fi_ca, ca0, e2+1, ca1);

    // Twin connections between the three infinite faces (rays to/from v_inf):
    //   ray inf<->b: ab0(inf->b) <-> bc1(b->inf)
    //                bc1+1(inf->b) <-> ab0+1(b->inf)
    //   ray inf<->c: bc0(inf->c) <-> ca1(c->inf)
    //                ca1+1(inf->c) <-> bc0+1(c->inf)
    //   ray inf<->a: ca0(inf->a) <-> ab1(a->inf)
    //                ab1+1(inf->a) <-> ca0+1(a->inf)

    edges[ab0].twin   = bc1;   edges[bc1].twin   = ab0;
    edges[ab0+1].twin = bc1+1; edges[bc1+1].twin = ab0+1;
    edges[bc0].twin   = ca1;   edges[ca1].twin   = bc0;
    edges[bc0+1].twin = ca1+1; edges[ca1+1].twin = bc0+1;
    edges[ca0].twin   = ab1;   edges[ab1].twin   = ca0;
    edges[ca0+1].twin = ab1+1; edges[ab1+1].twin = ca0+1;

    vertices[v_inf].edge = ab0;

    return f0;
}