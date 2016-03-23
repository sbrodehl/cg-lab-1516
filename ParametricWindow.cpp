#include "ParametricWindow.h"
#include "Parametrics.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QKeyEvent>

#include <iostream>

ParametricWindow::ParametricWindow(QWidget *parent) : QMainWindow(parent) {
    resize(256, 256);

    area = new ParamRenderArea(QPainterPath(), this);

    // Create a nice frame to put around the OpenGL widget
    QFrame *f = new QFrame(this);
    f->setFrameStyle(QFrame::Sunken | QFrame::Panel);
    f->setLineWidth(2);

    QMenu *file = new QMenu("&File", this);
    addAction(file->addAction("Save", this, SLOT(saveShapes())));
    addAction(file->addAction("Load", this, SLOT(loadShapes())));
    addAction(file->addAction("Clear", this, SLOT(clearShapes())));
    menuBar()->addMenu(file);

    // Put the GL widget inside the frame
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(area);
    layout->setMargin(0);
    f->setLayout(layout);

    setCentralWidget(f);

    statusBar()->showMessage("Draw a shape and press enter.");
}

void ParametricWindow::initParams(Parametrics *p) {
    // init window with parametric eq ...
    QString title(p->getName() + QString(" :: ") + p->getUID().toString());
    setWindowTitle(title);
}

ParamRenderArea *ParametricWindow::getArea() const {
    return area;
}

void ParametricWindow::keyPressEvent(QKeyEvent *event) {
    area->keyPressEvent(event);
}

void ParametricWindow::clearShapes() {
    area->clearShapes();
}

void ParametricWindow::mouseDoubleClickEvent(QMouseEvent *event) {
    area->mouseDoubleClickEvent(event);
}

void ParametricWindow::drawTriangulation(std::vector<ParameterTriangle *> triangulation) {
    area->drawTriangulation(triangulation);
}

void ParametricWindow::loadShapes() {
    //TODO implement loading shapes from file
}

void ParametricWindow::saveShapes() {
    QJsonArray json;
    for (QList<QPointF> shape : area->getShapes()) {
        QJsonArray s;
        for (QPointF point : shape) {
            QJsonObject p;
            p["x"] = point.x();
            p["y"] = point.y();
            s.push_back(p);
        }
        json.push_back(s);
    }
    QJsonDocument saveDoc(json);
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    QString fname = dialog.getSaveFileName();
    std::cout << "Saving shapes in " << fname.toStdString() << std::endl;
    QFile file(fname);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << saveDoc.toJson() << endl;
    }
    //TODO implement saving shapes to file
}
