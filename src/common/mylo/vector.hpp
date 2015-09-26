#include "mylo.hpp"

/*

BUGS:

  The shift for whatever reason is skipping pointers with null values for whatever reason... might break something later.

*/


#ifndef __VECTOR_HPP
#define __VECTOR_HPP

__MYLO_NAMESPACE_BEGIN

template <class T, class S = unsigned long> class __MYLO_INTERNAL_DLL_EXPORT vector : public random_access< pair<S, T> > {
public:
  typedef vector<T, S> type;
  typedef type_traits<T> traits_first;
  typedef type_traits<S> traits_second;
  
public:
  vector() {
    initialize();
  }
  
  vector(size_t n) {
    initialize();
    this->reserve(n);
  }
  
  vector(const type &in) {
    initialize();
    copy(in);
  }
  
  ~vector() {
    release();
  }

  void reserve(size_t n, bool initialize=false) {
    this->index(0);
    this->index(n-1);
  }
  
  void resize(size_t n) {
    this->index(0);
    this->index(n-1);
  }
  
  type &operator=(const T &in) {
    push(in);
    return(*this);
  }
  
  type &operator=(const type &in) {
    if(this != &in) {
      release();
      copy(in);
    }
    return(*this);
  }

  T &operator[](const S &relative_index) {
    return(this->index(relative_index));
  }
  
  T &index(const S &relative_index) {
    if(pointer && m_storage_size && relative_index <= m_relative_back && relative_index >= m_relative_front) {
      return(pointer[relative_index-m_relative_front]);
    }
    
	  S allocation = 0;
	  S move       = 0;
	  
	  S delta = relative_index > m_relative_front ? relative_index - m_relative_front : m_relative_front - relative_index;
	  
	  if(!pointer) {
	    m_relative_front = relative_index;
	    allocation = 1;
	  }
	  else {
	    if(relative_index < m_relative_front) {
	      m_relative_front = relative_index;
	      allocation = delta + m_storage_size;
	      move = 1;
	    }
	    if(relative_index > m_relative_front && delta+1 > m_storage_size) {
	      allocation = m_storage_size + (delta+1 - m_storage_size);
	    }
	  }
	  
	  if(allocation) {
	    T *new_buffer = new T[allocation];
	    for(size_t i=0; i<allocation; i++) {
	      traits_first::initialize(new_buffer[i]);
	    }
	    if(pointer) {
	      for(size_t i=0, offset=delta * move; i<m_storage_size; i++) {
	        new_buffer[i + offset] = pointer[i];
	      }
	      delete [] (pointer);
	    }
	    m_storage_size  = allocation;
	    pointer = new_buffer;
	  }
	  
	  S target = relative_index - m_relative_front;
	  
	  if(relative_index > m_relative_back) {
	     m_relative_back = relative_index;
	  }
	  
	  return(pointer[target]);
  }
  
  bool exists(S relative_index) {
    if(pointer && m_storage_size && relative_index <= m_relative_back && relative_index >= m_relative_front) {
      return(pointer[relative_index]);
    }
    return(false);
  }
  
  void insert(basic_iterator< pair<S, T> > &it, const T &value) {
    S target = it->first;
    S storage_index = m_storage_size-1;
    for(; storage_index+m_relative_front > it->first; storage_index--) {
      (*this)[m_relative_front+storage_index+1] = (*this)[m_relative_front+storage_index];
    }
    (*this)[target] = value;
  }

  void push_back(const T &value) {
    push(value);
  }
  
  void push(const T &value) {    
    (*this)[m_storage_size?m_relative_back+1:0] = value;
  }
  
  basic_iterator< pair<S, T> > pop_back() {
    return(pop());
  }
  basic_iterator< pair<S, T> > pop() {
    if(m_relative_front > m_relative_back) {
      return(basic_iterator< pair<S, T> >::terminator);
    }
    
    pair<S, T> *array = new pair<S, T>[1];
    
    S storage_index = m_relative_back - m_relative_front;
    for(; storage_index>m_storage_offset; storage_index--) {
      if(traits_first::is_pointer && !traits_first::has_value(pointer[storage_index])) {
        continue;
      }
      array[0].first = m_relative_front + storage_index;
      array[0].second = pointer[storage_index];
      break;
    }
    if(storage_index == m_storage_offset) {
      array[0].first = m_relative_front;
      array[0].second = pointer[storage_index];
    }
    
    basic_iterator< pair<S, T> > result(array, 1);
    
    m_relative_back = m_relative_front + storage_index - 1;
    
    return(result);
  }

  basic_iterator< pair<S, T> > shift() {
    if(m_storage_offset >= m_storage_size || m_relative_front > m_relative_back) {
      return(basic_iterator< pair<S, T> >::terminator);
    }
    
    pair<S, T> *array = new pair<S, T>[1];
    
    S storage_index = m_storage_offset;
    while(storage_index<=m_storage_size-1) {
      // why the hell is this here???  ... still can't remember 2015/02
      //if(traits_first::is_pointer && !traits_first::has_value(pointer[storage_index])) {
      //  storage_index += 1;
      //  continue;
      //}
      array[0].first = m_relative_front + storage_index - m_storage_offset;
      array[0].second = pointer[storage_index];
      storage_index += 1;
      break;
    }
    
    basic_iterator< pair<S, T> > result(array, 1);
    
    m_relative_front += storage_index - m_storage_offset;
    m_storage_offset = storage_index;
    
    return(result);
  }


  T &last() {
    return((*this)[m_relative_back]);
  }
  
  size_t capacity() const {
    return(size());
  }
  
  S size() const {
    return(m_storage_size ? m_relative_back-m_relative_front+1 : 0);
  }

  bool empty() const {
    return(size() == 0);
  }
  
  operator bool() const {
    return(!empty());
  }  
  
  S offset() {
    return(m_relative_front);
  }
  
  basic_iterator< pair<S, T> > begin() const {
    size_t allocation = allocated();
    if(allocation == 0) {
      return(basic_iterator< pair<S, T> >::terminator);
    }
    
    pair<S, T> *array = new pair<S, T>[allocation];

    S size = 0;
    for(S storage_index=m_storage_offset, end=m_relative_back-m_relative_front; storage_index<=end; storage_index++) {
      if(traits_first::is_pointer && !traits_first::has_value(pointer[storage_index])) {
        continue;
      }
      array[size].first = m_relative_front + storage_index;
      array[size].second = pointer[storage_index];
      size += 1;
    }
    
    basic_iterator< pair<S, T> > result(array, size);
    
    return(result);
  }
  
  void clear() {
    release(); 
  }
  
public:
  T *pointer;
  
private:
  void initialize() {
    pointer = NULL;
    m_storage_size = 0;
    m_storage_offset = 0;
    m_relative_front = 0;
    m_relative_back = 0;
  }
  
  void release() {
    delete [] (pointer);
    initialize();
  }
  
  void copy(const vector &in) {
    if(this == &in) {
      return;
    }
    if(!in.m_storage_size) {
      return;
    }
    pointer = new T[in.m_storage_size];
    for(size_t i=0; i<in.m_storage_size; i++) {
      pointer[i] = in.pointer[i];
    }
    m_storage_size = in.m_storage_size;
    m_storage_offset = in.m_storage_offset;
    m_relative_front = in.m_relative_front;
    m_relative_back = in.m_relative_back;
  }

  int allocated() const {
    unsigned long count = 0;
    if(m_storage_size && traits_first::is_pointer) {
      S storage_index = 0;
      S storage_end = m_relative_back-m_relative_front;
      for(; storage_index<=storage_end; storage_index++) {
        if(traits_first::has_value(pointer[storage_index])) {
          count += 1;
        }
      }
    }
    else {
      count = m_storage_size;
    }
    return(count);
  }
  
private:
  S m_storage_size;
  S m_storage_offset;
  S m_relative_front;
  S m_relative_back;
};

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
