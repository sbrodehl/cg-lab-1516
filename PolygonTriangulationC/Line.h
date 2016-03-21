#ifndef CG_LAB_1516_LINE_H_H
#define CG_LAB_1516_LINE_H_H

#include <QVector2D>

class Line {
private:
    // why do we use pointer here?
    QVector2D *v1;
    QVector2D *v2;

public:
    bool visited = false;

    Line(QVector2D *v1_new, QVector2D *v2_new) { // : v1(...) , v2(...) {
        v1 = v1_new;
        v2 = v2_new;
    }

    QVector2D *getv1() {
        return v1;
    }

    QVector2D *getv2() {
        return v2;
    }

    void setV1(QVector2D *v1_new) {
        v1 = v1_new;
    }

    void setV2(QVector2D *v2_new) {
        v2 = v2_new;
    }
};

#endif //CG_LAB_1516_LINE_H_H
