#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QHBoxLayout>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>

#include <QVector3D>

#include "DicomReader.h"

CGMainWindow *w;

CGMainWindow::CGMainWindow (QWidget* parent) : QMainWindow (parent) {
    resize (604, 614);

    // Create a menu
    QMenu *file = new QMenu("&File",this);
    file->addAction ("Load dicom", this, SLOT(loadDicom()), Qt::CTRL+Qt::Key_L);
    file->addAction ("Quit", qApp, SLOT(quit()), Qt::CTRL+Qt::Key_Q);

    menuBar()->addMenu(file);

    QMenu *view = new QMenu("&Extra",this);
    view->addAction ("Quit", qApp, SLOT(quit()), Qt::CTRL+Qt::Key_Q);
    menuBar()->addMenu(view);

    // Create a nice frame to put around the OpenGL widget
    QFrame* f = new QFrame (this);
    f->setFrameStyle(QFrame::Sunken | QFrame::Panel);
    f->setLineWidth(2);

    // Create our OpenGL widget
    ogl = new MyGLWidget (this,f);

    // Put the GL widget inside the frame
    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(ogl);
    layout->setMargin(0);
    f->setLayout(layout);

    setCentralWidget(f);

    statusBar()->showMessage("Ready",1000);
}

CGMainWindow::~CGMainWindow () {}

void CGMainWindow::keyPressEvent(QKeyEvent* event) {
    switch(event->key()) {
    case Qt::Key_Plus: break;
    case Qt::Key_Minus: break;
    case Qt::Key_Right: break;
    case Qt::Key_Left: break;
    case Qt::Key_Up: ogl->center += QVector3D(0,0,0.007); break;
    case Qt::Key_Down: ogl->center += QVector3D(0,0,-0.007); break;
    case Qt::Key_PageUp: break;
    case Qt::Key_PageDown: break;
    case Qt::Key_S: break;
    case Qt::Key_A: break;
    }

    ogl->updateGL();
}

bool intersectLine(const QVector3D& a,const QVector3D& b,const QVector3D& c,const QVector3D& p,const QVector3D& q,QVector3D& s,qreal& d,int dir) {
    QVector3D n = QVector3D::normal(a,b,c);
    if (dir != 0) {
        qreal npq = QVector3D::dotProduct(n,p-q);
        if ((npq > 0) && (dir < 0)) return false;
        if ((npq < 0) && (dir > 0)) return false;
    }
    d = QVector3D::dotProduct(n,q-p);
    if (d == 0) return false;
    d = QVector3D::dotProduct(n,a-p)/d;
    s = p+(q-p)*d;

    if (QVector3D::dotProduct(n,QVector3D::crossProduct(b-a,s-a)) < 0) return false;
    if (QVector3D::dotProduct(n,QVector3D::crossProduct(c-b,s-b)) < 0) return false;
    if (QVector3D::dotProduct(n,QVector3D::crossProduct(a-c,s-c)) < 0) return false;

    return true;
}

void getFromStlFile(std::vector<QVector3D>& triangles, const char *filename) {
    std::ifstream instream(filename,std::ios::binary);
    if (!instream) {
        std::cerr << "file does not exist!" << std::endl;
        return;
    }

    instream.seekg( 80, std::ios_base::beg ); // skip ascii header
    int trinum = 0;
    instream.read((char*) &trinum, 4 ); // number of triangles
    float tmp;
    for(int k = 0; k < trinum; k++) {
        for(int i=0;i < 3 ; i++ )
            instream.read( (char*) &tmp, 4 );
        for(int i = 0; i < 3; i++ ) {
            qreal v[3];
            for(int j = 0 ; j < 3 ; j++) {
                instream.read( (char*) &tmp, 4 );
                v[j] = tmp;
            }
            triangles.push_back(QVector3D(v[0],v[1],v[2]));
        }
        instream.read( (char*) &tmp, 2);
    }

    instream.close();
}

MyGLWidget::MyGLWidget (CGMainWindow *mainwindow,QWidget* parent ) : QGLWidget (parent) {
    main = mainwindow;
}

void MyGLWidget::initShaders() {
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

void MyGLWidget::initializeGL() {
    initializeGLFunctions();
    initShaders();

    qglClearColor(Qt::black);

    glEnable(GL_TEXTURE_3D);
    // glEnable(GL_DEPTH_TEST);

    zoom = 1.0;
    qNow = QQuaternion(1,0,0,0);
    min = QVector3D( 1.0, 1.0, 1.0);
    max = QVector3D(-1.0,-1.0,-1.0);
    center = QVector3D(0,0,0);

    glGenBuffers(1,&vboQuadId);

    std::vector<QVector3D> vertex;
    vertex.push_back(QVector3D(-2,-2, 0));
    vertex.push_back(QVector3D( 0, 0, 0));
    vertex.push_back(QVector3D( 2,-2, 0));
    vertex.push_back(QVector3D( 1, 0, 0));
    vertex.push_back(QVector3D( 2, 2, 0));
    vertex.push_back(QVector3D( 1, 1, 0));
    vertex.push_back(QVector3D(-2, 2, 0));
    vertex.push_back(QVector3D( 0, 1, 0));

    glBindBuffer(GL_ARRAY_BUFFER,vboQuadId);
    glBufferData(GL_ARRAY_BUFFER,vertex.size()*sizeof(QVector3D),vertex.data(),GL_STATIC_DRAW);

}

void MyGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    QMatrix4x4 P(projection);
    P.scale(zoom,zoom,zoom);

    modelView.setToIdentity();
    modelView.rotate(qNow.conjugate());
    modelView.translate(-center);

    program.bind();

    program.setUniformValue("uPMat", P);
    program.setUniformValue("uMVMat", modelView);
    program.setUniformValue("uColor", QVector4D(1.0,0.0,0.0,1.0));

    glBindBuffer(GL_ARRAY_BUFFER, vboQuadId);
    int vertexLocation = program.attributeLocation("a_position");
    program.enableAttributeArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 2*sizeof(QVector3D), 0);
    int texAttribLoc = program.attributeLocation("aTexCoord");
    program.enableAttributeArray(texAttribLoc);
    glVertexAttribPointer(texAttribLoc, 3, GL_FLOAT, GL_FALSE, 2*sizeof(QVector3D), (const void*) sizeof(QVector3D));

    glDrawArrays(GL_QUADS,0,4);
}

void MyGLWidget::mouseToTrackball(int x, int y, int W, int H, QVector3D &v) {
        if (W > H) {
                v[0] = (2.0*x-W)/H;
                v[1] = 1.0-y*2.0/H;
        } else {
                v[0] = (2.0*x-W)/W;
                v[1] = (H-2.0*y)/W;
        }
        double d = v[0]*v[0]+v[1]*v[1];
        if (d > 1.0) {
                v[2] = 0.0;
                v /= sqrt(d);
        } else v[2] = sqrt(1.0-d*d);
}

QQuaternion MyGLWidget::trackball(const QVector3D& u, const QVector3D& v) {
        QVector3D uxv = QVector3D::crossProduct(u,v);
        QQuaternion ret(1.0+QVector3D::dotProduct(u,v),uxv[0],uxv[1],uxv[2]);
        ret.normalize();
        return ret;
}

void MyGLWidget::resizeGL(int w, int h) {
    width = w;
    height = h;
    glViewport(0,0,width,height);
    projection.setToIdentity();
    if (width > height) {
        qreal ratio = width/(qreal) height;
        projection.ortho(-ratio,ratio,-1.0,1.0,-10.0,10.0);
    } else {
        qreal ratio = height/(qreal) width;
        projection.ortho(-1.0,1.0,-ratio,ratio,-10.0,10.0);
    }
}

void MyGLWidget::pickLine(int x,int y,QVector3D& u,QVector3D& v) {
    modelView.setToIdentity();
    modelView.scale(zoom,zoom,zoom);
    modelView.translate(-center);

    QMatrix4x4 T = (projection*modelView).inverted();

    qreal xn = (2.0*x)/width-1.0;
    qreal yn = 1.0-(2.0*y)/height;

    u = T * QVector3D(xn,yn,0.0);
    v = T * QVector3D(xn,yn,1.0);
}

bool MyGLWidget::pick(int x,int y,QVector3D& pickPoint) {
    QVector3D u,v;
    pickLine(x,y,u,v);

    qreal inf = std::numeric_limits<qreal>::infinity();
    qreal dmin = inf;

    return (dmin < inf);
}

void MyGLWidget::mousePressEvent(QMouseEvent *event) {
    button = event->button();
    oldX = event->x();
    oldY = event->y();

    pointPicked = pick(oldX,oldY,pickPoint);
    if (pointPicked) std::cout << "pickPoint = [" << pickPoint.x() << "," << pickPoint.y() << "," << pickPoint.z() << "]" << std::endl;

    update();
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent*) {}

void MyGLWidget::wheelEvent(QWheelEvent* event) {
    if (event->delta() < 0) zoom *= 1.2; else zoom *= 1/1.2;
    update();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent* event) {
    if (button == Qt::LeftButton) {
        QVector3D p1,p2;

        mouseToTrackball(oldX,oldY,width,height,p1);
        mouseToTrackball(event->x(),event->y(),width,height,p2);

        QQuaternion q = trackball(p1,p2);
        QQuaternion qT = qNow.conjugate() * q.conjugate() * qNow;
        qNow = q * qNow;
        qNow.normalize();
        if (pointPicked)
            center = qT.rotatedVector(center - pickPoint) + pickPoint;
    }

    oldX = event->x();
    oldY = event->y();

    updateGL();
}

int main (int argc, char **argv) {
    QApplication app(argc, argv);

    if (!QGLFormat::hasOpenGL()) {
        qWarning ("This system has no OpenGL support. Exiting.");
        return 1;
    }

    w = new CGMainWindow(NULL);

    w->show();

    return app.exec();
}

