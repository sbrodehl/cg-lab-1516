#ifndef STLREADER_H
#define STLREADER_H

#include <QMainWindow>
#include <QGridLayout>
#include <QFrame>
#include <QtCore>
#include <QLocale>
#include <QInputDialog>

#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QGLFunctions>
#include <QtOpenGL/QGLShaderProgram>

#include <vector>
#include <iostream>
#include "Parametrics.h"

class CGView;

class CGMainWindow : public QMainWindow {
    Q_OBJECT

public:

    CGMainWindow (QWidget* parent = 0); // , Qt::WFlags flags = Qt::Window);
    ~CGMainWindow ();
    CGView *ogl;

public slots:
    void loadModel();
    void loadTorusParam();
    void loadTrefoilParam();

protected:
    void loadEq(Parametrics&);
    void keyPressEvent(QKeyEvent*);
};

class CGView : public QGLWidget, public QGLFunctions {
    Q_OBJECT

public:

    CGView(CGMainWindow*,QWidget*);
    void initShaders();
    void initializeGL();

    void pickLine(int,int,QVector3D&,QVector3D&);
    bool pick(int,int,QVector3D&);
    void initSolidCubeVBO();
    void initSolidSphereVBO();
    void initSolidCylinderVBO();
    void refineSolidSphere(const std::vector<QVector3D>&, std::vector<QVector3D>&);
    void initTrianglesVBO(const std::vector<QVector3D>&);
    void initVBO(const std::vector<QVector3D>&);
    std::vector<QVector3D> createTriangles(const std::vector<QVector3D>&, int);

    QVector3D min, max, center;
    qreal zoom, phi, theta;

    std::vector<QVector3D> triangles;
    std::vector<GLuint> vboTrianglesId;
    std::vector<int> vboTrianglesSize;
    GLuint vboSolidCubeId,vboSolidSphereId,vboSolidCylinderId,vboTmpId;
    int vboSolidCubeSize,vboSolidSphereSize,vboSolidCylinderSize,vboTmpSize;

    QVector3D pickPoint;
    bool pointPicked;

protected:

    void paintGL();
    void resizeGL(int,int);

    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void wheelEvent(QWheelEvent*);
    void drawSolidSphere(const QVector3D&,qreal);
    void drawSolidCylinder(const QVector3D&,const QVector3D&,qreal);
    void drawAABB(const QVector3D&,const QVector3D&);
    void drawOBB(const QMatrix4x4&,const QVector3D&);

    CGMainWindow *main;
    int oldX,oldY;

private:

    QGLShaderProgram program;
    QMatrix4x4 projection,modelView;
    int width, height;
};

#endif
