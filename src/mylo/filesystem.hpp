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

#ifndef __FILESYSTEM_HPP
#define __FILESYSTEM_HPP

__MYLO_NAMESPACE_BEGIN

__MYLO_DLL_EXPORT bool rm(my::string filename);
__MYLO_DLL_EXPORT bool mv(my::string src, my::string dest);
__MYLO_DLL_EXPORT bool cp(my::string src, my::string dest);

__MYLO_DLL_EXPORT bool mkdir(my::string path, my::vector<my::string> mask);
__MYLO_DLL_EXPORT bool rmdir(my::string path);

__MYLO_DLL_EXPORT my::string pwd(my::string path="");

__MYLO_DLL_EXPORT my::vector<unsigned long> stat(my::string path);
__MYLO_DLL_EXPORT my::vector<unsigned long> lstat(my::string path);

__MYLO_DLL_EXPORT bool exists(my::string path);

__MYLO_DLL_EXPORT my::string filetype(my::string path);

class directory_handle;

class __MYLO_DLL_EXPORT directory {
public:
  directory(my::string path);
  ~directory(void);

  bool open(my::string path);
  bool close();

  my::string next();

  operator my::vector<my::string> (void);

private:
  my::string m_path;

  directory_handle *m_dha;
};


class __MYLO_DLL_EXPORT file : public my::serializable {
public:
  enum modes {
    ro = 0x01,
    wo = 0x02,
    rw = 0x03,
  };

  file(void) : m_fp(NULL) { /* NULL */ }
  file(my::string in) : m_fp(NULL) {
    open(in);
  }

  ~file(void) {
    close();
  }
  
  bool open() { 
    return(open(m_file)); 
  }
  bool open(my::string in, modes mode = rw);
  
  void close(void);

  off_t location(void);
  bool seek(off_t pos);
  
  off_t read(unsigned char *buffer, off_t bytes);
  off_t write(unsigned char *buffer, off_t bytes);
  
  static bool exists(my::string file);
  
protected:
  my::string m_file;
  
  off_t m_size;
  
  FILE *m_fp;
  int m_fd;
};

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
