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
    memset(keys, 0, 20);
  }

  virtual void touch_press(float x, float y) = 0;
  virtual void touch_release(float x, float y) = 0;
  virtual void touch_drag(float x, float y) = 0;
  virtual void touch_scale(float x, float y, float z) = 0;
  virtual void touch_zoom_in() = 0;
  virtual void touch_zoom_out() = 0;

  virtual void key_down(int key) = 0;
  virtual void key_up(int key) = 0;

  virtual void mouse_down(int button, long int x, long int y) = 0;
  virtual void mouse_up(int button, long int x, long int y) = 0;

  virtual void mouse_move(long int x, long int y) = 0;

public:
  long int mouse_x;
  long int mouse_y;
  char buttons[20];
  char keys[256];

  struct metadata {
    char *label;
    int value;
    char *description;
  };
  static metadata metadata_keys[256];
  static metadata metadata_buttons[20];
};

namespace generic {

  class input : public input_interface {
  public:
    virtual void touch_press(float x, float y) {
      platform::controller::api->on_touch_press(x, y);
    }
    virtual void touch_release(float x, float y) {
      platform::controller::api->on_touch_release(x, y);
    }
    virtual void touch_drag(float x, float y) {
      platform::controller::api->on_touch_drag(x, y);
    }
    virtual void touch_scale(float x, float y, float z) {
      platform::controller::api->on_touch_scale(x, y, z);
    }
    virtual void touch_zoom_in() {
      platform::controller::api->on_touch_zoom_in();
    }
    virtual void touch_zoom_out() {
      platform::controller::api->on_touch_zoom_out();
    }

    virtual void key_down(int key) {
      keys[key] = 1;
      platform::controller::api->on_key_down(key);
    }
    virtual void key_up(int key) {
      keys[key] = 0;
      platform::controller::api->on_key_up(key);
    }

    virtual void mouse_down(int button, long int x, long int y) {
      mouse_x = x;
      mouse_y = y;
      buttons[button] = 1;
      platform::controller::api->on_mouse_down(button, x, y);
    }
    virtual void mouse_up(int button, long int x, long int y) {
      mouse_x = x;
      mouse_y = y;
      buttons[button] = 0;
      platform::controller::api->on_mouse_up(button, x, y);
    }

    virtual void mouse_move(long int x, long int y) {
      mouse_x = x;
      mouse_y = y;
      platform::controller::api->on_mouse_move(x, y);
    }
  };

}

class input {
public: static input_interface *api;
};

__PLATFORM_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:

