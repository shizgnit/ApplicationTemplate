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

#ifndef __STRING_HPP
#define __STRING_HPP

__MYLO_NAMESPACE_BEGIN

template <class T> class __MYLO_DLL_EXPORT basic_string {
public:
  static const size_t npos = -1;
  
  typedef typename my::basic_string<T> type;
  
  basic_string() {
    initialize();
  }
  
  basic_string(const basic_string &in) {
    initialize();
    copy(in.m_data);
  }
  
  basic_string(const T *in) {
    initialize();
    copy(in);
  }
  
  virtual ~basic_string() {
    release();
  }
  
  basic_string &operator = (const basic_string &in) {
    if(this != &in) {
      release();
      copy(in.m_data);
    }
    return(*this);
  }
  
  basic_string &operator = (const T *in) {
    if(m_data != in) {
      release();
      copy(in);
    }
    return(*this);
  }

  bool operator == (const T *in) {
    if(in == NULL || m_data == NULL) {
      return(in == m_data);
    }

    size_t index = 0;
    while(in[index]) {
      if(!m_data[index]) {
        return(false);
      }
      if(in[index] != m_data[index]) {
        return(false);
      }
      index += 1;
    }
    return(!m_data[index]);
  }

  bool operator != (const T *in) {
    return(!(*this == in));
  }

  int operator [] (const unsigned long &pos) { return(get(pos)); }
  int operator [] (const unsigned int &pos)  { return(get(pos)); }
  int operator [] (const long &pos)          { return(get(pos)); }
  int operator [] (const int &pos)           { return(get(pos)); }
  //int operator [] (const size_t &pos)        { return(get(pos)); }

  virtual int get(size_t pos) const {
    int rval = m_data[pos];
    return(rval);
  }

  virtual void set(size_t pos, int byte) {
    m_data[pos] = byte;
  }
  
  operator T *() const {
    return(m_data);
  }
  
  T *c_str() {
    return(reinterpret_cast<T *>(m_data));
  }
  
  virtual size_t capacity() const {
    return(size()); 
  }
  
  virtual size_t size() const {
    if(m_data == NULL) {
      return(0);
    }
    size_t units = 0;
    while(m_data[units]) { units += 1; }
    return(units);
  }

  virtual size_t length() const {
    return(size());
  }
  
  virtual bool empty() const {
    return(m_data == NULL || length() == 0);
  }
  
  virtual operator bool() const {
    return(m_data != NULL);
  }  
  
  virtual void append(const basic_string &in) {
    append(in.m_data);
  }
  
  virtual void append(const T *in) {
    insert(in, npos);
  }
  
  virtual basic_string<T> &insert(const T *in, size_t pos) {
    if(in == NULL) {
      return(*this);
    }

    if(m_data == NULL) {
      copy(in);
      return(*this);
    }
    
    size_t current = size();
    size_t allocation = 0;
    while(in[allocation]) {
      allocation += 1;
    }
    
    T *combined = new T[current + allocation+1];
    if(pos != npos) {
      memcpy(combined, m_data, pos * sizeof(T));
      memcpy(combined+pos, in, allocation * sizeof(T));
      memcpy(combined+pos+allocation, m_data+pos, current-pos * sizeof(T));
    }
    else {
      memcpy(combined, m_data, current * sizeof(T));
      memcpy(combined+current, in, (allocation+1) * sizeof(T));
    }
    
    release();
    m_data = combined;
    
    return(*this);
  }
  
  virtual basic_string<T> substr(size_t pos = 0, size_t n = npos) {
    basic_string<T> result;
    
    T *start = indexer(pos);
    T *end = n == npos ? m_data + size() : indexer(pos+n);
    
    if (end > indexer(size())) {
      end = indexer(size());
    }

    size_t length = end - start;
    
    T *temporary = new T[length+1];
    memcpy(temporary, start, length);
    temporary[length] = 0;
     
    result = temporary;
    
    delete [] (temporary);
    
    return(result);
  }

  virtual int compare(const T *rval) {
    return(strcmp((char *)m_data, (char *)rval));
  }

  virtual void clear() {
    release();
  }
  
  virtual size_t find(const T *str, size_t pos=0) {
    return(find(basic_string<T>(str), pos));
  }
  virtual size_t find(const basic_string<T> &str, size_t pos=0) {
    size_t ref = 0;
    for(size_t i=pos, stop=length(); i < stop; i++) {    
      if(get(i) == str.get(ref)) {
        ref += 1;
        if(ref >= str.length()) {
          return(i);
        }
      }
      else {
        ref = 0;
      }
    }
    return(npos);
  }
  
  virtual size_t rfind(const T *str, size_t pos=0) {
    return(find(basic_string<T>(str), pos));
  }
  virtual size_t rfind(const basic_string<T> &str, size_t pos=npos) {
    size_t ref = str.length();
    for(size_t i=pos==npos?length():pos; i >= 0; i--) {
      if(get(i) == str.get(ref)) {
        if(ref == 0) {
          return(i);
        }
        ref -= 1;
      }
      else {
        ref = str.length();
      }
    }
    return(npos);
  }
  
  virtual basic_string<T> &replace(size_t pos, size_t n, basic_string<T> str) {
    T *pointer = indexer(pos);
    
    size_t size_current = size();
    size_t size_front = pointer - m_data;
    size_t size_middle = str.size();
    size_t size_back = size_current - size_front - n;
    size_t size_total = size_current - n + size_middle;
    
    T *combined = new T[size_total+1];
    memcpy(combined, m_data, size_front);
    memcpy(combined + size_front, str.m_data, size_middle);
    memcpy(combined + size_front + size_middle, m_data + size_front + n, size_back);
    combined[size_total] = 0;
     
    release();
    m_data = combined;
    
    return(*this);
  }
  
protected:
  void initialize() {
    m_data = NULL;
  }
  
  void release() {
    delete [] (m_data);
    initialize();
  }
  
  void copy(const T *in) {
    if(in == NULL) {
      return;
    }

    size_t allocation = 0;
    while(in[allocation]) {
      allocation += 1;
    }
    
    m_data = new T[allocation+1];
    memcpy(m_data, in, (allocation+1) * sizeof(T));
  }
  
  T *indexer(size_t pos) {
    if(m_data == NULL) {
      return(NULL);
    }
    return(m_data+pos);
  }
  
protected:
  T *m_data;
};

