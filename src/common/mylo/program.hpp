#include "mylo.hpp"

#ifndef __PROGRAM_HPP
#define __PROGRAM_HPP

__MYLO_NAMESPACE_BEGIN

class __MYLO_DLL_EXPORT program {
public:
  program() {
    fragment = NULL;
    vertex = NULL;
  }

  my::frag *fragment;
  my::vert *vertex;

  my::frag &operator << (my::frag &rval) {
    fragment = &rval;
    return(rval);
  }
  my::vert &operator << (my::vert &rval) {
    vertex = &rval;
    return(rval);
  }

  unsigned int context;
};

__MYLO_NAMESPACE_END

#endif

#include "mylo.hpp"
