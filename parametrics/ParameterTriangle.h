#ifndef CG_LAB_1516_PARAMETERTRIANGLE_H
#define CG_LAB_1516_PARAMETERTRIANGLE_H

#include <QVector2D>
#include <QPair>
#include <QLineF>

#include <cmath>

class ParameterTriangle {
    QVector2D points[3] = {};
    QPair<QVector2D, QVector2D> segments[3] = {};
    ParameterTriangle *neighbors[3] = {};

public:
    ParameterTriangle(QVector2D a, QVector2D b, QVector2D c) {
        if (leftTurn(a, b, c)) {
            points[0] = a;
            points[1] = b;
            points[2] = c;
        } else {
            points[0] = a;
            points[1] = c;
            points[2] = b;
        }

        segments[0] = QPair<QVector2D, QVector2D>(points[0], points[1]);
        segments[1] = QPair<QVector2D, QVector2D>(points[1], points[2]);
        segments[2] = QPair<QVector2D, QVector2D>(points[2], points[0]);
    }

    void setNeighbor(int id, ParameterTriangle *neighbor) {
        neighbors[id] = neighbor;
    }

    ParameterTriangle *getNeighbor(int id) {
        return neighbors[id];
    }

    QPair<QVector2D, QVector2D> getSegment(int id) {
        return segments[id];
    }

    QVector2D getPoint(int id) {
        return points[id];
    }

    bool leftTurn(QVector2D a, QVector2D b, QVector2D c) {
        double ux = b.x() - a.x();
        double uy = b.y() - a.y();
        double vx = c.x() - a.x();
        double vy = c.y() - a.y();
        return (ux * vy - uy * vx >= 0 || std::abs(ux * vy - uy * vx) < std::pow(10, -10));
    }

    bool isQualityTriangle(float d) {
        for (QPair<QVector2D, QVector2D> seg : segments) {
            QLineF line = QLineF(seg.first.toPointF(), seg.second.toPointF());
            if (line.length() > d)
                return false;
        }
        return true;
    };

    std::vector<ParameterTriangle *> refine(float edgeLength) {
        typedef QPair<QVector2D, QVector2D> Segment;
        typedef std::vector<ParameterTriangle *> TriaL;
        TriaL refined;
        Segment a = segments[0];
        QVector2D splitPa = a.first + 0.5 * (a.second - a.first);
        Segment b = segments[1];
        QVector2D splitPb = b.first + 0.5 * (b.second - b.first);
        Segment c = segments[2];
        QVector2D splitPc = c.first + 0.5 * (c.second - c.first);
        ParameterTriangle *tA = new ParameterTriangle(a.first, splitPa, splitPc);
        if (tA->isQualityTriangle(edgeLength))
            refined.push_back(tA);
        else {
            TriaL rtA = tA->refine(edgeLength);
            refined.insert(refined.end(), rtA.begin(), rtA.end());
        }
        ParameterTriangle *tB = new ParameterTriangle(b.first, splitPb, splitPa);
        if (tB->isQualityTriangle(edgeLength))
            refined.push_back(tB);
        else {
            TriaL rtB = tB->refine(edgeLength);
            refined.insert(refined.end(), rtB.begin(), rtB.end());
        }
        ParameterTriangle *tC = new ParameterTriangle(c.first, splitPc, splitPb);
        if (tC->isQualityTriangle(edgeLength))
            refined.push_back(tC);
        else {
            TriaL rtC = tC->refine(edgeLength);
            refined.insert(refined.end(), rtC.begin(), rtC.end());
        }
        ParameterTriangle *tT = new ParameterTriangle(splitPa, splitPb, splitPc);
        if (tT->isQualityTriangle(edgeLength))
            refined.push_back(tT);
        else {
            TriaL rtT = tT->refine(edgeLength);
            refined.insert(refined.end(), rtT.begin(), rtT.end());
        }
        return refined;
    };
};

#endif //CG_LAB_1516_PARAMETERTRIANGLE_H
