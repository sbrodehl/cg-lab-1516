#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

attribute vec3 a_position;
attribute vec3 aTexCoord;

varying vec3 vTexCoord;

uniform mat4 uMVMat;
uniform mat4 uPMat;

void main() {
	vec4 p = uMVMat * vec4(a_position,1.0);
	gl_Position = uPMat * vec4(a_position,1.0);
	vTexCoord = vec3(0.5*p.x+0.5,0.5*p.y+0.5,0.5*p.z+0.5);
}
