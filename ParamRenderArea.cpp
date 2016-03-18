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
    QColor rndC = QColor::fromRgb(qrand() % (255 + 1), qrand() % (255 + 1), qrand() % (255 + 1));
    setFillGradient(rndC, rndC);
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

void ParamRenderArea::setPenWidth(float width) {
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

    // draw all outer shapes
    for (QPainterPath p : outerShapes) {
        QColor rndC = QColor::fromRgb(qrand() % (255 + 1), qrand() % (255 + 1), qrand() % (255 + 1));
        QLinearGradient g(0, 0, 0, 100);
        g.setColorAt(0.0, rndC);
        g.setColorAt(1.0, rndC);
        painter.setBrush(g);
        painter.drawPath(p);
    }

    // draw all inner shapes
    for (QPainterPath p : innerShapes) {
        QColor rndC = QColor::fromRgb(qrand() % (255 + 1), qrand() % (255 + 1), qrand() % (255 + 1));
        QLinearGradient g(0, 0, 0, 100);
        g.setColorAt(0.0, rndC);
        g.setColorAt(1.0, rndC);
        painter.setBrush(g);
        painter.drawPath(p);
    }

    // draw current path
    QLinearGradient gradient(0, 0, 0, 100);
    gradient.setColorAt(0.0, fillColor1);
    gradient.setColorAt(1.0, fillColor2);
    painter.setBrush(gradient);
    painter.drawPath(path);
}

void ParamRenderArea::mousePressEvent(QMouseEvent *event) {
    QPointF pos = convertPos(event);
    if (event->button() == Qt::RightButton) {
        QList<QPointF> shape;
        std::copy(waypoints.begin(), waypoints.end(), std::back_inserter(shape));
        shapeList.append(shape);
        path = QPainterPath();
        waypoints.clear();
        QColor rndC = QColor::fromRgb(qrand() % (255 + 1), qrand() % (255 + 1), qrand() % (255 + 1));
        setFillGradient(rndC, rndC);
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

QPainterPath ParamRenderArea::getPath() {
    return path;
}

QList<QList<QPointF> > ParamRenderArea::getShapes() {
    return shapeList;
}

void ParamRenderArea::setShapeType(ShapeType type) {
    currentType = type;
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
        p.moveTo(waypoints[0]);
        for (int i = 1; i < waypoints.size(); ++i) {
            p.lineTo(waypoints[i]);
        }
        p.closeSubpath();
    }
    waypoints.clear();
    switch (currentType) {
        case ShapeType::INNER:
            innerShapes.append(p);
            break;
        case ShapeType::OUTER:
            outerShapes.append(p);
            break;
        default:
            break;
    }
}

void ParamRenderArea::clearShapes() {
    innerShapes.clear();
    outerShapes.clear();
    waypoints.clear();
    update();
}
