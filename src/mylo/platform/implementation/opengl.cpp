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

void opengl::init(void) {
  DEBUG_TRACE << "Attempting to create the draw surface" << my::endl;

  //glDisable(GL_DEPTH_TEST);

  glDepthFunc(GL_LEQUAL);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void opengl::clear(void) {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void opengl::compile(my::shader *shader, unsigned int type) { DEBUG_SCOPE;
  DEBUG_TRACE << "compile shader: " << type << my::endl;

  shader->context = glCreateShader(type);
  if (!shader->context) {
    return;
  }

  GLchar *text = shader->text.c_str();
  GLint length = strlen(text);
  glShaderSource(shader->context, 1, (const GLchar **)&text, &length);

  glCompileShader(shader->context);

  GLint compiled = 0;
  glGetShaderiv(shader->context, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    glGetShaderiv(shader->context, GL_INFO_LOG_LENGTH, &length);
    if (length) {
      char *info = (char*)malloc(length);
      glGetShaderInfoLog(shader->context, length, NULL, info);
      DEBUG_TRACE << "Could not compile shader " << type << ", " << info;

      free(info);
      glDeleteShader(shader->context);
      shader->context = 0;
    }
  }
}

void opengl::compile(my::program &program) {
  compile(program.vertex, GL_VERTEX_SHADER);
  compile(program.fragment, GL_FRAGMENT_SHADER);

  program.context = glCreateProgram();
  if (!program.context) {
    return;
  }

  glAttachShader(program.context, program.vertex->context);

  glAttachShader(program.context, program.fragment->context);

  glLinkProgram(program.context);

  GLint linkStatus = GL_FALSE;
  glGetProgramiv(program.context, GL_LINK_STATUS, &linkStatus);
  if (linkStatus != GL_TRUE) {
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
}

void opengl::compile(my::object &object) {
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

void opengl::compile(my::objects &objects) {
  for (my::map<my::string, my::shared_ptr<my::object> >::iterator it = objects.object.begin(); it != objects.object.end(); it++) {
    compile(*it->second);
  }
}

void opengl::draw(my::object &object, mat4x4 matrix) {
  glUseProgram(program.context);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, object.texture->context);

  glUniformMatrix4fv(u_mvp_matrix_location, 1, GL_FALSE, (GLfloat*)matrix);
  glUniform1i(u_texture_unit_location, 0);

  glBindBuffer(GL_ARRAY_BUFFER, object.context);
  glVertexAttribPointer(a_position_location, 4, GL_FLOAT, GL_FALSE, sizeof(my::object::vertex), BUFFER_OFFSET(0));
  glVertexAttribPointer(a_texture_coordinates_location, 2, GL_FLOAT, GL_FALSE, sizeof(my::object::vertex), BUFFER_OFFSET(4 * sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(a_position_location);
  glEnableVertexAttribArray(a_texture_coordinates_location);

  //glDrawArrays(GL_TRIANGLE_FAN, 0, object.vertices.size());
  glDrawArrays(GL_TRIANGLES, 0, object.vertices.size());

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void opengl::draw(my::objects &objects, mat4x4 matrix) {
  for (my::map<my::string, my::shared_ptr<my::object> >::iterator it = objects.object.begin(); it != objects.object.end(); it++) {
    draw(*it->second, matrix);
  }
}

void opengl::draw(my::string text, mat4x4 matrix) {
  int prior = 0;
  for (unsigned int i = 0; i < text.length(); i++) {
    mat4x4_translate_in_place(matrix, (float)font.kern(prior, text[i]), 0.0f, 0.0f);
    mat4x4 temporary;
    mat4x4_dup(temporary, matrix);
    mat4x4_translate_in_place(temporary, (float)font.glyphs[text[i]]->xoffset, 0.0f, 0.0f);
    //mat4x4_translate_in_place(temporary, font.glyphs[text[i]]->xoffset, font.glyphs[text[i]]->yoffset, 0.0f);
    draw(*font.glyphs[text[i]]->quad, temporary);
    mat4x4_translate_in_place(matrix, (float)font.glyphs[text[i]]->xadvance, 0.0f, 0.0f);
    prior = text[i];
  }
}

void opengl::set_font(my::string file) {
  font << my::asset(file);
  int x = font.glyphs.size();
  for (unsigned int i = font.glyphs.offset(); i < font.glyphs.size(); i++) {
    if (font.glyphs[i]->identifier) {
      compile(*font.glyphs[i]->quad);
    }
  }
}

void opengl::set_program(my::string vert_file, my::string frag_file) {
  program << frag << my::asset(frag_file);
  program << vert << my::asset(vert_file);

  DEBUG_TRACE << "attempting to compile" << my::endl;

  compile(program);

  DEBUG_TRACE << "compiled" << my::endl;

  a_position_location = glGetAttribLocation(program.context, "a_Position");
  a_texture_coordinates_location = glGetAttribLocation(program.context, "a_TextureCoordinates");
  u_mvp_matrix_location = glGetUniformLocation(program.context, "u_MvpMatrix");
  u_texture_unit_location = glGetUniformLocation(program.context, "u_TextureUnit");
}


__PLATFORM_NAMESPACE_END

// Local Variables:
// mode:C++
// End:

