#include "mylo.hpp"

#ifndef __SHADER_HPP
#define __SHADER_HPP

__MYLO_NAMESPACE_BEGIN

class __MYLO_DLL_EXPORT shader: public datatype<shader> {
public:
  my::buffer buffer;
  my::string text;

  unsigned int context;
};

__MYLO_NAMESPACE_END

#endif
