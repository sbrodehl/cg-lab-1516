#ifndef MESH_H
#define MESH_H

/* Copyright (C) 2015 Tassilo Kugelstadt */

#include <vector>
#include <set>
#include <unordered_set>
#include <iostream>

#if _MSC_VER
#include <gl/glu.h>
#elif __APPLE__
#include <OpenGL/glu.h>
#else

#include <GL/glu.h>

#endif

#include "vecmath.h"

struct vertex {
    Vector3d pos;
    int halfedge;

    vertex() {
        pos = Vector3d();
        halfedge = -1;
    }

    vertex(Vector3d pos, int halfedge) : pos(pos), halfedge(halfedge) { }
};

struct edge {
    int vertex; //where the edge points to
    int pred, succ, opposite;
    int face;

    edge() { vertex = pred = succ = opposite = face = -1; }

    edge(int vertex, int pred, int succ, int opposite, int face) : vertex(vertex), pred(pred), succ(succ),
                                                                   opposite(opposite), face(face) { }
};

struct face {
    int halfedge;
    Vector3d normal;

    face(int halfedge, Vector3d normal) : halfedge(halfedge), normal(normal) { }
};

class Mesh {
public:
    Mesh() {};

    void addTetrahedron(Vector3d &a, Vector3d &b, Vector3d &c, Vector3d &d);

    void addTriangle(const Vector3d &a, const Vector3d &b, const Vector3d &c); //creates triangle from the points abc
    void addTriangle(int ei, const Vector3d &c); //creates triangle from the existing edge e and new point p
    void addTriangle(int e1, int e2); //creates triangle from the two existing edges e1 and e2
    void addTriangle(int e1, int e2, int e3); //creates triangle from the three existing edges e1, e2 and e3

    std::vector<vertex> verts;
    std::vector<edge> edges;
    std::vector<face> faces;
};


#endif
