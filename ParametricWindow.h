#ifndef CG_LAB_1516_PARAMETRICWINDOW_H
#define CG_LAB_1516_PARAMETRICWINDOW_H

#include <QMainWindow>
#include "ParamRenderArea.h"

class CGMainWindow;
class Parametrics;

class ParametricWindow : public QMainWindow {
Q_OBJECT

public:
    ParametricWindow(QWidget *parent = 0);
    void initParams(Parametrics *p);
    ParamRenderArea *getArea() const;

public slots:

    void updateTriangulation();

private:
    ParamRenderArea *area;
};

#endif //CG_LAB_1516_PARAMETRICWINDOW_H
