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

#include "buffer.hpp"

__MYLO_NAMESPACE_BEGIN

__MYLO_DLL_EXPORT void buffer::reset() {
  m_cache = new unsigned char [buffer::alloc+1];
  m_cache_used = 0;
  m_cache_allocated = stream::alloc;
  m_cache_read = 0;
  *(&m_cache) = 0;
}

__MYLO_DLL_EXPORT off_t buffer::location(void) {
  return(m_cache_read);
}

__MYLO_DLL_EXPORT bool buffer::seek(off_t pos) {
  if(pos > m_cache_used) {
    return(false);
  }
  m_cache_read = pos;
  return(true);
}

__MYLO_DLL_EXPORT off_t buffer::read(unsigned char *data, off_t size) {
  off_t read = size > my::stream::alloc || size == 0 ? my::stream::alloc : size;
  if (read > m_cache_used) {
    read = m_cache_used;
  }

  memcpy(data, &m_cache + m_cache_read, read);
  m_cache_used -= read;
  m_cache_read += read;
  return(read);
}

__MYLO_DLL_EXPORT off_t buffer::write(unsigned char *data, off_t size) {
  if(size == 0) {
    return(0);
  }

  off_t cache_allocation = m_cache_allocated;
  while(m_cache_used + size > cache_allocation) {
    cache_allocation += buffer::alloc;
  }
  if(cache_allocation != m_cache_allocated) {
    unsigned char *reallocation = new unsigned char [cache_allocation+1];
    memcpy(reallocation, m_cache, m_cache_used);
    m_cache = reallocation;
    m_cache_allocated = cache_allocation;
  }

  memcpy(&m_cache + m_cache_used, data, size);
  m_cache_used += size;

  *(&m_cache + m_cache_used) = 0; 
 
  //DEBUG_TRACE << "buffer size " << m_cache_used << " (" << size << ", " << m_cache_allocated << ")" << my::endl;

  return(size);
}

__MYLO_NAMESPACE_END

// Local Variables:
// mode:C++
// End:
