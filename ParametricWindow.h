#ifndef CG_LAB_1516_PARAMETRICWINDOW_H
#define CG_LAB_1516_PARAMETRICWINDOW_H

#include <QMainWindow>

class Parametrics;

class ParametricWindow : public QMainWindow {

public:
    ParametricWindow(QWidget *parent = 0);

    void initParams(Parametrics *p);
};

#endif //CG_LAB_1516_PARAMETRICWINDOW_H
