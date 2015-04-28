#include "mylo.hpp"

#ifndef __ENTITY_HPP
#define __ENTITY_HPP

__MYLO_NAMESPACE_BEGIN

class __MYLO_DLL_EXPORT entity: public datatype<entity> {
public:
  my::map< my::string, my::shared_ptr<object> > objects;

  my::buffer buffer;
};

__MYLO_NAMESPACE_END

#endif
