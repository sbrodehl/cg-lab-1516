#ifndef CG_LAB_1516_TUBE_H
#define CG_LAB_1516_TUBE_H

#include "Parametrics.h"

class Tube : public Parametrics {

private:
    // hard coded main circle radius R and sub circle radius r
    float R = 0;
    float H = 0;

    double scaled(double x) {
        return x * 2 * PI;
    }

public:

    Tube(float R_in, float H_in) {
        R = R_in;
        H = H_in;
        std::string classname = typeid(*this).name();
        setName(QString(classname.c_str()));
    }

    double x(double u, double v) {
        float theta = (float) scaled(u);

        return R * cos(theta);
    }

    double y(double u, double v) {
        float theta = (float) scaled(u);

        return R * sin(theta);
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
        float theta = (float) scaled(u);

        QVector3D first((float) (-1.0f * R * sin(theta)),
                        (float) (R * cos(theta)),
                        (float) (0.0));
        QVector3D second((float) (0.0),
                         (float) (0.0),
                         (H));

        QVector3D n = QVector3D::crossProduct(first, second);
        n.normalize();

        return n;
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

    Mesh getMesh(double delta, double eps) { return Mesh(); }
};

#endif //CG_LAB_1516_TUBE_H
