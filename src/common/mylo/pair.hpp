#include "mylo.hpp"

#ifndef __PAIR_HPP
#define __PAIR_HPP

__MYLO_NAMESPACE_BEGIN

template <class T, class S> class pair {
public:
  typedef pair<T, S> type;
  typedef type_traits<T> traits_first;
  typedef type_traits<S> traits_second;
  
public:
  pair() {
    traits_first::initialize(first);
    traits_second::initialize(second);
  }
  
  pair(const pair &in) {
    copy(in);
  }

  pair(const T &_first, const S &_second) {
    first = _first;
    second = _second;
  }

  pair &operator = (const pair &in) {
    copy(in);
    return(*this);
  }

  operator bool() const {
    return(traits_first::has_value(first) || traits_second::has_value(second)); 
  }
  
public:
  T first;
  S second;

protected:

  void copy(const pair &in) {
    if(this == &in) {
      return;
    }
    this->first = in.first;
    this->second = in.second;
  }
};

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
