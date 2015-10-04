precision mediump float;
uniform sampler2D u_TextureUnit;
varying vec2 v_TextureCoordinates;
void main()
{
  gl_FragColor = gl_FragColor + texture2D(u_TextureUnit, v_TextureCoordinates) * vec4(texture2D(u_TextureUnit, v_TextureCoordinates).a);
}
