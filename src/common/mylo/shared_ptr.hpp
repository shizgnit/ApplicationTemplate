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

#ifndef __SHARED_PTR_HPP
#define __SHARED_PTR_HPP

__MYLO_NAMESPACE_BEGIN

template <class T, class S = allocation_single> class __MYLO_INTERNAL_DLL_EXPORT shared_ptr {
public:
  typedef shared_ptr<T, S> type;
  typedef type_traits<T> traits_first;
  typedef type_traits<S> traits_second;
  
public:
  shared_ptr() {
    initialize();
  }
  shared_ptr(const shared_ptr &in) {
    copy(in);
  }
  shared_ptr(T* in) {
    initialize();
    attach(in);
  }
  
  virtual ~shared_ptr() {
    release();
  }
  
  shared_ptr &operator=(const shared_ptr &in) {
    if(this != in.ref()) {
      release();
      copy(in);
    }
    return(*this);
  }
  shared_ptr &operator=(T* in) {
    release();
    attach(in);
    return(*this);
  }
 
  T &operator * () {
    return(*m_ptr);
  }

  operator T () {
    return(*m_ptr);
  }

  T *operator & () const {
    return(m_ptr);
  }
  
  operator T* () {
    return(m_ptr);
  }
  
  T* operator -> () {
    return(m_ptr);
  }
  T* operator -> () const {
    return(m_ptr);
  }

  //operator void* () {
  //  return(reinterpret_cast<void*>(m_ptr));
  //}
  
  operator bool () const {
    return(m_ptr == NULL ? false : true);
  }
  
  unsigned long count() {
    return(m_count ? *m_count : 0);
  }
  
private:
  const shared_ptr *ref() const {
    return(this); 
  }
  
  void initialize(void) {
    m_ptr = NULL;
    m_count = NULL;
  }
  
  void copy(const shared_ptr &in) {
    m_ptr = in.m_ptr;
    m_count = in.m_count;
    if(m_count) {
      *m_count += 1;
    }
  }
  
  void attach(T* in) {
    m_ptr = in;
    m_count = new unsigned long;
    *m_count = 1;
  }
  
  void release(void) {
    if(m_count != NULL && *m_count) {
      if((*m_count -= 1) == 0) {
        if(m_ptr != NULL && traits_second::is_allocation) {
          if(traits_second::is_array) {
            delete [] (m_ptr);
          }
          else {
            delete(m_ptr);
          }
          m_ptr = NULL;
        }
        delete(m_count);
        m_count = NULL;
      }
    }
  }
  
private:
  T *m_ptr;
  unsigned long *m_count;
};

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
