#include "mylo.hpp"

#ifndef __MTL_HPP
#define __MTL_HPP

__MYLO_NAMESPACE_BEGIN

class __MYLO_DLL_EXPORT mtl: public material {
public:
  mtl() {
    callback_write("MTL", write);
  }

  static off_t write(material *context, unsigned char *buffer, off_t bytes) {
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

  static void process(material *context, my::string command, my::vector<my::string> arguments) {
    //static material *mat;
    material *mat = context;
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
      //mat = new material;
      mat->id = arguments[1];
      //context->layers[arguments[1]] = mat;
	  }
	  if(command.compare("Ka") == 0) { //ambient
      mat->ambient[0] = (float)atof(arguments[1].c_str());
      mat->ambient[1] = (float)atof(arguments[2].c_str());
      mat->ambient[2] = (float)atof(arguments[3].c_str());
      mat->ambient[3] = 1.0f;
	  }
	  if(command.compare("Kd") == 0) { //diffuse
      mat->diffuse[0] = (float)atof(arguments[1].c_str());
      mat->diffuse[1] = (float)atof(arguments[2].c_str());
      mat->diffuse[2] = (float)atof(arguments[3].c_str());
      mat->diffuse[3] = 1.0f;
	  }
	  if(command.compare("Ka") == 0) { //specular
      mat->specular[0] = (float)atof(arguments[1].c_str());
      mat->specular[1] = (float)atof(arguments[2].c_str());
      mat->specular[2] = (float)atof(arguments[3].c_str());
      mat->specular[3] = 1.0f;
	  }
	  if(command.compare("Ke") == 0) { //emission
      mat->emission[0] = (float)atof(arguments[1].c_str());
      mat->emission[1] = (float)atof(arguments[2].c_str());
      mat->emission[2] = (float)atof(arguments[3].c_str());
      mat->emission[3] = 1.0f;
	  }
	  if(command.compare("Ns") == 0) { //shininess
      mat->shininess = (float)atof(arguments[1].c_str());
	  }
	  if(command.compare("d") == 0) { //opacity
      mat->opacity = (float)atof(arguments[1].c_str());
	  }
	  if(command.compare("illum") == 0) { //illumination
      mat->illumination = (float)atof(arguments[1].c_str());
	  }
	  if(command.compare("map_Kd") == 0) { //texture
      my::string extension = arguments[0].substr(arguments[1].rfind("."), arguments[1].length());
      if(extension.compare(".tga") == 0) {
        mat->map = new tga();
        *mat->map<<file(arguments[1]);
      }
    }
  }
};

__MYLO_NAMESPACE_END

#endif
