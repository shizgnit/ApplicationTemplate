#include "mylo.hpp"

#ifndef __MATERIAL_HPP
#define __MATERIAL_HPP

__MYLO_NAMESPACE_BEGIN

class __MYLO_DLL_EXPORT material: public datatype<material> {
public:
  my::string id;

  float ambient[4];
  float diffuse[4];
  float specular[4];
  float emission[4];
	
  float shininess;
  float opacity;
  float illumination;

  my::shared_ptr<my::image> map;

  unsigned int context;

  my::buffer buffer;
};

__MYLO_NAMESPACE_END

#endif
