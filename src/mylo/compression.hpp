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

#ifndef __COMPRESSION_HPP
#define __COMPRESSION_HPP

__MYLO_NAMESPACE_BEGIN

class __MYLO_DLL_EXPORT compression {
public:
  static my::string describe(int code);
};

class abstract_zlib;

class __MYLO_DLL_EXPORT compress : public serializable {
public:
  enum {
    COMPRESS_ZLIB                  = 0x10,
    COMPRESS_ZLIB_DEFAULT          = 0x11,
    COMPRESS_ZLIB_BEST_COMPRESSION = 0x12,
    COMPRESS_ZLIB_OPTIMIZED        = 0x14,
    COMPRESS_ZLIB_RLE              = 0x18,
  };

  compress();
  ~compress();

  off_t read(unsigned char *buffer, off_t bytes);
  off_t write(unsigned char *buffer, off_t bytes);
  
protected:
  abstract_zlib *m_zlib;
  int m_level;
  int m_strategy;
  int m_init;
};

class __MYLO_DLL_EXPORT decompress : public serializable {
public:
  decompress();
  ~decompress();

  off_t read(unsigned char *buffer, off_t bytes);
  off_t write(unsigned char *buffer, off_t bytes);
  
protected:
  abstract_zlib *m_zlib;
  int m_init;
};

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
