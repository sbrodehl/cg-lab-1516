#include "ParametricWindow.h"
#include "Parametrics.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QStatusBar>
#include <iostream>

ParametricWindow::ParametricWindow(QWidget *parent) : QMainWindow(parent) {
    resize(256, 256);

    area = new ParamRenderArea(QPainterPath(), this);

    // Create a nice frame to put around the OpenGL widget
    QFrame *f = new QFrame(this);
    f->setFrameStyle(QFrame::Sunken | QFrame::Panel);
    f->setLineWidth(2);

    // Put the GL widget inside the frame
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(area);
    layout->setMargin(0);
    f->setLayout(layout);

    setCentralWidget(f);

    statusBar()->showMessage("Ready!");
}

void ParametricWindow::initParams(Parametrics *p) {
    // init window with parametric eq ...
    QString title(p->getName() + QString(" :: ") + p->getUID().toString());
    setWindowTitle(title);
}

void ParametricWindow::updateTriangulation() {
    std::cout << "push it up..." << std::endl;
}


ParamRenderArea *ParametricWindow::getArea() const {
    return area;
}