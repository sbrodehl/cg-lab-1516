
#ifndef CG_LAB_1516_VOLUME_H
#define CG_LAB_1516_VOLUME_H

#include <QVector3D>

class Volume {

public:
    virtual std::vector<QVector3D> triangulate(double delta, double eps) = 0;

    virtual std::vector<QPair<QVector3D, QVector3D> > getSegments() = 0;
};

#endif //CG_LAB_1516_VOLUME_H
