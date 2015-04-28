#include "mylo.hpp"

#ifndef __VARIANT_HPP
#define __VARIANT_HPP

__MYLO_NAMESPACE_BEGIN

struct variant {
  union {
    long long _long;
    double _double;
    void *_ptr;
  };
  my::string _string;
  
  variant() {}
  variant(const int &var) { _long = var; }
  variant(const float &var) { _double = var; }
  variant(void *var) { _ptr = var; }
  
  operator int() const { return((int)_long); }
  operator long() const { return((long)_long); }
  operator unsigned int() const { return((unsigned int)_long); }
  operator unsigned long() const { return((unsigned long)_long); }
  operator void* () const { return((void *)_ptr); }
  operator float() const { return((float)_double); } 
  operator bool() const { return(_long || _double || !_string.empty()); }
};

__MYLO_NAMESPACE_END

#endif