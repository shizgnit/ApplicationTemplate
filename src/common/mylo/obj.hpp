#include "mylo.hpp"

#ifndef __OBJ_HPP
#define __OBJ_HPP

__MYLO_NAMESPACE_BEGIN

class __MYLO_DLL_EXPORT obj: public object {
public:
  obj() {
    callback_write("OBJ", write);
  }

  static off_t write(object *context, unsigned char *buffer, off_t bytes) {
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

  static void process(object *context, my::string command, my::vector<my::string> arguments) {
    object *model = context;
    material *mat;

    my::vector<float> vertices;
    my::vector<float> normals;
    my::vector<float> textures;


    /*
	  mtllib apple.mtl
	  o sphere5
	  #20 vertices, 24 faces
	  v 0.77081294 1.37713812 0.18578580
	  vt 0.0000000e+0 0.83301356
	  vn 0.56772715 -0.18355389 0.80249228
	  g sphere5_leaf
	  usemtl leaf
	  f 1/3/1 7/13/7 12/16/12 6/10/6
	  ...
	  o cylinder3
	  #176 vertices, 204 faces
	  v -2.3757008e-2 1.51884413 -3.6220710e-18
	  ...
	  */

	  if(command.compare("mtllib") == 0) { //material
      mat = new material;
      *mat << file(arguments[1]);
    }
	  if(command.compare("o") == 0) { //entity
      vertices.clear();
      textures.clear();
      normals.clear();
	  }
	  if(command.compare("v") == 0) { //vertex
      vertices.push_back((float)atof(arguments[1].c_str()));
      vertices.push_back((float)atof(arguments[2].c_str()));
      vertices.push_back((float)atof(arguments[3].c_str()));
	  }
	  if(command.compare("vt") == 0) { //texture coordinates
      textures.push_back((float)atof(arguments[1].c_str()));
      textures.push_back((float)atof(arguments[2].c_str()));
	  }
	  if(command.compare("vn") == 0) { //normals
      normals.push_back((float)atof(arguments[1].c_str()));
      normals.push_back((float)atof(arguments[2].c_str()));
      normals.push_back((float)atof(arguments[3].c_str()));
	  }
	  if(command.compare("g") == 0) { //group
	  }
	  if(command.compare("usemtl") == 0) { //material
      //my::map<my::string, shared_ptr<material::layer> >::iterator it = mat->layers.find(arguments[1]);
      //if(it != mat->layers.end()) {
      //  model->material = it->second;
      //}
      model->texture = mat;
	  }
    if (command.compare("f") == 0) { //faces
      //vertex->sides = arguments.size() - 1;

      for (unsigned int i = 1; i < arguments.size(); i++) {
        object::vertex vertex;
        model->vertices.push_back(vertex);

        my::vector<my::string> parts = tokenize(arguments[i], "/");

        int vi = (atoi(parts[0].c_str()) - 1) * 3;
        int ti = (atoi(parts[1].c_str()) - 1) * 2;
        int ni = (atoi(parts[2].c_str()) - 1) * 3;

        vertex.coordinate[0] = vertices[vi];
        vertex.coordinate[1] = vertices[vi + 1];
        vertex.coordinate[2] = vertices[vi + 2];
        vertex.coordinate[3] = 1.0f;

        vertex.texture[0] = vertices[ti];
        vertex.texture[1] = vertices[ti + 1];

        vertex.normal[0] = vertices[ni];
        vertex.normal[1] = vertices[ni + 1];
        vertex.normal[2] = vertices[ni + 2];
      }
    }

  }
};

__MYLO_NAMESPACE_END

#endif
