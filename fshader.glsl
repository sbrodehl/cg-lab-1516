#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

varying vec3 vTexCoord;
uniform sampler3D uSampler;

void main()
{
//  if ((vTexCoord.x < 0) || (vTexCoord.x > 1) || 
//      (vTexCoord.y < 0) || (vTexCoord.y > 1) || 
//      (vTexCoord.z < 0) || (vTexCoord.z > 1))
//    gl_FragColor = vec4(0,0,0,1);
//  else {
      vec4 c = texture3D(uSampler,vTexCoord);
//    int i = int(mod(floor(512.0*vTexCoord.x),4));
//    gl_FragColor = vec4(c[i],c[i],c[i],1);
      gl_FragColor = vec4(c[0],c[0],c[0],1);
//  }
}

