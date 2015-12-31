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

#ifndef __INPUT_INTERFACE_HPP
#define __INPUT_INTERFACE_HPP

__PLATFORM_NAMESPACE_BEGIN

class input_interface {
public:
  input_interface() {
    reset();
  }

  void reset() {
    mouse_x = 0;
    mouse_y = 0;
    memset(keys, 0, 256);
  }

  virtual void touch_press(float x, float y) = 0;
  virtual void touch_release(float x, float y) = 0;
  virtual void touch_drag(float x, float y) = 0;
  virtual void touch_scale(float x, float y, float z) = 0;
  virtual void touch_zoom_in() = 0;
  virtual void touch_zoom_out() = 0;

  virtual void key_down(int key) = 0;
  virtual void key_up(int key) = 0;

  virtual void mouse_move(long int x, long int y) = 0;

public:
  long int mouse_x;
  long int mouse_y;
  char keys[256];

  struct def_struct {
    char *label;
    int value;
    char *description;
  };

  static def_struct def[256];
};

namespace generic {

  class input : public input_interface {
  public:
    void touch_press(float x, float y) {
      on_touch_press(x, y);
    }
    void touch_release(float x, float y) {
      on_touch_release(x, y);
    }
    void touch_drag(float x, float y) {
      on_touch_drag(x, y);
    }
    void touch_scale(float x, float y, float z) {
      on_touch_scale(x, y, z);
    }
    void touch_zoom_in() {
      on_touch_zoom_in();
    }
    void touch_zoom_out() {
      on_touch_zoom_out();
    }

    void key_down(int key) {
      keys[key] = 1;
      on_key_down(key);
    }
    void key_up(int key) {
      keys[key] = 0;
      on_key_up(key);
    }

    void mouse_move(long int x, long int y) {
      mouse_x = x;
      mouse_y = y;
      on_mouse_move(x, y);
    }
  };

}

__PLATFORM_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:

