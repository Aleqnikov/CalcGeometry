#ifndef HULL_H
#define HULL_H


/**
 * @brief Данный класс является реализацией множества точек, есть методы которые позволяют получить 
 * выпуклую оболочку, при помощи одного из двух алгоритмов.
 */

#include "../algorithms/HullAlgorithms.h"

class Hull {

public:
    std::vector<Point2D> hull_;


    Hull(int);
    std::vector<Point2D> ConvexJarvis();
    std::vector<Point2D> ConvexAngrews();
    void Regenerate();
};



#endif //HULL_H
