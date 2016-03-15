#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QSlider>

#include "StlReader.h"
#include "Trefoil.h"
#include "Torus.h"
#include "Zylinder.h"
#include "Kegel.h"
#include "Volume.h"
#include "Volume_Zylinder.h"
#include "Volume_Kegel.h"

#include <fstream>

CGMainWindow *w;

CGMainWindow::CGMainWindow(QWidget *parent)
        : QMainWindow(parent) {
    resize(768, 768);

    // Create a menu
    QMenu *file = new QMenu("&File", this);
    file->addAction("Load STL", this, SLOT(loadModel()));
    file->addAction("Quit", qApp, SLOT(quit()), Qt::CTRL + Qt::Key_Q);

    menuBar()->addMenu(file);

    QMenu *view = new QMenu("&Parametrics", this);
    view->addAction("Torus", this, SLOT(loadTorusParam()));
    view->addAction("Kegel", this, SLOT(loadKegelParam()));
    view->addAction("Zylinder", this, SLOT(loadZylinderParam()));
    menuBar()->addMenu(view);

    QSlider *slider = new QSlider(Qt::Vertical, this);
    slider->setMinimum(0);
    slider->setMaximum(100);
    slider->setSliderPosition(50);
    // The smaller of two natural steps that an abstract sliders provides
    // and typically corresponds to the user pressing an arrow key.
    slider->setSingleStep(5);
    // The larger of two natural steps that an abstract slider provides
    // and typically corresponds to the user pressing PageUp or PageDown.
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

CGMainWindow::~CGMainWindow() { }

void CGMainWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Plus:
            break;
        case Qt::Key_Minus:
            break;
        case Qt::Key_Right:
            break;
        case Qt::Key_Left:
            break;
        case Qt::Key_Up:
            break;
        case Qt::Key_Down:
            break;
        case Qt::Key_PageUp:
            break;
        case Qt::Key_PageDown:
            break;
        case Qt::Key_S:
            break;
        case Qt::Key_A:
            break;
    }

    ogl->updateGL();
}

bool intersectLine(const QVector3D &a, const QVector3D &b, const QVector3D &c, const QVector3D &p, const QVector3D &q,
                   QVector3D &s, qreal &d, int dir) {
    QVector3D n = QVector3D::normal(a, b, c);
    if (dir != 0) {
        qreal npq = QVector3D::dotProduct(n, p - q);
        if ((npq > 0) && (dir < 0)) return false;
        if ((npq < 0) && (dir > 0)) return false;
    }
    d = QVector3D::dotProduct(n, q - p);
    if (d == 0) return false;
    d = QVector3D::dotProduct(n, a - p) / d;
    s = p + (q - p) * d;

    if (QVector3D::dotProduct(n, QVector3D::crossProduct(b - a, s - a)) < 0) return false;
    if (QVector3D::dotProduct(n, QVector3D::crossProduct(c - b, s - b)) < 0) return false;
    if (QVector3D::dotProduct(n, QVector3D::crossProduct(a - c, s - c)) < 0) return false;

    return true;
}

void getFromStlFile(std::vector<QVector3D> &triangles, const char *filename) {
    std::ifstream instream(filename, std::ios::binary);
    if (!instream) {
        std::cerr << "file does not exist!" << std::endl;
        return;
    }

    instream.seekg(80, std::ios_base::beg); // skip ascii header
    int trinum = 0;
    instream.read((char *) &trinum, 4); // number of triangles
    float tmp;
    for (int k = 0; k < trinum; k++) {
        for (int i = 0; i < 3; i++)
            instream.read((char *) &tmp, 4);
        for (int i = 0; i < 3; i++) {
            qreal v[3];
            for (int j = 0; j < 3; j++) {
                instream.read((char *) &tmp, 4);
                v[j] = tmp;
            }
            triangles.push_back(QVector3D(v[0], v[1], v[2]));
        }
        instream.read((char *) &tmp, 2);
    }

    instream.close();
}

