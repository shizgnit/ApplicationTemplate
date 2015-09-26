#include "mylo.hpp"

#ifndef __MTL_HPP
#define __MTL_HPP

__MYLO_NAMESPACE_BEGIN

class __MYLO_DLL_EXPORT mtl: public materials {
public:
  mtl() {
    callback_write("MTL", write);
  }

  static off_t write(materials *context, unsigned char *buffer, off_t bytes) {
    if(bytes) {
      context->buffer.write(buffer, bytes);
      return(bytes);
    }

    my::vector<char *> lines = my::segment((char *)context->buffer.raw(), '\n');
    for (int i = 0; i < lines.size(); i++) {
      if (lines[i][0] == '#') {
        continue;
      }
      my::vector<char *> line = my::segment(lines[i], ' ');
      if (line[0] == 0) {
        continue;
      }
      process(context, line[0], line);
    }

    return(0);
  }

  static void process(materials *context, char *command, my::vector<char *> arguments) {
    /*
    newmtl meat
    Ns 100.000
    d 1.00000
    illum 2
    Kd 0.567100 0.567100 0.567100
    Ka 0.766900 0.766900 0.766900
    Ks 1.00000 1.00000 1.00000
    Ke 1.00000e-3 0.00000e+0 0.00000e+0
    map_Kd meat.tga
    */
    
    if (strcmp(command, "newmtl") == 0) { //material
      context->active = new my::material;
      context->active->id = arguments[1];
      context->material[arguments[1]] = context->active;
    }
    if (strcmp(command, "Ka") == 0) { //ambient
      context->active->ambient[0] = (float)atof(arguments[1]);
      context->active->ambient[1] = (float)atof(arguments[2]);
      context->active->ambient[2] = (float)atof(arguments[3]);
      context->active->ambient[3] = 1.0f;
    }
    if (strcmp(command, "Kd") == 0) { //diffuse
      context->active->diffuse[0] = (float)atof(arguments[1]);
      context->active->diffuse[1] = (float)atof(arguments[2]);
      context->active->diffuse[2] = (float)atof(arguments[3]);
      context->active->diffuse[3] = 1.0f;
    }
    if (strcmp(command, "Ka") == 0) { //specular
      context->active->specular[0] = (float)atof(arguments[1]);
      context->active->specular[1] = (float)atof(arguments[2]);
      context->active->specular[2] = (float)atof(arguments[3]);
      context->active->specular[3] = 1.0f;
    }
    if (strcmp(command, "Ke") == 0) { //emission
      context->active->emission[0] = (float)atof(arguments[1]);
      context->active->emission[1] = (float)atof(arguments[2]);
      context->active->emission[2] = (float)atof(arguments[3]);
      context->active->emission[3] = 1.0f;
    }
    if (strcmp(command, "Ns") == 0) { //shininess
      context->active->shininess = (float)atof(arguments[1]);
    }
    if (strcmp(command, "d") == 0) { //opacity
      context->active->opacity = (float)atof(arguments[1]);
    }
    if (strcmp(command, "illum") == 0) { //illumination
      context->active->illumination = (float)atof(arguments[1]);
    }
    if (strcmp(command, "map_Kd") == 0) { //texture
      if (strcmp(arguments[1] + strlen(arguments[1]) - 4, ".tga") == 0) {
        context->active->map = new tga();
      }
      if (strcmp(arguments[1] + strlen(arguments[1]) - 4, ".png") == 0) {
        context->active->map = new png();
      }
      *context->active->map << my::asset(my::join("/", (context->search_path, arguments[1])));
    }
  }

};

__MYLO_NAMESPACE_END

#endif
