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
   vec4 AmbientLight = vec4(0.4);
   vec4 DirectionalLightPosition = vec4(1.0, 1.0, 1.0, 1.0);

   vec4 L = normalize(DirectionalLightPosition - v_Vertex);
   vec4 E = normalize(-v_Vertex);
   vec4 R = normalize(-reflect(L,v_Normal));
   
   vec4 Iamb = AmbientLight;
   
   float d = max(dot(v_Normal, L), 0.0);
   float c = 0.4;
   
   if (d > 0.95)
	 c = 1.5;
   else if (d > 0.5)
     c = 0.6;
   else if (d > 0.25)
     c = 0.4;
   
   vec4 Light = vec4(c, c, c, 1.0);
  
   gl_FragColor = gl_FragColor + texture2D(u_SurfaceTextureUnit, v_Texture) * vec4(texture2D(u_SurfaceTextureUnit, v_Texture).a) * Light;
}