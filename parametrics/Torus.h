#ifndef CG_LAB_1516_TORUS_H
#define CG_LAB_1516_TORUS_H


#include "Parametrics.h"

class Torus : public Parametrics {

private:
    float R;
    float r;

    double scaled(double x) {
        return x * 2 * PI;
    }

public:

    Torus(float R_, float r_) {
        R = R_;
        r = r_;
        std::string classname = typeid(*this).name();
        setName(QString(classname.c_str()));
    }

    Torus() : Torus(10, 1) {
    }

    ~Torus() {

    }

    double x(double u, double v) {
        float theta = (float) scaled(u);
        float phi = (float) scaled(v);

        return (R + r * cos(phi)) * cos(theta);
    }

    double y(double u, double v) {
        float theta = (float) scaled(u);
        float phi = (float) scaled(v);

        return (R + r * cos(phi)) * sin(theta);
    }

    double z(double u, double v) {
        float phi = (float) scaled(v);

        return r * sin(phi);
    }

    QVector3D getPoint(double u, double v) {
        return QVector3D((float) x(u, v),
                         (float) y(u, v),
                         (float) z(u, v));
    }

    QVector3D getNormal(double u, double v) {
        float theta = (float) scaled(u);
        float phi = (float) scaled(v);

        return QVector3D((float) ((R + r * cos(phi)) * r * cos(theta) * cos(phi)),
                         (float) ((R + r * cos(phi)) * r * sin(theta) * cos(phi)),
                         (float) ((R + r * cos(phi)) * r * sin(phi)));
    }

    std::vector<QVector3D> triangulate(double delta, double eps) {
        std::vector<QVector3D> pointvec;

        for (int i = 0; i <= delta; i++) {
            for (int j = 0; j <= eps; j++) {
                pointvec.push_back(getPoint((float) i / delta, (float) j / eps));
                pointvec.push_back(getNormal((float) i / delta, (float) j / eps));
            }
        }

        return createTriangles(pointvec, (int) (delta + 1));
    }

    Mesh getMesh(double delta, double eps) {
        Mesh mesh;

        std::vector<QVector3D> pointsWN;
        int bucketsize = (int) (delta + 1);

        for (int i = 0; i <= delta; i++) {
            for (int j = 0; j <= eps; j++) {
                pointsWN.push_back(getPoint((float) i / delta, (float) j / eps));
                pointsWN.push_back(getNormal((float) i / delta, (float) j / eps));
            }
        }

        for (size_t i = 0; i < pointsWN.size() - (2 * bucketsize); i += 2 * bucketsize) {
            for (size_t l = 0; l < 2 * bucketsize - 2; l += 2) {
                size_t leftRoot = i + l;
                size_t rightRoot = i + l + 2 * bucketsize;
                size_t nextLeft = i + l + 2;
                size_t nextRight = i + l + 2 + 2 * bucketsize;
                QVector3D leftR = pointsWN[leftRoot];
                QVector3D leftRNormal = pointsWN[leftRoot + 1];
                QVector3D rightR = pointsWN[rightRoot];
                QVector3D rightRNormal = pointsWN[rightRoot + 1];
                QVector3D neleftR = pointsWN[nextLeft];
                QVector3D neleftRNormal = pointsWN[nextLeft + 1];
                QVector3D nerightR = pointsWN[nextRight];
                QVector3D nerightRNormal = pointsWN[nextRight + 1];
            }
        }
        return mesh;
    }

};

#endif //CG_LAB_1516_TORUS_H
