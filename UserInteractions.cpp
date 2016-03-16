#include <QKeyEvent>

#include "StlReader.h"

bool intersectLine(const QVector3D &a, const QVector3D &b, const QVector3D &c,
                   const QVector3D &p, const QVector3D &q, QVector3D &s, qreal &d, int dir) {
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

void CGView::clearGL() {
    qglClearColor(Qt::black);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_ARRAY_BUFFER);
    triangles = std::vector<QVector3D>();
    vboTrianglesId = std::vector<GLuint>();
    vboTrianglesSize = std::vector<int>();
    qreal inf = std::numeric_limits<qreal>::infinity();
    min = QVector3D(inf, inf, inf);
    max = QVector3D(-inf, -inf, -inf);
    center = QVector3D(0, 0, 0);
    phi = 0.0f;
    theta = 0.0f;
    updateGL();
}


void CGMainWindow::keyPressEvent(QKeyEvent *event) {
//    switch (event->key()) {
//        case Qt::Key_Plus:
//            break;
//    }
    ogl->updateGL();
}
