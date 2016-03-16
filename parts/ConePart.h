#ifndef CG_LAB_1516_VOLUME_KEGEL_H
#define CG_LAB_1516_VOLUME_KEGEL_H

#include "Part.h"
#include "Cone.h"
#include "Disc.h"

class ConePart : public Part {

public:

    ConePart() {
        parametrics.push_back(new Cone(5, 10));
        parametrics.push_back(new Disc(5, 10));
    }

    std::vector<QPair<QVector3D, QVector3D> > getSegments() {
        return std::vector<QPair<QVector3D, QVector3D> >();
    }
};

#endif //CG_LAB_1516_VOLUME_KEGEL_H
