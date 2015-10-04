uniform mat4 u_MvpMatrix;
attribute vec4 a_Position;
attribute vec2 a_TextureCoordinates;
varying vec2 v_TextureCoordinates;
void main()
{
  v_TextureCoordinates = a_TextureCoordinates;
  gl_Position = u_MvpMatrix * a_Position;
  gl_Position.z = -1.0 + (gl_Position.z / 10.0);
}
