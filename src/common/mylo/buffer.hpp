#include "mylo.hpp"

#ifndef __BUFFER_HPP
#define __BUFFER_HPP

__MYLO_NAMESPACE_BEGIN

class __MYLO_DLL_EXPORT buffer : public serializable {
public:
  static const size_t alloc = 65536;

  buffer() { reset(); }
  ~buffer() { reset(); }

  void reset();

  off_t location(void);
  bool seek(off_t pos);

  off_t read(unsigned char *buffer, off_t bytes);
  off_t write(unsigned char *buffer, off_t bytes);

  off_t size() {
    return(m_cache_used > m_cache_read ? m_cache_used - m_cache_read : 0);
  }

  unsigned char *raw() {
    return(&m_cache + m_cache_read);
  }

private:
  my::shared_ptr< unsigned char, allocation_array > m_cache;
  off_t m_cache_used;
  off_t m_cache_read;
  off_t m_cache_allocated;
};

//template class __MYLO_DLL_EXPORT my::shared_ptr< unsigned char, allocation_array >;

__MYLO_NAMESPACE_END

#endif