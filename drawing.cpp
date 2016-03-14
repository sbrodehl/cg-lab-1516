#include "StlReader.h"

void CGView::initSolidCubeVBO() {
    std::vector<QVector3D> vertexWithNormal;
    GLuint id;
    glGenBuffers(1,&id);

    vertexWithNormal.push_back(QVector3D(-1,-1,-1));
    vertexWithNormal.push_back(QVector3D( 0,-1, 0));
    vertexWithNormal.push_back(QVector3D( 1,-1,-1));
    vertexWithNormal.push_back(QVector3D( 0,-1, 0));
    vertexWithNormal.push_back(QVector3D( 1,-1, 1));
    vertexWithNormal.push_back(QVector3D( 0,-1, 0));
    vertexWithNormal.push_back(QVector3D( 1,-1, 1));
    vertexWithNormal.push_back(QVector3D( 0,-1, 0));
    vertexWithNormal.push_back(QVector3D(-1,-1, 1));
    vertexWithNormal.push_back(QVector3D( 0,-1, 0));
    vertexWithNormal.push_back(QVector3D(-1,-1,-1));
    vertexWithNormal.push_back(QVector3D( 0,-1, 0));

    vertexWithNormal.push_back(QVector3D(-1, 1,-1));
    vertexWithNormal.push_back(QVector3D( 0, 1, 0));
    vertexWithNormal.push_back(QVector3D(-1, 1, 1));
    vertexWithNormal.push_back(QVector3D( 0, 1, 0));
    vertexWithNormal.push_back(QVector3D( 1, 1, 1));
    vertexWithNormal.push_back(QVector3D( 0, 1, 0));
    vertexWithNormal.push_back(QVector3D( 1, 1, 1));
    vertexWithNormal.push_back(QVector3D( 0, 1, 0));
    vertexWithNormal.push_back(QVector3D( 1, 1,-1));
    vertexWithNormal.push_back(QVector3D( 0, 1, 0));
    vertexWithNormal.push_back(QVector3D(-1, 1,-1));
    vertexWithNormal.push_back(QVector3D( 0, 1, 0));

    vertexWithNormal.push_back(QVector3D(-1, 1, 1));
    vertexWithNormal.push_back(QVector3D(-1, 0, 0));
    vertexWithNormal.push_back(QVector3D(-1, 1,-1));
    vertexWithNormal.push_back(QVector3D(-1, 0, 0));
    vertexWithNormal.push_back(QVector3D(-1,-1,-1));
    vertexWithNormal.push_back(QVector3D(-1, 0, 0));
    vertexWithNormal.push_back(QVector3D(-1,-1,-1));
    vertexWithNormal.push_back(QVector3D(-1, 0, 0));
    vertexWithNormal.push_back(QVector3D(-1,-1, 1));
    vertexWithNormal.push_back(QVector3D(-1, 0, 0));
    vertexWithNormal.push_back(QVector3D(-1, 1, 1));
    vertexWithNormal.push_back(QVector3D(-1, 0, 0));

    vertexWithNormal.push_back(QVector3D( 1, 1, 1));
    vertexWithNormal.push_back(QVector3D( 1, 0, 0));
    vertexWithNormal.push_back(QVector3D( 1,-1, 1));
    vertexWithNormal.push_back(QVector3D( 1, 0, 0));
    vertexWithNormal.push_back(QVector3D( 1,-1,-1));
    vertexWithNormal.push_back(QVector3D( 1, 0, 0));
    vertexWithNormal.push_back(QVector3D( 1,-1,-1));
    vertexWithNormal.push_back(QVector3D( 1, 0, 0));
    vertexWithNormal.push_back(QVector3D( 1, 1,-1));
    vertexWithNormal.push_back(QVector3D( 1, 0, 0));
    vertexWithNormal.push_back(QVector3D( 1, 1, 1));
    vertexWithNormal.push_back(QVector3D( 1, 0, 0));

    vertexWithNormal.push_back(QVector3D( 1,-1,-1));
    vertexWithNormal.push_back(QVector3D( 0, 0,-1));
    vertexWithNormal.push_back(QVector3D(-1,-1,-1));
    vertexWithNormal.push_back(QVector3D( 0, 0,-1));
    vertexWithNormal.push_back(QVector3D(-1, 1,-1));
    vertexWithNormal.push_back(QVector3D( 0, 0,-1));
    vertexWithNormal.push_back(QVector3D(-1, 1,-1));
    vertexWithNormal.push_back(QVector3D( 0, 0,-1));
    vertexWithNormal.push_back(QVector3D( 1, 1,-1));
    vertexWithNormal.push_back(QVector3D( 0, 0,-1));
    vertexWithNormal.push_back(QVector3D( 1,-1,-1));
    vertexWithNormal.push_back(QVector3D( 0, 0,-1));

    vertexWithNormal.push_back(QVector3D(-1, 1, 1));
    vertexWithNormal.push_back(QVector3D( 0, 0, 1));
    vertexWithNormal.push_back(QVector3D(-1,-1, 1));
    vertexWithNormal.push_back(QVector3D( 0, 0, 1));
    vertexWithNormal.push_back(QVector3D( 1,-1, 1));
    vertexWithNormal.push_back(QVector3D( 0, 0, 1));
    vertexWithNormal.push_back(QVector3D( 1,-1, 1));
    vertexWithNormal.push_back(QVector3D( 0, 0, 1));
    vertexWithNormal.push_back(QVector3D( 1, 1, 1));
    vertexWithNormal.push_back(QVector3D( 0, 0, 1));
    vertexWithNormal.push_back(QVector3D(-1, 1, 1));
    vertexWithNormal.push_back(QVector3D( 0, 0, 1));

    glBindBuffer(GL_ARRAY_BUFFER,id);
    glBufferData(GL_ARRAY_BUFFER,vertexWithNormal.size()*sizeof(QVector3D),vertexWithNormal.data(),GL_STATIC_DRAW);

    vboSolidCubeId = id;
    vboSolidCubeSize = 3*12;
}

