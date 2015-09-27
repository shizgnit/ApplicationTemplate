#include "mylo.hpp"

#ifndef __OBJ_HPP
#define __OBJ_HPP

__MYLO_NAMESPACE_BEGIN

class __MYLO_DLL_EXPORT obj: public objects {
public:
  obj() {
    callback_write("OBJ", write);
  }

  static off_t write(objects *context, unsigned char *buffer, off_t bytes) {
    if(bytes) {
      context->buffer.write(buffer, bytes);
      return(bytes);
    }

    my::vector<char *> lines = my::segment((char *)context->buffer.raw(), '\n');
    for (unsigned int i = 0; i < lines.size(); i++) {
      if (lines[i][0] == '#') {
        continue;
      }
      my::vector<char *> line = my::segment(lines[i], ' ');
      process(context, line[0], line);
    }

    return(0);
  }

  static void process(objects *context, char *command, my::vector<char *> &arguments) {
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

    if (strcmp(command, "mtllib") == 0) { //material
      context->mats = new my::mtl;
      context->mats->search_path = context->search_path;
      *context->mats << my::asset(my::join("/", (context->search_path, arguments[1])));
    }
    if (strcmp(command, "o") == 0) { //entity
      context->active = new my::object;
      context->active->id = arguments[1];
      context->object[arguments[1]] = context->active;
    }
    if (strcmp(command, "v") == 0) { //vertex
      context->buffer_vertices.push_back((float)atof(arguments[1]));
      context->buffer_vertices.push_back((float)atof(arguments[2]));
      context->buffer_vertices.push_back((float)atof(arguments[3]));
    }
    if (strcmp(command, "vt") == 0) { //texture coordinates
      context->buffer_textures.push_back((float)atof(arguments[1]));
      context->buffer_textures.push_back((float)atof(arguments[2]));
    }
    if (strcmp(command, "vn") == 0) { //normals
      context->buffer_normals.push_back((float)atof(arguments[1]));
      context->buffer_normals.push_back((float)atof(arguments[2]));
      context->buffer_normals.push_back((float)atof(arguments[3]));
    }
    if (strcmp(command, "g") == 0) { //group
    }
    if (strcmp(command, "usemtl") == 0) { //material
      my::map<my::string, my::shared_ptr<my::material> >::iterator it = context->mats->material.find(arguments[1]);
      if (it != context->mats->material.end()) {
        context->active->texture = it->second;
      }
    }
    if (strcmp(command, "f") == 0) { //faces

      if (arguments.size() >= 4) {
        my::vector<my::object::vertex> points;

        for (unsigned int i = 1; i < arguments.size(); i++) {
          my::object::vertex vertex;

          my::vector<char *> parts = segment(arguments[i], '/');

          int vi = (atoi(parts[0]) - 1) * 3;
          int ti = (atoi(parts[1]) - 1) * 2;
          int ni = (atoi(parts[2]) - 1) * 3;

          vertex.coordinate[0] = context->buffer_vertices[vi];
          vertex.coordinate[1] = context->buffer_vertices[vi + 1];
          vertex.coordinate[2] = context->buffer_vertices[vi + 2];
          vertex.coordinate[3] = 1.0f;

          vertex.texture[0] = context->buffer_textures[ti];
          vertex.texture[1] = context->buffer_textures[ti + 1];
          vertex.texture[2] = 0.0f;
          vertex.texture[3] = 0.0f;

          vertex.normal[0] = context->buffer_normals[ni];
          vertex.normal[1] = context->buffer_normals[ni + 1];
          vertex.normal[2] = context->buffer_normals[ni + 2];
          vertex.normal[3] = 0.0f;

          points.push_back(vertex);
        }

        if (points.size() == 3) {
          context->active->vertices.push_back(points[0]);
          context->active->vertices.push_back(points[1]);
          context->active->vertices.push_back(points[2]);
        }
        if (points.size() == 4) {
          context->active->vertices.push_back(points[0]);
          context->active->vertices.push_back(points[1]);
          context->active->vertices.push_back(points[2]);

          context->active->vertices.push_back(points[0]);
          context->active->vertices.push_back(points[2]);
          context->active->vertices.push_back(points[3]);
        }
      }

    }

  }
};

__MYLO_NAMESPACE_END

#endif
