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

#ifndef __LIST_HPP
#define __LIST_HPP

__MYLO_NAMESPACE_BEGIN

class abstract_node {
public: virtual ~abstract_node() {}
};

template <class T, class S = my::shared_ptr<abstract_node> > class __MYLO_INTERNAL_DLL_EXPORT list : public random_access< pair<S, T> > {
public:
  typedef list<T, S> type;
  typedef type_traits<T> traits_first;
  typedef type_traits<S> traits_second;
  
public:
  list() {
    initialize();
  }
  
  list(const type &in) {
    initialize();
    copy(in);
  }
  
  ~list() {
    release();
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
  
  void insert(basic_iterator< pair<S, T> > &it, const T &value) {
    if (!it.first) {
      return; // case where attempts are made to add with a popped or shifted iterator
    }

    node *n = new node();
    n->entry = new pair<S, T>();

    n->entry->first = n;
    n->entry->second = value;

    if (m_tail == it.first) {
      m_tail = n;
    }
    n->next = it.first->next;
    n->prior = it.first;
    
    it.first->next = n;

    m_count += 1;
  }

  void push_back(const T &value) {
    push(value);
  }
  
  void push(const T &value) {
    node *n = new node();
    n->entry = new pair<S, T>();

    n->entry->first = n;
    n->entry->second = value;

    if (m_tail == NULL) {
      m_head = n;
      m_tail = n;
    }
    else {
      n->prior = m_tail;
      m_tail->next = n;
      m_tail = n;
    }

    m_count += 1;
  }
  
  basic_iterator< pair<S, T> > pop_back() {
    return(pop());
  }
  basic_iterator< pair<S, T> > pop() {
    if (m_tail == NULL) {
      return(basic_iterator< pair<S, T> >::terminator);
    }

    pair<S, T> *array = new pair<S, T>[1];

    node *current = m_tail;

    array[0].first = m_tail->entry->first;
    array[0].second = m_tail->entry->second;

    basic_iterator< pair<S, T> > result(array, 1);

    if (m_tail->prior != NULL) {
      m_tail->prior->next = NULL;
      m_tail = m_tail->prior;
    }
    else {
      m_head = NULL;
      m_tail = NULL;
    }

    current->entry->first = NULL;
    m_count -= 1;

    return(result);
  }

  basic_iterator< pair<S, T> > shift() {
    if(m_head == NULL) {
      return(basic_iterator< pair<S, T> >::terminator);
    }
    
    pair<S, T> *array = new pair<S, T>[1];

    node *current = m_head;

    array[0].first = m_head->entry->first;
    array[0].second = m_head->entry->second;

    basic_iterator< pair<S, T> > result(array, 1);

    if (m_head->next != NULL) {
      m_head->next->prior = NULL;
      m_head = m_head->next;
    }
    else {
      m_head = NULL;
      m_tail = NULL;
    }

    current->entry->first = NULL;
    m_count -= 1;

    return(result);
  }


  size_t capacity() const {
    return(size());
  }
  
  size_t size() const {
    return(m_count);
  }

  bool empty() const {
    return(size() == 0);
  }
  
  operator bool() const {
    return(!empty());
  }  
  
  T &tail() {
    return(m_tail->entry->second);
  }

  basic_iterator< pair<S, T> > begin() const {
    size_t allocation = m_count;
    if(allocation == 0) {
      return(basic_iterator< pair<S, T> >::terminator);
    }
    
    pair<S, T> *array = new pair<S, T>[allocation];

    node *current = m_head;
    size_t size = 0;
    while (current) {
      array[size].first = current->entry->first;
      array[size].second = current->entry->second;
      current = current->next;
      size += 1;
    }
    
    basic_iterator< pair<S, T> > result(array, size);
    
    return(result);
  }
  
  void clear() {
    release(); 
  }
  
private:
  class node : public abstract_node {
  public:
    node() {
      entry = NULL;
      prior = NULL;
      next = NULL;
    }

    ~node() {
      delete(entry);
    }

  public:
    pair<S, T> *entry;
    node *prior;
    node *next;
  };

  node *m_head;
  node *m_tail;

  size_t m_count;

  void initialize() {
    m_head = NULL;
    m_tail = NULL;
    m_count = 0;
  }

  void copy(const list &in) {
    if (this == &in) {
      return;
    }
    my::basic_iterator< pair<S, T> > it = in.begin();
    for (; it != in.end(); it++) {
      this->insert(it);
    }
  }

  void release() {
    delete(m_head);
  }
};

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
