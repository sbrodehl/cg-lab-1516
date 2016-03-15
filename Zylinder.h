#ifndef CG_LAB_1516_ZYLINDER_H
#define CG_LAB_1516_ZYLINDER_H

#include "Parametrics.h"

class Zylinder : public Parametrics {

private:
    // hard coded main circle radius R and sub circle radius r
    float R = 10;
    float r = 1;

public:
    double x(double u, double v) {
        float u_scaled = (float) (u * 2 * PI);
        float v_scaled = (float) (v * 2 * PI);

        return 0.0;
    }

    double y(double u, double v) {
        float u_scaled = (float) (u * 2 * PI);
        float v_scaled = (float) (v * 2 * PI);

        return 0.0;
    }

    double z(double u, double v) {
        float u_scaled = (float) (u * 2 * PI);
        float v_scaled = (float) (v * 2 * PI);

        return 0.0;
    }

    QVector3D getPoint(double u, double v) {
        return QVector3D((float) x(u, v),
                         (float) y(u, v),
                         (float) z(u, v));
    }

    QVector3D getNormal(double u, double v) {
        float u_scaled = (float) (u * 2 * PI);
        float v_scaled = (float) (v * 2 * PI);

        return QVector3D((float)(0.0),
                         (float)(0.0),
                         (float)(0.0));
    }

    std::vector<QPair<QVector3D, QVector3D> > getSegments() {
        return std::vector<QPair<QVector3D, QVector3D> >();
    }

};

#endif //CG_LAB_1516_ZYLINDER_H
