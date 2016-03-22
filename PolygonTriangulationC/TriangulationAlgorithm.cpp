#include <iostream>
#include <queue>
#include <stack>
#include <QVector3D>
#include <QPair>
#include <QList>

#include "TriangulationAlgorithm.h"

bool TriangulationAlgorithm::canSee(Vertex* v, Vertex* next, Vertex* vertex, std::vector<Vertex*> left) {
    if (getIndex(left, v) != -1) {
        return leftTurn(*next->getPoint(), *v->getPoint(), *vertex->getPoint());
    } else {
        return leftTurn(*v->getPoint(), *next->getPoint(), *vertex->getPoint());
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
    createPolygons();
    for (std::vector<Vertex *> polygon : polygons) {
        triangulateMonotone(polygon);
    }

    polygons.clear();
    for(std::pair<Vertex*, std::vector<Edge*>> evpair : graph) {
        for(Edge * e : evpair.second){
            e->visited = false;
        }
    }
    createPolygons();
    std::vector<ParameterTriangle *> triangles = createParameterTriangles(polygons);

    return triangles;
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

std::vector<ParameterTriangle *> TriangulationAlgorithm::createParameterTriangles(std::vector<std::vector<Vertex *>> polygons){
    std::vector<ParameterTriangle *> triangles;

    for (std::vector<Vertex *> polygon : polygons) {
        ParameterTriangle* t = new ParameterTriangle(*polygon[0]->getPoint(), *polygon[1]->getPoint(), *polygon[2]->getPoint());
        triangles.push_back(t);
    }

    return triangles;
}

void TriangulationAlgorithm::triangulateMonotone(std::vector<Vertex *> polygon){
    std::priority_queue<Vertex *, std::vector<Vertex *>, yPriority> pq;

    for (Vertex *v : polygon) {
        pq.push(v);
    }

    // get min
    Vertex* min = polygon[0];
    double min_val = polygon[0]->y();

    for(Vertex* v : polygon){
        if(v->y() < min_val){
            min_val = v->y();
            min = v;
        }
        if(v->y() == min_val && v->x() > min->x()){
            min_val = v->y();
            min = v;
        }
    }

    // create paths
    std::vector<Vertex*> left;
    std::vector<Vertex*> right;

    Vertex* max = pq.top();
    Vertex* actual = polygon[(getIndex(polygon, max)+1) % polygon.size()];
    while(actual != min){
        left.push_back(actual);
        actual = polygon[(getIndex(polygon, actual)+1) % polygon.size()];
    }

    actual = polygon[(getIndex(polygon, actual)+1) % polygon.size()];
    while(actual != max){
        right.push_back(actual);
        actual = polygon[(getIndex(polygon, actual)+1) % polygon.size()];
    }

    std::stack<Vertex*> stack;

    stack.push(pq.top());
    pq.pop();
    stack.push(pq.top());
    pq.pop();

    Vertex* v_last = stack.top();

    while(pq.size() > 1){
        Vertex* v = pq.top();
        pq.pop();
        Vertex* top = stack.top();

        if(( (getIndex(left, v) != -1) && (getIndex(right, top) != -1) ) || ( (getIndex(right, v) != -1) && (getIndex(left, top) != -1) )){
            while(!stack.empty()){
                Vertex* next = stack.top();
                stack.pop();
                if(!stack.empty()){
                    lines.push_back(new Line(v->getPoint(), next->getPoint()));
                    graph[v].push_back(new Edge(v, next));
                    graph[next].push_back(new Edge(next, v));
                }
            }
            stack.push(v_last);
            stack.push(v);
        }else{
            Vertex* next = stack.top();
            stack.pop();

            while(!stack.empty() && canSee(v, next, stack.top(), left)){
                next = stack.top();
                stack.pop();
                lines.push_back(new Line(v->getPoint(), next->getPoint()));
                graph[v].push_back(new Edge(v, next));
                graph[next].push_back(new Edge(next, v));
            }
            stack.push(next);
            stack.push(v);
        }

        v_last = v;
    }

    Vertex* v = pq.top();
    pq.pop();

    stack.pop();
    while(!stack.empty()){
        Vertex* next = stack.top();
        stack.pop();
        if(!stack.empty()){
            lines.push_back(new Line(v->getPoint(), next->getPoint()));
            graph[v].push_back(new Edge(v, next));
            graph[next].push_back(new Edge(next, v));
        }
    }
}

void TriangulationAlgorithm::createPolygons(){
    for(std::pair<Vertex*, std::vector<Edge*>> evpair : graph){
        std::vector<Edge*> ev = evpair.second;

        for(Edge* e : ev){
            if(e->visited){
                continue;
            }

            std::vector<Vertex*> polygon;
            Vertex* start = e->getv1();
            Edge* actual = e;
            do{
                actual->visited = true;
                polygon.push_back(actual->getv1());

                std::vector<Edge*> outEdges = graph[actual->getv2()];
                // get most left edge
                Edge* out = outEdges[0];
                double min_angle = start->angleBetween2Lines(*actual->getv1()->getPoint(), *actual->getv2()->getPoint(), *out->getv2()->getPoint());
                for(Edge* o : outEdges){
                    if(o->visited){
                        continue;
                    }
                    if(actual->getv1() == o->getv2()){
                        continue;
                    }
                    double angle = start->angleBetween2Lines(*actual->getv1()->getPoint(), *actual->getv2()->getPoint(), *o->getv2()->getPoint());
                    if(angle < min_angle){
                        min_angle = angle;
                        out = o;
                    }
                }

                actual = out;

            }while(actual->getv1() != start);
            polygons.push_back(polygon);
        }
    }
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
    switch (p->getType()) {
        case Vertex::START:
            handleStart(p);
            break;
        case Vertex::SPLIT:
            handleSplit(p);
            break;
        case Vertex::END:
            handleEnd(p);
            break;
        case Vertex::MERGE:
            handleMerge(p);
            break;
        case Vertex::REGULAR:
            handleRegular(p);
            break;
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