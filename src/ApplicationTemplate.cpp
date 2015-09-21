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
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#endif

#include "mylo.hpp"

#include "linmath.h"

#include <AL/al.h>
#include <AL/alc.h>


static inline float deg_to_radf(float deg) {
  return deg * (float) M_PI / 180.0f;
}

// const char *v = (const char *) glGetString(s);
// for (GLint error = glGetError(); error; error = glGetError()) {

GLuint gvPositionHandle;

GLuint gTextureProgram;
GLuint gvTexturePositionHandle;

GLint a_position_location;
GLint a_texture_coordinates_location;
GLint u_mvp_matrix_location;
GLint u_texture_unit_location;

#define BUFFER_OFFSET(i) ((void*)(i))

static mat4x4 projection_matrix;
//static mat4x4 model_matrix;
//static mat4x4 view_matrix;

//static mat4x4 view_projection_matrix;
//static mat4x4 model_view_projection_matrix;
//static mat4x4 inverted_view_projection_matrix;

//static mat4x4 background_projection_matrix;

my::fnt font;

my::program color_program;
my::frag color_fragment;
my::vert color_vertex;

my::program texture_alpha_program;
my::frag texture_alpha_fragment;
my::vert texture_alpha_vertex;

my::shared_ptr<my::object> gbackground;

my::png gtexture;

my::obj gapple;

my::wav sound;


GLuint compile(my::shader *shader, GLuint type) { DEBUG_SCOPE;

  DEBUG_TRACE << "compile shader: " << type << my::endl;

  shader->context = glCreateShader(type);
  if (!shader->context) {
    return 0;
  }

  GLint length;
  GLchar *text = shader->text.c_str();
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

  return shader->context;
}

GLuint compile(my::program &program) {
  if (!compile(program.vertex, GL_VERTEX_SHADER)) {
    return 0;
  }

  if (!compile(program.fragment, GL_FRAGMENT_SHADER)) {
    return 0;
  }

  program.context = glCreateProgram();
  if (!program.context) {
    return 0;
  }

  glAttachShader(program.context, program.vertex->context);
  //checkGlError("glAttachShader");

  glAttachShader(program.context, program.fragment->context);
  //checkGlError("glAttachShader");

  glLinkProgram(program.context);

  GLint linkStatus = GL_FALSE;
  glGetProgramiv(program.context, GL_LINK_STATUS, &linkStatus);
  if (linkStatus != GL_TRUE) {
    GLint length = 0;
    glGetProgramiv(program.context, GL_INFO_LOG_LENGTH, &length);
    if (length) {
      char *info = (char*)malloc(length);
      glGetProgramInfoLog(program.context, length, NULL, info);
      //DEBUG("Could not link program:\n%s\n", info);
      free(info);
    }
    glDeleteProgram(program.context);
    program.context = 0;
  }

  return program.context;
}


void compile(my::shared_ptr<my::object> object) {
  glGenBuffers(1, &object->context);
  glBindBuffer(GL_ARRAY_BUFFER, object->context);
  glBufferData(GL_ARRAY_BUFFER, sizeof(my::object::vertex) * object->vertices.size(), object->vertices.pointer, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenTextures(1, &object->texture->context);
  glBindTexture(GL_TEXTURE_2D, object->texture->context);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, object->texture->map->header.width, object->texture->map->header.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, object->texture->map->raster);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}
void compile(my::objects &objects) {
  for (my::map<my::string, my::shared_ptr<my::object> >::iterator it = objects.object.begin(); it != objects.object.end(); it++) {
    compile(it->second);
  }
}

