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

#ifndef __STREAM_HPP
#define __STREAM_HPP

__MYLO_NAMESPACE_BEGIN

class serializable;

//template class __MYLO_DLL_EXPORT my::shared_ptr< my::vector<my::serializable *> >;

class __MYLO_DLL_EXPORT manipulator {
public:
  manipulator() {}
};

class __MYLO_DLL_EXPORT end_of_line : public manipulator {
public:
  end_of_line() {}
};

static end_of_line endl;

class __MYLO_DLL_EXPORT stream {
public:
  static const size_t alloc = 65536;
  //static const size_t alloc = 16384;
  
  stream() {
    m_chain = new my::vector<my::serializable *>();
  }
  
  stream(const my::serializable *lval, const my::serializable *rval);

  stream(const stream &in);
  
  ~stream();
  
  stream &operator << (const my::serializable &rval);
  stream &operator >> (const my::serializable &rval);
  
private:
  my::shared_ptr< my::vector<my::serializable *> > m_chain;
};

class buffer;

class __MYLO_DLL_EXPORT serializable {
public:
  serializable();
  virtual ~serializable();
  
  stream operator << (const serializable &rval) {
    return(stream(&rval, this));
  }
  stream operator >> (const serializable &rval) {
    return(stream(this, &rval));
  }

  virtual off_t expect(void) {
    return(stream::alloc);
  }

  virtual off_t offset(void) {
    return(0);
  }

  virtual off_t location(void) {
    return(0);
  }
  virtual bool seek(off_t pos) {
    return(false);
  }
  
  virtual off_t read(unsigned char *buffer, off_t bytes) = 0;
  virtual off_t write(unsigned char *buffer, off_t bytes) = 0;
  
  virtual my::string getline(void);

  operator my::string (void);
  operator my::vector<my::string> (void);
  
  virtual off_t write(my::string &line);
  virtual off_t write(my::vector<my::string> &lines);
  
  virtual off_t cached();

  virtual void path(my::string rpath);

public:
  my::string search_path;
protected:
  my::shared_ptr<buffer> m_cache;
};


//template class __MYLO_DLL_EXPORT my::shared_ptr<buffer>;

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