void CGView::refineSolidSphere(const std::vector<QVector3D>& sphere,std::vector<QVector3D>& refined) {
    for(size_t i=0;i<sphere.size()/3;i++) {
        const QVector3D& a = sphere[3*i+0];
        const QVector3D& b = sphere[3*i+1];
        const QVector3D& c = sphere[3*i+2];
        
        QVector3D ab = a+b;
        QVector3D bc = b+c;
        QVector3D ca = c+a;
        
        ab.normalize();
        bc.normalize();
        ca.normalize();
        
        refined.push_back(a);
        refined.push_back(ab);
        refined.push_back(ca);
        
        refined.push_back(ab);
        refined.push_back(b);
        refined.push_back(bc);
        
        refined.push_back(bc);
        refined.push_back(c);
        refined.push_back(ca);
        
        refined.push_back(ab);
        refined.push_back(bc);
        refined.push_back(ca);
    }
}

void CGView::initSolidSphereVBO() {
    std::vector<QVector3D> ico;
    qreal gr = 0.5*(1.0+sqrt(5.0));

    ico.push_back( QVector3D(gr,1.0,0.0));
    ico.push_back( QVector3D(1.0,0.0,gr));
    ico.push_back( QVector3D(gr,-1.0,0.0)); 
    
    ico.push_back( QVector3D(gr,1.0,0.0));
    ico.push_back( QVector3D(gr,-1.0,0.0));
    ico.push_back( QVector3D(1.0,0.0,-gr));
    
    ico.push_back( QVector3D(gr,1.0,0.0));
    ico.push_back( QVector3D(0.0,gr,-1.0));
    ico.push_back( QVector3D(0.0,gr,1.0));
    
    ico.push_back( QVector3D(gr,1.0,0.0));
    ico.push_back( QVector3D(0.0,gr,1.0));
    ico.push_back( QVector3D(1.0,0.0,gr));
    
    ico.push_back( QVector3D(gr,1.0,0.0));
    ico.push_back( QVector3D(1.0,0.0,-gr));
    ico.push_back( QVector3D(0.0,gr,-1.0));
    
    ico.push_back( QVector3D(-gr,-1.0,0.0));
    ico.push_back( QVector3D(-1.0,0.0,gr));
    ico.push_back( QVector3D(-gr,1.0,0.0));
    
    ico.push_back( QVector3D(-gr,-1.0,0.0));
    ico.push_back( QVector3D(-gr,1.0,0.0));
    ico.push_back( QVector3D(-1.0,0.0,-gr));
    
    ico.push_back( QVector3D(-gr,-1.0,0.0));
    ico.push_back( QVector3D(0.0,-gr,-1.0));
    ico.push_back( QVector3D(0.0,-gr,1.0));
    
    ico.push_back( QVector3D(-gr,-1.0,0.0));
    ico.push_back( QVector3D(0.0,-gr,1.0));
    ico.push_back( QVector3D(-1.0,0.0,gr));
    
    ico.push_back( QVector3D(-gr,-1.0,0.0));
    ico.push_back( QVector3D(-1.0,0.0,-gr));
    ico.push_back( QVector3D(0.0,-gr,-1.0));
    
    ico.push_back( QVector3D(1.0,0.0,gr));
    ico.push_back( QVector3D(-1.0,0.0,gr));
    ico.push_back( QVector3D(0.0,-gr,1.0));
    
    ico.push_back( QVector3D(1.0,0.0,gr));
    ico.push_back( QVector3D(0.0,gr,1.0));
    ico.push_back( QVector3D(-1.0,0.0,gr));
    
    ico.push_back( QVector3D(0.0,gr,1.0));
    ico.push_back( QVector3D(-gr,1.0,0.0));
    ico.push_back( QVector3D(-1.0,0.0,gr));
    
    ico.push_back( QVector3D(0.0,gr,1.0));
    ico.push_back( QVector3D(0.0,gr,-1.0));
    ico.push_back( QVector3D(-gr,1.0,0.0));
    
    ico.push_back( QVector3D(0.0,gr,-1.0));
    ico.push_back( QVector3D(-1.0,0.0,-gr));
    ico.push_back( QVector3D(-gr,1.0,0.0));
    
    ico.push_back( QVector3D(-1.0,0.0,-gr));
    ico.push_back( QVector3D(0.0,gr,-1.0));
    ico.push_back( QVector3D(1.0,0.0,-gr));
    
    ico.push_back( QVector3D(-1.0,0.0,-gr));
    ico.push_back( QVector3D(1.0,0.0,-gr));
    ico.push_back( QVector3D(0.0,-gr,-1.0));
    
    ico.push_back( QVector3D(0.0,-gr,-1.0));
    ico.push_back( QVector3D(1.0,0.0,-gr));
    ico.push_back( QVector3D(gr,-1.0,0.0));
    
    ico.push_back( QVector3D(0.0,-gr,-1.0));
    ico.push_back( QVector3D(gr,-1.0,0.0));
    ico.push_back( QVector3D(0.0,-gr,1.0));
    
    ico.push_back( QVector3D(0.0,-gr,1.0));
    ico.push_back( QVector3D(gr,-1.0,0.0));
    ico.push_back( QVector3D(1.0,0.0,gr));

    for(size_t i=0;i<ico.size();i++) ico[i].normalize();

    for(int i=0;i<3;i++) {
        std::vector<QVector3D> ico_refined;
        refineSolidSphere(ico,ico_refined);
        ico = ico_refined;
    }

    std::vector<QVector3D> vertexWithNormal;
    GLuint id;
    glGenBuffers(1,&id);

    for(size_t i=0;i<ico.size();i++) {
	vertexWithNormal.push_back(ico[i]);
	vertexWithNormal.push_back(ico[i]);
    }

    glBindBuffer(GL_ARRAY_BUFFER,id);
    glBufferData(GL_ARRAY_BUFFER,vertexWithNormal.size()*sizeof(QVector3D),vertexWithNormal.data(),GL_STATIC_DRAW);

    vboSolidSphereId = id;
    vboSolidSphereSize = static_cast<int>(ico.size());
}

