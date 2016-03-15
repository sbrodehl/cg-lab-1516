#ifndef CG_LAB_1516_PARAMETRICS_H
#define CG_LAB_1516_PARAMETRICS_H

#include <QVector3D>
#include <QPair>
#include <cmath>

class Parametrics {

public:

    const double PI = std::atan(1.0) * 4;

    virtual double x(double u, double v) = 0;

    virtual double y(double u, double v) = 0;

    virtual double z(double u, double v) = 0;

    virtual QVector3D getPoint(double u, double v) = 0;

    virtual QVector3D getNormal(double u, double v) = 0;

    virtual std::vector<QPair<QVector3D, QVector3D> > getSegments() = 0;
};


#endif //CG_LAB_1516_PARAMETRICS_H
