/*
================================================================================

Copyright (c) 2012, Dee E. Abbott
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.
* Neither the name of the organization nor the names of its contributors may
be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

================================================================================
*/


#include "mylo.hpp"

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
    S storage_index = relative_index - m_storage_offset;

    if(pointer && m_storage_size && relative_index >= m_storage_offset && storage_index < m_storage_size) {
      if (storage_index > m_relative_back) {
        m_relative_back = storage_index;
      }
      return(pointer[storage_index]);
    }
    
	  S allocation = 0;
	  S move       = 0;
	  
    S delta = 0;

    if (relative_index >= m_storage_offset) {
      if (storage_index + 1 > m_storage_size) {
        delta = storage_index + 1 - m_storage_size;
      }
    }
    else {
      delta = m_storage_offset - relative_index;
    }

	  if(!pointer) {
		  m_relative_front = relative_index;
      m_relative_back = relative_index;
      m_storage_offset = relative_index;
      storage_index -= m_storage_offset;
		  allocation = 1;
	  }
	  else {
		  if(relative_index < m_storage_offset) {
        m_storage_offset = relative_index;
			  m_relative_front = relative_index;
        storage_index = 0;
			  allocation = delta + m_storage_size;
			  move = 1;
		  }
		  if(relative_index > m_storage_offset && delta) {
			  allocation = m_storage_size + delta;
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
	  
	  if(relative_index > m_relative_back) {
	      m_relative_back = relative_index;
	  }
	  
	  return(pointer[storage_index]);
  }
  
  bool exists(S relative_index) {
	  S storage_index = relative_index + m_relative_front - m_storage_offset;
	  if (pointer && m_storage_size && relative_index >= m_storage_offset && storage_index < m_storage_size) {
		  return(pointer[storage_index]);
	  }
	  return(false);
  }
  
  void insert(basic_iterator< pair<S, T> > &it, const T &value) {
    S target = it->first;
    S move = m_relative_back;
    for(; move > target; move--) {
      (*this)[move +1] = (*this)[move];
    }
    (*this)[target] = value;
  }

  void push_back(const T &value) {
    push(value);
  }
  
  void push(const T &value) {
    (*this)[m_storage_size?m_relative_back - m_relative_front + m_storage_offset + 1:0] = value;
  }
  
  basic_iterator< pair<S, T> > pop_back() {
    return(pop());
  }
  basic_iterator< pair<S, T> > pop() {
    if(m_relative_back < m_relative_front) {
      return(basic_iterator< pair<S, T> >::terminator);
    }
    
    pair<S, T> *array = new pair<S, T>[1];
    
    S storage_index = m_relative_back - m_storage_offset;
    for(; storage_index>m_storage_offset; storage_index--) {
      m_relative_back -= 1;
      if(traits_first::is_pointer && !traits_first::has_value(pointer[storage_index])) {
        continue;
      }
      array[0].first = storage_index + m_storage_offset;
      array[0].second = pointer[storage_index];
      break;
    }

    basic_iterator< pair<S, T> > result(array, 1);
    
    return(result);
  }

  basic_iterator< pair<S, T> > shift() {
    if(m_relative_front > m_relative_back) {
      return(basic_iterator< pair<S, T> >::terminator);
    }
    
    pair<S, T> *array = new pair<S, T>[1];
    
    S storage_index = m_relative_front - m_storage_offset;
    for (; storage_index < m_relative_back - m_storage_offset; storage_index++) {
      m_relative_front += 1;
      if (traits_first::is_pointer && !traits_first::has_value(pointer[storage_index])) {
        continue;
      }
      array[0].first = storage_index + m_storage_offset;
      array[0].second = pointer[storage_index];
      break;
    }
    
    basic_iterator< pair<S, T> > result(array, 1);
    
    return(result);
  }


  T &last() {
    return((*this)[m_relative_back-m_storage_offset]);
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
    return(m_relative_front - m_storage_offset);
  }

  basic_iterator< pair<S, T> > begin() const {
    size_t allocation = allocated();
    if(allocation == 0) {
      return(basic_iterator< pair<S, T> >::terminator);
    }
    
    pair<S, T> *array = new pair<S, T>[allocation];

    S size = 0;
    for(S storage_index=m_relative_front - m_storage_offset, end=m_relative_back - m_storage_offset; storage_index<=end; storage_index++) {
      if(traits_first::is_pointer && !traits_first::has_value(pointer[storage_index])) {
        continue;
      }
      array[size].first = storage_index + m_storage_offset;
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
