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

#include "Part.h"
#include "Parametrics.h"
#include "ParametricWindow.h"

class CGView;

class CGMainWindow : public QMainWindow {
Q_OBJECT

public:
    CGMainWindow(QWidget *parent = 0);

    ~CGMainWindow();

    CGView *ogl;
    Part *viewPart = nullptr;

public slots:

    void loadTorusPart();

    void loadTrefoilPart();

    void loadZylinderPart();

    void loadKegelPart();

    void changedDeltaSlider(int);

    void updateTriangulation();

    void toggleWireframe();

protected:
    void loadEq(Part &);

    void updateEq();

    void keyPressEvent(QKeyEvent *);
};

class CGView : public QGLWidget, public QGLFunctions {
Q_OBJECT

public:
    CGView(CGMainWindow *, QWidget *);

    void clearGL(bool soft = false);

    void initShaders();

    void initializeGL();

    void drawMesh(Mesh m);

    void initVBO(const std::vector<QVector3D> &);

    void toggleWireframe();

    QVector3D min, max, center;
    float zoom;

    std::vector<QVector3D> triangles;
    std::vector<GLuint> vboTrianglesId;
    std::vector<int> vboTrianglesSize;
    GLuint vboTmpId;

protected:
    void paintGL();

    void resizeGL(int, int);

    void mouseMoveEvent(QMouseEvent *);

    void mousePressEvent(QMouseEvent *);

    void mouseReleaseEvent(QMouseEvent *);

    void wheelEvent(QWheelEvent *);

    void mouseToTrackball(int x, int y, int W, int H, QVector3D &v);

    QQuaternion trackball(const QVector3D &, const QVector3D &);

    CGMainWindow *main;
    int oldX, oldY;
    QQuaternion q_now;

private:
    QGLShaderProgram program;
    QMatrix4x4 projection, modelView;
    int width, height;
    bool wireframe = false;

    void drawBoundingBox();
};

#endif
