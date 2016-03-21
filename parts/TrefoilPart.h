#ifndef CG_LAB_1516_TREFOILPART_H
#define CG_LAB_1516_TREFOILPART_H

#include "Part.h"
#include "Trefoil.h"

class TrefoilPart : public Part {

public:

    TrefoilPart() {
        parametrics.push_back(std::shared_ptr<Parametrics>(new Trefoil()));
    }

};


#endif //CG_LAB_1516_TREFOILPART_H
