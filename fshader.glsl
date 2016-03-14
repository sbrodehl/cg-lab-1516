#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform vec4 uColor;
varying float vIntensity;

void main()
{
    gl_FragColor = vIntensity*uColor;
}

