#ifndef CG_LAB_1516_PARAMETRICWINDOW_H
#define CG_LAB_1516_PARAMETRICWINDOW_H

#include <QMainWindow>

class CGMainWindow;
class Parametrics;

class ParametricWindow : public QMainWindow {
Q_OBJECT

public:
    ParametricWindow(QWidget *parent = 0);

    void initParams(Parametrics *p);

public slots:

    void updateTriangulation();

private:

    CGMainWindow *mainWindow;
};

#endif //CG_LAB_1516_PARAMETRICWINDOW_H
