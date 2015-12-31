#include "mylo.hpp"

static inline float deg_to_radf(float deg) {
  return deg * (float) M_PI / 180.0f;
}

my::shared_ptr<my::object> gbackground;

my::png gtexture;

my::obj gapple;

my::wav sound;

my::fnt font;

my::program shader_with_lighting;
my::program shader_basic;

float input_x;
float input_y;
float input_z;

float mouse_x;
float mouse_y;

my::trace::console tracer;

my::spatial::position camera;

void on_startup(void *asset_manager) { DEBUG_SCOPE;

  DEBUG_TRACE << "graphics init" << my::endl;

  platform::api::graphics->init();

  DEBUG_TRACE << "audio init" << my::endl;

  platform::api::audio->init(4);

  
  DEBUG_TRACE << "asset init" << my::endl;
  
  platform::api::asset->init(asset_manager);

  DEBUG_TRACE << "Attempting to create the draw surface" << my::endl;

  shader_basic.vertex << platform::api::asset->retrieve("shaders/shader_basic.vert");
  shader_basic.fragment << platform::api::asset->retrieve("shaders/shader_basic.frag");
  platform::api::graphics->compile(shader_basic);

  shader_with_lighting.vertex << platform::api::asset->retrieve("shaders/shader_with_lighting.vert");
  shader_with_lighting.fragment << platform::api::asset->retrieve("shaders/shader_with_lighting.frag");
  platform::api::graphics->compile(shader_with_lighting);

  //
  // Just testing out some file system abstractions
  //
  DEBUG_TRACE << "pwd: " << platform::api::filesystem->pwd() << my::endl;
  my::directory dir(platform::api::filesystem->pwd());
  while(my::string current = dir.next()) {
    DEBUG_TRACE << "contents: " << current << my::endl;
  }

  //
  // Load up an apple
  //
  gapple.search_path = "models/obj";
  gapple << platform::api::asset->retrieve("models/obj/apple.obj");

  platform::api::graphics->compile(gapple);

  //
  // Load up the font
  //
  font << platform::api::asset->retrieve("fonts/arial2.fnt");
  platform::api::graphics->compile(font);

  //
  // Load up the background
  //
  gbackground = my::primitive::quad(256, 256);
  my::image *img = new my::png;
  *img << platform::api::asset->retrieve("textures/landscape2.png");
  gbackground->xy_projection(img, 0, 0, 256, 256);
  platform::api::graphics->compile(*gbackground);

  //
  // Default the inputs
  //
  input_x = 320.0f;
  input_y = 240.0f;
  input_z = 1.0f;

  //
  // Init sound
  //
  sound << platform::api::asset->retrieve("sounds/mind_is_going.wav");
  
  DEBUG_TRACE << "sound file loaded " << sound.size << " bytes" << my::endl;
  
  platform::api::audio->compile(sound);

  camera.move(4.0f);
}

void on_shutdown() {
}

int screen_width;
int screen_height;

void on_resize(int width, int height) {
  glViewport(0, 0, width, height);
  
  screen_width = width;
  screen_height = height;
}


void print(float x, float y, my::spatial::matrix mat) {
  my::spatial::matrix placement;
  placement.identity();
  placement.scale(0.003f);

  placement.translate(x, y, 0.0f);

  for (int i = 0; i < 4; i++) {
    my::string text = my::format("%f, %f, %f, %f", mat.r[i][0], mat.r[i][1], mat.r[i][2], mat.r[i][3]);
    platform::api::graphics->draw(text, font, shader_basic, placement, my::spatial::matrix(), my::spatial::matrix());
    placement.translate(0.0f, -30.0f, 0.0f);
  }
}

void print(float x, float y, my::spatial::vector vec) {
  my::spatial::matrix placement;
  placement.identity();
  placement.scale(0.003f);

  placement.translate(x, y, 0.0f);
  my::string text = my::format("%f, %f, %f, %f", vec.x, vec.y, vec.z);
  platform::api::graphics->draw(text, font, shader_basic, placement, my::spatial::matrix(), my::spatial::matrix());
}

void print(float x, float y, my::string text) {
  my::spatial::matrix placement;
  placement.identity();
  placement.scale(0.003f);

  placement.translate(x, y, 0.0f);
  platform::api::graphics->draw(text, font, shader_basic, placement, my::spatial::matrix(), my::spatial::matrix());
}

class text {
public:
  text() {}

  ~text() {
    int i = 0;

  }

  void display() {
    my::spatial::matrix text;
    text.identity();
    text.scale(0.003f);

    text.translate(-300.0f, 300.0f, 0.0f);

    my::list<my::string>::iterator it = buffer.begin();
    for (; it != buffer.end(); it++) {
      text.translate(0.0f, -30.0f, 0.0f);
      if (it->second.length()) {
        platform::api::graphics->draw(it->second, font, shader_basic, text, my::spatial::matrix(), my::spatial::matrix());
      }
    }
  }

