//
// Created by clamber on 15.03.2016.
//

#ifndef CG_LAB_1516_KEGEL_H
#define CG_LAB_1516_KEGEL_H

#include "Parametrics.h"

class Kegel : public Parametrics {

private:
    // hard coded main circle radius R and height H
    float R = 0;
    float H = 0;

public:

    Kegel(float R_in, float H_in) {
        R = R_in;
        H = H_in;
    }

    double x(double u, double v) {
        float u_scaled = (float) (u * 2 * PI);

        return R * cos(u_scaled) * v;
    }

    double y(double u, double v) {
        float u_scaled = (float) (u * 2 * PI);

        return R * sin(u_scaled) * v;
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
        QVector3D first((float) (-1.0f * R * sin(u_scaled) * v),
                        (float) (R * cos(u_scaled) * v),
                        (float) (0.0));
        QVector3D second((float) (R * cos(u_scaled)),
                         (float) (R * sin(u_scaled)),
                         (H));

        QVector3D n = QVector3D::crossProduct(first, second);
        n.normalize();

        return n;
    }

    std::vector<QPair<QVector3D, QVector3D> > getSegments() {
        return std::vector<QPair<QVector3D, QVector3D> >();
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

#endif //CG_LAB_1516_KEGEL_H
