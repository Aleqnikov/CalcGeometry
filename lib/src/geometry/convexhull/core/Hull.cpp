#include "Hull.h"

#include <random>


Hull::Hull(int count_dots)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<> dist(0, count_dots);

    for(int i = 0; i < count_dots; i++)
        hull_.push_back(Point2D(dist(gen), dist(gen)));
};

std::vector<Point2D> Hull::ConvexJarvis()
{
    return HullAlgs::JarvisMarch(hull_.begin(), hull_.end());
}

std::vector<Point2D> Hull::ConvexAngrews()
{
    return HullAlgs::Andrews(hull_.begin(), hull_.end());
}

void Hull::Regenerate()
{
    Hull temp_hull(hull_.size()); 

    *this = std::move(temp_hull); 
}

