precision mediump float;
uniform sampler2D u_TextureUnit;
uniform vec4 u_LightPosition;

varying vec2 v_TextureCoordinates;
varying vec4 v_Vertex;
varying vec4 v_Normal;

void main()
{
   vec3 L = normalize(u_LightPosition - v_Vertex);   
   vec4 Idiff = 10.0 * max(dot(v_Normal,L), 0.0);  
   Idiff = clamp(Idiff, 0.80, 1.0); 
  
  //vec4 LightIntensity = clamp(;

  gl_FragColor = gl_FragColor + texture2D(u_TextureUnit, v_TextureCoordinates) * vec4(texture2D(u_TextureUnit, v_TextureCoordinates).a) * vec4(Idiff.xyz, 1.0);
}
