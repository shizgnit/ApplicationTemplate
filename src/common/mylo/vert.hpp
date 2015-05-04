#include "mylo.hpp"

#ifndef __VERT_HPP
#define __VERT_HPP

__MYLO_NAMESPACE_BEGIN

class __MYLO_DLL_EXPORT vert: public shader {
public:
  vert() {
    callback_write("VERT", write);
  }

  static off_t write(shader *context, unsigned char *buffer, off_t bytes) {
    if(bytes) {
      context->buffer.write(buffer, bytes);
      return(bytes);
    }
    context->text = context->buffer;
    return(0);
  }

};

__MYLO_NAMESPACE_END

#endif
