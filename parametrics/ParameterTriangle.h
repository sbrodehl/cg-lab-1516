#ifndef CG_LAB_1516_PARAMETERTRIANGLE_H
#define CG_LAB_1516_PARAMETERTRIANGLE_H

#include <QVector2D>
#include <QPair>

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
        return (ux * vy - uy * vx >= 0 || std::abs(ux * vy - uy * vx) < pow(10, -10));
    }

};

#endif //CG_LAB_1516_PARAMETERTRIANGLE_H
