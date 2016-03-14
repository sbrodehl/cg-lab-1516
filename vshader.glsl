#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

attribute vec3 a_position;
attribute vec3 a_normal;

uniform mat4 uMVMat;
uniform mat4 uPMat;
uniform mat3 uNMat;

varying float vIntensity;

void main() {
	gl_Position = uPMat * uMVMat * vec4(a_position,1.0);
	vec3 n = normalize(uNMat * a_normal);
	vIntensity = abs(n.z);
}
