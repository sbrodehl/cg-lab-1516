#include <ostream>
#include <iostream>
#include "Parametrics.h"

void Parametrics::loadEq() {
    std::cout << "Hello World!" << std::endl;
}

QVector3D Parametrics::getPoint(double u, double v) {
    return QVector3D();
}

QVector3D Parametrics::parameterizedTorus(float u, float v){
    // u and v in [0, 1)
    float u_scaled = u*2*PI;
    float v_scaled = v*2*PI;
    // hard coded main circle radius R and sub circle radius r
    float R = 5;
    float r = 1;

    float x = (R + r*cos(v_scaled))*cos(u_scaled);
    float y = (R + r*cos(v_scaled))*sin(u_scaled);
    float z = r*sin(v_scaled);

    return QVector3D(x, y, z);
}

QVector3D Parametrics::parameterizedTrefoilKnot(float u, float v){
    // u and v in [0, 1)
    float u_scaled = u*2*PI;
    float v_scaled = v*2*PI;
    // hard coded main circle radius R and sub circle radius r
    float r = 0.5;

    float x = (3+cos(3*u_scaled))*cos(2*u_scaled) + r*cos(v_scaled)*cos(2*u_scaled);
    float y = (3+cos(3*u_scaled))*sin(2*u_scaled) + r*cos(v_scaled)*sin(2*u_scaled);
    float z = sin(3*u_scaled) + r*sin(v_scaled);

    return QVector3D(x, y, z);
}

Parametrics::Parametrics() {
    // constructor
}

Parametrics::~Parametrics() {
    // deconstructor
}
