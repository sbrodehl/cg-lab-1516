#ifndef CG_LAB_1516_VOLUME_ZYLINDER_H
#define CG_LAB_1516_VOLUME_ZYLINDER_H

#include "Part.h"
#include "Tube.h"
#include "Disc.h"

class CylinderPart : public Part {

public:

    CylinderPart() {
        parametrics.push_back(new Tube(5, 10));
        parametrics.push_back(new Disc(5, 10));
        parametrics.push_back(new Disc(5, 0));
    }

    std::vector<QPair<QVector3D, QVector3D> > getSegments() {
        return std::vector<QPair<QVector3D, QVector3D> >();
    }
};

#endif //CG_LAB_1516_VOLUME_ZYLINDER_H
