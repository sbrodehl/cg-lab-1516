#ifndef CG_LAB_1516_TREFOILPART_H
#define CG_LAB_1516_TREFOILPART_H

#include "Part.h"
#include "Trefoil.h"

class TrefoilPart : public Part {

public:

    TrefoilPart(){
        parametrics.push_back(new Trefoil());
    }

    std::vector<QPair<QVector3D, QVector3D> > getSegments() {
        return std::vector<QPair<QVector3D, QVector3D> >();
    }
};


#endif //CG_LAB_1516_TREFOILPART_H
