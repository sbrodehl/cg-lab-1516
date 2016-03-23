#ifndef CG_LAB_1516_PARAMETRICWINDOW_H
#define CG_LAB_1516_PARAMETRICWINDOW_H

#include <QMainWindow>

#include "ParameterTriangle.h"
#include "ParamRenderArea.h"

class CGMainWindow;
class Parametrics;

class ParametricWindow : public QMainWindow {
Q_OBJECT

public:
    ParametricWindow(QWidget *parent = 0);
    void initParams(Parametrics *p);
    ParamRenderArea *getArea() const;
    void drawTriangulation(std::vector<ParameterTriangle *> triangulation);

    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

public slots:
    void saveShapes();
    void loadShapes();
    void clearShapes();

private:
    ParamRenderArea *area;
};

#endif //CG_LAB_1516_PARAMETRICWINDOW_H
