#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QSlider>

#include "ParametricsViewer.h"
#include "CylinderPart.h"
#include "ConePart.h"
#include "TorusPart.h"
#include "TrefoilPart.h"

#include <fstream>
#include <PolygonTriangulationC/TriangulationAlgorithm.h>

CGMainWindow *w;

CGMainWindow::CGMainWindow(QWidget *parent) : QMainWindow(parent) {
    resize(512, 512);

    // Create a menu
    QMenu *file = new QMenu("&File", this);
    addAction(file->addAction("Refresh", this, SLOT(updateTriangulation()), Qt::Key_F5));
    addAction(file->addAction("Wireframe", this, SLOT(toggleWireframe())));
    addAction(file->addAction("Quit", qApp, SLOT(quit()), Qt::CTRL + Qt::Key_Q));
    menuBar()->addMenu(file);

    QMenu *view = new QMenu("&Parametrics", this);
    view->addAction("Torus", this, SLOT(loadTorusPart()));
    view->addAction("Trefoil", this, SLOT(loadTrefoilPart()));
    view->addAction("Cone", this, SLOT(loadKegelPart()));
    view->addAction("Cylinder", this, SLOT(loadZylinderPart()));
    menuBar()->addMenu(view);

    QSlider *slider = new QSlider(Qt::Vertical, this);
    slider->setMinimum(1);
    slider->setMaximum(100);
    slider->setSliderPosition(100);
    setRefinementDelta(100);
    slider->setSingleStep(1);
    slider->setPageStep(2);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(changedDeltaSlider(int)));

    // Create a nice frame to put around the OpenGL widget
    QFrame *f = new QFrame(this);
    f->setFrameStyle(QFrame::Sunken | QFrame::Panel);
    f->setLineWidth(0);

    // Create our OpenGL widget
    ogl = new CGView(this, f);

    // Put the GL widget inside the frame
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(ogl);
    layout->addWidget(slider);
    layout->setMargin(0);
    f->setLayout(layout);

    setCentralWidget(f);

    statusBar()->showMessage("Ready", 1000);
}

CGMainWindow::~CGMainWindow() { }

CGView::CGView(CGMainWindow *mainwindow, QWidget *parent) : QGLWidget(parent) {
    main = mainwindow;
    // get those keyboard events ...
    setFocusPolicy(Qt::StrongFocus);
}

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    if (!QGLFormat::hasOpenGL()) {
        qWarning("This system has no OpenGL support. Exiting.");
        return 1;
    }
    w = new CGMainWindow(NULL);
    w->show();
    return app.exec();
}

void CGMainWindow::loadEq(Part &part) {
    part.showParamWindows(this);
    ogl->triangles = part.triangulate(refineDelta);
    statusBar()->showMessage("Loaded " + QString::number(ogl->triangles.size() / 6) + " triangles.");
    ogl->initVBO(ogl->triangles);

    float x1, x2, y1, y2, z1, z2;
    x1 = ogl->min.x();
    y1 = ogl->min.y();
    z1 = ogl->min.z();
    x2 = ogl->max.x();
    y2 = ogl->max.y();
    z2 = ogl->max.z();
    for (size_t i = 0; i < ogl->triangles.size(); i++) {
        x1 = std::min(x1, ogl->triangles[i].x());
        x2 = std::max(x2, ogl->triangles[i].x());
        y1 = std::min(y1, ogl->triangles[i].y());
        y2 = std::max(y2, ogl->triangles[i].y());
        z1 = std::min(z1, ogl->triangles[i].z());
        z2 = std::max(z2, ogl->triangles[i].z());
    }
    ogl->min = QVector3D(x1, y1, z1);
    ogl->max = QVector3D(x2, y2, z2);
    QVector3D extent = ogl->max - ogl->min;
    ogl->zoom = 1.5f / std::max(std::max(extent.x(), extent.y()), extent.z());
    ogl->center = (ogl->min + ogl->max) / 2;
    ogl->updateGL();
}

void CGMainWindow::loadTrefoilPart() {
    ogl->clearGL();
    if (viewPart != nullptr)
        viewPart->destroyWindows();
    viewPart = new TrefoilPart();
    loadEq(*viewPart);
}

void CGMainWindow::loadTorusPart() {
    ogl->clearGL();
    if (viewPart != nullptr)
        viewPart->destroyWindows();
    viewPart = new TorusPart();
    loadEq(*viewPart);
}

void CGMainWindow::loadZylinderPart() {
    ogl->clearGL();
    if (viewPart != nullptr)
        viewPart->destroyWindows();
    viewPart = new CylinderPart();
    loadEq(*viewPart);
}

void CGMainWindow::loadKegelPart() {
    ogl->clearGL();
    if (viewPart != nullptr)
        viewPart->destroyWindows();
    viewPart = new ConePart();
    loadEq(*viewPart);
}

void CGMainWindow::changedDeltaSlider(int value) {
    setRefinementDelta(value);
    statusBar()->showMessage("Refinement delta set to " + QString::number(refineDelta));
}

void CGMainWindow::updateTriangulation() {
    updateEq();
}

void CGMainWindow::updateEq() {
    ogl->clearGL();
    if (viewPart != nullptr)
        ogl->triangles = viewPart->triangulate(refineDelta);
    ogl->initVBO(ogl->triangles);
    float x1, x2, y1, y2, z1, z2;
    x1 = ogl->min.x();
    y1 = ogl->min.y();
    z1 = ogl->min.z();
    x2 = ogl->max.x();
    y2 = ogl->max.y();
    z2 = ogl->max.z();
    for (size_t i = 0; i < ogl->triangles.size(); i++) {
        x1 = std::min(x1, ogl->triangles[i].x());
        x2 = std::max(x2, ogl->triangles[i].x());
        y1 = std::min(y1, ogl->triangles[i].y());
        y2 = std::max(y2, ogl->triangles[i].y());
        z1 = std::min(z1, ogl->triangles[i].z());
        z2 = std::max(z2, ogl->triangles[i].z());
    }
    ogl->min = QVector3D(x1, y1, z1);
    ogl->max = QVector3D(x2, y2, z2);
    QVector3D extent = ogl->max - ogl->min;
    ogl->zoom = 1.5f / std::max(std::max(extent.x(), extent.y()), extent.z());
    ogl->center = (ogl->min + ogl->max) / 2;
    statusBar()->showMessage("Loaded " + QString::number(ogl->triangles.size() / 6) + " triangles.");
    ogl->updateGL();
}

void CGMainWindow::toggleWireframe() {
    ogl->toggleWireframe();
}

void CGMainWindow::setRefinementDelta(int value) {
    refineDelta = value * 0.01f;
}


