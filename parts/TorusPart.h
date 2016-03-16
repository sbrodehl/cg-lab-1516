#ifndef CG_LAB_1516_TORUSPART_H
#define CG_LAB_1516_TORUSPART_H

#include "Part.h"
#include "Torus.h"

class TorusPart : public Part {

public:

    TorusPart() {
        parametrics.push_back(std::shared_ptr<Parametrics>(new Torus(10, 1)));
    }

    std::vector<QPair<QVector3D, QVector3D> > getSegments() {
        return std::vector<QPair<QVector3D, QVector3D> >();
    }
};


#endif //CG_LAB_1516_TORUSPART_H
