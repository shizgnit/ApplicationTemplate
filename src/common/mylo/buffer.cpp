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
 
  DEBUG_TRACE << "buffer size " << m_cache_used << " (" << size << ", " << m_cache_allocated << ")" << my::endl;

  return(size);
}

__MYLO_NAMESPACE_END

// Local Variables:
// mode:C++
// End:
