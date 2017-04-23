#include "ApplicationTemplate.hpp"

static inline float deg_to_radf(float deg) {
  return deg * (float) M_PI / 180.0f;
}

my::shared_ptr<my::object> gbackground;

my::png gtexture;

my::obj gapple;
my::obj bunny;

my::wav sound;

view::text tbuffer;

my::program shader_with_lighting;
my::program shader_basic;

float input_x;
float input_y;
float input_z;

float mouse_x;
float mouse_y;

my::trace::console tracer;

my::spatial::position camera;

int apples = 60;
my::list<my::shared_ptr<my::spatial::position> > positions;

void main::on_startup(void *asset_manager) { DEBUG_SCOPE;

  DEBUG_TRACE << "graphics init" << my::endl;

  platform::graphics::api->init();

  DEBUG_TRACE << "audio init" << my::endl;

  platform::audio::api->init(4);

  DEBUG_TRACE << "asset init" << my::endl;

  platform::asset::api->init(asset_manager);

  DEBUG_TRACE << "Attempting to create the draw surface" << my::endl;

  shader_basic.vertex << platform::asset::api->retrieve("shaders/texture_shader.vert");
  shader_basic.fragment << platform::asset::api->retrieve("shaders/texture_shader.frag");
  platform::graphics::api->compile(shader_basic);

  shader_with_lighting.vertex << platform::asset::api->retrieve("shaders/texture_alpha_shader_toon_lighting.vert");
  shader_with_lighting.fragment << platform::asset::api->retrieve("shaders/texture_alpha_shader_toon_lighting.frag");
  platform::graphics::api->compile(shader_with_lighting);

  //
  // Just testing out some file system abstractions
  //
  DEBUG_TRACE << "pwd: " << platform::filesystem::api->pwd() << my::endl;
  my::directory dir(platform::filesystem::api->pwd());
  while(my::string current = dir.next()) {
    DEBUG_TRACE << "contents: " << current << my::endl;
  }

  //
  // Load up an apple
  //
  gapple.search_path = "models/obj";
  gapple << platform::asset::api->retrieve("models/obj/apple.obj");

  platform::graphics::api->compile(gapple);

  bunny.search_path = "models/bunny";
  bunny << platform::asset::api->retrieve("models/bunny/poly.obj");

  platform::graphics::api->compile(bunny);

  //
  // Load up the font
  //
  tbuffer.font("fonts/arial2.fnt");

  //
  // Load up the background
  //
  gbackground = my::primitive::quad(256, 256);
  my::image *img = new my::png;
  *img << platform::asset::api->retrieve("textures/landscape2.png");
  gbackground->xy_projection(img, 0, 0, 256, 256);
  //*img << platform::asset::api->retrieve("models/bunny/lowpoly.armature..animation.texture.unwrapped.textured.png");
  //gbackground->xy_projection(img, 0, 0, 2048, 2048);
  platform::graphics::api->compile(*gbackground);

  //
  // Default the inputs
  //
  input_x = 320.0f;
  input_y = 240.0f;
  input_z = 1.0f;

  //
  // Init sound
  //
  sound << platform::asset::api->retrieve("sounds/mind_is_going.wav");

  DEBUG_TRACE << "sound file loaded " << sound.size << " bytes" << my::endl;

  platform::audio::api->compile(sound);

  camera.move(4.0f);

  //
  // Randomize the object positions
  //
  /*
  for (int i = 0; i < apples; i++) {
    my::spatial::position *instance = new my::spatial::position();

    my::spatial::vector v;

    float random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

    v.x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 6.0f)) - 3.0f;
    v.y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 6.0f)) - 3.0f;
    v.z = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 6.0f)) - 3.0f;

    instance->center += v;
    instance->eye += v;

    //instance->rotation.y = (float)(rand() % 20);

    positions.push_back(instance);
  }
  */
}

void main::on_shutdown() {
}

int screen_width;
int screen_height;

void main::on_resize(int width, int height) {
  glViewport(0, 0, width, height);

  screen_width = width;
  screen_height = height;
}


my::spatial::position pos;

void main::on_draw() {
  platform::graphics::api->clear();

  static float angle = 0.0f;
  angle -= 0.1f;
  if (angle <= -90.0f) {
    angle = 0.0f;
  }

  //
  // render background
  //
  my::spatial::matrix background;
  background.identity();
  background.scale(0.01f);
  background.translate(-130.0f, -120.0f, 1000.1f);

  //platform::graphics::api->draw(*gbackground, shader_basic, background, my::spatial::matrix(), my::spatial::matrix());

  //
  // position
  //
  static float rotation = 0.0f;

  rotation += 1.0f;

  my::spatial::matrix view;

  view.lookat(camera.eye, camera.center, camera.up);

  my::spatial::matrix perspective;
  perspective.perspective(deg_to_radf(90), (float)screen_width / (float)screen_height, -1.0f, 1.0f);

  my::spatial::matrix ortho;
  ortho.ortho(0, screen_width, 0, screen_height);

  my::spatial::matrix view_projection = perspective * view;

  my::spatial::matrix b;
  b.identity();
  b.translate(10, 10, 0);

  platform::graphics::api->draw(*gbackground, shader_basic, b, my::spatial::matrix(), ortho);

  /*
  my::spatial::matrix model;
  model.identity();

  // position
  pos.rotate(1.0f, 0.0f);
  pos.move(0.02f);
  model.translate(pos.eye, pos.center, pos.up);

  // direct
  //model.translate()
  //model.rotate_x(rotation);

  //quaternion
  //my::spatial::quaternion quat;
  //my::spatial::vector vec;
  //quat.euler(rotation, 0.0f, 0.0f, 0.0f);
  //vec.x = rotation;
  //quat += vec;
  //quat.euler(1.0f, 0.0f, 0.0f, rotation);
  //model = quat;

  my::spatial::matrix mvp = view_projection * model;

  platform::graphics::api->draw(gapple, shader_with_lighting, model, view, projection);
  */

  my::spatial::matrix model;
  model.identity();

  // position
  pos.rotate(0.0f, 1.0f);
  pos.move(0.02f);
  model.translate(pos.eye, pos.center, pos.up);

  platform::graphics::api->draw(bunny, shader_with_lighting, model, view, perspective);

  /*
  my::list<my::shared_ptr<my::spatial::position> >::iterator it = positions.begin();
  for (float i=0; it != positions.end(); it++) {
    my::spatial::matrix model;
    model.identity();

    it->second->rotate(1.0f, 0.0f); //  i++ / apples);

    //float rotation_y = it->second->rotation.y;

    it->second->move(0.02f);
    model.translate(it->second->eye, it->second->center, it->second->up);

    platform::graphics::api->draw(gapple, shader_with_lighting, model, view, perspective);
  }
  */

  //print(-300, -60, mvp);

  tbuffer.print(-300, -220, my::format("input: %f, %f", input_x, input_y));
  tbuffer.print(-300, -250, my::format("mouse: %f, %f", mouse_x, mouse_y));

  static int frame = 0;
  static int start = time(NULL);

  frame++;

  tbuffer.print(-300, -280, my::format("fps: %f", (float) frame / (time(NULL) - start) ));

  //
  // font
  //
  //static int counter = 0;
  //tbuffer << counter++ << ") " << input_x << "," << input_y << my::endl;

  tbuffer.draw();
}

