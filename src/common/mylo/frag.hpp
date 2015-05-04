#include "mylo.hpp"

#ifndef __FRAG_HPP
#define __FRAG_HPP

__MYLO_NAMESPACE_BEGIN

class __MYLO_DLL_EXPORT frag: public shader {
public:
  frag() {
    callback_write("FRAG", write);
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
