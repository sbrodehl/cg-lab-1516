//
// Created by clamber on 15.03.2016.
//

#ifndef CG_LAB_1516_KEGEL_H
#define CG_LAB_1516_KEGEL_H

#include "Parametrics.h"

class Kegel : public Parametrics {

private:
    // hard coded main circle radius R and height H
    float R = 5;
    float H = 10;

public:
    double x(double u, double v) {
        float u_scaled = (float) (u * 2 * PI);

        return R * cos(u_scaled) * (1-v);
    }

    double y(double u, double v) {
        float u_scaled = (float) (u * 2 * PI);

        return R * sin(u_scaled) * (1-v);
    }

    double z(double u, double v) {
        return H * v;
    }

    QVector3D getPoint(double u, double v) {
        return QVector3D((float) x(u, v),
                         (float) y(u, v),
                         (float) z(u, v));
    }

    QVector3D getNormal(double u, double v) {
        float u_scaled = (float) (u * 2 * PI);
        QVector3D first((float) (-1.0f * R * sin(u_scaled) * (1-v)),
                        (float) (R * cos(u_scaled) * (1-v)),
                        (float) (0.0));
        QVector3D second((float) (-1.0f * R * cos(u_scaled)),
                         (float) (-1.0f *R * sin(u_scaled)),
                         (H));

        QVector3D n = QVector3D::crossProduct(first, second);
        n.normalize();

        return n;
    }

    std::vector<QPair<QVector3D, QVector3D> > getSegments() {
        return std::vector<QPair<QVector3D, QVector3D> >();
    }

    std::vector<QVector3D> triangulate(double delta, double eps){
        return std::vector<QVector3D>();
    }

};

#endif //CG_LAB_1516_KEGEL_H
