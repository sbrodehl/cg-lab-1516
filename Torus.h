
#ifndef CG_LAB_1516_TREEFOIL_H
#define CG_LAB_1516_TREEFOIL_H


#include "Parametrics.h"

class Torus : public Parametrics {

private:
    // hard coded main circle radius R and sub circle radius r
    float R = 10;
    float r = 1;

public:
    double x(double u, double v) {
        float u_scaled = (float) (u * 2 * PI);
        float v_scaled = (float) (v * 2 * PI);

        return (R + r*cos(v_scaled))*cos(u_scaled);
    }

    double y(double u, double v) {
        float u_scaled = (float) (u * 2 * PI);
        float v_scaled = (float) (v * 2 * PI);

        return (R + r*cos(v_scaled))*sin(u_scaled);
    }

    double z(double u, double v) {
        float u_scaled = (float) (u * 2 * PI);
        float v_scaled = (float) (v * 2 * PI);

        return r*sin(v_scaled);
    }

    QVector3D getPoint(double u, double v) {
        return QVector3D((float) x(u, v),
                         (float) y(u, v),
                         (float) z(u, v));
    }

    QVector3D n(double u, double v) {
        float u_scaled = (float) (u * 2 * PI);
        float v_scaled = (float) (v * 2 * PI);

        return QVector3D((R+r*cos(v_scaled))*r*cos(u_scaled)*cos(v_scaled),
                         (R+r*cos(v_scaled))*r*sin(u_scaled)*cos(v_scaled),
                         (R+r*cos(v_scaled))*r*sin(v_scaled));
    }

};

#endif //CG_LAB_1516_TREEFOIL_H
