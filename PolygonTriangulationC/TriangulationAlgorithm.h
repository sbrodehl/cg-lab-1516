#ifndef CG_LAB_1516_TRIANGULATIONALGORITHM_H
#define CG_LAB_1516_TRIANGULATIONALGORITHM_H

#include <QVector2D>
#include <ParameterTriangle.h>
#include <set>
#include "Vertex.h"
#include "Edge.h"
#include "Line.h"

class TriangulationAlgorithm {
private:
    bool canSee(QVector2D v, QVector2D next, QVector2D vertex, std::vector<QVector2D> left);

    bool leftTurn(QVector2D a, QVector2D b, QVector2D c);

    bool contains(std::vector<QVector2D> vec, QVector2D v);

    void buildVertices();

    void makeMonotone();

    struct yPriority;

    struct yPriorityLine;

    void handleVertex(Vertex* p, std::set<Line*, yPriorityLine> SL, std::map<Line*, Vertex*> helpers);

    void handleStart(Vertex* p, std::set<Line*, yPriorityLine> SL, std::map<Line*, Vertex*> helpers);

    void handleEnd(Vertex* p, std::set<Line*, yPriorityLine> SL, std::map<Line*, Vertex*> helpers);

    void handleSplit(Vertex* p, std::set<Line*, yPriorityLine> SL, std::map<Line*, Vertex*> helpers);

    void handleMerge(Vertex* p, std::set<Line*, yPriorityLine> SL, std::map<Line*, Vertex*> helpers);

    void handleRegular(Vertex* p, std::set<Line*, yPriorityLine> SL, std::map<Line*, Vertex*> helpers);

    template <typename T> int getIndex(std::vector<T*> vec, T* item);

public:
    std::vector<QVector2D*> points;
    std::vector<Line*> lines;
    std::vector<Vertex*> vertices;
    std::map<Vertex*, std::vector<Edge*>> graph;
    std::vector<std::vector<Vertex*>> polygons;

    std::vector<ParameterTriangle *> triangulate(std::vector<std::vector<QVector2D*>> polygon_in);
};

#endif //CG_LAB_1516_TRIANGULATIONALGORITHM_H
