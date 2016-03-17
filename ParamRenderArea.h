#ifndef CG_LAB_1516_PARAMRENDERAREA_H
#define CG_LAB_1516_PARAMRENDERAREA_H

#include <QPainterPath>
#include <QWidget>

class ParamRenderArea : public QWidget {
Q_OBJECT

public:
    explicit ParamRenderArea(const QPainterPath &path, QWidget *parent = 0);

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    QPainterPath getPath();

public slots:

    void setFillRule(Qt::FillRule rule);
    void setFillGradient(const QColor &color1, const QColor &color2);
    void setPenWidth(int width);
    void setPenColor(const QColor &color);
    void setRotationAngle(int degrees);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    std::vector<QPointF> waypoints;
    QPainterPath path;
    QColor fillColor1;
    QColor fillColor2;
    int penWidth;
    QColor penColor;

    QPoint offset;
    int rotationAngle;

    QPointF convertPos(QMouseEvent *pEvent);
};


#endif //CG_LAB_1516_PARAMRENDERAREA_H
