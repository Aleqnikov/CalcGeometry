#include "ConvexPn.h"

ConvexPolygon::ConvexPolygon(int cnt_verts)
{
    std::set<int> vertices_uniq;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> dist(0, 2*cnt_verts - 1);

    while(vertices_uniq.size() != cnt_verts)
        vertices_uniq.insert(dist(gen));

    for(auto angle : vertices_uniq)
        vertices.push_back(Point2D(
            cnt_verts*std::cos(2* M_PI * angle / (2*cnt_verts )),
            cnt_verts*std::sin(2* M_PI * angle / (2*cnt_verts )))
    );

    // Находим точку, которая точно принадлежить многоугольнику.
    centroid = {0, 0};

    for(const auto& v : vertices) {
        centroid.x += v.x;
        centroid.y += v.y;
    }
    centroid.x /= vertices.size();
    centroid.y /= vertices.size();
}



void ConvexPolygon::Regenerate()
{
    ConvexPolygon temp_polygon(vertices.size()); 
    *this = std::move(temp_polygon);
}


bool ConvexPolygon::inPolygon(Point2D point) 
{
    return PointInPn::InStarPn(point, centroid,  vertices.begin(), vertices.end());
}