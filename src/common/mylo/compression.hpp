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