void compile(my::audio &sound) {
  alGenSources(1, &sound.source);

  alSourcef(sound.source, AL_PITCH, 1.0f);
  alSourcef(sound.source, AL_GAIN, 1.0f);
  alSource3f(sound.source, AL_POSITION, 0.0f, 0.0f, -1.0f);
  alSource3f(sound.source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
  alSourcei(sound.source, AL_LOOPING, AL_FALSE);

  alGenBuffers(1, &sound.context);
  
  //alBufferData(sound.context, AL_FORMAT_MONO16, sound.data, sound.size, 44100);
  alBufferData(sound.context, AL_FORMAT_MONO8, sound.data, sound.size, 11000);
}

void draw(my::shared_ptr<my::object> object, mat4x4 matrix) {
  glUseProgram(texture_alpha_program.context);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, object->texture->context);

  glUniformMatrix4fv(u_mvp_matrix_location, 1, GL_FALSE, (GLfloat*)matrix);
  glUniform1i(u_texture_unit_location, 0);

  glBindBuffer(GL_ARRAY_BUFFER, object->context);
  glVertexAttribPointer(a_position_location, 4, GL_FLOAT, GL_FALSE, sizeof(my::object::vertex), BUFFER_OFFSET(0));
  glVertexAttribPointer(a_texture_coordinates_location, 2, GL_FLOAT, GL_FALSE, sizeof(my::object::vertex), BUFFER_OFFSET(4 * sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(a_position_location);
  glEnableVertexAttribArray(a_texture_coordinates_location);

  //glDrawArrays(GL_TRIANGLE_FAN, 0, object->vertices.size());
  glDrawArrays(GL_TRIANGLES, 0, object->vertices.size());

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void draw(my::objects &objects, mat4x4 matrix) {
  for (my::map<my::string, my::shared_ptr<my::object> >::iterator it = objects.object.begin(); it != objects.object.end(); it++) {
    draw(it->second, matrix);
  }
}


void draw(my::string text, mat4x4 matrix) {
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

void play(my::audio &sound) {
  alSourcei(sound.source, AL_BUFFER, sound.context);
  alSourcePlay(sound.source);
  alSource3f(sound.source, AL_POSITION, 0.0f, 0.0f, -1.0f);
  alSource3f(sound.source, AL_VELOCITY, 0.0f, 0.0f, -1.0f);
}


float input_x;
float input_y;

float input_z;

my::trace::console tracer;

void on_startup(void *asset_manager) {
  DEBUG_TRACE << "Attempting to create the draw surface" << my::endl;

  //glDisable(GL_DEPTH_TEST);

  glDepthFunc(GL_LEQUAL);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  my::asset::manager(asset_manager);

  color_program << color_fragment << my::asset("shaders/color_shader.fsh");
  color_program << color_vertex << my::asset("shaders/color_shader.vsh");

  DEBUG_TRACE << "Fragment shader: " << my::endl << color_fragment.text << my::endl;
  DEBUG_TRACE << "Vertex shader: " << my::endl << color_vertex.text << my::endl;

  compile(color_program);

  gvPositionHandle = glGetAttribLocation(color_program.context, "vPosition");
	//checkGlError("glGetAttribLocation");

  DEBUG_TRACE << "Attempting to create the draw surface" << my::endl;

  texture_alpha_program << texture_alpha_fragment << my::asset("shaders/texture_alpha_shader.frag");
  texture_alpha_program << texture_alpha_vertex << my::asset("shaders/texture_alpha_shader.vert");

  compile(texture_alpha_program);

  a_position_location = glGetAttribLocation(texture_alpha_program.context, "a_Position");
  a_texture_coordinates_location = glGetAttribLocation(texture_alpha_program.context, "a_TextureCoordinates");
  u_mvp_matrix_location = glGetUniformLocation(texture_alpha_program.context, "u_MvpMatrix");
  u_texture_unit_location = glGetUniformLocation(texture_alpha_program.context, "u_TextureUnit");

  //
  // Just testing out some file system abstractions
  //
  DEBUG_TRACE << "pwd: " << my::pwd() << my::endl;
  my::directory dir(my::pwd());
  while(my::string current = dir.next()) {
    DEBUG_TRACE << "contents: " << current << my::endl;
  }

  //
  // Load up an apple
  //
  gapple.search_path = "models/obj";
  gapple << my::asset("models/obj/apple.obj");

  compile(gapple);

  //
  // Load up the font
  //
  font << my::asset("fonts/arial.fnt");
  int x = font.glyphs.size();
  for (unsigned int i = font.glyphs.offset(); i < font.glyphs.size(); i++) {
    if (font.glyphs[i]->identifier) {
      compile(font.glyphs[i]->quad);
    }
  }

  //
  // Load up the background
  //
  gbackground = my::primitive::quad(256, 256);
  my::image *img = new my::png;
  *img << my::asset("textures/landscape2.png");
  gbackground->xy_projection(img, 0, 0, 256, 256);
  compile(gbackground);

//  gbackground = my::primitive::quad(256, 256);
//  my::image *img = new my::tga;
//  *img << my::asset("models/obj/sphere1_auv.tga");
//  gbackground->xy_projection(img, 0, 0, 1024, 1024);
//  compile(gbackground);

//  gbackground = my::primitive::quad(256, 256);
//  my::image *img = new my::png;
//  *img << my::asset("models/obj/cube1_auv.png");
//  gbackground->xy_projection(img, 0, 0, 1024, 1024);
//  compile(gbackground);

  //
  // Default the inputs
  //
  input_x = 320.0f;
  input_y = 240.0f;
  input_z = 1.0f;

  //
  // Init sound
  //
  ALCint attributes[] = { ALC_FREQUENCY, 44100, 0 };

  ALCdevice* device = alcOpenDevice(NULL);
  ALCcontext* context = alcCreateContext(device, attributes);

  alcMakeContextCurrent(context);

  float orientation[6] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };

  alListener3f(AL_POSITION, 0., 1.5, 0.);
  alListener3f(AL_VELOCITY, 0., 0., 0.);
  alListenerfv(AL_ORIENTATION, orientation);

  sound << my::asset("sounds/mind_is_going.wav");
  compile(sound);
}

void on_shutdown() {
  alDeleteSources(1, &sound.source);
  alDeleteBuffers(1, &sound.context);
  //alcDestroyContext(context);
  //alcCloseDevice(device);
}

int screen_width;
int screen_height;

void on_resize(int width, int height) {
  //DEBUG("Draw surface changed\n");
  glViewport(0, 0, width, height);
  //checkGlError("glViewport");
  
  screen_width = width;
  screen_height = height;

  //mat4x4_perspective(projection_matrix, deg_to_radf(90), (float)width / (float)height, 0.0f, 20.0f);
  mat4x4_perspective(projection_matrix, deg_to_radf(90), (float)width / (float)height, -1.0f, 1.0f);
}


static mat4x4 identity;

void pprint(mat4x4 mat) {
  printf("----------------------------\n");
  for (int i = 0; i < 4; i++) {
    printf( "%f,%f,%f,%f\n", mat[i][0], mat[i][1], mat[i][2], mat[i][3] );
  }
}


void on_draw() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  //checkGlError("glClearColor");
  glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  //checkGlError("glClear");

  static float angle = 0.0f;
  angle -= 0.1f;
  if (angle <= -90.0f) {
    angle = 0.0f;
  }

  mat4x4_identity(identity);

  mat4x4 scale;

  mat4x4_identity(scale);

  mat4x4_scale( scale , identity, 0.01f);
  scale[3][3] = 1.0f;
  mat4x4_translate_in_place( scale, -130.0f, -120.0f, input_z);

  //
  // render background
  //
  //draw(gbackground, scale);

  //
  // position
  //
  /*
  mat4x4_mul(view_projection_matrix, projection_matrix, view_matrix);
  mat4x4_invert(inverted_view_projection_matrix, view_projection_matrix);

  mat4x4 rotated_model_matrix;
  mat4x4_identity(model_matrix);

  mat4x4_rotate_X(rotated_model_matrix, model_matrix, deg_to_radf(angle));
  mat4x4_translate_in_place(rotated_model_matrix, 0.0f, 0.0f, 1.0f);

  mat4x4_mul(model_view_projection_matrix, view_projection_matrix, rotated_model_matrix);
  */

//  float eye[3] = { 10.0f, 4.0f, 1.0f };
//  float center[3] = { 10.0f, 4.0f, 0.0f };
//  float up[3] = { 0.0f, 1.0f, 0.0f };

  static mat4x4 view_matrix;

//  float eye[3] = { input_x / 10, input_y / 10, 1.0f };
//  float center[3] = { input_x / 10, input_y / 10, 0.0f };
//  float up[3] = { 0.0f, 1.0f, 0.0f };

  float eye[3] = { ((input_x - (screen_width/2)) / 10) * -1, (input_y - (screen_height/2)) / 10, 1.0f + input_z };
  float center[3] = { ((input_x - (screen_width/2)) / 10) * -1, (input_y - (screen_height/2)) / 10, 0.0f + input_z };
  float up[3] = { 0.0f, 1.0f, 0.0f };

//  float eye[3] = { 10.0f, 4.0f, 1.0f };
//  float center[3] = { 10.0f, 4.0f, 0.0f };
//  float up[3] = { 0.0f, 1.0f, 0.0f };

  mat4x4_look_at(view_matrix, eye, center, up);

  mat4x4 model_view_projection_matrix;
  mat4x4 model_matrix;

  mat4x4_identity(model_matrix);

  static float rotation = 0.0f;

  rotation += 0.05f;

  mat4x4 model_scale_matrix;
  mat4x4_identity(model_scale_matrix);

  mat4x4_rotate_Y(model_scale_matrix, model_matrix, rotation);

//  mat4x4_scale(model_scale_matrix, model_matrix, input_y);
//  mat4x4_translate_in_place(model_scale_matrix, 0.0f, 0.0f, input_x);

  mat4x4 view_projection_matrix;

  mat4x4_mul(view_projection_matrix, projection_matrix, view_matrix);

  mat4x4_mul(model_view_projection_matrix, view_projection_matrix, model_scale_matrix);

  draw(gapple, model_view_projection_matrix);

  //
  // font 
  //
  mat4x4 letter;

  mat4x4_identity(letter);
  letter[0][0] = 0.01f;
  letter[1][1] = 0.01f;
  letter[2][2] = 0.01f;
  mat4x4_translate_in_place(letter, -100.0f, 0.0f, 0.08f);
  draw("C", model_view_projection_matrix);

  mat4x4_identity(letter);
  letter[0][0] = 0.01f;
  letter[1][1] = 0.01f;
  letter[2][2] = 0.01f;
  mat4x4_translate_in_place(letter, -100.0f, 20.0f, 0.08f);
  draw(my::type_cast<my::string>(input_x), letter);

  mat4x4_identity(letter);
  letter[0][0] = 0.01f;
  letter[1][1] = 0.01f;
  letter[2][2] = 0.01f;
  mat4x4_translate_in_place(letter, -100.0f, 40.0f, 0.08f);
  draw(my::type_cast<my::string>(input_y), letter);

  mat4x4_identity(letter);
  letter[0][0] = 0.01f;
  letter[1][1] = 0.01f;
  letter[2][2] = 0.01f;
  mat4x4_translate_in_place(letter, -100.0f, 60.0f, 0.08f);
  draw(my::type_cast<my::string>(input_z), letter);

}

void on_touch_press(float normalized_x, float normalized_y) {
  input_x = normalized_x;
  input_y = normalized_y;

  play(sound);
}

void on_touch_drag(float normalized_x, float normalized_y) {
  input_x = normalized_x;
  input_y = normalized_y;
}

void on_touch_release(float normalized_x, float normalized_y) {
  input_x = normalized_x;
  input_y = normalized_y;
}

void on_touch_zoom_in() {
  input_z += 0.1f;
}

void on_touch_zoom_out() {
  input_z -= 0.1f;
}
