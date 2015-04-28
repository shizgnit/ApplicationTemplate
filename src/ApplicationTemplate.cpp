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

typedef struct {
  GLuint program;

  GLint a_position_location;
  GLint a_texture_coordinates_location;
  GLint u_mvp_matrix_location;
  GLint u_texture_unit_location;
} TextureProgram;

typedef struct {
  GLuint program;

  GLint a_position_location;
  GLint u_mvp_matrix_location;
  GLint u_color_location;
} ColorProgram;

static inline float deg_to_radf(float deg) {
  return deg * (float) M_PI / 180.0f;
}

TextureProgram get_texture_program(GLuint program)
{
  TextureProgram temp = {
    program,
    glGetAttribLocation(program, "a_Position"),
    glGetAttribLocation(program, "a_TextureCoordinates"),
    glGetUniformLocation(program, "u_MvpMatrix"),
    glGetUniformLocation(program, "u_TextureUnit")
  };
  return(temp);
}

ColorProgram get_color_program(GLuint program)
{
  ColorProgram color =  {
    program,
    glGetAttribLocation(program, "a_Position"),
    glGetUniformLocation(program, "u_MvpMatrix"),
    glGetUniformLocation(program, "u_Color")
  };
  return(color);
}


#ifndef _WIN32
#define  LOG_TAG    "ApplicationTemplate"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#else
#define  LOGI(...)  printDebug(__VA_ARGS__)
#define  LOGE(...)  printDebug(__VA_ARGS__)
#endif

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
#ifndef _WIN32
//    LOGI("GL %s = %s\n", name, v);
#endif
}

static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
#ifndef _WIN32
//        LOGI("after %s() glError (0x%x)\n", op, error);
#endif
    }
}

#if defined _WIN32
static void printDebug(const char* szFormat, ...)
{
  char szBuff[1024];
  va_list arg;
  va_start(arg, szFormat);
  _vsnprintf(szBuff, sizeof(szBuff), szFormat, arg);
  va_end(arg);
  OutputDebugString(my::wstring(szBuff));
}
#endif

static const char gVertexShader[] =
  "attribute vec4 vPosition;\n"
  "void main() {\n"
  "  gl_Position = vPosition;\n"
  "}\n";

static const char gFragmentShader[] =
  "precision mediump float;\n"
  "void main() {\n"
  "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
  "}\n";

static const char gTextureVertexShader[] =
  "uniform mat4 u_MvpMatrix;\n"
  "attribute vec4 a_Position;\n"
  "attribute vec4 a_ActualPosition;\n"
  "varying vec4 v_ActualPosition;\n"
  "attribute vec2 a_TextureCoordinates;\n"
  "varying vec2 v_TextureCoordinates;\n"
  "void main()\n"
  "{\n"
  "  v_ActualPosition = u_MvpMatrix * a_ActualPosition;\n"
  "  v_TextureCoordinates = a_TextureCoordinates;\n"
  "  gl_Position = u_MvpMatrix * a_Position;\n"
  "}\n";

static const char gTextureFragmentShader[] =
  "precision mediump float;\n"
  "uniform sampler2D u_TextureUnit;\n"
  "uniform sampler2D u_FakeDepthUnit;\n"
  "varying vec4 v_ActualPosition;\n"
  "varying vec2 v_TextureCoordinates;\n"
  "void main()\n"
  "{\n"
  "  gl_FragColor = gl_FragColor + texture2D(u_TextureUnit, v_TextureCoordinates) * vec4(texture2D(u_TextureUnit, v_TextureCoordinates).a);\n"
  "}\n";

GLuint loadShader(GLenum shaderType, const char* pSource) {
  GLuint shader = glCreateShader(shaderType);
  if (shader) {
    LOGI("Shader loaded\n");
    glShaderSource(shader, 1, &pSource, NULL);
    glCompileShader(shader);
    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
      GLint infoLen = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
      if (infoLen) {
        char* buf = (char*) malloc(infoLen);
        if (buf) {
          glGetShaderInfoLog(shader, infoLen, NULL, buf);
          LOGE("Could not compile shader %d:\n%s\n", shaderType, buf);
          free(buf);
        }
        glDeleteShader(shader);
        shader = 0;
      }
    }
  }
  else {
    LOGI("Shader failed to load\n");
  }
  return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
  LOGI("vertex shader\n");
  GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
  if (!vertexShader) {
    return 0;
  }

  LOGI("pixel shader\n");
  GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
  if (!pixelShader) {
    return 0;
  }

  GLuint program = glCreateProgram();
  if (program) {
    glAttachShader(program, vertexShader);
    checkGlError("glAttachShader");
    glAttachShader(program, pixelShader);
    checkGlError("glAttachShader");
    glLinkProgram(program);
    GLint linkStatus = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
      GLint bufLength = 0;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
      if (bufLength) {
        char* buf = (char*) malloc(bufLength);
        if (buf) {
          glGetProgramInfoLog(program, bufLength, NULL, buf);
          LOGE("Could not link program:\n%s\n", buf);
          free(buf);
        }
      }
      glDeleteProgram(program);
      program = 0;
    }
  }
  return program;
}