  void append(const my::string &rval) {
    if (buffer.size() == 0) {
      newline();
    }
    buffer.tail().append(rval);
  }

  void newline() {
    if (buffer.size() >= 10) {
      buffer.shift();
    }
    buffer.push_back(my::string());
  }

  text &operator << (int rval) {
    append(my::type_cast<my::string>(rval));
    return(*this);
  }

  text &operator << (float rval) {
    append(my::type_cast<my::string>(rval));
    return(*this);
  }

  text &operator << (const my::string &rval) {
    append(rval);
    return(*this);
  }

  text &operator << (const my::end_of_line &rval) {
    newline();
    return(*this);
  }

private:
  my::list<my::string> buffer;
};

text tbuffer;

my::spatial::position pos;


void on_draw() {
  platform::api::graphics->clear();

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

  platform::api::graphics->draw(*gbackground, shader_basic, background, my::spatial::matrix(), my::spatial::matrix());

  //
  // position
  //
  static float rotation = 0.0f;

  rotation += 1.0f;

  my::spatial::matrix view;

  view.lookat(camera.eye, camera.center, camera.up);

  my::spatial::matrix projection;
  projection.perspective(deg_to_radf(90), (float)screen_width / (float)screen_height, -1.0f, 1.0f);

  my::spatial::matrix view_projection = projection * view;

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

  platform::api::graphics->draw(gapple, shader_with_lighting, model, view, projection);

  print(-300, -60, mvp);

  print(-300, -220, my::format("input: %f, %f", input_x, input_y));
  print(-300, -250, my::format("mouse: %f, %f", mouse_x, mouse_y));

  //
  // font 
  //
  //static int counter = 0;
  //tbuffer << counter++ << ") " << input_x << "," << input_y << my::endl;

  tbuffer.display();
}

float move_scale = 0.2f;
float rotate_scale = 1.0f;

void on_proc() {
  static int count = 0;
  //tbuffer << "process loop: " << count++ << my::endl;

  if (platform::api::input->keys[37]) { // left
    camera.rotate(0.0f, rotate_scale * -1.0f);
  }
  if (platform::api::input->keys[38]) { // up
    camera.rotate(rotate_scale, 0.0f, 0.0f);
  }
  if (platform::api::input->keys[39]) { // right
    camera.rotate(0.0f, rotate_scale);
  }
  if (platform::api::input->keys[40]) { // down
    camera.rotate(rotate_scale * -1.0f, 0.0f, 0.0f);
  }

  if (platform::api::input->keys[87]) { // W
    camera.vertical(move_scale * -1.0f);
  }
  if (platform::api::input->keys[83]) { // S
    camera.vertical(move_scale);
  }
  if (platform::api::input->keys[65]) { // A
    camera.strafe(move_scale * -1.0f);
  }
  if (platform::api::input->keys[68]) { // D
    camera.strafe(move_scale);
  }

  if (platform::api::input->keys[32]) { // Space
    shader_with_lighting = my::program();
    shader_with_lighting.vertex << platform::api::asset->retrieve("shaders/shader_with_lighting.vert");
    shader_with_lighting.fragment << platform::api::asset->retrieve("shaders/shader_with_lighting.frag");
    platform::api::graphics->compile(shader_with_lighting);
  }

}

void on_touch_press(float x, float y) {
  input_x = x - (screen_width / 2);
  input_y = y - (screen_height / 2);

  tbuffer << "press: " << input_x << ", " << input_y << my::endl;

  //platform::api::audio->play(sound);
}

void on_touch_drag(float x, float y) {
  input_x = x - (screen_width / 2);
  input_y = y - (screen_height / 2);

  tbuffer << "drag: " << input_x << ", " << input_y << my::endl;

  //camera.rotate(input_x > 0.0f ? 1.0f : -1.0f, input_y > 0.0f ? -1.0f : 1.0f);

  //camera.rotate(input_x / 100,  input_y / 100);
  //camera.rotate(0.1f, 0.1f);
}

void on_touch_release(float x, float y) {
  //input_x = x;
  //input_y = y;
}

void on_touch_zoom_in() {
  camera.move(move_scale);
  tbuffer << "zoom in" << my::endl;
  input_z += 0.1f;
}

void on_touch_zoom_out() {
  camera.move(move_scale * -1.0f);
  tbuffer << "zoom out" << my::endl;
  input_z -= 0.1f;
}

void on_touch_scale(float x, float y, float z) {
  input_z += z;
  //camera.move(input_z >= 0 ? 1.0f : -1.0f);
}

void on_key_down(int key) {
  tbuffer << "key down: " << key << my::endl;

}

void on_key_up(int key) {
  tbuffer << "key up: " << key << my::endl;
  //platform::api::audio->play(sound);

  //camera.rotate(input_x, input_y);
}

void on_mouse_move(long int x, long int y) {
  mouse_x = x;
  mouse_y = y;
}