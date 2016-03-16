#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QSlider>

#include "StlReader.h"
#include "CylinderPart.h"
#include "ConePart.h"
#include "TorusPart.h"
#include "TrefoilPart.h"

#include <fstream>

CGMainWindow *w;

CGMainWindow::CGMainWindow(QWidget *parent) : QMainWindow(parent) {
    resize(512, 512);

    // Create a menu
    QMenu *file = new QMenu("&File", this);
    file->addAction("Quit", qApp, SLOT(quit()), Qt::CTRL + Qt::Key_Q);
    menuBar()->addMenu(file);

    QMenu *view = new QMenu("&Parametrics", this);
    view->addAction("Torus", this, SLOT(loadTorusPart()));
    view->addAction("Trefoil", this, SLOT(loadTrefoilPart()));
    view->addAction("Cone", this, SLOT(loadKegelPart()));
    view->addAction("Cylinder", this, SLOT(loadZylinderPart()));
    menuBar()->addMenu(view);

    QSlider *slider = new QSlider(Qt::Vertical, this);
    slider->setMinimum(0);
    slider->setMaximum(100);
    slider->setSliderPosition(50);
    slider->setSingleStep(5);
    slider->setPageStep(20);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(changedDeltaSlider(int)));

    // Create a nice frame to put around the OpenGL widget
    QFrame *f = new QFrame(this);
    f->setFrameStyle(QFrame::Sunken | QFrame::Panel);
    f->setLineWidth(2);

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

CGMainWindow::~CGMainWindow() {}

CGView::CGView(CGMainWindow *mainwindow, QWidget *parent) : QGLWidget(parent) {
    main = mainwindow;
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
    int delta = 64;
    int eps = 64;

    ogl->triangles = part.triangulate(delta, eps);

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
    ogl->initVBO(ogl->triangles);

    QVector3D extent = ogl->max - ogl->min;
    ogl->zoom = 1.5 / std::max(std::max(extent.x(), extent.y()), extent.z());
    ogl->center = (ogl->min + ogl->max) / 2;

    part.showParamWindows(this);

    statusBar()->showMessage("Loaded " + QString::number(ogl->triangles.size()) + " triangles.");

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
    // std::cout << value << std::endl;
}
