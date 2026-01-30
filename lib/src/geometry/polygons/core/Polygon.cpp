#include "Polygon.h"

/**
 * Создается массив, и затем он заполняется углами, после этого по тригонаметрическим формулам
 * поучаем поординаты для каждой точки.
 */
Polygon::Polygon(int count_vertices) 
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> angleDist(0, 2 * M_PI);

    std::vector<double> angles(count_vertices);

    for (int i = 0; i < count_vertices; ++i) 
        angles[i] = angleDist(gen);
    
    std::sort(angles.begin(), angles.end());                                    

    std::uniform_real_distribution<> radDist(0, 2 * count_vertices);

    for (double a : angles) {
        double r = radDist(gen);              
        vertices.push_back({r * std::cos(a), r * std::sin(a)});
    }
}

void Polygon::Regenerate() 
{
    Polygon temp_polygon(vertices.size()); 
    *this = std::move(temp_polygon); 
}

bool Polygon::inPolygon(Point2D point) 
{
    return PointInPn::InAllPn(point, vertices.begin(), vertices.end());
}