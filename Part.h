#ifndef CG_LAB_1516_VOLUME_H
#define CG_LAB_1516_VOLUME_H

#include "Parametrics.h"

#include <QVector3D>

class Part {

protected:
    std::vector<Parametrics *> parametrics;

public:
    std::vector<QVector3D> triangulate(double delta, double eps) {
        std::vector<QVector3D> pointvec;

        for (Parametrics *parametric : parametrics) {
            std::vector<QVector3D> p = parametric->triangulate(delta, eps);
            pointvec.insert(std::end(pointvec), std::begin(p), std::end(p));
        }

        return pointvec;
    }

    virtual std::vector<QPair<QVector3D, QVector3D> > getSegments() = 0;
};

#endif //CG_LAB_1516_VOLUME_H
