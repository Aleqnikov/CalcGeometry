#include "StarPn.h"

StarPolygon::StarPolygon(int cnt_verts)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	double R = 2.0 * cnt_verts; 
	double r = 0.5 * R;            

	std::uniform_real_distribution<> noise(-0.1 * R, 0.1 * R);

	for(int i = 0; i < cnt_verts; ++i) {
		double angle = 2.0 * M_PI * i / cnt_verts;
		double radius = (i % 2 == 0) ? R : r;
		radius += noise(gen); 

		vertices.push_back({
			radius * std::cos(angle),
			radius * std::sin(angle)
		});
	}
}


void StarPolygon::Regenerate()
{
    StarPolygon temp_polygon(vertices.size()); 
    *this = std::move(temp_polygon);
}


bool StarPolygon::inPolygon(Point2D point) 
{
    return PointInPn::InStarPn(point, Point2D(0, 0),  vertices.begin(), vertices.end());
}