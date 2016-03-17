#include "ParamRenderArea.h"
#include "ParametricWindow.h"

#include <QPainter>
#include <QKeyEvent>
#include <iostream>

ParamRenderArea::ParamRenderArea(const QPainterPath &path, QWidget *parent)
        : QWidget(parent), path(path) {
    penWidth = 1;
    rotationAngle = 0;
    offset.setX(50);
    offset.setY(50);
    setBackgroundRole(QPalette::Base);
}

QSize ParamRenderArea::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize ParamRenderArea::sizeHint() const {
    return QSize(100, 100);
}

void ParamRenderArea::setFillRule(Qt::FillRule rule) {
    path.setFillRule(rule);
    update();
}

void ParamRenderArea::setFillGradient(const QColor &color1, const QColor &color2) {
    fillColor1 = color1;
    fillColor2 = color2;
    update();
}

void ParamRenderArea::setPenWidth(int width) {
    penWidth = width;
    update();
}

void ParamRenderArea::setPenColor(const QColor &color) {
    penColor = color;
    update();
}

void ParamRenderArea::setRotationAngle(int degrees) {
    rotationAngle = degrees;
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
    QLinearGradient gradient(0, 0, 0, 100);
    gradient.setColorAt(0.0, fillColor1);
    gradient.setColorAt(1.0, fillColor2);
    painter.setBrush(gradient);
    painter.drawPath(path);
}

void ParamRenderArea::mousePressEvent(QMouseEvent *event) {
    QPointF pos = convertPos(event);
    if (event->button() == Qt::RightButton) {
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
