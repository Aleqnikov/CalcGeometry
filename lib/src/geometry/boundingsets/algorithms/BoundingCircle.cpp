#include "BoundingCircle.h"

namespace BoundingCircle {

    Circle circleFrom2Points(const Point2D& a, const Point2D& b) 
    {
        Point2D center((a.x + b.x) / 2.0, (a.y + b.y) / 2.0);
        double dx = b.x - a.x;
        double dy = b.y - a.y;
        double radius = std::sqrt(dx * dx + dy * dy) / 2.0;
        return Circle(center, radius);
    }

    Circle circleFrom3Points(const Point2D& a, const Point2D& b, const Point2D& c) 
    {

        double ax = a.x, ay = a.y;
        double bx = b.x, by = b.y;
        double cx = c.x, cy = c.y;

        double d = 2 * (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by));
        
        if (std::abs(d) < 1e-10) {
            return circleFrom2Points(a, b);
        }

        double aSq = ax * ax + ay * ay;
        double bSq = bx * bx + by * by;
        double cSq = cx * cx + cy * cy;

        double ux = (aSq * (by - cy) + bSq * (cy - ay) + cSq * (ay - by)) / d;
        double uy = (aSq * (cx - bx) + bSq * (ax - cx) + cSq * (bx - ax)) / d;

        Point2D center(ux, uy);
        double dx = ax - ux;
        double dy = ay - uy;
        double radius = std::sqrt(dx * dx + dy * dy);

        return Circle(center, radius);
    }

    Circle enclosingCircle(const std::vector<Point2D>& boundary) 
    {
        if (boundary.empty()) {
            return Circle();
        } else if (boundary.size() == 1) {
            return Circle(boundary[0], 0);
        } else if (boundary.size() == 2) {
            return circleFrom2Points(boundary[0], boundary[1]);
        } else {
            return circleFrom3Points(boundary[0], boundary[1], boundary[2]);
        }
    }

    bool isNotObtuseTriangle(const Point2D& a, const Point2D& b, const Point2D& c) 
    {

        double ab2 = LinAl::distance(a, b);
        double bc2 = LinAl::distance(b, c);
        double ca2 = LinAl::distance(c, a);

        return (ab2 + bc2 >= ca2) && (bc2 + ca2 >= ab2) && (ca2 + ab2 >= bc2);
    }


    double circlePredicate(const Point2D& a, const Point2D& b, 
                          const Point2D& c, const Point2D& p) 
    {
        double ax = a.x, ay = a.y;
        double bx = b.x, by = b.y;
        double cx = c.x, cy = c.y;
        double px = p.x, py = p.y;

        double aSq = ax * ax + ay * ay;
        double bSq = bx * bx + by * by;
        double cSq = cx * cx + cy * cy;
        double pSq = px * px + py * py;

        double det = 
            aSq * (bx * cy - by * cx - bx * py + by * px + cx * py - cy * px) +
            ax * (bSq * cy - by * cSq - bSq * py + by * pSq + cSq * py - cy * pSq) +
            ay * (-bSq * cx + bx * cSq + bSq * px - bx * pSq - cSq * px + cx * pSq) +
            bSq * (cx * py - cy * px) +
            bx * (-cSq * py + cy * pSq) +
            by * (cSq * px - cx * pSq);

        return det;
    }


    Circle NaiveAlgorithm(
        std::vector<Point2D>::const_iterator begin,
        std::vector<Point2D>::const_iterator end)
    {
        std::vector<Point2D> points(begin, end);
        int n = points.size();

        if (n == 0) return Circle();
        if (n == 1) return Circle(points[0], 0);
        if (n == 2) return circleFrom2Points(points[0], points[1]);

        Circle minCircle;
        minCircle.radius = std::numeric_limits<double>::max();

        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                Circle c = circleFrom2Points(points[i], points[j]);
                
                bool valid = true;
                for (int k = 0; k < n; k++) {
                    if (!c.contains(points[k])) {
                        valid = false;
                        break;
                    }
                }

                if (valid && c.radius < minCircle.radius) {
                    minCircle = c;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                for (int k = j + 1; k < n; k++) {
                    if (!isNotObtuseTriangle(points[i], points[j], points[k]))
                        continue;

                    Circle c = circleFrom3Points(points[i], points[j], points[k]);
                    
                    bool valid = true;
                    for (int m = 0; m < n; m++) {
                        if (!c.contains(points[m])) {
                            valid = false;
                            break;
                        }
                    }

                    if (valid && c.radius < minCircle.radius) {
                        minCircle = c;
                    }
                }
            }
        }

        return minCircle;
    }


    Circle welzlHelper(std::vector<Point2D>& points, 
                      std::vector<Point2D> boundary, 
                      int n) 
    {

        if (n == 0 || boundary.size() == 3) {
            return enclosingCircle(boundary);
        }


        int idx = rand() % n;
        Point2D p = points[idx];

        std::swap(points[idx], points[n - 1]);

        Circle disc = welzlHelper(points, boundary, n - 1);

        if (disc.contains(p)) {
            return disc;
        }

        boundary.push_back(p);
        return welzlHelper(points, boundary, n - 1);
    }


    Circle WelzlAlgorithm(
        std::vector<Point2D>::const_iterator begin,
        std::vector<Point2D>::const_iterator end)
    {
        std::vector<Point2D> points(begin, end);
        
        if (points.empty()) return Circle();
        if (points.size() == 1) return Circle(points[0], 0);


        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(points.begin(), points.end(), g);

        std::vector<Point2D> boundary;
        return welzlHelper(points, boundary, points.size());
    }

}