float move_scale = 0.2f;
float rotate_scale = 1.0f;

void main::on_proc() {
  static int count = 0;
  //tbuffer << "process loop: " << count++ << my::endl;

  if (platform::input::api->keys[37]) { // left
    camera.rotate(0.0f, rotate_scale * -1.0f);
  }
  if (platform::input::api->keys[38]) { // up
    camera.rotate(rotate_scale, 0.0f, 0.0f);
  }
  if (platform::input::api->keys[39]) { // right
    camera.rotate(0.0f, rotate_scale);
  }
  if (platform::input::api->keys[40]) { // down
    camera.rotate(rotate_scale * -1.0f, 0.0f, 0.0f);
  }

  if (platform::input::api->keys[87]) { // W
    camera.vertical(move_scale * -1.0f);
  }
  if (platform::input::api->keys[83]) { // S
    camera.vertical(move_scale);
  }
  if (platform::input::api->keys[65]) { // A
    camera.strafe(move_scale * -1.0f);
  }
  if (platform::input::api->keys[68]) { // D
    camera.strafe(move_scale);
  }

  if (platform::input::api->keys[32]) { // Space
    static int which = 0;

    switch (which) {
    case(0):
      shader_with_lighting = my::program();
      shader_with_lighting.vertex << platform::asset::api->retrieve("shaders/shader_with_lighting.vert");
      shader_with_lighting.fragment << platform::asset::api->retrieve("shaders/shader_with_lighting.frag");
      platform::graphics::api->compile(shader_with_lighting);
      which += 1;
      break;
    case(1):
      shader_with_lighting = my::program();
      shader_with_lighting.vertex << platform::asset::api->retrieve("shaders/shader_basic.vert");
      shader_with_lighting.fragment << platform::asset::api->retrieve("shaders/shader_basic.frag");
      platform::graphics::api->compile(shader_with_lighting);
      which += 1;
      break;
    case(2):
      shader_with_lighting.vertex << platform::asset::api->retrieve("shaders/texture_alpha_shader_toon_lighting.vert");
      shader_with_lighting.fragment << platform::asset::api->retrieve("shaders/texture_alpha_shader_toon_lighting.frag");
      platform::graphics::api->compile(shader_with_lighting);
      which = 0;
      break;
    };
  }

}

void main::on_touch_press(float x, float y) {
  input_x = x - (screen_width / 2);
  input_y = y - (screen_height / 2);

  tbuffer << "press: " << input_x << ", " << input_y << my::endl;

  //platform::audio::api->play(sound);
}

void main::on_touch_drag(float x, float y) {
  input_x = x - (screen_width / 2);
  input_y = y - (screen_height / 2);

  tbuffer << "drag: " << input_x << ", " << input_y << my::endl;

  //camera.rotate(input_x > 0.0f ? 1.0f : -1.0f, input_y > 0.0f ? -1.0f : 1.0f);

  //camera.rotate(input_x / 100,  input_y / 100);
  //camera.rotate(0.1f, 0.1f);
}

void main::on_touch_release(float x, float y) {
  //input_x = x;
  //input_y = y;
  tbuffer << "release: " << input_x << ", " << input_y << my::endl;
}

void main::on_touch_zoom_in() {
  camera.move(move_scale);
  tbuffer << "zoom in" << my::endl;
  input_z += 0.1f;
}

void main::on_touch_zoom_out() {
  camera.move(move_scale * -1.0f);
  tbuffer << "zoom out" << my::endl;
  input_z -= 0.1f;
}

void main::on_touch_scale(float x, float y, float z) {
  input_z += z;
  //camera.move(input_z >= 0 ? 1.0f : -1.0f);
}

void main::on_key_down(int key) {
  tbuffer << "key down: " << key << my::endl;
}

void main::on_key_up(int key) {
  tbuffer << "key up: " << key << my::endl;
  //platform::audio::api->play(sound);
  //camera.rotate(input_x, input_y);
}

void main::on_mouse_down(int button, long int x, long int y) {

}

void main::on_mouse_up(int button, long int x, long int y) {

}

void main::on_mouse_move(long int x, long int y) {
  mouse_x = x;
  mouse_y = y;
}
