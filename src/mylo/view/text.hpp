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

#ifndef __TEXT_HPP
#define __TEXT_HPP

__VIEW_NAMESPACE_BEGIN

class text {
public:
  text() {
    m_ready = false;
  }

  ~text() {
    int i = 0;
  }

  void font(const my::string &ttf) {
    m_font << platform::asset::api->retrieve(ttf);
    platform::graphics::api->compile(m_font);

    m_shader.vertex << platform::asset::api->retrieve("shaders/texture_shader.vert");
    m_shader.fragment << platform::asset::api->retrieve("shaders/texture_shader.frag");
    platform::graphics::api->compile(m_shader);

    m_ready = true;
  }

  void draw(my::program &shader, my::spatial::matrix &pespective) {
    if(m_ready == false) {
      return;
    }
    my::spatial::matrix text;
    text.identity();
    text.scale(0.003f);

    text.translate(-300.0f, 300.0f, 0.0f);

    my::list<my::string>::iterator it = buffer.begin();
    for (; it != buffer.end(); it++) {
      text.translate(0.0f, -30.0f, 0.0f);
      if (it->second.length()) {
        platform::graphics::api->draw(it->second, m_font, shader, text, my::spatial::matrix(), pespective);
      }
    }
  }

  void append(const my::string &value) {
    if (buffer.size() == 0) {
      newline();
    }
    buffer.tail().append(value);
  }

  void newline() {
    if (buffer.size() >= 10) {
      buffer.shift();
    }
    buffer.push_back(my::string());
  }

  text &operator << (int operand) {
    append(my::type_cast<my::string>(operand));
    return(*this);
  }

  text &operator << (float operand) {
    append(my::type_cast<my::string>(operand));
    return(*this);
  }

  text &operator << (const my::string &operand) {
    append(operand);
    return(*this);
  }

  text &operator << (const my::end_of_line &operand) {
    newline();
    return(*this);
  }

  void print(float x, float y, my::spatial::matrix mat) {
    if(m_ready == false) {
      return;
    }
    my::spatial::matrix placement;
    placement.identity();
    placement.scale(0.003f);

    placement.translate(x, y, 0.0f);

    for (int i = 0; i < 4; i++) {
      my::string text = my::format("%f, %f, %f, %f", mat.r[i][0], mat.r[i][1], mat.r[i][2], mat.r[i][3]);
      platform::graphics::api->draw(text, m_font, m_shader, placement, my::spatial::matrix(), my::spatial::matrix());
      placement.translate(0.0f, -30.0f, 0.0f);
    }
  }

  void print(float x, float y, my::spatial::vector vec) {
    if(m_ready == false) {
      return;
    }
    my::spatial::matrix placement;
    placement.identity();
    placement.scale(0.003f);

    placement.translate(x, y, 0.0f);
    my::string text = my::format("%f, %f, %f, %f", vec.x, vec.y, vec.z);
    platform::graphics::api->draw(text, m_font, m_shader, placement, my::spatial::matrix(), my::spatial::matrix());
  }

  void print(float x, float y, my::string text) {
    if(m_ready == false) {
      return;
    }
    my::spatial::matrix placement;
    placement.identity();
    placement.scale(0.003f);

    placement.translate(x, y, 0.0f);
    platform::graphics::api->draw(text, m_font, m_shader, placement, my::spatial::matrix(), my::spatial::matrix());
  }

private:
  my::list<my::string> buffer;
  my::fnt m_font;
  my::program m_shader;
  bool m_ready;
};

__VIEW_NAMESPACE_END

#endif
