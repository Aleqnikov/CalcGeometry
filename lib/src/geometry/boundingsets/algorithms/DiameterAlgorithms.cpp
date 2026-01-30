#include "DiameterAlgorithms.h"

namespace DiameterAlgorithms {

    std::pair<Point2D, Point2D> NaiveAlgorithm(
        std::vector<Point2D>::const_iterator begin,
        std::vector<Point2D>::const_iterator end)
    {
        int n = std::distance(begin, end);
        if (n < 2) {
            if (n == 1) return {*begin, *begin};
            return {Point2D(), Point2D()};
        }

        double maxDist = 0;
        Point2D p1 = *begin;
        Point2D p2 = *begin;

        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                double dist = LinAl::distance(begin[i], begin[j]);
                if (dist > maxDist) {
                    maxDist = dist;
                    p1 = begin[i];
                    p2 = begin[j];
                }
            }
        }

        return {p1, p2};
    }
    double signedAreaPerp(const Point2D& a, const Point2D& b, const Point2D& c) 
    {
        Point2D ab = b - a;
        Point2D ac = c - a;
        return ab.x * ac.x + ab.y * ac.y;
    }

    double signedArea(const Point2D& a, const Point2D& b, const Point2D& c) 
    {
        return LinAl::pscalar(b - a, c - a);
    }


    int furthest(const std::vector<Point2D>& hull, int start, int a, int b) 
    {
        int n = hull.size();
        int maxIdx = start;
        double maxDist = std::abs(signedArea(hull[a], hull[b], hull[start]));

        for (int i = 0; i < n; i++) {
            int idx = (start + i) % n;
            double dist = std::abs(signedArea(hull[a], hull[b], hull[idx]));
            if (dist > maxDist) {
                maxDist = dist;
                maxIdx = idx;
            }
        }

        return maxIdx;
    }


    std::pair<Point2D, Point2D> RotatingCalipers(
        std::vector<Point2D>::const_iterator begin,
        std::vector<Point2D>::const_iterator end)
    {
        std::vector<Point2D> hull(begin, end);
        int n = hull.size();

        if (n < 2) {
            if (n == 1) return {hull[0], hull[0]};
            return {Point2D(), Point2D()};
        }

        if (n == 2) return {hull[0], hull[1]};

        double maxDist = 0;
        Point2D p1 = hull[0];
        Point2D p2 = hull[1];


        int bottom = 0, top = 0;
        for (int i = 0; i < n; i++) {
            if (hull[i].y < hull[bottom].y || 
                (hull[i].y == hull[bottom].y && hull[i].x < hull[bottom].x))
                bottom = i;
            if (hull[i].y > hull[top].y || 
                (hull[i].y == hull[top].y && hull[i].x > hull[top].x))
                top = i;
        }

        int i = bottom;
        int j = top;

        for (int k = 0; k < n; k++) {
            double dist = LinAl::distance(hull[i], hull[j]);
            if (dist > maxDist) {
                maxDist = dist;
                p1 = hull[i];
                p2 = hull[j];
            }

            Point2D edge_i = hull[(i + 1) % n] - hull[i];
            Point2D edge_j = hull[(j + 1) % n] - hull[j];


            double cross = LinAl::pscalar(edge_i, edge_j);

            if (cross < 0) {
                i = (i + 1) % n;
            } else if (cross > 0) {
                j = (j + 1) % n;
            } else {
                i = (i + 1) % n;
                j = (j + 1) % n;
            }
        }

        return {p1, p2};
    }

    std::pair<Point2D, Point2D> DiameterWithHull(
        std::vector<Point2D>::const_iterator begin,
        std::vector<Point2D>::const_iterator end)
    {
        std::vector<Point2D> hull = HullAlgs::Andrews(begin, end);
        
        return RotatingCalipers(hull.begin(), hull.end());
    }

}