void CGMainWindow::loadModel() {
    QString filename = QFileDialog::getOpenFileName(this, "Load model ...", QString(), "(*.stl)");

    if (filename.isEmpty()) return;
    statusBar()->showMessage("Loading model ...");

    if (filename.endsWith(".stl"))
        getFromStlFile(ogl->triangles, filename.toLatin1());

    if (ogl->triangles.size() > 0) {
        std::cout << "nu tris = " << ogl->triangles.size() / 3 << std::endl;

        qreal x1, x2, y1, y2, z1, z2;
        x1 = ogl->min.x();
        y1 = ogl->min.y();
        z1 = ogl->min.z();
        x2 = ogl->max.x();
        y2 = ogl->max.y();
        z2 = ogl->max.z();

        for (size_t i = 0; i < ogl->triangles.size(); i++) {
            x1 = std::min((float) x1, ogl->triangles[i].x());
            x2 = std::max((float) x2, ogl->triangles[i].x());
            y1 = std::min((float) y1, ogl->triangles[i].y());
            y2 = std::max((float) y2, ogl->triangles[i].y());
            z1 = std::min((float) z1, ogl->triangles[i].z());
            z2 = std::max((float) z2, ogl->triangles[i].z());
        }

        ogl->min = QVector3D(x1, y1, z1);
        ogl->max = QVector3D(x2, y2, z2);

        ogl->initTrianglesVBO(ogl->triangles);

        QVector3D extent = ogl->max - ogl->min;
        std::cout << "b = [" << ogl->min.x() << "," << ogl->max.x() << "] x [ "
        << ogl->min.y() << "," << ogl->max.y() << "] x [ "
        << ogl->min.z() << "," << ogl->max.z() << "]" << std::endl;
        std::cout << "e = " << extent.x() << " " << extent.y() << " " << extent.z() << std::endl;
        ogl->zoom = 1.5 / std::max(std::max(extent.x(), extent.y()), extent.z());
        ogl->center = (ogl->min + ogl->max) / 2;
    }

    statusBar()->showMessage("Loading model done.", 3000);
    ogl->updateGL();
}

CGView::CGView(CGMainWindow *mainwindow, QWidget *parent) : QGLWidget(parent) {
    main = mainwindow;
}

void CGView::initShaders() {
    setlocale(LC_NUMERIC, "C");

    if (!program.addShaderFromSourceFile(QGLShader::Vertex, ":/vshader.glsl"))
        close();

    if (!program.addShaderFromSourceFile(QGLShader::Fragment, ":/fshader.glsl"))
        close();

    if (!program.link())
        close();

    if (!program.bind())
        close();

    setlocale(LC_ALL, "");
}

void CGView::initializeGL() {
    initializeGLFunctions();
    initShaders();
    initSolidCubeVBO();
    initSolidSphereVBO();
    initSolidCylinderVBO();
    glGenBuffers(1, &vboTmpId);

    qglClearColor(Qt::black);

    glEnable(GL_DEPTH_TEST);

    zoom = 1.0;
    phi = 0.0;
    theta = 0.0;
    // phi = -20.7;
    // theta = 19.47;

    qreal inf = std::numeric_limits<qreal>::infinity();
    min = QVector3D(inf, inf, inf);
    max = QVector3D(-inf, -inf, -inf);
    center = QVector3D(0, 0, 0);
    pointPicked = false;
}

void CGView::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    modelView.setToIdentity();
    modelView.rotate(theta, 1.0, 0.0, 0.0);
    modelView.rotate(phi, 0.0, 1.0, 0.0);
    modelView.scale(zoom, zoom, zoom);

    // program.setUniformValue("uPMat", projection);
    // program.setUniformValue("uColor", QVector4D(0.5,0.5,0.5,1.0));
    // drawSolidSphere(QVector3D(0.0,0.0,0.0),1.0);
    // drawSolidCylinder(QVector3D(0.0,0.0,0.0),QVector3D(1.0,1.0,0.0),1.0);
    // return;

    modelView.translate(-center);

    if (pointPicked) {
        program.setUniformValue("uColor", QVector4D(1.0, 0.0, 0.0, 1.0));
        drawSolidSphere(pickPoint, 0.01);
    }

    for (size_t i = 0; i < vboTrianglesId.size(); i++) {
        program.setUniformValue("uPMat", projection);
        program.setUniformValue("uMVMat", modelView);
        program.setUniformValue("uNMat", modelView.normalMatrix());
        program.setUniformValue("uColor", QVector4D(0.5, 0.5, 0.0, 1.0));

        glBindBuffer(GL_ARRAY_BUFFER, vboTrianglesId[i]);
        int vertexLocation = program.attributeLocation("a_position");
        program.enableAttributeArray(vertexLocation);
        glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(QVector3D), 0);
        int normalLocation = program.attributeLocation("a_normal");
        program.enableAttributeArray(normalLocation);
        glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(QVector3D),
                              (const void *) sizeof(QVector3D));

        glDrawArrays(GL_TRIANGLES, 0, vboTrianglesSize[i]);
    }
}

