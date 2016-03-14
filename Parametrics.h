
#ifndef CG_LAB_1516_PARAMETRICS_H
#define CG_LAB_1516_PARAMETRICS_H

#include <QVector3D>

class Parametrics {

public:

    Parametrics();
    ~Parametrics();

    void loadEq();
    QVector3D getPoint(double u, double v);

};


#endif //CG_LAB_1516_PARAMETRICS_H
