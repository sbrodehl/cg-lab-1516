#ifndef CG_LAB_1516_VERTEX_H
#define CG_LAB_1516_VERTEX_H

#include <QVector2D>
#include <QtMath>

class Vertex {

private:
    const double PI = M_PI;
    QVector2D *myPoint;
    Vertex *a;
    Vertex *b;

    bool startOrSplit() {
        return isAbove(*a) && isAbove(*b);
    }

    bool endOrMerge() {
        return isBelow(*a) && isBelow(*b);
    }

    bool isAbove(Vertex v) {
        if (y() == v.y())
            return x() < v.x();
        return y() > v.y();
    }

    bool isBelow(Vertex v) {
        if (y() == v.y())
            return x() > v.x();
        return y() < v.y();
    }

public:
    enum Type {
        REGULAR, START, END, SPLIT, MERGE
    };
    Type t;

    Vertex(double d1, double d2) {
        myPoint = new QVector2D(d1, d2);
    }

    Vertex(QVector2D p) {
        myPoint = new QVector2D(p.x(), p.y());
    }

    double x() {
        return myPoint->x();
    }

    double y() {
        return myPoint->y();
    }

    void setNeighbors(Vertex *a_new, Vertex *b_new) {
        a = a_new;
        b = b_new;
    }

    void setNeighbor(int n, Vertex *v) {
        if (n == 0) {
            a = v;
        }
        if (n == 1) {
            b = v;
        }
    }

    Vertex *getNeighbor(int n) {
        if (n == 0) {
            return a;
        }
        if (n == 1) {
            return b;
        }
        return nullptr;
    }

    void setType(Type t_new) {
        t = t_new;
    }

    Type getType() {
        return t;
    }

    void setType() {
        if (a != 0 && b != 0) {
            if (startOrSplit()) {
                double angle = angleBetween2Lines(*a->myPoint, *myPoint, *b->myPoint);
                if (angle < PI) {
                    t = START;
                } else {
                    t = SPLIT;
                }
            } else if (endOrMerge()) {
                double angle = angleBetween2Lines(*a->myPoint, *myPoint, *b->myPoint);


                if (angle < PI) {
                    t = END;
                } else {
                    t = MERGE;
                }
            } else {
                t = REGULAR;
            }

        }
    }

    double angleBetween2Lines(QVector2D n1, QVector2D a, QVector2D n2) {
        QVector2D v1 = QVector2D(n1.x() - a.x(), n1.y() - a.y());
        QVector2D v2 = QVector2D(n2.x() - a.x(), n2.y() - a.y());

        double norm1 = sqrt(v1.x() * v1.x() + v1.y() * v1.y());
        double norm2 = sqrt(v2.x() * v2.x() + v2.y() * v2.y());

        double scp = v1.x() * v2.x() + v1.y() * v2.y();

        double alpha = acos(scp / (norm1 * norm2));

        if (leftTurn(n1, a, n2)) {
            return alpha;
        } else {
            return 2 * PI - alpha;
        }
    }

    bool leftTurn(QVector2D a, QVector2D b, QVector2D c) {
        double ux = b.x() - a.x();
        double uy = b.y() - a.y();
        double vx = c.x() - a.x();
        double vy = c.y() - a.y();
        return (ux * vy - uy * vx >= 0);
    }

    QVector2D *getPoint() {
        return myPoint;
    }
};

#endif //CG_LAB_1516_VERTEX_H
