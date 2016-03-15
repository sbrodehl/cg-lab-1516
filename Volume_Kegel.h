#ifndef CG_LAB_1516_VOLUME_KEGEL_H
#define CG_LAB_1516_VOLUME_KEGEL_H

#include "Parametrics.h"
#include "Volume.h"
#include "Kegel.h"
#include "ClosedCircle.h"

class Volume_Kegel : public Volume {
private:
    std::vector<Parametrics*> parametrics;

public:

    Volume_Kegel(){
        parametrics.push_back(new Kegel(5, 10));
        parametrics.push_back(new ClosedCircle(5, 0));
    }

    std::vector<QVector3D> triangulate(double delta, double eps) {
        std::vector<QVector3D> pointvec;

        for(Parametrics* parametric : parametrics){
            std::vector<QVector3D> p = parametric->triangulate(delta, eps);
            pointvec.insert(std::end(pointvec), std::begin(p), std::end(p));
        }

        return pointvec;
    }

    std::vector<QPair<QVector3D, QVector3D> > getSegments() {
        return std::vector<QPair<QVector3D, QVector3D> >();
    }
};

#endif //CG_LAB_1516_VOLUME_KEGEL_H
