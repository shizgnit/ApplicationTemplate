#include "mylo.hpp"

#ifndef __ITERATOR_HPP
#define __ITERATOR_HPP

__MYLO_NAMESPACE_BEGIN

template <class T> class basic_iterator {
public:
  typedef basic_iterator<T> type;
  typedef type_traits<T> traits;
  
  static const basic_iterator<T> terminator;
  
public:
  basic_iterator() : m_size(0), m_index(-1) { }
  
  basic_iterator(T array[], size_t size, size_t offset=0) {
    m_array = array;
    m_size = size;
    m_index = offset;
  }
  
  basic_iterator(const basic_iterator &in) {
    m_array = in.m_array;
    m_size = in.m_size;
    m_index = in.m_index;
  }
  
  operator T () {
    return(*((T*)m_array + m_index));
  }

  T* operator -> () {
    return((T*)m_array + m_index);
  }
  
  basic_iterator<T> operator ++ () {
    if(m_index >= m_size-1) {
      m_index = -1;
      return(terminator);
    }
    m_index += 1;
    return(*this);
  }
  
  basic_iterator<T> operator ++ (int) {
    if(m_index >= m_size-1) {
      m_index = -1;
      return(terminator);
    }
    basic_iterator<T> current = *this;
    m_index += 1;
    return(current);
  }
  
  bool operator == (const basic_iterator &in) {
    return(this->m_index == in.m_index);
  }

  bool operator != (const basic_iterator &in) {
    return(this->m_index != in.m_index);
  }
  
  operator bool() {
    bool result = m_index == -1 ? false : (*((T*)m_array + m_index));
    return(result);
  }
  
private:
  shared_ptr<T, allocation_array> m_array;
  size_t m_size;
  size_t m_index;
};

template<class T> const basic_iterator<T> basic_iterator<T>::terminator;

template <class T> class random_access {
public:
  typedef typename my::basic_iterator<T> iterator;
  
public:
  virtual basic_iterator<T> begin() const = 0;
  virtual basic_iterator<T> end() const {
    return(basic_iterator<T>::terminator);
  }
};

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