class __MYLO_DLL_EXPORT string : public basic_string<char> {
public:
  static const unsigned int byte_order_mark = 0xEFBBBF;

  string() : basic_string<char>() { }
  string(const basic_string<char> &in) : basic_string<char>(in) { }
  string(const string &in) : basic_string<char>(in) { }
  string(const char *in) : basic_string<char>(in) { }
  
  string(const int *in, size_t n) : basic_string<char>() {
    if(in == NULL) {
      return;
    }
    char *buffer = new char[n*6];
    char *current = buffer;
    for(size_t i=0; i<n; i++) {
      size_t bytes;
      encode(in[i], current, bytes);
      current += bytes;
    }
    *current = 0;
    
    copy(buffer);
    delete [] buffer;
  }
  
  string(const wchar_t *in, size_t n) : basic_string<char>() {
    if(in == NULL) {
      return;
    }
    char *buffer = new char[n*6];
    char *current = buffer;
    for(size_t i=0; i<n; i++) {
      size_t bytes;
      encode(in[i], current, bytes);
      current += bytes;
    }
    *current = 0;
    
    copy(buffer);
    delete [] buffer;
  }
  
  ~string() { }
  
  // utf16n = 0xFEFF,
  // utf16  = 0xFFFE,
  // utf32n = 0x0000FEFF,
  // utf32  = 0xFFFE0000
  
  size_t length() const {
    if(m_data == NULL) {
      return(0);
    }
    size_t codepoints, i;
    count(codepoints, i);
    return(codepoints);
  }
  
  int get(size_t pos) const {
    if(m_data == NULL) {
      return(0);
    }
    size_t codepoints, i;
    count(codepoints, i, pos);
    return(decode(&m_data[i]));
  }
  
  void set(size_t pos, int codepoint) {
    size_t codepoints, i;
    count(codepoints, i, pos);
    char buffer[7];
    size_t bytes;
    encode(codepoint, buffer, bytes);
    insert(buffer, i);
  }
  
protected:
  static const unsigned char mask_encoded = 0x3F;
  static const unsigned char mask_multibyte = 0x80;
  static const unsigned char mask_2bytes = 0xC0;
  static const unsigned char mask_3bytes = 0xE0;
  static const unsigned char mask_4bytes = 0xF0;
  static const unsigned char mask_5bytes = 0xF8;
  static const unsigned char mask_6bytes = 0xFC;
  
