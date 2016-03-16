#ifndef CG_LAB_1516_PARAMETRICWINDOW_H
#define CG_LAB_1516_PARAMETRICWINDOW_H

#include <QMainWindow>
#include "Part.h"


class ParametricWindow : public QMainWindow {

public:
    ParametricWindow(Part &part, QWidget *parent = 0);
};


#endif //CG_LAB_1516_PARAMETRICWINDOW_H
