#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define GLEW_STATIC

#ifndef _WIN32
#include <GLES2/gl2.h>
#include <android/log.h>
#else
#include <GL\glew.h>
#include <GL\gl.h>
#include <GL\glu.h>
#endif

#include "mylo.hpp"

#include "linmath.h"

static inline float deg_to_radf(float deg) {
  return deg * (float) M_PI / 180.0f;
}

void graphics::compile(my::shader *shader, unsigned int type) { DEBUG_SCOPE;
  DEBUG_TRACE << "compile shader: " << type << my::endl;

  shader->context = glCreateShader(type);
  if (!shader->context) {
    return 0;
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

void graphics::compile(my::program &program) {
  compile(program.vertex, GL_VERTEX_SHADER);
  compile(program.fragment, GL_FRAGMENT_SHADER);

  program.context = glCreateProgram();
  if (!program.context) {
    return 0;
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

void graphics::compile(my::object &object) {
  glGenBuffers(1, &object.context);
  glBindBuffer(GL_ARRAY_BUFFER, object.context);
  glBufferData(GL_ARRAY_BUFFER, sizeof(my::object::vertex) * object.vertices.size(), object.vertices.pointer, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenTextures(1, &object.texture->context);
  glBindTexture(GL_TEXTURE_2D, object.texture->context);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, object.texture->map->header.width, object.texture->map->header.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, object->texture->map->raster);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void graphics::compile(my::objects &objects) {
  for (my::map<my::string, my::shared_ptr<my::object> >::iterator it = objects.object.begin(); it != objects.object.end(); it++) {
    compile(it->second);
  }
}

void graphics::draw(my::object &object, mat4x4 matrix) {
  glUseProgram(texture_alpha_program.context);

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

void graphics::draw(my::objects &objects, mat4x4 matrix) {
  for (my::map<my::string, my::shared_ptr<my::object> >::iterator it = objects.object.begin(); it != objects.object.end(); it++) {
    draw(it->second, matrix);
  }
}

void graphics::draw(my::string text, mat4x4 matrix) {
  int prior = 0;
  for (unsigned int i = 0; i < text.length(); i++) {
    mat4x4_translate_in_place(matrix, (float)font.kern(prior, text[i]), 0.0f, 0.0f);
    mat4x4 temporary;
    mat4x4_dup(temporary, matrix);
    mat4x4_translate_in_place(temporary, (float)font.glyphs[text[i]]->xoffset, 0.0f, 0.0f);
    //mat4x4_translate_in_place(temporary, font.glyphs[text[i]]->xoffset, font.glyphs[text[i]]->yoffset, 0.0f);
    draw(font.glyphs[text[i]]->quad, temporary);
    mat4x4_translate_in_place(matrix, (float)font.glyphs[text[i]]->xadvance, 0.0f, 0.0f);
    prior = text[i];
  }

}

