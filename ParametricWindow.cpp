#include "ParametricWindow.h"
#include "Parametrics.h"

ParametricWindow::ParametricWindow(QWidget *parent) : QMainWindow(parent) {
    resize(256, 256);
}

void ParametricWindow::initParams(Parametrics *p) {
    // init window with parametric eq ...
    QString title(p->getName() + QString(" :: ") + p->getUID().toString());
    setWindowTitle(title);
}
