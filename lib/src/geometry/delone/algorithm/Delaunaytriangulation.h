#ifndef DELAUNAY_TRIANGULATION_H
#define DELAUNAY_TRIANGULATION_H

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