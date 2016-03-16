#ifndef CG_LAB_1516_DISC_H
#define CG_LAB_1516_DISC_H

#include "Parametrics.h"

class Disc : public Parametrics {


private:
    // hard coded circle radius R and position H
    float R = 0;
    float H = 0;

    double scaled(double x) {
        return x * 2 * PI;
    }

public:

    Disc(float R_in, float H_in) {
        R = R_in;
        H = H_in;
        std::string classname = typeid(*this).name();
        setName(QString(classname.c_str()));
    }

    double x(double u, double v) {
        float theta = (float) scaled(u);

        return R * cos(theta) * v;
    }

    double y(double u, double v) {
        float theta = (float) scaled(u);

        return R * sin(theta) * v;
    }

    double z(double u, double v) {
        return H;
    }

    QVector3D getPoint(double u, double v) {
        return QVector3D((float) x(u, v),
                         (float) y(u, v),
                         (float) z(u, v));
    }

    QVector3D getNormal(double u, double v) {

        QVector3D n = QVector3D(0, 0, 1);
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

#endif //CG_LAB_1516_DISC_H
