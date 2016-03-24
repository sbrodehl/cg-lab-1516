#include <algorithm>

#include "ParamRenderArea.h"

#include <QPainter>
#include <QKeyEvent>
#include <iostream>

ParamRenderArea::ParamRenderArea(const QPainterPath &path, QWidget *parent)
        : QWidget(parent), path(path) {
    penWidth = 0.1f;
    rotationAngle = 0;
    offset.setX(50);
    offset.setY(50);
    triangulation = std::vector<ParameterTriangle *>();
    setBackgroundRole(QPalette::Base);
    std::random_shuffle(colors.begin(), colors.end());
}

QSize ParamRenderArea::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize ParamRenderArea::sizeHint() const {
    return QSize(100, 100);
}

void ParamRenderArea::setPenWidth(float width) {
    penWidth = width;
    update();
}

void ParamRenderArea::setPenColor(const QColor &color) {
    penColor = color;
    update();
}

void ParamRenderArea::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.scale(width() / 100.0, height() / 100.0);
    painter.translate(offset);
    painter.rotate(-rotationAngle);
    painter.translate(-offset);
    painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    int colorID = 0;

    // draw all shapes
    for (QList<QPointF> sha : shapeList) {
        QLinearGradient g(0, 0, 0, 100);
        g.setColorAt(0.0, colors.at(colorID));
        g.setColorAt(1.0, colors.at(colorID));
        painter.setBrush(g);
        QPainterPath tP;
        tP.moveTo(sha.first());
        for (int i = 0; i < sha.size(); ++i) {
            tP.lineTo(sha.at(i));
        }
        painter.drawPath(tP);
        colorID = (colorID + 1) % colors.size();
    }

    // draw current path
    QLinearGradient gradient(0, 0, 0, 100);
    gradient.setColorAt(0.0, colors.at(colorID));
    gradient.setColorAt(1.0, colors.at(colorID));
    painter.setBrush(gradient);
    painter.drawPath(path);

    // draw parameter triangulation
    QPainter trip(this);
    trip.setRenderHint(QPainter::Antialiasing);
    trip.scale(width() / 100.0, height() / 100.0);
    trip.translate(offset);
    trip.rotate(-rotationAngle);
    trip.translate(-offset);
    trip.setPen(QPen(Qt::black, 1.0f * penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    for (ParameterTriangle *t : triangulation) {
        QPainterPath tP;
        tP.moveTo(denormalize(t->getPoint(0)));
        tP.lineTo(denormalize(t->getPoint(1)));
        tP.lineTo(denormalize(t->getPoint(2)));
        tP.lineTo(denormalize(t->getPoint(0)));
        trip.drawPath(tP);
    }
}

void ParamRenderArea::mousePressEvent(QMouseEvent *event) {
    QPointF pos = convertPos(event);
    if (event->button() == Qt::RightButton) {
        QList<QPointF> shape;
        std::copy(waypoints.begin(), waypoints.end(), std::back_inserter(shape));
        path = QPainterPath();
        waypoints.clear();
    } else {
        waypoints.push_back(pos);
        path = QPainterPath();
        path.moveTo(waypoints[0]);
        for (int i = 1; i < waypoints.size(); ++i) {
            path.lineTo(waypoints[i]);
        }
        path.closeSubpath();
    }
    update();
}

QPointF ParamRenderArea::convertPos(QMouseEvent *e) {
    return QPointF(100 * e->x() / width(), 100 * e->y() / height());
}

QList<QList<QPointF> > ParamRenderArea::getShapes() {
    if (shapeList.isEmpty()) {
        QList<QPointF> s;
        s.push_back(denormalize(QVector2D(0.0f, 0.0f+0.00001*(rand()%10))));
        s.push_back(denormalize(QVector2D(1.0f, 0.0f+0.00001*(rand()%10))));
        s.push_back(denormalize(QVector2D(1.0f, 1.0f-0.00001*(rand()%10))));
        s.push_back(denormalize(QVector2D(0.0f, 1.0f-0.00001*(rand()%10))));
        shapeList.append(s);
    }
    return shapeList;
}

void ParamRenderArea::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Backspace:
            removeLastWaypoint();
            break;
        case Qt::Key_Return:
            saveCurrentShape();
            break;
        default:
            break;
    }
}

void ParamRenderArea::removeLastWaypoint() {
    if (!waypoints.empty()) {
        waypoints.pop_back();
    }
    if (!waypoints.empty()) {
        path = QPainterPath();
        path.moveTo(waypoints[0]);
        for (int i = 1; i < waypoints.size(); ++i) {
            path.lineTo(waypoints[i]);
        }
        path.closeSubpath();
    }
    update();
}

void ParamRenderArea::saveCurrentShape() {
    QPainterPath p;
    if (!waypoints.empty()) {
        waypoints[0].setY(waypoints[0].y()+0.001*((rand()%19)-9));
        p.moveTo(waypoints[0]);
        for (int i = 1; i < waypoints.size(); ++i) {
            waypoints[i].setY(waypoints[i].y()+0.001*((rand()%19)-9));
            p.lineTo(waypoints[i]);
        }
        p.closeSubpath();
    }
    QList<QPointF> wp;
    for (QPointF po : waypoints) {
        wp.push_back(po);
    }
    shapeList.append(wp);
    waypoints.clear();
}

void ParamRenderArea::clearShapes() {
    shapeList.clear();
    waypoints.clear();
    triangulation.clear();
    update();
}

void ParamRenderArea::mouseDoubleClickEvent(QMouseEvent *event) { }

void ParamRenderArea::drawTriangulation(std::vector<ParameterTriangle *> triangulation_) {
    triangulation = triangulation_;
    update();
}

QPointF ParamRenderArea::denormalize(QVector2D v) {
    return QPointF(100.0f * v.x(), 100.0f - 100.f * v.y());
}


