#ifndef CG_LAB_1516_TORUS_H
#define CG_LAB_1516_TORUS_H


#include "Parametrics.h"

class Torus : public Parametrics {

private:
    // hard coded main circle radius R and sub circle radius r
    float R = 10;
    float r = 1;

    double scaled(double x) {
        return x * 2 * PI;
    }

public:
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

    std::vector<QPair<QVector3D, QVector3D> > getSegments() {
        return std::vector<QPair<QVector3D, QVector3D> >();
    }

};

#endif //CG_LAB_1516_TORUS_H
