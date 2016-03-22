#include <iostream>
#include <queue>

#include <QVector3D>
#include <QPair>

#include "TriangulationAlgorithm.h"

bool TriangulationAlgorithm::canSee(QVector2D v, QVector2D next, QVector2D vertex, std::vector<QVector2D> left) {
    if (contains(left, v)) {
        return leftTurn(next, v, vertex);
    } else {
        return leftTurn(v, next, vertex);
    }
}

bool TriangulationAlgorithm::leftTurn(QVector2D a, QVector2D b, QVector2D c) {
    double ux = b.x() - a.x();
    double uy = b.y() - a.y();
    double vx = c.x() - a.x();
    double vy = c.y() - a.y();
    return (ux * vy - uy * vx >= 0 || std::abs(ux * vy - uy * vx) < pow(10, -10));
}

bool TriangulationAlgorithm::contains(std::vector<QVector2D> vec, QVector2D v) {
    return std::find(vec.begin(), vec.end(), v) != vec.end();
}

std::vector<ParameterTriangle *> TriangulationAlgorithm::triangulate(
        std::vector<std::vector<QVector2D *>> polygon_in) {
    for (std::vector<QVector2D *> pointVector : polygon_in) {
        points.insert(points.end(), pointVector.begin(), pointVector.end());
        for (int i = 0; i < pointVector.size(); i++) {
            lines.push_back(new Line(pointVector[i], pointVector[(i + 1) % pointVector.size()]));
            vertices.push_back(new Vertex(*pointVector[i]));
        }
    }

    buildVertices();
    makeMonotone();
    // createPolygons();
    for (std::vector<Vertex *> polygon : polygons) {
        // triangulateMonotone(polygon);
    }
    return std::vector<ParameterTriangle *>();
}

template<typename T>
int TriangulationAlgorithm::getIndex(std::vector<T *> vec, T *item) {
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] == item) {
            return i;
        }
    }
    return -1;
}

void TriangulationAlgorithm::buildVertices() {
    for (Line *line : lines) {
        Vertex *v1 = vertices[getIndex(points, line->getv1())];
        Vertex *v2 = vertices[getIndex(points, line->getv2())];

        v1->setNeighbor(1, v2);
        v2->setNeighbor(0, v1);

        graph[v1] = std::vector<Edge *>();
        graph[v1].push_back(new Edge(v1, v2));
    }
    for (Vertex *v : vertices) {
        v->setType();
    }
}

void TriangulationAlgorithm::makeMonotone() {
    std::priority_queue<Vertex *, std::vector<Vertex *>, yPriority> pq;

    for (Vertex *v : vertices) {
        pq.push(v);
    }

    while (!pq.empty()) {
        Vertex *p = pq.top();
        pq.pop();
        handleVertex(p);
    }
}

void TriangulationAlgorithm::handleVertex(Vertex *p) {
    if (p->getType() == Vertex::START) {
        handleStart(p);
    }
    if (p->getType() == Vertex::SPLIT) {
        handleSplit(p);
    }
    if (p->getType() == Vertex::END) {
        handleEnd(p);
    }
    if (p->getType() == Vertex::MERGE) {
        handleMerge(p);
    }
    if (p->getType() == Vertex::REGULAR) {
        handleRegular(p);
    }
}

void TriangulationAlgorithm::handleStart(Vertex *p) {
    int idx = getIndex(vertices, p);
    SL.insert(lines[idx]);
    helpers[lines[idx]] = p;
}

void TriangulationAlgorithm::handleEnd(Vertex *p) {
    int idx = getIndex(vertices, p->getNeighbor(0));
    Line *e = lines[idx];

    Vertex *p2 = helpers[e];
    if (p2->getType() == Vertex::MERGE) {
        lines.push_back(new Line(p->getPoint(), p2->getPoint()));
        graph[p].push_back(new Edge(p, p2));
        graph[p2].push_back(new Edge(p2, p));
    }
    SL.erase(e);
}

void TriangulationAlgorithm::handleSplit(Vertex *p) {
    std::set<Line *>::iterator it = SL.lower_bound(new Line(p->getPoint(), p->getPoint()));
    it--;
    Line *e = *it;
    Vertex *p2 = helpers[e];
    lines.push_back(new Line(p->getPoint(), p2->getPoint()));
    graph[p].push_back(new Edge(p, p2));
    graph[p2].push_back(new Edge(p2, p));

    helpers[e] = p;

    int idx = getIndex(vertices, p);
    SL.insert(lines[idx]);
    helpers[lines[idx]] = p;
}

void TriangulationAlgorithm::handleMerge(Vertex *p) {
    int idx = getIndex(vertices, p->getNeighbor(0));
    Line *e = lines[idx];

    Vertex *p2 = helpers[e];
    if (p2->getType() == Vertex::MERGE) {
        lines.push_back(new Line(p->getPoint(), p2->getPoint()));
        graph[p].push_back(new Edge(p, p2));
        graph[p2].push_back(new Edge(p2, p));
    }
    SL.erase(e);

    std::set<Line *>::iterator it = SL.lower_bound(new Line(p->getPoint(), p->getPoint()));
    it--;
    Line *e2 = *it;
    Vertex *p3 = helpers[e2];

    if (p3->getType() == Vertex::MERGE) {
        lines.push_back(new Line(p->getPoint(), p3->getPoint()));
        graph[p].push_back(new Edge(p, p3));
        graph[p3].push_back(new Edge(p3, p));
    }
    helpers[e2] = p;
}

void TriangulationAlgorithm::handleRegular(Vertex *p) {
    if (p->getNeighbor(0)->y() > p->y()) {
        int idx = getIndex(vertices, p->getNeighbor(0));
        Line *e = lines[idx];

        Vertex *p2 = helpers[e];
        if (p2->getType() == Vertex::MERGE) {
            lines.push_back(new Line(p->getPoint(), p2->getPoint()));
            graph[p].push_back(new Edge(p, p2));
            graph[p2].push_back(new Edge(p2, p));
        }
        SL.erase(e);
        Line *e1 = lines[getIndex(vertices, p)];
        SL.insert(e1);
        helpers[e1] = p;
    } else {
        std::set<Line *>::iterator it = SL.lower_bound(new Line(p->getPoint(), p->getPoint()));
        it--;
        Line *e = *it;
        Vertex *p2 = helpers[e];

        if (p2->getType() == Vertex::MERGE) {
            lines.push_back(new Line(p->getPoint(), p2->getPoint()));
            graph[p].push_back(new Edge(p, p2));
            graph[p2].push_back(new Edge(p2, p));
        }
        helpers[e] = p;
    }
}