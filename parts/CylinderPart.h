#ifndef CG_LAB_1516_VOLUME_ZYLINDER_H
#define CG_LAB_1516_VOLUME_ZYLINDER_H

#include "Part.h"
#include "Tube.h"
#include "Disc.h"

class CylinderPart : public Part {

public:

    CylinderPart() {
        parametrics.push_back(std::shared_ptr<Parametrics>(new Tube(5, 10)));
        parametrics.push_back(std::shared_ptr<Parametrics>(new Disc(5, 10)));
        parametrics.push_back(std::shared_ptr<Parametrics>(new Disc(5, 0)));
    }

};

#endif //CG_LAB_1516_VOLUME_ZYLINDER_H
