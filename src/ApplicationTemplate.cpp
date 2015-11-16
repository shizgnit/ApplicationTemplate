#include "mylo.hpp"

static inline float deg_to_radf(float deg) {
  return deg * (float) M_PI / 180.0f;
}

my::shared_ptr<my::object> gbackground;

my::png gtexture;

my::obj gapple;

my::wav sound;

float input_x;
float input_y;
float input_z;

my::trace::console tracer;

void on_startup(void *asset_manager) { DEBUG_SCOPE;

  DEBUG_TRACE << "graphics init" << my::endl;

  platform::api::graphics->init();

  DEBUG_TRACE << "audio init" << my::endl;

  platform::api::audio->init();

  
  DEBUG_TRACE << "asset init" << my::endl;
  
  platform::api::asset->init(asset_manager);

  DEBUG_TRACE << "Attempting to create the draw surface" << my::endl;

  platform::api::graphics->set_program("shaders/texture_alpha_shader.vert", "shaders/texture_alpha_shader.frag");

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
  platform::api::graphics->set_font("fonts/arial2.fnt");

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


void pprint(my::spatial::matrix mat) {
  printf("----------------------------\n");
  for (int i = 0; i < 4; i++) {
    printf("%f,%f,%f,%f\n", mat.r[i][0], mat.r[i][1], mat.r[i][2], mat.r[i][3]);
  }
}

void pprint(my::spatial::vector vec) {
  printf("%f,\t%f,\t%f,\t%f\n", vec.x, vec.y, vec.z, vec.w);
}

my::spatial::position camera;


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
        platform::api::graphics->draw(it->second, text);
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

  platform::api::graphics->draw(*gbackground, background);

  //
  // position
  //
  my::spatial::vector eye( ((input_x - (screen_width/2)) / 10) * -1, (input_y - (screen_height/2)) / 10, 1.0f + input_z );
  my::spatial::vector center( ((input_x - (screen_width / 2)) / 10) * -1, (input_y - (screen_height / 2)) / 10, 0.0f + input_z);
  my::spatial::vector up(0.0f, 1.0f, 0.0f);

  static float rotation = 0.0f;

  rotation += 0.05f;

  my::spatial::matrix view;

  //view.lookat(eye, center, up);
  view.lookat(camera.eye, camera.center, camera.up);

  my::spatial::matrix projection;
  projection.perspective(deg_to_radf(90), (float)screen_width / (float)screen_height, -1.0f, 1.0f);

  my::spatial::matrix view_projection = projection * view;

  my::spatial::matrix model;
  model.identity();
  model.rotate_y(rotation);

  my::spatial::matrix mvp = view_projection * model;

  platform::api::graphics->draw(gapple, mvp);

  //
  // font 
  //
  static int counter = 0;
  tbuffer << counter++ << ") " << input_x << "," << input_y << my::endl;

  tbuffer.display();
}

void on_touch_press(float normalized_x, float normalized_y) {
  input_x = normalized_x - (screen_width / 2);
  input_y = normalized_y - (screen_height / 2);

  platform::api::audio->play(sound);
}

void on_touch_drag(float normalized_x, float normalized_y) {
  input_x = normalized_x - (screen_width / 2);
  input_y = normalized_y - (screen_height / 2);

  camera.rotate(normalized_x, normalized_y);
}

void on_touch_release(float normalized_x, float normalized_y) {
  //input_x = normalized_x;
  //input_y = normalized_y;
}

void on_touch_zoom_in() {
  camera.move(0.01f);
  input_z += 0.1f;
}

void on_touch_zoom_out() {
  camera.move(-0.01f);
  input_z -= 0.1f;
}

void on_touch_scale(float normalized_x, float normalized_y, float normalized_z) {
  input_z += normalized_z;
}