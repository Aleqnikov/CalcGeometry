#include "TriangulationAlgorithms.h"

namespace Triangulation {

    struct VertexNode {
        Point2D p;
        VertexNode* prev;
        VertexNode* next;
        bool isConcave;
        bool isEar;

        std::list<VertexNode*>::iterator concaveIt;
        std::list<VertexNode*>::iterator earIt;
    };

    bool isConcavePoint(const Point2D &p1, const Point2D &p2, const Point2D &p3)
    {
        return LinAl::orientation(p1, p2, p3) != LinAl::Orient::left;
    }

    bool isEar(VertexNode* v, const std::list<VertexNode*> &concaveList)
    {
        const Point2D &pPrev = v->prev->p;
        const Point2D &pCurr = v->p;
        const Point2D &pNext = v->next->p;

        if (isConcavePoint(pPrev, pCurr, pNext))
            return false;

        for (auto concaveVertex : concaveList) {
            if (concaveVertex == v || concaveVertex == v->prev || concaveVertex == v->next) 
                continue;

            if (LinAl::isPointInTriangle(concaveVertex->p, pPrev, pCurr, pNext))
                return false;
        }

        return true;
    }

    void removeFromConcaveList(VertexNode* v, std::list<VertexNode*> &concaveList) 
    {
        if (v->isConcave) {
            concaveList.erase(v->concaveIt);
            v->isConcave = false;
        }
    }

    void removeFromEarList(VertexNode* v, std::list<VertexNode*> &earList) 
    {
        if (v->isEar) {
            earList.erase(v->earIt);
            v->isEar = false;
        }
    }

    void updateVertexStatus(VertexNode* v,
                           std::list<VertexNode*> &concaveList,
                           std::list<VertexNode*> &earList)
    {
        removeFromConcaveList(v, concaveList);
        removeFromEarList(v, earList);

        bool wasConcave = v->isConcave;
        v->isConcave = isConcavePoint(v->prev->p, v->p, v->next->p);
        
        if (v->isConcave) {
            concaveList.push_back(v);
            v->concaveIt = std::prev(concaveList.end());
        }

        v->isEar = isEar(v, concaveList);
        
        if (v->isEar) {
            earList.push_back(v);
            v->earIt = std::prev(earList.end());
        }
    }

    std::vector<Triangle> EarClipping(std::vector<Point2D> polygon) 
    {
        std::vector<Triangle> res;

        if (polygon.size() < 3) return res;
        if (polygon.size() == 3) {
            res.emplace_back(polygon[0], polygon[1], polygon[2]);
            return res;
        }

        std::vector<VertexNode> vertices(polygon.size());
        std::list<VertexNode*> vertexList;
        std::list<VertexNode*> concaveList;
        std::list<VertexNode*> earList;

        for (size_t i = 0; i < polygon.size(); ++i) {
            vertices[i].p = polygon[i];
            vertices[i].prev = &vertices[(i + polygon.size() - 1) % polygon.size()];
            vertices[i].next = &vertices[(i + 1) % polygon.size()];
            vertices[i].isConcave = false;
            vertices[i].isEar = false;
            vertexList.push_back(&vertices[i]);
        }

        for (auto v : vertexList) {
            v->isConcave = isConcavePoint(v->prev->p, v->p, v->next->p);
            if (v->isConcave) {
                concaveList.push_back(v);
                v->concaveIt = std::prev(concaveList.end());
            }
        }

        for (auto v : vertexList) {
            v->isEar = isEar(v, concaveList);
            if (v->isEar) {
                earList.push_back(v);
                v->earIt = std::prev(earList.end());
            }
        }

        while (vertexList.size() > 3) {
            if (earList.empty()) {
                break;
            }

            VertexNode* ear = earList.front();
            VertexNode* prevVertex = ear->prev;
            VertexNode* nextVertex = ear->next;

            res.emplace_back(prevVertex->p, ear->p, nextVertex->p);

            removeFromEarList(ear, earList);
            removeFromConcaveList(ear, concaveList);
            
            auto it = std::find(vertexList.begin(), vertexList.end(), ear);
            if (it != vertexList.end()) {
                vertexList.erase(it);
            }

            prevVertex->next = nextVertex;
            nextVertex->prev = prevVertex;

            if (vertexList.size() == 3) {
                break;
            }

            updateVertexStatus(prevVertex, concaveList, earList);
            updateVertexStatus(nextVertex, concaveList, earList);
        }

        if (vertexList.size() == 3) {
            auto it = vertexList.begin();
            VertexNode* v1 = *it++;
            VertexNode* v2 = *it++;
            VertexNode* v3 = *it;
            res.emplace_back(v1->p, v2->p, v3->p);
        }

        return res;
    }
}