#ifndef CG_LAB_1516_PART_H
#define CG_LAB_1516_PART_H

#include <QVector3D>

#include <memory>

#include "Parametrics.h"
#include "ParametricWindow.h"

class Part {

protected:
    std::vector<std::shared_ptr<Parametrics> > parametrics;
    std::vector<std::shared_ptr<ParametricWindow> > windows;

public:
    std::vector<QVector3D> triangulate(float delta) {
        std::vector<QVector3D> pointvec;
        for (auto parametric : parametrics) {
            std::vector<QVector3D> p = parametric->getPolygonTriangulation(delta);
            pointvec.insert(std::end(pointvec), std::begin(p), std::end(p));
        }
        return pointvec;
    }

    std::vector<Mesh> getMeshes(double delta, double eps) {
        std::vector<Mesh> meshes;
        for (auto parametric : parametrics) {
            meshes.push_back(parametric->getMesh(delta, eps));
        }
        return meshes;
    }

    void showParamWindows(QWidget *parent) {
        for (auto parametric : parametrics) {
            ParametricWindow *pw = parametric->createWindow(parent);
            windows.push_back(std::shared_ptr<ParametricWindow>(pw));
            pw->show();
        }
    };

    void destroyWindows() {
        for (auto w : windows) {
            w->close();
        }

    }
};

#endif //CG_LAB_1516_PART_H
