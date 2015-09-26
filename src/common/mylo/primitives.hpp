#include "mylo.hpp"

#ifndef __PRIMITIVES_HPP
#define __PRIMITIVES_HPP

__MYLO_NAMESPACE_BEGIN

class __MYLO_DLL_EXPORT primitive {
public:

 static my::object *quad(float width, float height) {
    int factor = 1;

    my::object *obj = new my::object;
    obj->vertices.reserve(6 * factor * factor);

    float x = 0.0f;
    float y = 0.0f;
	
    float dx = width / factor;
    float dy = height / factor;

    int index = 0;

    for (int i = 0; i < factor; i++) {
      x = dx * i;
      for (int j = 0; j < factor; j++) {
        y = dy * j;

        obj->vertices[index++].set_coordinate(x + dx, y + dy);
        obj->vertices[index++].set_coordinate(x, y + dy);
        obj->vertices[index++].set_coordinate(x, y);

        obj->vertices[index++].set_coordinate(x + dx, y + dy);
        obj->vertices[index++].set_coordinate(x, y);
        obj->vertices[index++].set_coordinate(x + dx, y);
      }
    }

    return(obj);
  }

};



__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
