#include "mylo.hpp"

#ifndef __FRAG_HPP
#define __FRAG_HPP

__MYLO_NAMESPACE_BEGIN

class __MYLO_DLL_EXPORT frag: public shader {
public:
  frag() {
    callback_write("FSH", write);
  }

  static off_t write(shader *context, unsigned char *buffer, off_t bytes) {
    context->text.write(buffer, bytes);
    return(bytes);
  }

};

__MYLO_NAMESPACE_END

#endif
