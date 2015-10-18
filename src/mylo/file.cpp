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

#include "file.hpp"

__MYLO_NAMESPACE_BEGIN


__MYLO_DLL_EXPORT bool file::open(my::string in, file::modes mode) {
  if(m_fp == NULL) {
    m_size = 0;
    m_file = in;
    if(mode & file::wo && !exists(m_file)) {
      FILE *init;
      if(init = fopen(m_file.c_str(), "wb+")) {
        fclose(init);
      }
    }
    if(mode & file::ro) {
      if(mode & file::wo) {
        m_fp = fopen(m_file.c_str(), "rb+");
      }
      if(!m_fp) {
        m_fp = fopen(m_file.c_str(), "rb");
      }
      if(m_fp) {
        fseeko(m_fp, 0, SEEK_END);
        m_size = location();
        fseeko(m_fp, 0, SEEK_SET);
      }
    }
    if(m_fp != NULL) {
#if defined __PLATFORM_WINDOWS
      m_fd = _fileno(m_fp);
#else
      m_fd = fileno(m_fp);
#endif
    }
  }
  return(m_fp ? true : false);
}

__MYLO_DLL_EXPORT void file::close(void) {
  if(m_fp != NULL) {
    fflush(m_fp);
    fclose(m_fp);
  }
  m_fp = NULL;
}

__MYLO_DLL_EXPORT off_t file::location(void) {
  return(ftello(m_fp));
}

__MYLO_DLL_EXPORT bool file::seek(off_t pos) {
  bool success = (fseeko(m_fp, pos, SEEK_SET) == 0);
  return(success);
}

__MYLO_DLL_EXPORT bool file::exists(my::string file) {
#if defined __PLATFORM_WINDOWS
  struct _stat sst;
  return(_stat(file.c_str(), &sst) == 0);
#else
  struct stat sst;
  return(stat(file.c_str(), &sst) == 0);
#endif
}

__MYLO_DLL_EXPORT off_t file::read(unsigned char *buffer, off_t bytes) {
  off_t request = bytes + location() > m_size ? m_size - location() : bytes; 
  return(feof(m_fp) ? 0 : fread(buffer, 1, request, m_fp));
}

__MYLO_DLL_EXPORT off_t file::write(unsigned char *buffer, off_t size) {
  return(fwrite(buffer, 1, size, m_fp));
}

__MYLO_NAMESPACE_END

// Local Variables:
// mode:C++
// End:
