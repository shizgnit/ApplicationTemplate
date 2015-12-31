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
   gl_FragColor = gl_FragColor + texture2D(u_SurfaceTextureUnit, v_Texture) * vec4(texture2D(u_SurfaceTextureUnit, v_Texture).a);
}
