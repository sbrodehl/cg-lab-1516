#include "ParametricsViewer.h"

void CGView::initShaders() {
    setlocale(LC_NUMERIC, "C");
    if (!program.addShaderFromSourceFile(QGLShader::Vertex, ":/vshader.glsl"))
        close();
    if (!program.addShaderFromSourceFile(QGLShader::Fragment, ":/fshader.glsl"))
        close();
    if (!program.link())
        close();
    if (!program.bind())
        close();
    setlocale(LC_ALL, "");
}

void CGView::initializeGL() {
    initializeGLFunctions();
    initShaders();

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glGenBuffers(1, &vboTmpId);
    glClearColor(0.4, 0.4, 0.5, 1.0);

    zoom = 1.0;
    q_now = QQuaternion();
    float inf = std::numeric_limits<float>::infinity();
    min = QVector3D(inf, inf, inf);
    max = QVector3D(-inf, -inf, -inf);
    center = QVector3D();
}

void CGView::paintGL() {
    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    modelView.setToIdentity();
    modelView.rotate(q_now);
    modelView.scale(zoom);
    modelView.translate(-center);

    for (size_t i = 0; i < vboTrianglesId.size(); i++) {
        program.setUniformValue("uPMat", projection);
        program.setUniformValue("uMVMat", modelView);
        program.setUniformValue("uNMat", modelView.normalMatrix());
        program.setUniformValue("uColor", QVector4D(0.0, 1.0, 0.0, 1.0));

        glBindBuffer(GL_ARRAY_BUFFER, vboTrianglesId[i]);
        int vertexLocation = program.attributeLocation("a_position");
        program.enableAttributeArray(vertexLocation);
        glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(QVector3D), 0);
        int normalLocation = program.attributeLocation("a_normal");
        program.enableAttributeArray(normalLocation);
        glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(QVector3D),
                              (const void *) sizeof(QVector3D));
        glDrawArrays(GL_TRIANGLES, 0, vboTrianglesSize[i]);
    }
    drawBoundingBox();
}

void CGView::resizeGL(int w, int h) {
    width = w;
    height = h;
    glViewport(0, 0, width, height);
    projection.setToIdentity();
    if (width > height) {
        qreal ratio = width / (qreal) height;
        projection.ortho(-ratio, ratio, -1.0f, 1.0f, -10.0f, 10.0f);
    } else {
        qreal ratio = height / (qreal) width;
        projection.ortho(-1.0f, 1.0f, -ratio, ratio, -10.0f, 10.0f);
    }
}

void CGView::drawBoundingBox() {
    double maxX = 1, maxY = 1, maxZ = 1;
    double minX = -maxX, minY = -maxY, minZ = -maxZ;
    glDisable(GL_LIGHTING);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex3d(minX, minY, minZ);
    glVertex3d(maxX, minY, minZ);
    glVertex3d(maxX, maxY, minZ);
    glVertex3d(minX, maxY, minZ);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3d(minX, minY, maxZ);
    glVertex3d(maxX, minY, maxZ);
    glVertex3d(maxX, maxY, maxZ);
    glVertex3d(minX, maxY, maxZ);
    glEnd();
    glBegin(GL_LINES);
    glVertex3d(minX, minY, minZ);
    glVertex3d(minX, minY, maxZ);
    glVertex3d(maxX, minY, minZ);
    glVertex3d(maxX, minY, maxZ);
    glVertex3d(minX, maxY, minZ);
    glVertex3d(minX, maxY, maxZ);
    glVertex3d(maxX, maxY, minZ);
    glVertex3d(maxX, maxY, maxZ);
    glEnd();
}

void CGView::initVBO(const std::vector<QVector3D> &trianglesWN) {
    GLuint id;
    glGenBuffers(1, &id);

    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, trianglesWN.size() * sizeof(QVector3D), trianglesWN.data(), GL_STATIC_DRAW);

    vboTrianglesId.push_back(id);
    vboTrianglesSize.push_back(static_cast<int>(triangles.size()));
}

void CGView::mouseToTrackball(int x, int y, int W, int H, QVector3D &v) {
    if (W > H) {
        v[0] = (float) ((2.0 * x - W) / H);
        v[1] = (float) (1.0 - y * 2.0 / H);
    } else {
        v[0] = (float) ((2.0 * x - W) / W);
        v[1] = (float) ((H - 2.0 * y) / W);
    }
    double d = v[0] * v[0] + v[1] * v[1];
    if (d > 1.0) {
        v[2] = 0.0;
        v /= sqrt(d);
    } else v[2] = (float) sqrt(1.0 - d * d);
}

QQuaternion CGView::trackball(const QVector3D &u, const QVector3D &v) {
    QVector3D uxv = QVector3D::crossProduct(u, v);
    QQuaternion ret(1.0f + QVector3D::dotProduct(u, v), uxv);
    ret.normalize();
    return ret;
}

void CGView::drawMesh(Mesh m) {
    std::vector<QVector3D> poWN;
    for (face &f: m.faces) {
        if (f.halfedge == -1) continue;
        edge &e1 = m.edges[f.halfedge];
        edge &e2 = m.edges[e1.succ];
        edge &e3 = m.edges[e1.pred];
        QVector3D a = m.verts[e1.vertex].pos;
        QVector3D aN = m.verts[e1.vertex].normal;
        QVector3D b = m.verts[e2.vertex].pos;
        QVector3D bN = m.verts[e2.vertex].normal;
        QVector3D c = m.verts[e3.vertex].pos;
        QVector3D cN = m.verts[e3.vertex].normal;

        poWN.push_back(a);
        poWN.push_back(aN);
        poWN.push_back(b);
        poWN.push_back(bN);
        poWN.push_back(c);
        poWN.push_back(cN);
    }
    if (!poWN.empty()) {
        triangles.insert(triangles.end(), poWN.begin(), poWN.end());
        //initVBO(poWN);
    }
}

void CGView::toggleWireframe() {
    wireframe = !wireframe;
    update();
}
