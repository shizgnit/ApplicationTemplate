/*
================================================================================

Copyright (c) 2017, Dee E. Abbott
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

#ifndef __WIDGET_HPP
#define __WIDGET_HPP

__VIEW_NAMESPACE_BEGIN

class widget {
public:
  widget() {
    memset(&prop, 0, sizeof(prop));
  }

  void SetPosition(int x, int y, int z=0) {
    prop.geometry.x = x;
    prop.geometry.y = y;
    prop.geometry.z = z;
  }

  void SetGeometry(int width, int height) {
    prop.geometry.width = width;
    prop.geometry.height = height;
  }

  virtual void on_draw() {};

  virtual void on_hover() {};

  virtual void on_click() {};
  virtual void on_click_down() {};
  virtual void on_click_up() {};

private:
  struct attributes {
    struct {
      int x;
      int y;
      int z;
      int width;
      int height;
    } geometry;
    bool click_through;
    bool visible;
  } prop;
};

__VIEW_NAMESPACE_END

#endif
