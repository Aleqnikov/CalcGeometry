#pragma once
#include <vector>
#include <optional>
#include "DCEL.h"

struct VoronoiEdge {
    Point2D                p0;
    std::optional<Point2D> p1;
    std::optional<Point2D> direction;
};

struct VoronoiDiagram {
    std::vector<VoronoiEdge> edges;
};

VoronoiDiagram voronoi_from_delaunay(const DCEL& dcel);