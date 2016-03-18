/* Copyright (C) 2015 Tassilo Kugelstadt */

#define _USE_MATH_DEFINES

#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>
#include <algorithm>
#include <random>
#include <queue>
#include <QtOpenGL>

#include "Mesh.h"

//initializes the mesh as a tetrahedron that is formed by 4 points
void Mesh::addTetrahedron(Vector3d &a, Vector3d &b, Vector3d &c, Vector3d &d) {
    //test on which side of triangle abc the point d lies
    Vector3d n = (b - a) % (c - b);
    double n0 = n * a;
    if (n * d > n0) { //above abc -> n points inwards
        //switch b and c so that normal points outwards and d is below abc
        Vector3d tmp = b;
        b = c;
        c = tmp;
        n *= -1.0;
    }
    addTriangle(a, b, c);
    addTriangle(0, d);
    addTriangle(edges[edges[0].opposite].pred, edges[0].succ);
    int e1i = edges.size() - 1;   //last added edge has no opposite
    int e2i = edges[edges[edges[e1i].succ].opposite].succ;
    int e3i = edges[edges[edges[e2i].succ].opposite].succ;
    addTriangle(e1i, e2i, e3i);
}

//creates triangle from the points abc
void Mesh::addTriangle(const Vector3d &a, const Vector3d &b, const Vector3d &c) {
    int ai = verts.size(), bi = ai + 1, ci = ai + 2;
    int e1i = edges.size(), e2i = e1i + 1, e3i = e1i + 2;
    int fi = faces.size();
    Vector3d n = (b - a) % (c - b);
    verts.push_back(vertex(a, e1i));
    verts.push_back(vertex(b, e2i));
    verts.push_back(vertex(c, e3i));
    edges.push_back(edge(bi, e3i, e2i, -1, fi));    //e1
    edges.push_back(edge(ci, e1i, e3i, -1, fi));    //e2
    edges.push_back(edge(ai, e2i, e1i, -1, fi));    //e3
    faces.push_back(face(e1i, n));
}

//creates triangle from the existing edge e and new point p
void Mesh::addTriangle(int ei, const Vector3d &c) {
    int ai = edges[ei].vertex;
    int bi = edges[edges[ei].pred].vertex;
    int ci = verts.size();
    int e1i = edges.size(), e2i = e1i + 1, e3i = e2i + 1;
    int fi = faces.size();
    Vector3d a = verts[ai].pos;
    Vector3d b = verts[bi].pos;
    Vector3d n = (b - a) % (c - b);
    verts.push_back(vertex(c, e1i));
    edges.push_back(edge(bi, e3i, e2i, ei, fi)); //e1
    edges[ei].opposite = e1i;
    edges.push_back(edge(ci, e1i, e3i, -1, fi));//e2
    edges.push_back(edge(ai, e2i, e1i, -1, fi));//e3
    faces.push_back(face(e1i, n));
}

//creates triangle from the existing adjacent edges e1 and e2
//e2 points from e1.vertex to e2.vertex
void Mesh::addTriangle(int e1, int e2) {
    int ai = edges[e2].vertex;
    int bi = edges[e1].vertex;
    int ci = edges[edges[e1].pred].vertex;
    int e1i = edges.size(), e2i = edges.size() + 1, e3i = edges.size() + 2;
    int fi = faces.size();
    Vector3d n = (verts[bi].pos - verts[ai].pos) % (verts[ci].pos - verts[bi].pos);
    edges.push_back(edge(bi, e3i, e2i, e2, fi));//e1
    edges[e2].opposite = e1i;
    edges.push_back(edge(ci, e1i, e3i, e1, fi));//e2
    edges[e1].opposite = e2i;
    edges.push_back(edge(ai, e2i, e1i, -1, fi));//e3
    faces.push_back(face(e1i, n));
}

//creates triangle from the three existing edges e1, e2 and e3
void Mesh::addTriangle(int e1, int e2, int e3) {
    int ai = edges[e1].vertex;
    int bi = edges[e3].vertex;
    int ci = edges[e2].vertex;
    int e1i = edges.size(), e2i = edges.size() + 1, e3i = edges.size() + 2;
    int fi = faces.size();
    edges.push_back(edge(bi, e3i, e2i, e1, fi));//e1
    edges[e1].opposite = e1i;
    edges.push_back(edge(ci, e1i, e3i, e3, fi));//e2
    edges[e3].opposite = e2i;
    edges.push_back(edge(ai, e2i, e1i, e2, fi));//e3
    edges[e2].opposite = e3i;
    Vector3d n = (verts[bi].pos - verts[ai].pos) % (verts[ci].pos - verts[bi].pos);
    faces.push_back(face(e1i, n));
}