void CGView::initSolidCylinderVBO() {
    std::vector<QVector3D> vertexWithNormal;
    const int n = 64;
    std::vector<QVector3D> circle(n);

    circle[0]     = QVector3D( 1.0, 0.0, 0.0);
    circle[n/4]   = QVector3D( 0.0, 1.0, 0.0);
    circle[n/2]   = QVector3D(-1.0, 0.0, 0.0);
    circle[3*n/4] = QVector3D( 0.0,-1.0, 0.0);

    for(int r=n/4;r>1;r/=2)
       for(int i=0;i<n;i+=r) {
          circle[i+r/2] = circle[i]+circle[(i+r)%n];
          circle[i+r/2].normalize(); 
       }

    QVector3D ez(0.0,0.0,1.0);

    for(int i=0;i<n;i++) {
       vertexWithNormal.push_back(circle[i]);
       vertexWithNormal.push_back(circle[i]);
       vertexWithNormal.push_back(circle[(i+1)%n]);
       vertexWithNormal.push_back(circle[(i+1)%n]);
       vertexWithNormal.push_back(circle[i]+ez);
       vertexWithNormal.push_back(circle[i]);

       vertexWithNormal.push_back(circle[(i+1)%n]+ez);
       vertexWithNormal.push_back(circle[(i+1)%n]);
       vertexWithNormal.push_back(circle[i]+ez);
       vertexWithNormal.push_back(circle[i]);
       vertexWithNormal.push_back(circle[(i+1)%n]);
       vertexWithNormal.push_back(circle[(i+1)%n]);
    }

    GLuint id;
    glGenBuffers(1,&id);

    glBindBuffer(GL_ARRAY_BUFFER,id);
    glBufferData(GL_ARRAY_BUFFER,vertexWithNormal.size()*sizeof(QVector3D),vertexWithNormal.data(),GL_STATIC_DRAW);

    vboSolidCylinderId = id;
    vboSolidCylinderSize = static_cast<int>(vertexWithNormal.size()/2);
}

