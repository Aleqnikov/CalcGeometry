#include "point_in_polygon.h"


bool PointInPn::InAllPn(Point2D point,
                            std::vector<Point2D>::const_iterator start,
                            std::vector<Point2D>::const_iterator end) 
{

    int intersect_cnt = 0;
    int num_vert = std::distance(start, end);

    for (int i = 0; i < num_vert; ++i) {
        const Point2D& vertex_a = start[i];
        const Point2D& vertex_b = start[(i + 1) % num_vert];

        if (LinAl::cross_rib(vertex_a, vertex_b, point)) 
            intersect_cnt++;

    }

    return intersect_cnt % 2 == 1;
}


int findSector(const Point2D& q,
			   const std::vector<Point2D>& poly,
			   const Point2D& z)
{
	int n = poly.size();
	if (n < 3) return -1;

	int s = 0;          
	int e = n;          

	while (e - s > 1)
	{
		int m = (s + e) / 2;

		bool midLeft = (LinAl::orientation(q, poly[s], poly[m]) == LinAl::Orient::left);
		bool zPsRight = (LinAl::orientation(q, poly[s], z) == LinAl::Orient::right);
		bool zPmRight = (LinAl::orientation(q, poly[m], z) == LinAl::Orient::right);

		if (midLeft)
		{
			if (!zPsRight && zPmRight) {
				e = m;
			} else {
				s = m;
			}
		} else {
			if (zPsRight && !zPmRight) {
				s = m;
			} else {
				e = m;
			}
		}
	}

	return s % n;
}



bool PointInPn::InStarPn( Point2D z, 
                            Point2D center,
						    std::vector<Point2D>::const_iterator start,
						    std::vector<Point2D>::const_iterator end)
{
	std::vector<Point2D> poly(start, end);

	if (poly.size() < 3) return false;

	int i = findSector(center, poly, z);

	if (i < 0) 
        return false;

	int j = (i + 1) % poly.size();
	auto o = LinAl::orientation(poly[i], poly[j], z);

	return o == LinAl::Orient::left ||
		   o == LinAl::Orient::colin;  
} 