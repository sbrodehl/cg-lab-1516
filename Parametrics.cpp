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

void Parametrics::initializeParameterSpace(double delta, double eps) {
    std::vector<ParameterTriangle *> lowerTriangleLineOld;

    for (int j = 0; j < eps; j++) {
        ParameterTriangle *leftBound = 0;
        std::vector<ParameterTriangle *> lowerTriangleLineNew;

        for (int i = 0; i < delta; i++) {
            QVector2D a = QVector2D(i / delta, j / eps);
            QVector2D b = QVector2D((i + 1) / delta, j / eps);
            QVector2D c = QVector2D((i + 1) / delta, (j + 1) / eps);
            QVector2D d = QVector2D(i / delta, (j + 1) / eps);

            ParameterTriangle *abc = new ParameterTriangle(a, b, c);
            ParameterTriangle *acd = new ParameterTriangle(a, c, d);

            abc->setNeighbor(2, acd);
            acd->setNeighbor(0, abc);

            if (leftBound != 0) {
                acd->setNeighbor(2, leftBound);
                leftBound->setNeighbor(1, acd);
            }
            lowerTriangleLineNew.push_back(acd);

            if (j > 0) {
                abc->setNeighbor(0, lowerTriangleLineOld[i]);
                lowerTriangleLineOld[i]->setNeighbor(1, abc);
            }

            leftBound = abc;

            parameterSpace.push_back(abc);
            parameterSpace.push_back(acd);
        }

        lowerTriangleLineOld = lowerTriangleLineNew;
    }
}

bool Parametrics::leftTurn(QVector2D a, QVector2D b, QVector2D c) {
    double ux = b.x() - a.x();
    double uy = b.y() - a.y();
    double vx = c.x() - a.x();
    double vy = c.y() - a.y();
    return (ux * vy - uy * vx >= 0 || abs(ux * vy - uy * vx) < pow(10, -10));
}

ParameterTriangle Parametrics::locatePoint(ParameterTriangle actual, QVector2D point) {
    bool exit = true;
    int id = 0;
    for (int i = 0; i < 3; i++) {
        QPair<QVector2D, QVector2D> segment = actual.getSegment(i);
        if (!leftTurn(segment.first, segment.second, point)) {
            exit = false;
            id = i;
        }
    }
    if (exit) {
        return actual;
    }
    return locatePoint(*actual.getNeighbor(id), point);
}
