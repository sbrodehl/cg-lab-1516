#ifndef CG_LAB_1516_TRIANGULATIONALGORITHM_H
#define CG_LAB_1516_TRIANGULATIONALGORITHM_H

#include <QVector2D>
#include <set>

#include <ParameterTriangle.h>
#include "Vertex.h"
#include "Edge.h"
#include "Line.h"

class TriangulationAlgorithm {
private:
    bool canSee(Vertex* v, Vertex* next, Vertex* vertex, std::vector<Vertex*> left);
    bool leftTurn(QVector2D a, QVector2D b, QVector2D c);
    bool contains(std::vector<QVector2D> vec, QVector2D v);

    void buildVertices();
    void makeMonotone();
    void createPolygons();
    void triangulateMonotone(std::vector<Vertex *> polygon);
    std::vector<ParameterTriangle *> createParameterTriangles(std::vector<std::vector<Vertex *>> polygons);

    void handleVertex(Vertex *p);
    void handleStart(Vertex *p);
    void handleEnd(Vertex *p);
    void handleSplit(Vertex *p);
    void handleMerge(Vertex *p);
    void handleRegular(Vertex *p);

    struct yPriority {
        bool operator()(Vertex *a, Vertex *b) {
            if (a->y() < b->y())
                return true;
            if (a->y() > b->y())
                return false;
            if (a->x() > b->x())
                return true;
            if (a->x() < b->x())
                return false;
            return false;
        }
    };

    struct yPriorityLine {
        bool operator()(Line *a, Line *b) {
            QVector2D *p1 = a->getv1()->y() > a->getv2()->y() ? a->getv1() : a->getv2();
            QVector2D *p2 = b->getv1()->y() > b->getv2()->y() ? b->getv1() : b->getv2();
            return p1->x() > p2->x();
        }
    };

    template<typename T>
    int getIndex(std::vector<T *> vec, T *item);

    std::set<Line *, yPriorityLine> SL;
    std::map<Line *, Vertex *> helpers;

    std::vector<ParameterTriangle *> delaunay(std::vector<ParameterTriangle *> triangles);

public:
    std::vector<QVector2D *> points;
    std::vector<Line *> lines;
    std::vector<Vertex *> vertices;
    std::map<Vertex *, std::vector<Edge *>> graph;
    std::vector<std::vector<Vertex *>> polygons;

    std::vector<ParameterTriangle *> triangulate(std::vector<std::vector<QVector2D *>> polygon);
};

#endif //CG_LAB_1516_TRIANGULATIONALGORITHM_H