  void count(size_t &codepoints, size_t &i, size_t pos=npos) const {
    codepoints = i = 0;
    while(m_data[i]) {
      if(codepoints == pos) {
        break;
      }
      if(m_data[i] & mask_multibyte) {
        if((m_data[i] & mask_2bytes) == mask_2bytes)      { i += 2; }
        else if((m_data[i] & mask_3bytes) == mask_3bytes) { i += 3; }
        else if((m_data[i] & mask_4bytes) == mask_4bytes) { i += 4; }
      }
      else {
        i += 1;
      }
      codepoints += 1;
    }
  }
  
  char *indexer(size_t pos) {
    if(m_data == NULL) {
      return(NULL);
    }
    
    size_t codepoints, i;
    count(codepoints, i, pos);
    
    return(m_data+i);
  }
  
  int decode(char *ptr) const {
    int codepoint = ptr[0];
    if(ptr[0] & mask_multibyte) {
      // 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
      if((ptr[0] & mask_6bytes) == mask_6bytes) {
        codepoint = ((ptr[0] & 0x01) << 30) | 
                    ((ptr[1] & mask_encoded) << 24) | 
                    ((ptr[2] & mask_encoded) << 18) | 
                    ((ptr[3] & mask_encoded) << 12) | 
                    ((ptr[4] & mask_encoded) << 6) | 
                     (ptr[5] & mask_encoded);
      }
      // 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
      else if((ptr[0] & mask_5bytes) == mask_5bytes) {
        codepoint = ((ptr[0] & 0x03) << 24) | 
                    ((ptr[1] & mask_encoded) << 18) | 
                    ((ptr[2] & mask_encoded) << 12) | 
                    ((ptr[3] & mask_encoded) << 6) | 
                     (ptr[4] & mask_encoded);
      }
      // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
      else if((ptr[0] & mask_4bytes) == mask_4bytes) {
        codepoint = ((ptr[0] & 0x07) << 18) | 
                    ((ptr[1] & mask_encoded) << 12) | 
                    ((ptr[2] & mask_encoded) << 6) | 
                     (ptr[3] & mask_encoded);
      }
      // 1110xxxx 10xxxxxx 10xxxxxx
      else if((ptr[0] & mask_3bytes) == mask_3bytes) {
        codepoint = ((ptr[0] & 0x0F) << 12) | 
                    ((ptr[1] & mask_encoded) << 6) | 
                     (ptr[2] & mask_encoded);
      }
      // 110xxxxx 10xxxxxx
      else if((ptr[0] & mask_2bytes) == mask_2bytes) {
        codepoint = ((ptr[0] & 0x1F) << 6) |
                     (ptr[1] & mask_encoded);
      }
    }
    return(codepoint);
  }
  
  void encode(int codepoint, char *buffer, size_t &bytes) {
    // 0xxxxxxx
    bytes = 0;
    if(codepoint < 0x80) {
      buffer[bytes++] = (char)codepoint;
    }
    // 110xxxxx 10xxxxxx
    else if(codepoint < 0x800) {
      buffer[bytes++] = (char)(mask_2bytes | codepoint >> 6);
      buffer[bytes++] = (char)(mask_multibyte | codepoint & mask_encoded);
    }
    // 1110xxxx 10xxxxxx 10xxxxxx
    else if(codepoint < 0x10000) {
      buffer[bytes++] = (char)(mask_3bytes | codepoint >> 12);
      buffer[bytes++] = (char)(mask_multibyte | codepoint >> 6 & mask_encoded);
      buffer[bytes++] = (char)(mask_multibyte | codepoint & mask_encoded);
    }
    // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    else if(codepoint < 0x200000) {
      buffer[bytes++] = (char)(mask_4bytes | codepoint >> 18);
      buffer[bytes++] = (char)(mask_multibyte | codepoint >> 12 & mask_encoded);
      buffer[bytes++] = (char)(mask_multibyte | codepoint >> 6 & mask_encoded);
      buffer[bytes++] = (char)(mask_multibyte | codepoint & mask_encoded);
    }
    // 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
    else if(codepoint < 0x4000000) {
      buffer[bytes++] = (char)(mask_5bytes | codepoint >> 24);
      buffer[bytes++] = (char)(mask_multibyte | codepoint >> 18 & mask_encoded);
      buffer[bytes++] = (char)(mask_multibyte | codepoint >> 12 & mask_encoded);
      buffer[bytes++] = (char)(mask_multibyte | codepoint >> 6 & mask_encoded);
      buffer[bytes++] = (char)(mask_multibyte | codepoint & mask_encoded);
    }
    // 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
    else if(codepoint < 0x8000000) {
      buffer[bytes++] = (char)(mask_6bytes | codepoint >> 30);
      buffer[bytes++] = (char)(mask_multibyte | codepoint >> 24 & mask_encoded);
      buffer[bytes++] = (char)(mask_multibyte | codepoint >> 18 & mask_encoded);
      buffer[bytes++] = (char)(mask_multibyte | codepoint >> 12 & mask_encoded);
      buffer[bytes++] = (char)(mask_multibyte | codepoint >> 6 & mask_encoded);
      buffer[bytes++] = (char)(mask_multibyte | codepoint & mask_encoded);
    }
    buffer[bytes] = 0;
  }

};

