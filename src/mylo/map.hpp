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

#ifndef __MAP_HPP
#define __MAP_HPP

__MYLO_NAMESPACE_BEGIN

template <class T, class S> class __MYLO_INTERNAL_DLL_EXPORT map : public random_access< pair<T, S> > {
public:
  typedef map<T, S> type;
  typedef type_traits<T> traits_first;
  typedef type_traits<S> traits_second;
  
public:
  map() {
    //DEBUG<< this << "->map()" << std::endl;
    initialize();
  }
  
  map(const map &in) {
    initialize();
    copy(in);
  }

  ~map() {
    //DEBUG<< this << "->~map()" << std::endl;
    release();
  }

  map &operator = (const map&in) {
    copy(in);
    return(*this);
  }

  S &operator[](const T &key) {
    //DEBUG<< this << "->operator[](const T &key)" << std::endl;
    return(this->index(key));
  }
  
  S &index(const T &key) {
    //DEBUG<< this << "->index(const T &key)" << std::endl;
    node *current = locator(key, true);
    return(current->entry->second);
  };
  
  bool exists(const T &key) {
    //DEBUG<< this << "->exists(T key)" << std::endl;
    if(m_root == NULL) {
      return(false);
    }
    
    node *current = locator(key, false);
    if(current == NULL || current->entry == NULL) {
      return(false);
    }
    
    return(true);
  };
  
  basic_iterator< pair<T, S> > begin() const {
    //DEBUG<< this << "->begin()" << std::endl;
    size_t allocation = allocated(m_root);
    if(allocation == 0) {
      return(basic_iterator< pair<T, S> >::terminator);
    }
    
    pair<T, S> *array = new pair<T, S>[allocation];
    
    size_t size = 0;
    collect(m_root, array, size);

    basic_iterator< pair<T, S> > result(array, size);
    
    return(result);
  }
  
  basic_iterator< pair<T, S> > find(const T &key) {
    if(m_root == NULL || !exists(key)) {
      return(basic_iterator< pair<T, S> >::terminator);
    }
    
    node *current = locator(key, false);
    if(current == NULL || current->entry == NULL) {
      return(basic_iterator< pair<T, S> >::terminator);
    }
    
    pair<T, S> *array = new pair<T, S>[1];
    
    array[0] = *current->entry;
    
    basic_iterator< pair<T, S> > result(array, 1);
    
    return(result);
  }
  
  void insert(const pair<T, S> &in) {
    this->index(in.first) = in.second;
  }

  void clear() {
    release(); 
  }
    
protected:
  class node {
  public:
    node() {
      entry = NULL;
    }
    
    ~node() {
      for(unsigned char i=0; i < children.size(); i++) {
        delete(children.pointer[i]);
        children.pointer[i] = NULL;
      }
      if(entry) {
        traits_first::release(entry->first);
        delete(entry);
      }
    }
    
  public:
    my::vector<node *, unsigned char> children;
    pair<T, S> *entry;
  };
  
  node *m_root;
  
  void initialize() {
    m_root = NULL; 
  }
  
  void copy(const map &in) {
    if(this == &in) {
      return;
    }
    my::basic_iterator< pair<T, S> > it = in.begin();
    for(; it != in.end(); it++) {
      this->insert(it);
    }
  }
  
  void release() {
    delete(m_root); 
  }
  
  node *locator(const T &key, bool create) {
    if(m_root == NULL) {
      if(create) {
        m_root = new node();
      }
      else {
        return(NULL); 
      }
    }

    char *bytes = traits_first::is_pointer ? (char *)key : (char *)&key;
    
    node *current = m_root;
    for(size_t index=0, length=traits_first::size(key); index < length; index++) {
      if(current->children[bytes[index]] == NULL) {
        if(create) {
          current->children[bytes[index]] = new node();
        }
        else {
          return(NULL);
        }
      }
      current = current->children[bytes[index]];
    }
    
    if(current->entry == NULL) {
      if(create) {
        current->entry = new pair<T, S>();
        traits_first::copy(current->entry->first, key);
      }
      else {
        return(NULL); 
      }
    }
    
    return(current);
  }
  
  size_t allocated(node *current) const {
    size_t count = 0;
    
    if(current == NULL) {
      return(count);
    }
    
    for(unsigned char i=0; i < current->children.size(); i++) {
      if(current->children.pointer[i] == NULL) {
        continue;
      }
      if(current->children.pointer[i]->entry) {
        count += 1;
      }
      count += allocated(current->children.pointer[i]);
    }
    
    return(count);
  }
  
  void collect(node *current, pair<T, S> *collection, size_t &size) const {
    if(current == NULL) {
      return;
    }
    for(unsigned char i=0; i < current->children.size(); i++) {
      if(current->children.pointer[i] == NULL) {
        continue;
      }
      if(current->children.pointer[i]->entry) {
        collection[size++] = *current->children.pointer[i]->entry;
      }
      collect(current->children.pointer[i], collection, size);
    }
  }
};

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