std::vector<QVector3D> CGView::createTriangles(const std::vector<QVector3D> &points, int bucketsize) {
    std::vector<QVector3D> triangles;
    size_t s = points.size();
    for (size_t i = 0; i < s; i += bucketsize) {
        for (size_t l = 0; l < bucketsize; ++l) {
            size_t leftRoot = (i + l) % s;
            size_t rightRoot = (i + l + bucketsize) % s;
            size_t nextLeft = (i + ((l + 1) % bucketsize)) % s;
            size_t nextRight = (i + bucketsize + ((l + 1) % bucketsize)) % s;

            QVector3D leftR = points[leftRoot];
            QVector3D rightR = points[rightRoot];
            QVector3D neleftR = points[nextLeft];
            QVector3D nerightR = points[nextRight];

            triangles.push_back(neleftR);
            triangles.push_back(rightR);
            triangles.push_back(leftR);
            triangles.push_back(neleftR);
            triangles.push_back(nerightR);
            triangles.push_back(rightR);
        }
    }
    return triangles;
}

void CGView::initVBO(const std::vector<QVector3D>& trianglesWN) {
    GLuint id;
    glGenBuffers(1,&id);

    glBindBuffer(GL_ARRAY_BUFFER,id);
    glBufferData(GL_ARRAY_BUFFER,trianglesWN.size()*sizeof(QVector3D),trianglesWN.data(),GL_STATIC_DRAW);

    vboTrianglesId.push_back(id);
    vboTrianglesSize.push_back(static_cast<int>(triangles.size()));
}

void CGView::initTrianglesVBO(const std::vector<QVector3D>& triangles) {
	std::vector<QVector3D> vertexWithNormal;
	GLuint id;
	glGenBuffers(1,&id);

	for(size_t i=0;i<triangles.size();i+=3) {
		const QVector3D& a = triangles[i+0];
		const QVector3D& b = triangles[i+1];
		const QVector3D& c = triangles[i+2];
		QVector3D n = QVector3D::crossProduct(b-a,c-a);
		n.normalize();
		vertexWithNormal.push_back(a);
		vertexWithNormal.push_back(n);

		vertexWithNormal.push_back(b);
		vertexWithNormal.push_back(n);

		vertexWithNormal.push_back(c);
		vertexWithNormal.push_back(n);
	}

	glBindBuffer(GL_ARRAY_BUFFER,id);
	glBufferData(GL_ARRAY_BUFFER,vertexWithNormal.size()*sizeof(QVector3D),vertexWithNormal.data(),GL_STATIC_DRAW);

	vboTrianglesId.push_back(id);
	vboTrianglesSize.push_back(static_cast<int>(triangles.size()));
}