class wstring : public basic_string<wchar_t> {
public:
  static const unsigned int byte_order_mark = 0xFFFE;

  wstring() : basic_string<wchar_t>() { }
  wstring(const basic_string<wchar_t> &in) : basic_string<wchar_t>(in) { }
  wstring(const wstring &in) : basic_string<wchar_t>(in) { }
  wstring(const wchar_t *in) : basic_string<wchar_t>(in) { }
  
  wstring(const int *in, size_t n) : basic_string<wchar_t>() {
    if(in == NULL) {
      return;
    }
    wchar_t *buffer = new wchar_t[n*2];
    wchar_t *current = buffer;
    for(size_t i=0; i<n; i++) {
      size_t bytes;
      encode(in[i], current, bytes);
      current += bytes;
    }
    *current = 0;
    
    copy(buffer);
    delete [] buffer;
  }
  
  wstring(const string &in) : basic_string<wchar_t>() {
    size_t length = in.length();
    wchar_t *buffer = new wchar_t[length*2];
    wchar_t *current = buffer;
    for(size_t i=0, end=in.length(); i<end; i++) {
      size_t index;
      encode(in[i], current, index);
      current += index;
    }
    *current = 0;
    
    copy(buffer);
    delete [] buffer;
  }

  ~wstring() { }

  operator string() {
    size_t codepoints, i;
    count(codepoints, i);
    return(string(m_data, i));
  }

  // utf16n = 0xFEFF,
  // utf16  = 0xFFFE,
  // utf32n = 0x0000FEFF,
  // utf32  = 0xFFFE0000
  
  size_t length() const {
    if(m_data == NULL) {
      return(0);
    }
    size_t codepoints, i;
    count(codepoints, i);
    return(codepoints);
  }
  
  int get(size_t pos) const {
    if(m_data == NULL) {
      return(0);
    }
    size_t codepoints, i;
    count(codepoints, i, pos);
    return(decode(&m_data[i]));
  }
  
  void set(size_t pos, int codepoint) {
    size_t codepoints, i;
    count(codepoints, i, pos);
    wchar_t buffer[2];
    size_t bytes;
    encode(codepoint, buffer, bytes);
    insert(buffer, i);
  }
  
protected:
  static const unsigned short mask_encoded = 0x3F00;
  static const unsigned short mask_multibyte = 0x8000;
  static const unsigned short mask_4bytes = 0xC000;
  
  void count(size_t &codepoints, size_t &i, size_t pos=npos) const {
    codepoints = i = 0;
    while(m_data[i]) {
      if(codepoints == pos) {
        break;
      }
      if(m_data[i] & mask_multibyte) {
        if((m_data[i] & mask_4bytes) == mask_4bytes) { i += 2; }
      }
      else {
        i += 1;
      }
      codepoints += 1;
    }
  }
  
  wchar_t *indexer(size_t pos) {
    if(m_data == NULL) {
      return(NULL);
    }
    
    size_t codepoints, i;
    count(codepoints, i, pos);
    
    return(m_data+i);
  }
  
  int decode(wchar_t *ptr) const {
    int codepoint = ptr[0];
    return(codepoint);
  }
  
  void encode(int codepoint, wchar_t *buffer, size_t &index) {
    index = 0;
    buffer[index++] = (wchar_t)(codepoint);
    buffer[index] = 0;
  }
 
};

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
