/* Copyright (C) 2015 Tassilo Kugelstadt */

#include "Mesh.h"

//creates triangle from the points abc
void Mesh::addTriangle(const QVector3D &a, const QVector3D &b, const QVector3D &c) {
    int ai = (int) verts.size(),
            bi = ai + 1,
            ci = ai + 2;
    int e1i = (int) edges.size(),
            e2i = e1i + 1,
            e3i = e1i + 2;
    int fi = (int) faces.size();
    QVector3D n = QVector3D::crossProduct((b - a), (c - b));
    verts.push_back(vertex(a, e1i));
    verts.push_back(vertex(b, e2i));
    verts.push_back(vertex(c, e3i));
    edges.push_back(edge(bi, e3i, e2i, -1, fi)); //e1
    edges.push_back(edge(ci, e1i, e3i, -1, fi)); //e2
    edges.push_back(edge(ai, e2i, e1i, -1, fi)); //e3
    faces.push_back(face(e1i, n));
}

//creates triangle from the existing edge e and new point p
void Mesh::addTriangle(int ei, const QVector3D &c) {
    int ai = edges[ei].vertex;
    int bi = edges[edges[ei].pred].vertex;
    int ci = (int) verts.size();
    int e1i = (int) edges.size(),
            e2i = e1i + 1,
            e3i = e2i + 1;
    int fi = (int) faces.size();
    QVector3D a = verts[ai].pos;
    QVector3D b = verts[bi].pos;
    QVector3D n = QVector3D::crossProduct((b - a), (c - b));
    verts.push_back(vertex(c, e1i));
    edges.push_back(edge(bi, e3i, e2i, ei, fi)); //e1
    edges[ei].opposite = e1i;
    edges.push_back(edge(ci, e1i, e3i, -1, fi)); //e2
    edges.push_back(edge(ai, e2i, e1i, -1, fi)); //e3
    faces.push_back(face(e1i, n));
}

//creates triangle from the existing adjacent edges e1 and e2
//e2 points from e1.vertex to e2.vertex
void Mesh::addTriangle(int e1, int e2) {
    int ai = edges[e2].vertex;
    int bi = edges[e1].vertex;
    int ci = edges[edges[e1].pred].vertex;
    int e1i = (int) edges.size(),
            e2i = (int) (edges.size() + 1),
            e3i = (int) (edges.size() + 2);
    int fi = (int) faces.size();
    QVector3D n = QVector3D::crossProduct((verts[bi].pos - verts[ai].pos),
                                          (verts[ci].pos - verts[bi].pos));
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
    int e1i = (int) edges.size(),
            e2i = (int) (edges.size() + 1),
            e3i = (int) (edges.size() + 2);
    int fi = (int) faces.size();
    edges.push_back(edge(bi, e3i, e2i, e1, fi));//e1
    edges[e1].opposite = e1i;
    edges.push_back(edge(ci, e1i, e3i, e3, fi));//e2
    edges[e3].opposite = e2i;
    edges.push_back(edge(ai, e2i, e1i, e2, fi));//e3
    edges[e2].opposite = e3i;
    QVector3D n = QVector3D::crossProduct((verts[bi].pos - verts[ai].pos),
                                          (verts[ci].pos - verts[bi].pos));
    faces.push_back(face(e1i, n));
}