QMatrix4x4 orthonormalSystem(QVector3D e3) {
	qreal u[3],v[3];
        e3.normalize();
	v[0] = e3.x();
	v[1] = e3.y();
	v[2] = e3.z();
	int imax = 0;
	if (fabs(v[1]) > fabs(v[imax])) imax = 1;
	if (fabs(v[2]) > fabs(v[imax])) imax = 2;
	u[imax] = v[(imax+1)%3];
	u[(imax+1)%3] = -v[imax];
	u[(imax+2)%3] = 0.0;
	QVector3D e2(u[0],u[1],u[2]);
	e2.normalize();
	QVector3D e1 = QVector3D::crossProduct(e2,e3);
	e1.normalize();

	return QMatrix4x4(e1.x(),e2.x(),e3.x(),0.0,
	                  e1.y(),e2.y(),e3.y(),0.0,
	                  e1.z(),e2.z(),e3.z(),0.0,
	                  0.0,   0.0,   0.0,   1.0);
}

void CGView::drawSolidCylinder(const QVector3D& a, const QVector3D& b, qreal radius) {
	QMatrix4x4 M(modelView);
        M.translate(a);
	M *= orthonormalSystem(b-a);
        M.scale(radius,radius,(b-a).length());

	program.setUniformValue("uMVMat", M);
	program.setUniformValue("uNMat", M.normalMatrix());

	glBindBuffer(GL_ARRAY_BUFFER, vboSolidCylinderId);
	int vertexLocation = program.attributeLocation("a_position");
	program.enableAttributeArray(vertexLocation);
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 2*sizeof(QVector3D), 0);
	int normalLocation = program.attributeLocation("a_normal");
	program.enableAttributeArray(normalLocation);
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 2*sizeof(QVector3D), (const void*) sizeof(QVector3D));

	glDrawArrays(GL_TRIANGLES,0,vboSolidCylinderSize);
}

void CGView::drawSolidSphere(const QVector3D& c, qreal r) {
	QMatrix4x4 M(modelView);
	M.translate(c); 
	M.scale(r);

	program.setUniformValue("uMVMat", M);
	program.setUniformValue("uNMat", M.normalMatrix());

	glBindBuffer(GL_ARRAY_BUFFER, vboSolidSphereId);
	int vertexLocation = program.attributeLocation("a_position");
	program.enableAttributeArray(vertexLocation);
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 2*sizeof(QVector3D), 0);
	int normalLocation = program.attributeLocation("a_normal");
	program.enableAttributeArray(normalLocation);
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 2*sizeof(QVector3D), (const void*) sizeof(QVector3D));

	glDrawArrays(GL_TRIANGLES,0,vboSolidSphereSize);
}

void CGView::drawOBB(const QMatrix4x4& R,const QVector3D& l) {
	QMatrix4x4 M(modelView);
	M *= R;
	M.scale(l.x()+0.001,l.y()+0.001,l.z()+0.001);

	program.setUniformValue("uMVMat", M);
	program.setUniformValue("uNMat", M.normalMatrix());

	glBindBuffer(GL_ARRAY_BUFFER, vboSolidCubeId);
	int vertexLocation = program.attributeLocation("a_position");
	program.enableAttributeArray(vertexLocation);
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 2*sizeof(QVector3D), 0);
	int normalLocation = program.attributeLocation("a_normal");
	program.enableAttributeArray(normalLocation);
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 2*sizeof(QVector3D), (const void*) sizeof(QVector3D));

	glDrawArrays(GL_TRIANGLES,0,vboSolidCubeSize);
}

void CGView::drawAABB(const QVector3D& bMin, const QVector3D& bMax) {
	QMatrix4x4 M(modelView);
	M.translate((bMax.x()+bMin.x())/2,(bMax.y()+bMin.y())/2,(bMax.z()+bMin.z())/2);
	M.scale((bMax.x()-bMin.x())/2+0.001,(bMax.y()-bMin.y())/2+0.001,(bMax.z()-bMin.z())/2+0.001);

	program.setUniformValue("uMVMat", M);
	program.setUniformValue("uNMat", M.normalMatrix());

	glBindBuffer(GL_ARRAY_BUFFER, vboSolidCubeId);
	int vertexLocation = program.attributeLocation("a_position");
	program.enableAttributeArray(vertexLocation);
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 2*sizeof(QVector3D), 0);
	int normalLocation = program.attributeLocation("a_normal");
	program.enableAttributeArray(normalLocation);
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 2*sizeof(QVector3D), (const void*) sizeof(QVector3D));

	glDrawArrays(GL_TRIANGLES,0,vboSolidCubeSize);
}

