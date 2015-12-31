uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_PerspectiveMatrix;

uniform vec4 u_LightPosition;

attribute vec4 a_Position;
attribute vec4 a_TextureCoordinates;
attribute vec4 a_Normal;

varying vec2 v_TextureCoordinates;
varying vec4 v_Vertex;
varying vec4 v_Normal;

void main()
{
  mat4 MVP = u_PerspectiveMatrix * u_ViewMatrix * u_ModelMatrix;

  v_TextureCoordinates = a_TextureCoordinates;
  v_Vertex = MVP * a_Position;
  v_Normal = normalize(MVP * a_Normal);
  
  gl_Position = v_Vertex;
  gl_Position.z = -1.0 + (gl_Position.z / 10.0);
  
  // gl_Normal = v_Normal;
}
