#ifndef DCEL_H
#define DCEL_H

/**
 * Каждое полуребро (half-edge) хранит:
 *   - индекс вершины-источника
 *   - индекс грани (face)
 *   - twin, prev, next — индексы смежных полурёбер
 *
 * Индекс INVALID (SIZE_MAX) означает "нет объекта".
 * Бесконечная вершина помечается флагом infinite = true.
 */

#include <vector>
#include <cstddef>
#include <climits>
#include <cstdint>
#include "Point2D.h"

static const size_t INVALID = SIZE_MAX;

class DCEL {
public:

    size_t new_vertex(Point2D pos, bool infinite = false);
    size_t new_edge();
    size_t new_face(bool infinite = false);


    bool is_infinite_vertex(size_t v) const;
    bool is_infinite_edge(size_t e)   const;
    bool is_infinite_face(size_t f)   const;

    /// Вершины грани (против часовой стрелки)
    void face_vertices(size_t f, size_t& v0, size_t& v1, size_t& v2) const;

    size_t  vertex_count()        const { return vertices_.size(); }
    Point2D vertex_pos(size_t v)  const { return vertices_[v].pos;  }
    size_t  vertex_edge(size_t v) const { return vertices_[v].edge; }


    size_t edge_count()           const { return edges_.size();       }
    size_t edge_origin(size_t e)  const { return edges_[e].origin;    }
    size_t edge_twin(size_t e)    const { return edges_[e].twin;      }
    size_t edge_face(size_t e)    const { return edges_[e].face;      }
    size_t edge_prev(size_t e)    const { return edges_[e].prev;      }
    size_t edge_next(size_t e)    const { return edges_[e].next;      }


    size_t face_count()           const { return faces_.size();       }
    size_t face_edge(size_t f)    const { return faces_[f].edge;      }
    size_t face_node(size_t f)    const { return faces_[f].node;      }
    void   set_face_node(size_t f, size_t node) { faces_[f].node = node; }


    size_t infinite_vertex_idx()  const { return infinite_vertex_;    }

    /**
     * Разбить грань f на три, вставив точку p.
     * Грань может быть как конечной, так и бесконечной:
     * в последнем случае получаются 1 конечный и 2 бесконечных треугольника.
     * Возвращает индекс новой вершины.
     */
    size_t split_face(size_t f, Point2D p);

    /**
     * Разбить ребро e точкой p, лежащей строго на нём.
     * Две смежные грани заменяются четырьмя.
     * Нужно для входов с тремя и более коллинеарными точками
     * (сетки, точки на границе оболочки).
     * Возвращает индекс новой вершины.
     */
    size_t split_edge(size_t e, Point2D p);

    /**
     * Повернуть ребро e (flip).
     * e и его twin должны быть рёбрами двух смежных треугольников.
     * Возвращает индекс ребра e (после flip).
     */
    size_t flip_edge(size_t e);

    /**
     * Создать начальную структуру:
     *   - бесконечная (проективная) вершина v_inf
     *   - три конечных вершины a, b, c (обход против часовой стрелки)
     *   - один конечный треугольник abc
     *   - три «бесконечных» треугольника, примыкающих к abc снаружи
     * Возвращает индекс конечного треугольника.
     */
    size_t init_super_triangle(Point2D a, Point2D b, Point2D c);

private:

    struct Vertex {
        Point2D pos;
        size_t  edge     = INVALID;
        bool    infinite = false;
    };

    struct HalfEdge {
        size_t origin = INVALID;
        size_t twin   = INVALID;
        size_t face   = INVALID;
        size_t prev   = INVALID;
        size_t next   = INVALID;
    };

    struct Face {
        size_t edge     = INVALID;  ///< одно из инцидентных полурёбер
        size_t node     = INVALID;  ///< узел DelaunayTree
        bool   infinite = false;    ///< не используется: см. is_infinite_face
    };

    size_t infinite_vertex_ = INVALID;

    std::vector<Vertex>   vertices_;
    std::vector<HalfEdge> edges_;
    std::vector<Face>     faces_;


    /// Связать три полуребра ea→eb→ec→ea в грань f.
    void link_face(size_t f, size_t ea, size_t eb, size_t ec);
};

#endif // DCEL_H