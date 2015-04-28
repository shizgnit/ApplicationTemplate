#include "compression.hpp"

#if defined(__MYLO_ZLIB)
#  if defined(__MYLO_WINDOWS)
//#    define ZLIB_WINAPI
#  endif
#  include <zlib.h>
#endif

__MYLO_NAMESPACE_BEGIN

class abstract_zlib {
  public: z_stream stream;
};

__MYLO_DLL_EXPORT my::string compression::describe(int code) {
  switch(code) {
    case(0):  return("Z_OK");
    case(1):  return("Z_STREAM_END");
    case(2):  return("Z_NEED_DICT");
    case(-1): return("Z_ERRNO");
    case(-2): return("Z_STREAM_ERROR");
    case(-3): return("Z_DATA_ERROR");
    case(-4): return("Z_MEM_ERROR");
    case(-5): return("Z_BUF_ERROR");
    case(-6): return("Z_VERSION_ERROR");
  };
  return("Z_UNDEFINED_ERROR");
}

__MYLO_DLL_EXPORT compress::compress()  {
  m_zlib = new abstract_zlib();
  m_level = Z_DEFAULT_COMPRESSION;
  m_strategy = Z_DEFAULT_STRATEGY;
  m_init = Z_ERRNO;
}

__MYLO_DLL_EXPORT compress::~compress()  {
  delete(m_zlib);
}


__MYLO_DLL_EXPORT off_t compress::read(unsigned char *buffer, off_t bytes) {
  return(m_cache->read(buffer, bytes));
}

__MYLO_DLL_EXPORT off_t compress::write(unsigned char *buffer, off_t bytes) {
  if(m_init == Z_STREAM_END) {
    return(0); 
  }
  if(m_init != Z_OK) {
    m_zlib->stream.zalloc = Z_NULL;
    m_zlib->stream.zfree = Z_NULL;
    m_zlib->stream.opaque = Z_NULL;
    m_init = deflateInit2(&m_zlib->stream, m_level, Z_DEFLATED, 15+16, 9, m_strategy);
    if (m_init != Z_OK) {
      //ERROR(UNDEFINED_ERROR);
      return(0);
    }
  }
  
  int status, flush;
  size_t have;
  unsigned char out[stream::alloc];
  
  size_t allocation = 0;
  
  m_zlib->stream.avail_in = bytes;
  m_zlib->stream.next_in = buffer;
  
  flush = bytes == 0 || bytes < stream::alloc ? Z_FINISH : Z_NO_FLUSH;
  
  do {
    m_zlib->stream.avail_out = stream::alloc;
    m_zlib->stream.next_out = out;

    status = deflate(&m_zlib->stream, flush);
    
    have = stream::alloc - m_zlib->stream.avail_out;
    m_cache = new my::buffer();
    m_cache->write(out, have);
  } while (m_zlib->stream.avail_out == 0);
  
  if(flush == Z_FINISH) {
    deflateReset(&m_zlib->stream);
    deflateEnd(&m_zlib->stream);
    if(status != Z_OK && status != Z_STREAM_END) {
      //ERROR(UNDEFINED_ERROR);
      return(0);
    }
    m_init = Z_STREAM_END;
  }
  
  return(m_cache->size());
}

__MYLO_DLL_EXPORT decompress::decompress()  {
  m_zlib = new abstract_zlib();
  m_init = Z_ERRNO;
}

__MYLO_DLL_EXPORT decompress::~decompress()  {
  delete(m_zlib);
}


__MYLO_DLL_EXPORT off_t decompress::read(unsigned char *buffer, off_t bytes) {
  return(m_cache->read(buffer, bytes));
}

__MYLO_DLL_EXPORT off_t decompress::write(unsigned char *buffer, off_t bytes) {
  if(m_init == Z_STREAM_END) {
    return(0); 
  }
  if(m_init != Z_OK) {
    m_zlib->stream.zalloc = Z_NULL;
    m_zlib->stream.zfree = Z_NULL;
    m_zlib->stream.opaque = Z_NULL;
    m_init = inflateInit2(&m_zlib->stream, 15+32);
    if(m_init != Z_OK) {
      //ERROR(UNDEFINED_ERROR);
      return(0);
    }
  }
  
  int status, flush;
  size_t have;
  unsigned char out[stream::alloc];
  
  size_t allocation = 0;
  
  m_zlib->stream.avail_in = bytes;
  m_zlib->stream.next_in = buffer;
  
  flush = bytes == 0 || bytes < stream::alloc ? Z_FINISH : Z_NO_FLUSH;
  
  do {
    m_zlib->stream.avail_out = stream::alloc;
    m_zlib->stream.next_out = out;

    status = inflate(&m_zlib->stream, flush);
    
    have = stream::alloc - m_zlib->stream.avail_out;
    m_cache = new my::buffer();
    m_cache->write(out, have);
  } while (m_zlib->stream.avail_out == 0);
  
  if(flush == Z_FINISH) {
    inflateReset(&m_zlib->stream);
    inflateEnd(&m_zlib->stream);
    if(status != Z_OK && status != Z_STREAM_END) {
      //ERROR(UNDEFINED_ERROR);
    }
    m_init = Z_STREAM_END;
  }
  
  return(m_cache->size());
}

__MYLO_NAMESPACE_END

// Local Variables:
// mode:C++
// End:
