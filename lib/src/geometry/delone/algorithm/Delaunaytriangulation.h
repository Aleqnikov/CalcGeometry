#ifndef DELAUNAY_TRIANGULATION_H
#define DELAUNAY_TRIANGULATION_H

/**
 * @brief Триангуляция Делоне — randomized incremental algorithm.
 *
 * Реализация строго по псевдокоду лекции 7.
 *
 * Алгоритм:
 *   1. Суперструктура: бесконечная вершина + большой треугольник.
 *   2. Для каждой точки в случайном порядке:
 *      a. locate(p) — локализация в Delaunay Tree.
 *      b. split_face — разбиение грани на 3.
 *      c. for e in outgoing_edges(v): validate_edge(e.next_edge)
 *   3. validate_edge: flip + два рекурсивных вызова.
 */

#include <vector>
#include <array>
#include "DCEL.h"
#include "DelaunayTree.h"
#include "LinAl.h"

class DelaunayTriangulation {
public:
	DelaunayTriangulation();
	const DCEL& dcel() const { return dcel_; }
	void insert(Point2D p);

	std::vector<std::array<Point2D, 3>> get_triangles() const;

	static std::vector<std::array<Point2D, 3>>
	triangulate(std::vector<Point2D> points);

private:
	DCEL         dcel_;
	DelaunayTree tree_;

	static bool point_in_circumcircle(Point2D a, Point2D b, Point2D c, Point2D p);
	bool        is_invalid_edge(size_t e) const;
	void        validate_edge(size_t e);
};

#endif // DELAUNAY_TRIANGULATION_H