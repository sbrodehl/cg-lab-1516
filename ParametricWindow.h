#ifndef CG_LAB_1516_PARAMETRICWINDOW_H
#define CG_LAB_1516_PARAMETRICWINDOW_H

#include <QMainWindow>
#include "Part.h"


class ParametricWindow : public QMainWindow {

public:
    ParametricWindow(QWidget *parent = 0);

    ParametricWindow(Part &part);
};


#endif //CG_LAB_1516_PARAMETRICWINDOW_H
