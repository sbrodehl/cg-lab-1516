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

        return createTriangles(pointvec, delta + 1);
    }

};

#endif //CG_LAB_1516_TORUS_H
