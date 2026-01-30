#ifndef TRIANGULATION_ALGORITHMS_H
#define TRIANGULATION_ALGORITHMS_H

/**
 * @brief В данном файле находится алгоритм тринагуляции earclipping

 */

#include <vector>
#include <list>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

#include "../../../linalgebra/LinAl.h"

struct Triangle {
    Point2D p1, p2, p3;
    Triangle(Point2D a, Point2D b, Point2D c) : p1(a), p2(b), p3(c) {}
};

namespace Triangulation {

    std::vector<Triangle> EarClipping(std::vector<Point2D> polygon);

    bool isConcavePoint(const Point2D &p1, const Point2D &p2, const Point2D &p3);
    bool isConcave(std::list<Point2D>::iterator it_curr, const std::list<Point2D> &vertex);
    bool isEar(int vid, const std::list<Point2D> &vertexList, 
               const std::unordered_map<int, std::list<Point2D>::const_iterator> &idToIt,
               const std::unordered_set<int> &concaveSet);


    std::list<Point2D>::iterator getPrev(std::list<Point2D>::iterator it_curr, std::list<Point2D> &vertex);
    std::list<Point2D>::iterator getNext(std::list<Point2D>::iterator it_curr, std::list<Point2D> &vertex);

} 

#endif // TRIANGULATION_ALGORITHMS_H
