
#ifndef CG_LAB_1516_PARAMETRICS_H
#define CG_LAB_1516_PARAMETRICS_H

#include <QVector3D>
#include <cmath>

class Parametrics {

public:

    const double PI = std::atan(1.0)*4;

    Parametrics();
    ~Parametrics();

    void loadEq();
    QVector3D getPoint(double u, double v);
    QVector3D parameterizedTorus(float u, float v);
};


#endif //CG_LAB_1516_PARAMETRICS_H
