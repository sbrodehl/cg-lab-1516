#include <iostream>

#include "TriangulationAlgorithm.h"
#include "Parametrics.h"

std::vector<QVector3D> Parametrics::createTriangles(const std::vector<QVector3D> &points,
                                                    int bucketsize) {
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
    pw = new ParametricWindow(parent);
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
    return (ux * vy - uy * vx >= 0 || std::abs(ux * vy - uy * vx) < pow(10, -10));
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

std::vector<QVector3D> Parametrics::getPolygonTriangulation(float edgeLength) {
    QList<QList<QPointF> > shapeList = pw->getArea()->getShapes();
    float scale = 100.0f;

    std::vector<std::vector<QVector2D *>> polygon;
    for (QList<QPointF> shape : shapeList) {
        std::vector<QVector2D *> s;
        for (QPointF p : shape) {
            QVector2D *v = new QVector2D((float) (p.x() / scale),
                                         (float) ((scale - p.y()) / scale));
            s.push_back(v);
        }
        polygon.push_back(s);
    }
    TriangulationAlgorithm algorithm;
    std::vector<ParameterTriangle *> trias = algorithm.triangulate(polygon);
    std::vector<ParameterTriangle *> refined;

    std::vector<QVector3D> triangles;
    for (ParameterTriangle *t : trias) {
        if (!t->isQualityTriangle(edgeLength)) {
            for (ParameterTriangle *ref : t->refine(edgeLength)) {
                refined.push_back(ref);
                QVector2D p1 = ref->getPoint(0);
                triangles.push_back(getPoint(p1.x(), p1.y()));
                triangles.push_back(getNormal(p1.x(), p1.y()));
                QVector2D p2 = ref->getPoint(1);
                triangles.push_back(getPoint(p2.x(), p2.y()));
                triangles.push_back(getNormal(p2.x(), p2.y()));
                QVector2D p3 = ref->getPoint(2);
                triangles.push_back(getPoint(p3.x(), p3.y()));
                triangles.push_back(getNormal(p3.x(), p3.y()));
            }
        } else {
            refined.push_back(t);
            QVector2D p1 = t->getPoint(0);
            triangles.push_back(getPoint(p1.x(), p1.y()));
            triangles.push_back(getNormal(p1.x(), p1.y()));
            QVector2D p2 = t->getPoint(1);
            triangles.push_back(getPoint(p2.x(), p2.y()));
            triangles.push_back(getNormal(p2.x(), p2.y()));
            QVector2D p3 = t->getPoint(2);
            triangles.push_back(getPoint(p3.x(), p3.y()));
            triangles.push_back(getNormal(p3.x(), p3.y()));
        }
    }
    pw->drawTriangulation(refined);
    return triangles;
}
