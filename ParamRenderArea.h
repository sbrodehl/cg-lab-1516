#ifndef CG_LAB_1516_PARAMRENDERAREA_H
#define CG_LAB_1516_PARAMRENDERAREA_H

#include <QPainterPath>
#include <QWidget>

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

    QPainterPath getPath();

    QList<QList<QPointF> > getShapes();

    void setShapeType(ShapeType type);

    void keyPressEvent(QKeyEvent *event);

    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

public slots:

    void setFillRule(Qt::FillRule rule);

    void setFillGradient(const QColor &color1, const QColor &color2);

    void setPenWidth(float width);

    void setPenColor(const QColor &color);

    void setRotationAngle(int degrees);

    void clearShapes();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QList<QColor> colors = QList<QColor>() << Qt::GlobalColor::black << Qt::GlobalColor::yellow
                           << Qt::GlobalColor::blue << Qt::GlobalColor::green << Qt::GlobalColor::red <<
                           Qt::GlobalColor::magenta
                           << Qt::GlobalColor::gray << Qt::GlobalColor::cyan << Qt::GlobalColor::darkBlue;

    std::vector<QPointF> waypoints;
    QList<QList<QPointF> > shapeList;
    QList<QPainterPath> outerShapes;
    QList<QPainterPath> innerShapes;
    QPainterPath path;
    QColor fillColor1;
    QColor fillColor2;
    float penWidth;
    QColor penColor;
    ShapeType currentType = ParamRenderArea::ShapeType::OUTER;

    QPoint offset;
    int rotationAngle;

    QPointF convertPos(QMouseEvent *pEvent);

    void removeLastWaypoint();

    void saveCurrentShape();
};


#endif //CG_LAB_1516_PARAMRENDERAREA_H
