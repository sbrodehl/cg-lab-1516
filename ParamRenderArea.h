#ifndef CG_LAB_1516_PARAMRENDERAREA_H
#define CG_LAB_1516_PARAMRENDERAREA_H

#include <QPainterPath>
#include <QWidget>

#include "ParameterTriangle.h"

class ParamRenderArea : public QWidget {
Q_OBJECT

public:
    enum ShapeType {
        INNER,
        OUTER
    };

public:
    explicit ParamRenderArea(const QPainterPath &path, QWidget *parent = 0);
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    QList<QList<QPointF> > getShapes();
    void drawTriangulation(std::vector<ParameterTriangle *> triangulation);

public slots:
    void setPenWidth(float width);
    void setPenColor(const QColor &color);
    void clearShapes();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QList<QColor> colors = QList<QColor>() << Qt::GlobalColor::yellow
                           << Qt::GlobalColor::blue << Qt::GlobalColor::green
                           << Qt::GlobalColor::red << Qt::GlobalColor::magenta
                           << Qt::GlobalColor::gray << Qt::GlobalColor::cyan;

    std::vector<QPointF> waypoints;
    QList<QList<QPointF> > shapeList;
    QList<QPainterPath> outerShapes;
    QList<QPainterPath> innerShapes;
    QPainterPath path;
    float penWidth;
    float noise = 1.0e-5;
    QColor penColor;
    ShapeType currentType = ParamRenderArea::ShapeType::OUTER;

    QPoint offset;
    int rotationAngle;
    std::vector<ParameterTriangle *> triangulation;

    QPointF convertPos(QMouseEvent *pEvent);
    void removeLastWaypoint();
    void saveCurrentShape();

    QPointF denormalize(QVector2D vector2D);
};

#endif //CG_LAB_1516_PARAMRENDERAREA_H
