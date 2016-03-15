
#ifndef CG_LAB_1516_TREFOIL_H
#define CG_LAB_1516_TREFOIL_H


#include "Parametrics.h"

/* See for general formulae
 * http://ocw.nctu.edu.tw/upload/classbfs1209054641151019.pdf
 *
 *
 *
 * See http://www.openprocessing.org/sketch/8105 for these things below

float x = 2.0f*sin(3.0f*u) / (2+cos(v));
float del1 = (2.0f*(sin(u) + 2*sin(2.0f*u)));
float del2 = 2.0f+cos(v+2.0f*PI/3.0);
float y = del1 / del2;
float z = (cos(u) - 2.0f*cos(2.0f*u)) * (2.0f+cos(v)) * (2.0f+cos(v+2.0f*PI/3.0f)) / 4.0f;

*/

class Trefoil : public Parametrics {

private:
    // hard coded sub circle radius r
    float a = 2.0f;
    float b = 2.0f;
    float d = 10.0f;
    float q = 3.0f;
    float p = 2.0f;

public:
    double x(double u, double v) {
        float u_scaled = (float) (u * 2 * PI);
        float v_scaled = (float) (v * 2 * PI);

        return ((a * sin(q * u_scaled) + d) * sin(p * u_scaled) ) + b * sin(p * u_scaled + v_scaled);
    }

    double y(double u, double v) {
        float u_scaled = (float) (u * 2 * PI);
        float v_scaled = (float) (v * 2 * PI);

        return ((a * sin(q * u_scaled) + d) * cos(p * u_scaled) ) + b * cos(p * u_scaled + v_scaled);
    }

    double z(double u, double v) {
        float u_scaled = (float) (u * 2 * PI);
        float v_scaled = (float) (v * 2 * PI);

        return a * cos(q * u_scaled) + b * cos (q * u_scaled + v_scaled);
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

#endif //CG_LAB_1516_TREFOIL_H
