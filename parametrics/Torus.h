#ifndef CG_LAB_1516_TORUS_H
#define CG_LAB_1516_TORUS_H


#include "Parametrics.h"

class Torus : public Parametrics {

private:
    float R;
    float r;

    double scaled(double x) {
        return x * 2 * PI;
    }

public:

    Torus(float R_, float r_) {
        R = R_;
        r = r_;
        std::string classname = typeid(*this).name();
        setName(QString(classname.c_str()));
    }

    Torus() : Torus(10, 1) {
    }

    ~Torus() {

    }

    double x(double u, double v) {
        float theta = (float) scaled(u);
        float phi = (float) scaled(v);

        return (R + r * cos(phi)) * cos(theta);
    }

    double y(double u, double v) {
        float theta = (float) scaled(u);
        float phi = (float) scaled(v);

        return (R + r * cos(phi)) * sin(theta);
    }

    double z(double u, double v) {
        float phi = (float) scaled(v);

        return r * sin(phi);
    }

    QVector3D getPoint(double u, double v) {
        return QVector3D((float) x(u, v),
                         (float) y(u, v),
                         (float) z(u, v));
    }

    QVector3D getNormal(double u, double v) {
        float theta = (float) scaled(u);
        float phi = (float) scaled(v);

        return QVector3D((float) ((R + r * cos(phi)) * r * cos(theta) * cos(phi)),
                         (float) ((R + r * cos(phi)) * r * sin(theta) * cos(phi)),
                         (float) ((R + r * cos(phi)) * r * sin(phi)));
    }

    std::vector<QVector3D> triangulate(double delta, double eps) {
        std::vector<QVector3D> pointvec;

        for (int i = 0; i <= delta; i++) {
            for (int j = 0; j <= eps; j++) {
                pointvec.push_back(getPoint((float) i / delta, (float) j / eps));
                pointvec.push_back(getNormal((float) i / delta, (float) j / eps));
            }
        }

        return createTriangles(pointvec, (int) (delta + 1));
    }

    Mesh getMesh(double delta, double eps) {
        if (!mesh.isEmpty())
            return mesh;

        mesh = Mesh();

        std::vector<QVector3D> pointsWN;
        int bucketsize = (int) (delta + 1);

        for (int i = 0; i <= delta; i++) {
            for (int j = 0; j <= eps; j++) {
                pointsWN.push_back(getPoint((float) i / delta, (float) j / eps));
                pointsWN.push_back(getNormal((float) i / delta, (float) j / eps));
            }
        }

        for (size_t i = 0; i < pointsWN.size() - (2 * bucketsize); i += 2 * bucketsize) {
            int preLeft = -1;
            int preRight = -1;
            int preEdge = -1;
            int preBorderL = -1;
            for (size_t l = 0; l < 2 * bucketsize - 0; l += 2) {
                size_t leftRoot = i + l;
                size_t rightRoot = i + l + 2 * bucketsize;

                if (preLeft < 0 || preRight < 0) {
                    preLeft = (int) leftRoot;
                    preRight = (int) rightRoot;
                    continue;
                }

                if (preEdge < 0) {
                    mesh.addTriangle(pointsWN[leftRoot], pointsWN[preRight], pointsWN[preLeft]);
                    mesh.verts[mesh.verts.size() - 3].normal = pointsWN[leftRoot + 1]; //a
                    mesh.verts[mesh.verts.size() - 2].normal = pointsWN[preRight + 1]; //b
                    mesh.verts[mesh.verts.size() - 1].normal = pointsWN[preLeft + 1]; //c
                    preEdge = (int) mesh.edges.size() - 3;
                } else {
                    mesh.addTriangle(preEdge, pointsWN[leftRoot]);
                    mesh.verts[mesh.verts.size() - 1].normal = pointsWN[leftRoot + 1]; //c
                    preEdge = (int) mesh.edges.size() - 1;
                }

                mesh.addTriangle(preEdge, pointsWN[rightRoot]);
                mesh.verts[mesh.verts.size() - 1].normal = pointsWN[rightRoot + 1]; //c
                preEdge = (int) (mesh.edges.size() - 2);

                preLeft = (int) leftRoot;
                preRight = (int) rightRoot;
            }
        }
        return mesh;
    }
};

#endif //CG_LAB_1516_TORUS_H
