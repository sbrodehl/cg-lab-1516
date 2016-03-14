#ifndef DICOMREADER_H
#define DICOMREADER_H

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

class MyGLWidget;

class CGMainWindow : public QMainWindow {
    Q_OBJECT

public:

    CGMainWindow (QWidget* parent = 0); // , Qt::WFlags flags = Qt::Window);
    ~CGMainWindow ();
    MyGLWidget *ogl;

public slots:

protected:

    void keyPressEvent(QKeyEvent*);
};

class MyGLWidget : public QGLWidget, public QGLFunctions {
    Q_OBJECT

public:

    MyGLWidget(CGMainWindow*,QWidget*);
    void initShaders();
    void initializeGL();

    void mouseToTrackball(int,int,int,int,QVector3D&);
    QQuaternion trackball(const QVector3D&,const QVector3D&);

    void pickLine(int,int,QVector3D&,QVector3D&);
    bool pick(int,int,QVector3D&);

    QVector3D min, max, center;
    QQuaternion qNow;
    qreal zoom;

    GLuint vboQuadId;

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
    int oldX,oldY,button;

private:

    QGLShaderProgram program;
    QMatrix4x4 projection,modelView;
    int width, height;
};

#endif
