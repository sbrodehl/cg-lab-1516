#ifndef CG_LAB_1516_TORUSPART_H
#define CG_LAB_1516_TORUSPART_H

#include "Part.h"
#include "Torus.h"

class TorusPart : public Part {

public:

    TorusPart() {
        parametrics.push_back(std::shared_ptr<Parametrics>(new Torus(10, 1)));
    }

};


#endif //CG_LAB_1516_TORUSPART_H
