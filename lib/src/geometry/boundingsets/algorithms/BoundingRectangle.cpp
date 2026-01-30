#include "BoundingRectangle.h"
#include <limits>
#include <cmath>
#include <algorithm>
#include <vector>

namespace BoundingRectangle {

Rectangle RotatingCalipers(
    std::vector<Point2D>::const_iterator begin,
    std::vector<Point2D>::const_iterator end) 
{

    std::vector<Point2D> hull = HullAlgs::Andrews(begin, end);
    
    Rectangle bestRect;
    bestRect.area = std::numeric_limits<double>::infinity();
    bestRect.perimeter = std::numeric_limits<double>::infinity();

    int n = static_cast<int>(hull.size());
    if (n < 2) return bestRect;

    const double eps = 1e-9;
    
    int topIdx = 0, rightIdx = 0, bottomIdx = 0, leftIdx = 0;
    bool initialized = false;

    for (int i = 0; i < n; ++i) {
        Point2D pCurrent = hull[i];
        Point2D pNext = hull[(i + 1) % n];
        Point2D edge = pNext - pCurrent;

        double edgeLenSq = edge.x * edge.x + edge.y * edge.y;
        if (edgeLenSq < eps) continue;
        double edgeLen = std::sqrt(edgeLenSq);

        Point2D u(edge.x / edgeLen, edge.y / edgeLen); 
        Point2D v(-u.y, u.x);                          

        if (!initialized) {
            double maxU = -1e18, maxV = -1e18, minU = 1e18, minV = 1e18;
            for (int k = 0; k < n; ++k) {
                double projU = hull[k].x * u.x + hull[k].y * u.y;
                double projV = hull[k].x * v.x + hull[k].y * v.y;
                
                if (projU > maxU) { maxU = projU; rightIdx = k; }
                if (projV > maxV) { maxV = projV; topIdx = k; }
                if (projU < minU) { minU = projU; leftIdx = k; }
                if (projV < minV) { minV = projV; bottomIdx = k; }
            }
            initialized = true;
        } else {
            auto advance = [&](int& idx, const Point2D& direction) {
                while (true) {
                    int next = (idx + 1) % n;
                    double currP = hull[idx].x * direction.x + hull[idx].y * direction.y;
                    double nextP = hull[next].x * direction.x + hull[next].y * direction.y;
                    if (nextP <= currP + eps) break;
                    idx = next;
                }
            };

            advance(rightIdx, u);           
            advance(topIdx, v);             
            advance(leftIdx, Point2D(-u.x, -u.y)); 
            advance(bottomIdx, Point2D(u.y, -u.x)); 
        }

        double minU = hull[leftIdx].x * u.x + hull[leftIdx].y * u.y;
        double maxU = hull[rightIdx].x * u.x + hull[rightIdx].y * u.y;
        double minV = hull[bottomIdx].x * v.x + hull[bottomIdx].y * v.y;
        double maxV = hull[topIdx].x * v.x + hull[topIdx].y * v.y;

        double width = maxU - minU;
        double height = maxV - minV;
        double area = width * height;

        if (area < bestRect.area - eps) {
            bestRect.area = area;
            bestRect.perimeter = 2.0 * (width + height);
            
            bestRect.corners[0] = u * minU + v * minV; 
            bestRect.corners[1] = u * maxU + v * minV;
            bestRect.corners[2] = u * maxU + v * maxV;
            bestRect.corners[3] = u * minU + v * maxV;
        }
    }
    return bestRect;
}

}