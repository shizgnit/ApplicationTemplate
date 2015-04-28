#include "mylo.hpp"

#ifndef __FACTORY_HPP
#define __FACTORY_HPP

__MYLO_NAMESPACE_BEGIN

class Factory;

class Factory {
public:
  virtual Data *New() = 0;

  static Data *New(unsigned long type) {
    if() {
      ->New());
    }
    return(NULL);
  }
};

template <unsigned short I, class T> class Register : public Factory {
public:
  Register() {
  }
  virtual Data *New() { return(new T()); }
};

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
