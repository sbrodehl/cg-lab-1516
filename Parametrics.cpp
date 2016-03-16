#include "Parametrics.h"
#include "ParametricWindow.h"

std::vector<QVector3D> Parametrics::createTriangles(const std::vector<QVector3D> &points, int bucketsize) {
    std::vector<QVector3D> triangles;
    size_t s = points.size();

    for (size_t i = 0; i < s - (2 * bucketsize); i += 2 * bucketsize) {
        for (size_t l = 0; l < 2 * bucketsize - 2; l += 2) {
            size_t leftRoot = i + l;
            size_t rightRoot = i + l + 2 * bucketsize;
            size_t nextLeft = i + l + 2;
            size_t nextRight = i + l + 2 + 2 * bucketsize;
            QVector3D leftR = points[leftRoot];
            QVector3D leftRNormal = points[leftRoot + 1];
            QVector3D rightR = points[rightRoot];
            QVector3D rightRNormal = points[rightRoot + 1];
            QVector3D neleftR = points[nextLeft];
            QVector3D neleftRNormal = points[nextLeft + 1];
            QVector3D nerightR = points[nextRight];
            QVector3D nerightRNormal = points[nextRight + 1];

            triangles.push_back(neleftR);
            triangles.push_back(neleftRNormal);
            triangles.push_back(rightR);
            triangles.push_back(rightRNormal);
            triangles.push_back(leftR);
            triangles.push_back(leftRNormal);
            triangles.push_back(neleftR);
            triangles.push_back(neleftRNormal);
            triangles.push_back(nerightR);
            triangles.push_back(nerightRNormal);
            triangles.push_back(rightR);
            triangles.push_back(rightRNormal);
        }
    }
    return triangles;
}

ParametricWindow *Parametrics::createWindow(QWidget *parent) {
    ParametricWindow *pw = new ParametricWindow(parent);
    pw->initParams(this);
    return pw;
}
