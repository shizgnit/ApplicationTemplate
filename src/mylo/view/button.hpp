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

#ifndef __BUTTON_HPP
#define __BUTTON_HPP

__VIEW_NAMESPACE_BEGIN

class button : public view::widget {

public:
  button() {
  }

  void init(my::string source, int width, int height) {
    attr.geometry.width = width;
    attr.geometry.height = height;
    gbackground = my::primitive::quad((float)attr.geometry.width, (float)attr.geometry.height);
    my::image *img = new my::png;
    *img << platform::asset::api->retrieve(source);
    gbackground->xy_projection(img, 0, 0, attr.geometry.width, attr.geometry.height);
    platform::graphics::api->compile(*gbackground);
  }

  void draw(my::program &shader, my::spatial::matrix &pespective) {
    my::spatial::matrix local;
    local.identity();
    local.translate((float)attr.geometry.x, (float)attr.geometry.y, 0);
    platform::graphics::api->draw(*gbackground, shader, local, my::spatial::matrix(), pespective);
  }

  my::shared_ptr<my::object> gbackground;
};

__VIEW_NAMESPACE_END

#endif
