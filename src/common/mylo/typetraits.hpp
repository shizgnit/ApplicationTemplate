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

#ifndef __TYPETRAITS_HPP
#define __TYPETRAITS_HPP

__MYLO_NAMESPACE_BEGIN


typedef unsigned int type_t;

enum type_mask {
  UNSPECIFIED_TYPE = 0x0001,
  POINTER_TYPE     = 0x0002,
  UNSIGNED_TYPE    = 0x0004,
  LONG_TYPE        = 0x0008,
  CHARACTER_TYPE   = 0x0010,
  SHORT_TYPE       = 0x0020,
  INTEGER_TYPE     = 0x0040,
  FLOAT_TYPE       = 0x0080,
  DOUBLE_TYPE      = 0x0100,
  STRING_TYPE      = 0x0200,
};

static const char *type_text[] = {
  "UNSPECIFIED",
  "POINTER",
  "UNSIGNED",
  "LONG",
  "CHARACTER",
  "SHORT",
  "INTEGER",
  "FLOAT",
  "DOUBLE",
  "STRING"
};

template <class T>
struct type_traits_defaults {
  static const bool is_pointer = false;
  static const bool is_array = false;
  static const bool is_allocation = true;
  
  static bool has_value(const T &v) { return(v ? true : false); }
  static size_t size(const T &v) { return(sizeof(T)); }
  static void initialize(T &v) { }
  static void copy(T &l, const T &r) { l = r; }
  static void release(T &v) { }
  
  static my::string name(type_t type) {
    my::string text;
    int mask = 1;
    for(int i=0; i<10; i++) {
      if(type_text[i] != 0 && type & mask) {
        if(text.length()) {
          text.append(" ");
        }
        text.append(type_text[i]);
      }
      mask <<= 1;
    }
    return(text);
  }
  static my::string name() {
    return(type_traits_defaults::name(type));
  }
  static const type_t type = UNSPECIFIED_TYPE;
};

template <class T>
struct type_traits: public type_traits_defaults<T> {
  typedef T type;
};

template <class T>
struct type_traits<T *>: public type_traits_defaults<T> {
  static const bool is_pointer = true;
  static bool has_value(T *v) { return(v != NULL); }
  static size_t size(T *v) { return(sizeof(T)); }
  static void initialize(T *&v) { v = NULL; }
  static void copy(T *&l, T *r) { l = r; }
  static void release(T *) { }
  static my::string name() { return(type_traits_defaults<T>::name(type)); }
  static const type_t type = UNSPECIFIED_TYPE | POINTER_TYPE;
};

struct allocation_none { };
struct allocation_single { };
struct allocation_array { };

template <>
struct type_traits<allocation_array>: public type_traits_defaults<allocation_array> {
  static const bool is_array = true;
  static const bool is_allocation = true;
};

template <>
struct type_traits<allocation_none>: public type_traits_defaults<allocation_array> {
  static const bool is_array = false;
  static const bool is_allocation = false;
};

template <>
struct type_traits<char>: public type_traits_defaults<char> {
  static void initialize(char &v) { v = 0; }
  static my::string name() { return(type_traits_defaults::name(type)); }
  static const type_t type = INTEGER_TYPE;
};

template <>
struct type_traits<unsigned char>: public type_traits_defaults<unsigned char> {
  static void initialize(unsigned char &v) { v = 0; }
  static my::string name() { return(type_traits_defaults::name(type)); }
  static const type_t type = UNSIGNED_TYPE | INTEGER_TYPE;
};

template <>
struct type_traits<int>: public type_traits_defaults<int> {
  static void initialize(int &v) { v = 0; }
  static my::string name() { return(type_traits_defaults::name(type)); }
  static const type_t type = INTEGER_TYPE;
  static bool has_value(const int &v) { return(v != 0 ? true : false); }
};

template <>
struct type_traits<unsigned int>: public type_traits_defaults<unsigned int> {
  static void initialize(unsigned int &v) { v = 0; }
  static my::string name() { return(type_traits_defaults::name(type)); }
  static const type_t type = UNSIGNED_TYPE | INTEGER_TYPE;
};

template <>
struct type_traits<long>: public type_traits_defaults<long> {
  static void initialize(long &v) { v = 0; }
  static my::string name() { return(type_traits_defaults::name(type)); }
  static const type_t type = LONG_TYPE;
};

template <>
struct type_traits<unsigned long>: public type_traits_defaults<unsigned long> {
  static void initialize(unsigned long &v) { v = 0; }
  static my::string name() { return(type_traits_defaults::name(type)); }
  static const type_t type = UNSIGNED_TYPE | LONG_TYPE;
};

template <>
struct type_traits<float>: public type_traits_defaults<float> {
  static void initialize(float &v) { v = 0.0f; }
  static my::string name() { return(type_traits_defaults::name(type)); }
  static const type_t type = FLOAT_TYPE;
  static bool has_value(const float &v) { return(v != 0.0f ? true : false); }
};

template <>
struct type_traits<double>: public type_traits_defaults<double> {
  static void initialize(double &v) { v = 0.0f; }
  static my::string name() { return(type_traits_defaults::name(type)); }
  static const type_t type = DOUBLE_TYPE;
};

template <>
struct type_traits<char *>: public type_traits_defaults<char *> {
  static const bool is_pointer = true;
  static bool has_value(const char *v) { return(v != NULL); }
  static size_t size(const char *v) { return(strlen(v)); }
  static void initialize(char *&v) { v = NULL; }
  static void copy(char *&l, const char *r) {
    size_t length = strlen(r);
    l = new char[length+1];
    memcpy(l, r, length+1);
  }
  static void release(char *v) {
    delete [] (v);
  }
  static my::string name() { return(type_traits_defaults::name(type)); }
  static const type_t type = CHARACTER_TYPE | POINTER_TYPE;
};

template <>
struct type_traits<my::string>: public type_traits_defaults<my::string> {
  static const bool is_pointer = true;
  static bool has_value(const my::string &v) { return(!v.empty()); }
  static size_t size(const my::string &v) { return(v.size()); }
  static my::string name() { return(type_traits_defaults::name(type)); }
  static const type_t type = STRING_TYPE;
};

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
