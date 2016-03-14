
#ifndef CG_LAB_1516_TREEFOIL_H
#define CG_LAB_1516_TREEFOIL_H


#include "Parametrics.h"

class Treefoil : public Parametrics {

private:
    // hard coded sub circle radius r
    float r = 0.5;

public:
    double x(double u, double v) {
        float u_scaled = (float) (u * 2 * PI);
        float v_scaled = (float) (v * 2 * PI);

        return (3+cos(3*u_scaled))*cos(2*u_scaled) + r*cos(v_scaled)*cos(2*u_scaled);
    }

    double y(double u, double v) {
        float u_scaled = (float) (u * 2 * PI);
        float v_scaled = (float) (v * 2 * PI);

        return (3+cos(3*u_scaled))*sin(2*u_scaled) + r*cos(v_scaled)*sin(2*u_scaled);
    }

    double z(double u, double v) {
        float u_scaled = (float) (u * 2 * PI);
        float v_scaled = (float) (v * 2 * PI);

        return sin(3*u_scaled) + r*sin(v_scaled);
    }

    QVector3D getPoint(double u, double v) {
        return QVector3D((float) x(u, v),
                         (float) y(u, v),
                         (float) z(u, v));
    }

    QVector3D getNormal(double u, double v) {
        return QVector3D(0,
                         0,
                         0);
    }

};

#endif //CG_LAB_1516_TREEFOIL_H
