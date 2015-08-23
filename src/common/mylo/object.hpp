#include "mylo.hpp"

#ifndef __OBJECT_HPP
#define __OBJECT_HPP

__MYLO_NAMESPACE_BEGIN

class __MYLO_DLL_EXPORT object: public datatype<object> {
public:
  class vertex {
  public:
    float coordinate[4];
    float texture[4];
    float normal[4];

    void set_coordinate(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) {
      coordinate[0] = x;
      coordinate[1] = y;
      coordinate[2] = z;
      coordinate[3] = w;
    }
    void set_texture(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) {
      texture[0] = x;
      texture[1] = y;
      texture[2] = z;
      texture[3] = w;
    }
    void set_normal(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) {
      normal[0] = x;
      normal[1] = y;
      normal[2] = z;
      normal[3] = w;
    }

    operator bool() const {
      return(true);
    }
  };

	void xy_projection(my::shared_ptr<my::image> reference, unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
    if(&texture == NULL) {
      texture = new my::material;
		}

    texture->map = reference;
		
		float max_x = 0.0f;
		float max_y = 0.0f;
		
		for(unsigned int i=0; i<vertices.size(); i++) {
			if(vertices[i].coordinate[0] > max_x) {
				max_x = vertices[i].coordinate[0];
			}
			if(vertices[i].coordinate[1] > max_y) {
				max_y = vertices[i].coordinate[1];
			}
		}

    float texture_dx = 1 / (float)texture->map->header.width;
    float texture_dy = 1 / (float)texture->map->header.height;

		for(unsigned int i=0; i<vertices.size(); i++) {
		  float dx = vertices[i].coordinate[0] / max_x;
		  float dy = (max_y - vertices[i].coordinate[1]) / max_y;
		  
      float tx = (width * dx + x) * texture_dx;
      float ty = (height * dy + y) * texture_dy;
      //float ty = 1.0f - ((height * dy + y) * texture_dy);

		  vertices[i].texture[0] = tx;
		  vertices[i].texture[1] = ty;
      vertices[i].texture[2] = 0.0f;
      vertices[i].texture[3] = 0.0f;
    }
		
	}

  object *flat_normals() {
    int triangles = vertices.size() / 3;
		  
    for(unsigned int i=0; i<vertices.size()/3; i++) {
      int index = i * 3;
		
	    float u[3];
	    float v[3];
		
	    u[0] = vertices[index+1].coordinate[0] - vertices[index].coordinate[0];
	    u[1] = vertices[index+1].coordinate[1] - vertices[index].coordinate[1];
	    u[2] = vertices[index+1].coordinate[2] - vertices[index].coordinate[2];
		
	    v[0] = vertices[index+2].coordinate[0] - vertices[index].coordinate[0];
	    v[1] = vertices[index+2].coordinate[1] - vertices[index].coordinate[1];
	    v[2] = vertices[index+2].coordinate[2] - vertices[index].coordinate[2];

	    for(int j=index; j<index+3; j++) {
		    vertices[j].normal[0] = u[1]*v[2] - u[2]*v[1];
		    vertices[j].normal[1] = u[2]*v[0] - u[0]*v[2];
		    vertices[j].normal[2] = u[0]*v[1] - u[1]*v[0];
		    vertices[j].normal[3] = 0.0f;
	    }
 
    }

    return(NULL);
  }
	
  my::vector< vertex > vertices;
  vertex *active;

  my::vector< my::vector< my::shared_ptr<vertex> > > faces;

  my::shared_ptr< material > texture;

  my::string id;

  unsigned int context;

  my::vector< float > buffer_vertices;
  my::vector< float > buffer_textures;
  my::vector< float > buffer_normals;

  my::buffer buffer;
};

class __MYLO_DLL_EXPORT objects : public datatype < objects > {
public:

  my::map<my::string, my::shared_ptr<my::object> > object;
  my::object *active;

  my::shared_ptr< materials > materials;

  my::buffer buffer;

};


__MYLO_NAMESPACE_END

#endif
