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
    my::vector<my::string> lines = context->buffer;
    my::vector<my::string>::iterator it = lines.begin();
    for(;it != lines.end(); it++) {
      if(it->second[0] == '#') {
        continue;
      }
      my::vector<my::string> line = my::tokenize(it->second, " ");
      process(context, line[0], line);
    }

    return(0);
  }

  static void process(materials *context, my::string command, my::vector<my::string> arguments) {
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
	  if(command.compare("newmtl") == 0) { //material
      context->active = new my::material;
      context->active->id = arguments[1];
      context->material[arguments[1]] = context->active;
	  }
	  if(command.compare("Ka") == 0) { //ambient
      context->active->ambient[0] = (float)atof(arguments[1].c_str());
      context->active->ambient[1] = (float)atof(arguments[2].c_str());
      context->active->ambient[2] = (float)atof(arguments[3].c_str());
      context->active->ambient[3] = 1.0f;
	  }
	  if(command.compare("Kd") == 0) { //diffuse
      context->active->diffuse[0] = (float)atof(arguments[1].c_str());
      context->active->diffuse[1] = (float)atof(arguments[2].c_str());
      context->active->diffuse[2] = (float)atof(arguments[3].c_str());
      context->active->diffuse[3] = 1.0f;
	  }
	  if(command.compare("Ka") == 0) { //specular
      context->active->specular[0] = (float)atof(arguments[1].c_str());
      context->active->specular[1] = (float)atof(arguments[2].c_str());
      context->active->specular[2] = (float)atof(arguments[3].c_str());
      context->active->specular[3] = 1.0f;
	  }
	  if(command.compare("Ke") == 0) { //emission
      context->active->emission[0] = (float)atof(arguments[1].c_str());
      context->active->emission[1] = (float)atof(arguments[2].c_str());
      context->active->emission[2] = (float)atof(arguments[3].c_str());
      context->active->emission[3] = 1.0f;
	  }
	  if(command.compare("Ns") == 0) { //shininess
      context->active->shininess = (float)atof(arguments[1].c_str());
	  }
	  if(command.compare("d") == 0) { //opacity
      context->active->opacity = (float)atof(arguments[1].c_str());
	  }
	  if(command.compare("illum") == 0) { //illumination
      context->active->illumination = (float)atof(arguments[1].c_str());
	  }
	  if(command.compare("map_Kd") == 0) { //texture
      my::string extension = arguments[1].substr(arguments[1].rfind("."), arguments[1].length());
      if(extension.compare(".tga") == 0) {
        context->active->map = new tga();
        *context->active->map << my::asset(my::join("/", (context->search_path, arguments[1])));
      }
      if (extension.compare(".png") == 0) {
        context->active->map = new png();
        *context->active->map << my::asset(my::join("/", (context->search_path, arguments[1])));
      }
    }
  }
};

__MYLO_NAMESPACE_END

#endif