void CGView::resizeGL(int w, int h) {
    width = w;
    height = h;
    glViewport(0, 0, width, height);
    projection.setToIdentity();
    if (width > height) {
        qreal ratio = width / (qreal) height;
        projection.ortho(-ratio, ratio, -1.0, 1.0, -10.0, 10.0);
    } else {
        qreal ratio = height / (qreal) width;
        projection.ortho(-1.0, 1.0, -ratio, ratio, -10.0, 10.0);
    }
}

void CGView::pickLine(int x, int y, QVector3D &u, QVector3D &v) {
    modelView.setToIdentity();
    modelView.rotate(theta, 1.0, 0.0, 0.0);
    modelView.rotate(phi, 0.0, 1.0, 0.0);
    modelView.scale(zoom, zoom, zoom);
    modelView.translate(-center);

    QMatrix4x4 T = (projection * modelView).inverted();

    qreal xn = (2.0 * x) / width - 1.0;
    qreal yn = 1.0 - (2.0 * y) / height;

    u = T * QVector3D(xn, yn, 0.0);
    v = T * QVector3D(xn, yn, 1.0);
}

bool CGView::pick(int x, int y, QVector3D &pickPoint) {
    QVector3D u, v;
    pickLine(x, y, u, v);

    qreal inf = std::numeric_limits<qreal>::infinity();
    qreal dmin = inf;

    std::cout << triangles.size() << std::endl;

    for (size_t j = 0; j < triangles.size(); j += 3) {
        const QVector3D &a = triangles[j + 0];
        const QVector3D &b = triangles[j + 1];
        const QVector3D &c = triangles[j + 2];
        QVector3D s;
        qreal d;
        if (!intersectLine(a, b, c, u, v, s, d, 0)) continue;
        if (d < dmin) {
            dmin = d;
            pickPoint = s;
        }
    }

    return (dmin < inf);
}

void CGView::mousePressEvent(QMouseEvent *event) {
    oldX = event->x();
    oldY = event->y();

    pointPicked = pick(oldX, oldY, pickPoint);
    if (pointPicked)
        std::cout << "pickPoint = [" << pickPoint.x() << "," << pickPoint.y() << "," << pickPoint.z() << "]" <<
        std::endl;

    update();
}

void CGView::mouseReleaseEvent(QMouseEvent *) { }

void CGView::wheelEvent(QWheelEvent *event) {
    if (event->delta() < 0) zoom *= 1.2; else zoom *= 1 / 1.2;
    update();
}

void CGView::mouseMoveEvent(QMouseEvent *event) {
    phi += 0.2 * (event->x() - oldX);
    theta += 0.2 * (event->y() - oldY);
    oldX = event->x();
    oldY = event->y();

    updateGL();
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

void CGMainWindow::loadEq(Volume &volume) {
    // create points, triangulate and render here ...
    int delta = 64;
    int eps = 64;

    ogl->triangles = volume.triangulate(delta, eps);

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

    //ogl->initTrianglesVBO(ogl->triangles);
    ogl->initVBO(ogl->triangles);

    QVector3D extent = ogl->max - ogl->min;
    std::cout << "b = [" << ogl->min.x() << "," << ogl->max.x() << "] x [ "
    << ogl->min.y() << "," << ogl->max.y() << "] x [ "
    << ogl->min.z() << "," << ogl->max.z() << "]" << std::endl;
    std::cout << "e = " << extent.x() << " " << extent.y() << " " << extent.z() << std::endl;
    ogl->zoom = 1.5 / std::max(std::max(extent.x(), extent.y()), extent.z());
    ogl->center = (ogl->min + ogl->max) / 2;

    statusBar()->showMessage("Loading done.", 3000);
    ogl->updateGL();
}

void CGMainWindow::loadTorusParam() {
    Torus *torus = new Torus();
    // loadEq(*torus);
    delete torus;
}

void CGMainWindow::loadZylinderParam() {
    Volume_Zylinder *zylinder = new Volume_Zylinder();
    loadEq(*zylinder);
    delete zylinder;
}

void CGMainWindow::loadKegelParam() {
    Volume_Kegel *kegel = new Volume_Kegel();
    loadEq(*kegel);
    delete kegel;
}

void CGMainWindow::changedDeltaSlider(int value) {
    // std::cout << value << std::endl;
}
