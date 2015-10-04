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

#include "platform/interfaces/graphics.hpp"

#ifndef __OPENGL_HPP
#define __OPENGL_HPP

#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define GLEW_STATIC

#ifndef _WIN32
#include <GLES2/gl2.h>
#include <android/log.h>
#else
#include <GL\glew.h>
#include <GL\gl.h>
#include <GL\glu.h>
#endif

#include "linmath.h"

__PLATFORM_NAMESPACE_BEGIN

namespace opengl {

  #define BUFFER_OFFSET(i) ((void*)(i))

  static inline float deg_to_radf(float deg) {
    return deg * (float)M_PI / 180.0f;
  }

  class graphics : public graphics_interface {
  public:
    void init(void);
    void clear(void);

    void compile(my::shader *shader, unsigned int type);
    void compile(my::program &program);
    void compile(my::object &object);
    void compile(my::objects &objects);
    void draw(my::object &object, mat4x4 matrix);
    void draw(my::objects &objects, mat4x4 matrix);
    void draw(my::string text, mat4x4 matrix);

    void set_font(my::string file);
    void set_program(my::string vert, my::string frag);
  };

}

__PLATFORM_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:

