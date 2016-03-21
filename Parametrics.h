#ifndef CG_LAB_1516_PARAMETRICS_H
#define CG_LAB_1516_PARAMETRICS_H

#include <QVector3D>
#include <QPair>
#include <QString>
#include <QUuid>

#include <cmath>
#include <memory>
#include <QtCore/qmath.h>

#include "ParametricWindow.h"
#include "ParameterTriangle.h"
#include "Mesh.h"

class Parametrics {

private:

    QString name;
    QString description;
    QUuid uid = QUuid::createUuid();
    std::vector<ParameterTriangle *> parameterSpace;
    ParametricWindow *pw;
    Mesh mesh;

public:

    const double PI = M_PI;

    void setDescription(QString description_) { description = description_; };
    QString getDescription() { return description; };

    void setName(QString name_) { name = name_; };
    QString getName() { return name; };

    QUuid getUID() { return uid; };

    virtual double x(double u, double v) = 0;
    virtual double y(double u, double v) = 0;
    virtual double z(double u, double v) = 0;

    virtual QVector3D getPoint(double u, double v) = 0;
    virtual QVector3D getNormal(double u, double v) = 0;

    virtual Mesh getMesh(double delta, double eps) = 0;
    virtual std::vector<QVector3D> triangulate(double delta, double eps) = 0;
    std::vector<QPair<QVector3D, QVector3D> > getSegments();

    void initializeParameterSpace(double delta, double eps);
    bool leftTurn(QVector2D a, QVector2D b, QVector2D c);
    ParameterTriangle locatePoint(ParameterTriangle actual, QVector2D point);

    virtual std::vector<QVector3D> createTriangles(const std::vector<QVector3D> &points, int bucketsize);
    virtual ParametricWindow *createWindow(QWidget *parent);

};


#endif //CG_LAB_1516_PARAMETRICS_H
