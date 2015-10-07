#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "platform.hpp"
#include "mylo.hpp"

#include "linmath.h"

static inline float deg_to_radf(float deg) {
  return deg * (float) M_PI / 180.0f;
}

static mat4x4 projection_matrix;

my::shared_ptr<my::object> gbackground;

my::png gtexture;

my::obj gapple;

my::wav sound;

float input_x;
float input_y;
float input_z;

my::trace::console tracer;

void on_startup(void *asset_manager) {
  platform::graphics->init();

  my::asset::manager(asset_manager);

  DEBUG_TRACE << "Attempting to create the draw surface" << my::endl;

  platform::graphics->set_program("shaders/texture_alpha_shader.vert", "shaders/texture_alpha_shader.frag");

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

  platform::graphics->compile(gapple);

  //
  // Load up the font
  //
  platform::graphics->set_font("fonts/arial2.fnt");

  //
  // Load up the background
  //
  gbackground = my::primitive::quad(256, 256);
  my::image *img = new my::png;
  *img << my::asset("textures/landscape2.png");
  gbackground->xy_projection(img, 0, 0, 256, 256);
  platform::graphics->compile(*gbackground);

  //
  // Default the inputs
  //
  input_x = 320.0f;
  input_y = 240.0f;
  input_z = 1.0f;

  //
  // Init sound
  //
#ifndef __ANDROID__
  sound << my::asset("sounds/mind_is_going.wav");
  platform::audio->compile(sound);
#endif
}

void on_shutdown() {
}

int screen_width;
int screen_height;

void on_resize(int width, int height) {
  glViewport(0, 0, width, height);
  
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
  platform::graphics->clear();

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
  mat4x4_translate_in_place(scale, -130.0f, -120.0f, 1000.1f); // input_z);

  //
  // render background
  //
  platform::graphics->draw(*gbackground, scale);

  //
  // position
  //
  static mat4x4 view_matrix;

  float eye[3] = { ((input_x - (screen_width/2)) / 10) * -1, (input_y - (screen_height/2)) / 10, 1.0f + input_z };
  float center[3] = { ((input_x - (screen_width/2)) / 10) * -1, (input_y - (screen_height/2)) / 10, 0.0f + input_z };
  float up[3] = { 0.0f, 1.0f, 0.0f };

  mat4x4_look_at(view_matrix, eye, center, up);

  mat4x4 model_view_projection_matrix;
  mat4x4 model_matrix;

  mat4x4_identity(model_matrix);

  static float rotation = 0.0f;

  rotation += 0.05f;

  mat4x4 model_scale_matrix;
  mat4x4_identity(model_scale_matrix);

  mat4x4_rotate_Y(model_scale_matrix, model_matrix, rotation);

  mat4x4 view_projection_matrix;

  mat4x4_mul(view_projection_matrix, projection_matrix, view_matrix);

  mat4x4_mul(model_view_projection_matrix, view_projection_matrix, model_scale_matrix);

  platform::graphics->draw(gapple, model_view_projection_matrix);

  //
  // font 
  //
  mat4x4 letter;

  mat4x4_identity(letter);
  letter[0][0] = 0.01f;
  letter[1][1] = 0.01f;
  letter[2][2] = 0.01f;
  mat4x4_translate_in_place(letter, -100.0f, 0.0f, 0.08f);
  platform::graphics->draw("C", model_view_projection_matrix);

  mat4x4_identity(letter);
  letter[0][0] = 0.01f;
  letter[1][1] = 0.01f;
  letter[2][2] = 0.01f;
  mat4x4_translate_in_place(letter, -100.0f, 20.0f, 0.08f);
  platform::graphics->draw(my::type_cast<my::string>(input_x), letter);

  mat4x4_identity(letter);
  letter[0][0] = 0.01f;
  letter[1][1] = 0.01f;
  letter[2][2] = 0.01f;
  mat4x4_translate_in_place(letter, -100.0f, 40.0f, 0.08f);
  platform::graphics->draw(my::type_cast<my::string>(input_y), letter);

  mat4x4_identity(letter);
  letter[0][0] = 0.01f;
  letter[1][1] = 0.01f;
  letter[2][2] = 0.01f;
  mat4x4_translate_in_place(letter, -100.0f, 60.0f, 0.08f);
  platform::graphics->draw(my::type_cast<my::string>(input_z), letter);
}

void on_touch_press(float normalized_x, float normalized_y) {
  input_x = normalized_x;
  input_y = normalized_y;

  platform::audio->play(sound);
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
