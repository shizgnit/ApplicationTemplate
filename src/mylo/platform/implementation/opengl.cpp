/*
================================================================================

Copyright (c) 2012, Dee E. Abbott
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.
* Neither the name of the organization nor the names of its contributors may
be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

================================================================================
*/

#include "opengl.hpp"

__PLATFORM_NAMESPACE_BEGIN

void opengl::graphics::init(void) {
  DEBUG_TRACE << "Attempting to create the draw surface" << my::endl;

  //glDisable(GL_DEPTH_TEST);

  glDepthFunc(GL_LEQUAL);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void opengl::graphics::clear(void) {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void opengl::graphics::compile(my::shader &shader, unsigned int type) { DEBUG_SCOPE;
  DEBUG_TRACE << "compile shader: " << type << my::endl;

  shader.context = glCreateShader(type);
  if (!shader.context) {
	DEBUG_TRACE << "failed to create shader context" << my::endl;
    return;
  }

  DEBUG_TRACE << "pulling shader text" << my::endl;
  GLchar *text = shader.text.c_str();
  GLint length = strlen(text);
  DEBUG_TRACE << length << " bytes" << my::endl;
  glShaderSource(shader.context, 1, (const GLchar **)&text, &length);

  glCompileShader(shader.context);

  GLint compiled = GL_FALSE;
  glGetShaderiv(shader.context, GL_COMPILE_STATUS, &compiled);
  if (compiled == GL_FALSE) {
    glGetShaderiv(shader.context, GL_INFO_LOG_LENGTH, &length);
    if (length) {
      char *info = (char*)malloc(length);
      glGetShaderInfoLog(shader.context, length, NULL, info);
      DEBUG_TRACE << "Could not compile shader " << type << ", " << info;

      free(info);
      glDeleteShader(shader.context);
      shader.context = 0;
    }
  }
}

void opengl::graphics::compile(my::program &program) {
  compile(program.vertex, GL_VERTEX_SHADER);
  compile(program.fragment, GL_FRAGMENT_SHADER);

  program.context = glCreateProgram();
  if (!program.context) {
    return;
  }

  glAttachShader(program.context, program.vertex.context);

  glAttachShader(program.context, program.fragment.context);

  glLinkProgram(program.context);

  GLint linked = GL_FALSE;
  glGetProgramiv(program.context, GL_LINK_STATUS, &linked);
  if (linked != GL_TRUE) {
    GLint length = 0;
    glGetProgramiv(program.context, GL_INFO_LOG_LENGTH, &length);
    if (length) {
      char *info = (char*)malloc(length);
      glGetProgramInfoLog(program.context, length, NULL, info);
      free(info);
    }
    glDeleteProgram(program.context);
    program.context = 0;
  }

  program.a_Vertex = glGetAttribLocation(program.context, "a_Vertex");
  program.a_Texture = glGetAttribLocation(program.context, "a_Texture");
  program.a_Normal = glGetAttribLocation(program.context, "a_Normal");

  program.u_ModelMatrix = glGetUniformLocation(program.context, "u_ModelMatrix");
  program.u_ViewMatrix = glGetUniformLocation(program.context, "u_ViewMatrix");
  program.u_ProjectionMatrix = glGetUniformLocation(program.context, "u_ProjectionMatrix");

  program.u_SurfaceTextureUnit = glGetUniformLocation(program.context, "u_SurfaceTextureUnit");

  program.u_AmbientLight = glGetUniformLocation(program.context, "u_AmbientLight");
  program.u_DirectionalLight = glGetUniformLocation(program.context, "u_DirectionalLight");
  program.u_DirectionalLightPosition = glGetUniformLocation(program.context, "u_DirectionalLightPosition");
}

void opengl::graphics::compile(my::object &object) {
  glGenBuffers(1, &object.context);
  glBindBuffer(GL_ARRAY_BUFFER, object.context);
  glBufferData(GL_ARRAY_BUFFER, sizeof(my::object::vertex) * object.vertices.size(), object.vertices.pointer, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenTextures(1, &object.texture->context);
  glBindTexture(GL_TEXTURE_2D, object.texture->context);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, object.texture->map->header.width, object.texture->map->header.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, object.texture->map->raster);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void opengl::graphics::compile(my::objects &objects) {
  for (my::map<my::string, my::shared_ptr<my::object> >::iterator it = objects.object.begin(); it != objects.object.end(); it++) {
    compile(*it->second);
  }
}

void opengl::graphics::compile(my::font &font) {
  int x = font.glyphs.size();
  for (unsigned int i = font.glyphs.offset(); i < font.glyphs.size(); i++) {
    if (font.glyphs[i]->identifier) {
      compile(*font.glyphs[i]->quad);
    }
  }
}


void opengl::graphics::draw(my::object &object, my::program &shader, const my::spatial::matrix &model, const my::spatial::matrix &view, const my::spatial::matrix &projection) {
  glUseProgram(shader.context);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, object.texture->context);

  glUniformMatrix4fv(shader.u_ModelMatrix, 1, GL_FALSE, (GLfloat *)model.r);
  glUniformMatrix4fv(shader.u_ViewMatrix, 1, GL_FALSE, (GLfloat *)view.r);
  glUniformMatrix4fv(shader.u_ProjectionMatrix, 1, GL_FALSE, (GLfloat *)projection.r);
  glUniform1i(shader.u_SurfaceTextureUnit, 0);

  glUniform4f(shader.u_AmbientLight, 0.0f, 0.0f, 0.0f, 1.0f);

  glUniform4f(shader.u_DirectionalLight, 0.0f, 0.0f, 0.0f, 1.0f);
  glUniform4f(shader.u_DirectionalLightPosition, 1.0f, 1.0f, 0.1f, 1.0f);

  glBindBuffer(GL_ARRAY_BUFFER, object.context);
  glVertexAttribPointer(shader.a_Vertex, 4, GL_FLOAT, GL_FALSE, sizeof(my::object::vertex), BUFFER_OFFSET(0));
  glVertexAttribPointer(shader.a_Texture, 4, GL_FLOAT, GL_FALSE, sizeof(my::object::vertex), BUFFER_OFFSET(4 * sizeof(GL_FLOAT)));
  glVertexAttribPointer(shader.a_Normal, 4, GL_FLOAT, GL_TRUE, sizeof(my::object::vertex), BUFFER_OFFSET(8 * sizeof(GL_FLOAT)));

  glEnableVertexAttribArray(shader.a_Vertex);
  glEnableVertexAttribArray(shader.a_Texture);
  glEnableVertexAttribArray(shader.a_Normal);

  //glDrawArrays(GL_TRIANGLE_FAN, 0, object.vertices.size());
  glDrawArrays(GL_TRIANGLES, 0, object.vertices.size());

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void opengl::graphics::draw(my::objects &objects, my::program &shader, const my::spatial::matrix &model, const my::spatial::matrix &view, const my::spatial::matrix &projection) {
  for (my::map<my::string, my::shared_ptr<my::object> >::iterator it = objects.object.begin(); it != objects.object.end(); it++) {
    draw(*it->second, shader, model, view, projection);
  }
}

void opengl::graphics::draw(my::string text, my::font &font, my::program &shader, const my::spatial::matrix &model, const my::spatial::matrix &view, const my::spatial::matrix &projection) {
  int prior = 0;
  my::spatial::matrix position = model;
  for (unsigned int i = 0; i < text.length(); i++) {
    position.translate(my::spatial::vector((float)font.kern(prior, text[i]), 0.0f, 1.0f));
    my::spatial::matrix relative = position;
    relative.translate(my::spatial::vector((float)font.glyphs[text[i]]->xoffset, 0.0f, 1.0f));
    draw(*font.glyphs[text[i]]->quad, shader, relative, view, projection);
    position.translate(my::spatial::vector((float)font.glyphs[text[i]]->xadvance, 0.0f, 1.0f));
    prior = text[i];
  }
}


__PLATFORM_NAMESPACE_END

// Local Variables:
// mode:C++
// End:

