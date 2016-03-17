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
    float a = 5.0f;
    float b = 2.0f;
    float d = 10.0f;
    float p = 2.0f;
    float q = 3.0f;

    double scaled(double x) {
        return x * 2 * PI;
    }

public:

    Trefoil() : Trefoil(5, 2, 10, 2, 3) {
    }

    Trefoil(float a_, float b_, float d_, float p_, float q_) {
        a = a_;
        b = b_;
        d = d_;
        p = p_;
        q = q_;
        std::string classname = typeid(*this).name();
        setName(QString(classname.c_str()));
    }

    ~Trefoil() {

    }

    double x(double u, double v) {
        float theta = (float) scaled(u);
        float phi = (float) scaled(v);

        return ((a * sin(q * theta) + d) * sin(p * theta)) + b * sin(p * theta + phi);
    }

    double y(double u, double v) {
        float theta = (float) scaled(u);
        float phi = (float) scaled(v);

        return ((a * sin(q * theta) + d) * cos(p * theta)) + b * cos(p * theta + phi);
    }

    double z(double u, double v) {
        float theta = (float) scaled(u);
        float phi = (float) scaled(v);

        return a * cos(q * theta) + b * cos(q * theta + phi);
    }

    QVector3D getPoint(double u, double v) {
        return QVector3D((float) x(u, v),
                         (float) y(u, v),
                         (float) z(u, v));
    }

    QVector3D getNormal(double u, double v) {
        float theta = (float) scaled(u);
        float phi = (float) scaled(v);

        QVector3D dtheta((float) (b * p * cos(p * theta + phi) + a * q * cos(q * theta) * sin(p * theta) +
                                  p * cos(p * theta) * (d + a * sin(q * theta))),
                         (float) (a * q * cos(p * theta) * cos(q * theta) -
                                  p * sin(p * theta) * (d + a * sin(q * theta)) - b * p * sin(p * theta + phi)),
                         (float) (-q * (a * sin(q * theta) + b * sin(q * theta + phi))));

        QVector3D dphi((float) (b * cos(p * theta + phi)),
                       (float) (-b * sin(p * theta + phi)),
                       (float) (-b * sin(q * theta + phi)));

        QVector3D n = QVector3D::crossProduct(dtheta, dphi);
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
};

#endif //CG_LAB_1516_TREFOIL_H