GLuint gProgram;
GLuint gvPositionHandle;

GLuint gTextureProgram;
GLuint gvTexturePositionHandle;

GLint a_position_location;
GLint a_texture_coordinates_location;
GLint u_mvp_matrix_location;
GLint u_texture_unit_location;

#define BUFFER_OFFSET(i) ((void*)(i))

static mat4x4 projection_matrix;
static mat4x4 model_matrix;
static mat4x4 view_matrix;

static mat4x4 view_projection_matrix;
static mat4x4 model_view_projection_matrix;
static mat4x4 inverted_view_projection_matrix;

static mat4x4 background_projection_matrix;

my::fnt font;


void compile(my::shared_ptr<my::object> obj) {
  glGenBuffers(1, &obj->context);
  glBindBuffer(GL_ARRAY_BUFFER, obj->context);
  glBufferData(GL_ARRAY_BUFFER, sizeof(my::object::vertex) * obj->vertices.size(), obj->vertices.pointer, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenTextures(1, &obj->texture->context);
  glBindTexture(GL_TEXTURE_2D, obj->texture->context);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, obj->texture->map->header.width, obj->texture->map->header.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, obj->texture->map->raster);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}


void draw(my::shared_ptr<my::object> obj, mat4x4 matrix) {
  glUseProgram(gTextureProgram);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, obj->texture->context);

  glUniformMatrix4fv(u_mvp_matrix_location, 1, GL_FALSE, (GLfloat*)matrix);
  glUniform1i(u_texture_unit_location, 0);

  glBindBuffer(GL_ARRAY_BUFFER, obj->context);
  glVertexAttribPointer(a_position_location, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
  glVertexAttribPointer(a_texture_coordinates_location, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT), BUFFER_OFFSET(4 * sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(a_position_location);
  glEnableVertexAttribArray(a_texture_coordinates_location);

  glDrawArrays(GL_TRIANGLE_FAN, 0, obj->vertices.size());

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

my::shared_ptr<my::object> gbackground;

my::png gtexture;

void on_startup(void *asset_manager) {
  LOGI("Attempting to create the draw surface\n");

#ifdef _WIN32
  AllocConsole();

  HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
  int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
  FILE* hf_out = _fdopen(hCrt, "w");
  setvbuf(hf_out, NULL, _IONBF, 1);
  *stdout = *hf_out;

  HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
  hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
  FILE* hf_in = _fdopen(hCrt, "r");
  setvbuf(hf_in, NULL, _IONBF, 128);
  *stdin = *hf_in;
#endif

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  gProgram = createProgram(gVertexShader, gFragmentShader);
	if (!gProgram) {
    LOGE("Could not link program\n");
		return;
	}
	gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
	checkGlError("glGetAttribLocation");

  LOGI("Attempting to create the draw surface\n");
  gTextureProgram = createProgram(gTextureVertexShader, gTextureFragmentShader);
  if (!gProgram) {
    LOGE("Could not link program\n");
    return;
  }
  a_position_location = glGetAttribLocation(gTextureProgram, "a_Position");
  a_texture_coordinates_location = glGetAttribLocation(gTextureProgram, "a_TextureCoordinates");
  u_mvp_matrix_location = glGetUniformLocation(gTextureProgram, "u_MvpMatrix");
  u_texture_unit_location = glGetUniformLocation(gTextureProgram, "u_TextureUnit");

  my::asset::manager(asset_manager); 

  LOGI("pwd: %s\n", my::pwd().c_str());

  my::directory dir(my::pwd());
  while(my::string current = dir.next()) {
	  LOGI("contents: %s\n", current.c_str());
  }

  font << my::asset("fonts/arial.fnt");

  int x = font.glyphs.size();
  for (int i = font.glyphs.offset(); i < font.glyphs.size(); i++) {
    if (font.glyphs[i]->identifier) {
      compile(font.glyphs[i]->quad);
    }
  }

  gbackground = my::primitive::quad(256, 256);

  my::image *img = new my::png;
  *img << my::asset("textures/landscape2.png");

  gbackground->xy_projection(img, 0, 0, 256, 256);

  compile(gbackground);
}

void on_resize(int width, int height) {
  LOGI("Draw surface changed\n");
  glViewport(0, 0, width, height);
  checkGlError("glViewport");

  mat4x4_perspective(projection_matrix, deg_to_radf(90), (float)width / (float)height, 0.0f, 20.0f);

  float eye[3] = { 10.0f, 4.0f, 1.0f };
  float center[3] = { 10.0f, 4.0f, 0.0f };
  float up[3] = { 0.0f, 1.0f, 0.0f };

  mat4x4_look_at(view_matrix, eye, center, up);
}


/*

int current_x = this.x;
int current_y = this.y;

switch(this.hAlign) {
  case LEFT:   current_x = this.x;                   break;
  case RIGHT:  current_x = this.x - this.width;      break;
  case CENTER: current_x = this.x - (this.width/2);  break;
}

switch(this.vAlign) {
  case TOP:    current_y = this.y;                   break;
  case BOTTOM: current_y = this.y - this.height;     break;
  case CENTER: current_y = this.y - (this.height/2); break;
}

for(int i=0; i<length; i++) {
  this.font.glyphs.get(characters[i]).quad.setScale(this.scale[0], this.scale[1], this.scale[2]);

  if(prior != 0 && this.kerning) {
    FontKerning kerning = this.font.getKerning(prior, characters[i]);
    if(kerning != null) {
      current_x += (kerning.getAmount() * this.scale[0]);
    }
  }

  this.font.glyphs.get(characters[i]).draw(current_x + (int)(this.font.glyphs.get(characters[i]).xOffset * this.scale[0]), current_y + (int)(this.font.glyphs.get(characters[i]).yOffset * this.scale[1]), this.depth);

  current_x += (this.font.glyphs.get(characters[i]).xAdvance * this.scale[0]);

  prior = characters[i];
}

*/

static mat4x4 identity;

void pprint(mat4x4 mat) {
  printf("----------------------------\n");
  for (int i = 0; i < 4; i++) {
    printf( "%f,%f,%f,%f\n", mat[i][0], mat[i][1], mat[i][2], mat[i][3] );
  }
}


void on_draw() {
  static float grey = 0.0f;
  grey += 0.01f;
  if (grey > 1.0f) {
    grey = 0.0f;
  }

  glClearColor(grey, grey, grey, 1.0f);
  checkGlError("glClearColor");
  glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  checkGlError("glClear");

  static float angle = 0.0f;
  angle -= 0.1f;
  if (angle <= -90.0f) {
    angle = 0.0f;
  }

  mat4x4_identity(background_projection_matrix);

  mat4x4_identity(identity);


  mat4x4 scale;

  mat4x4_identity(scale);

  mat4x4_scale( scale , identity, 0.01f);
  scale[3][3] = 1.0f;
  mat4x4_translate_in_place( scale, -100.0f, -100.0f, -1.0f );

  pprint(scale);

  //int x = getchar();

  //
  // render
  //
  draw(gbackground, scale);

  //
  // position
  //
  mat4x4_mul(view_projection_matrix, projection_matrix, view_matrix);
  mat4x4_invert(inverted_view_projection_matrix, view_projection_matrix);

  mat4x4 rotated_model_matrix;
  mat4x4_identity(model_matrix);

  mat4x4_rotate_X(rotated_model_matrix, model_matrix, deg_to_radf(angle));
  mat4x4_translate_in_place(rotated_model_matrix, 0.0f, 0.0f, 1.0f);

  mat4x4_mul(model_view_projection_matrix, view_projection_matrix, rotated_model_matrix);


  //
  // font 
  //
  mat4x4 letter;
  mat4x4_identity(letter);

  letter[0][0] = 0.01f;
  letter[1][1] = 0.01f;
  letter[2][2] = 0.01f;

  mat4x4_translate_in_place(letter, 0.0f, 0.0f, -2.0f);

  //model_view_projection_matrix
  draw(font.glyphs[66]->quad, letter);

}

void on_touch_press(float normalized_x, float normalized_y) {

}

void on_touch_drag(float normalized_x, float normalized_y) {

}

