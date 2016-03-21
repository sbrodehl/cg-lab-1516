#include <QKeyEvent>

#include "StlReader.h"

void CGView::mousePressEvent(QMouseEvent *event) {
    oldX = event->x();
    oldY = event->y();
    update();
}

void CGView::mouseReleaseEvent(QMouseEvent *) { }

void CGView::wheelEvent(QWheelEvent *event) {
    if (event->delta() < 0) zoom *= 1.2; else zoom *= 1 / 1.2;
    update();
}

void CGView::mouseMoveEvent(QMouseEvent *event) {
    QVector3D p1, p2;

    mouseToTrackball(oldX, oldY, width, height, p1);
    mouseToTrackball(event->x(), event->y(), width, height, p2);

    QQuaternion q = trackball(p1, p2);
    q_now = q * q_now;
    q_now.normalize();

    oldX = event->x();
    oldY = event->y();
    updateGL();
}

void CGView::clearGL(bool soft) {
    glClearColor(0.4, 0.4, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_ARRAY_BUFFER);
    triangles = std::vector<QVector3D>();
    vboTrianglesId = std::vector<GLuint>();
    vboTrianglesSize = std::vector<int>();
    if (!soft) {
        qreal inf = std::numeric_limits<qreal>::infinity();
        min = QVector3D(inf, inf, inf);
        max = QVector3D(-inf, -inf, -inf);
        center = QVector3D(0, 0, 0);
        q_now = QQuaternion();
    }
    updateGL();
}


void CGMainWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        default:
            break;
    }
    ogl->updateGL();
}
