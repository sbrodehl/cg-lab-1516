//
// Created by sbrodehl on 14.03.16.
//

#ifndef CG_LAB_1516_TREEFOIL_H
#define CG_LAB_1516_TREEFOIL_H


#include "Parametrics.h"

class Treefoil : public Parametrics {

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
        // u and v in [0, 1]
        float u_scaled = (float) (u * 2 * PI);
        float v_scaled = (float) (v * 2 * PI);

        return QVector3D((float) ((R + r * cos(v_scaled)) * cos(u_scaled)),
                         (float) ((R + r * cos(v_scaled)) * sin(u_scaled)),
                         (float) (r * sin(v_scaled)));
    }

    QVector3D n(double u, double v) {
        return QVector3D(0,
                         0,
                         0);
    }

};

#endif //CG_LAB_1516_TREEFOIL_H
