#ifndef CG_LAB_1516_EDGE_H
#define CG_LAB_1516_EDGE_H

#include "Vertex.h"

class Edge {
private:
    Vertex *v1;
    Vertex *v2;

public:
    bool visited = false;

    Edge(Vertex *v1_new, Vertex *v2_new) {
        v1 = v1_new;
        v2 = v2_new;
    }

    Vertex *getv1() {
        return v1;
    }

    Vertex *getv2() {
        return v2;
    }

    void setV1(Vertex *v1_new) {
        v1 = v1_new;
    }

    void setV2(Vertex *v2_new) {
        v2 = v2_new;
    }
};

#endif //CG_LAB_1516_EDGE_H
