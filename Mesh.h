#ifndef MESH_H
#define MESH_H

/* Copyright (C) 2015 Tassilo Kugelstadt */

#include <QtOpenGL>

#include <vector>

struct vertex {
    QVector3D pos;
    QVector3D normal;
    int halfedge;

    vertex() {
        pos = QVector3D();
        normal = QVector3D();
        halfedge = -1;
    };

    vertex(QVector3D pos, int halfedge, QVector3D n) : pos(pos), halfedge(halfedge), normal(n) { };
    vertex(QVector3D pos, int halfedge) : pos(pos), halfedge(halfedge) { };
};

struct edge {
    int vertex; //where the edge points to
    int pred, succ, opposite;
    int face;

    edge() { vertex = pred = succ = opposite = face = -1; };
    edge(int vertex, int pred, int succ, int opposite, int face) : vertex(vertex), pred(pred),
                                                                   succ(succ), opposite(opposite),
                                                                   face(face) { };
};

struct face {
    int halfedge;
    QVector3D normal;

    face(int halfedge, QVector3D normal) : halfedge(halfedge), normal(normal) { };
};

class Mesh {
public:
    Mesh() { };

    bool isEmpty();

    void append(const Mesh &mesh);

    void addTriangle(const QVector3D &a, const QVector3D &b, const QVector3D &c); //creates triangle from the points abc
    void addTriangle(int ei, const QVector3D &c); //creates triangle from the existing edge e and new point p
    void addTriangle(int e1, int e2); //creates triangle from the two existing edges e1 and e2
    void addTriangle(int e1, int e2, int e3); //creates triangle from the three existing edges e1, e2 and e3

    std::vector<vertex> verts;
    std::vector<edge> edges;
    std::vector<face> faces;
};

#endif
