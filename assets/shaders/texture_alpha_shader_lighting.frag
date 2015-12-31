precision mediump float;

uniform sampler2D u_SurfaceTextureUnit;

uniform vec4 u_AmbientLight;
uniform vec4 u_DirectionalLight;
uniform vec4 u_DirectionalLightPosition;

varying vec2 v_Texture;
varying vec4 v_Vertex;
varying vec4 v_Normal;

void main()
{
   vec3 L = normalize(u_LightPosition - v_Vertex);
//   vec3 E = normalize(-v_Vertex);
//   vec3 R = normalize(-reflect(L,v_Normal))
   
   //vec4 Iamb = vec4(0.8,0.8,0.8,0.8)  
   vec4 Idiff = 0.6 * max(dot(v_Normal, L), 0.0);
   Idiff = clamp(Idiff, 0.0, 1.0);
   
   vec4 Light = Idiff;
   
   //vec4 Light = Iamb + Idiff;
  
   gl_FragColor = gl_FragColor + texture2D(u_SurfaceTextureUnit, v_Texture) * vec4(texture2D(u_SurfaceTextureUnit, v_Texture).a);// * Light;
}
