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

#ifndef __SPATIAL_HPP__
#define __SPATIAL_HPP__

__MYLO_NAMESPACE_BEGIN

namespace spatial {

  class matrix;

  class vector {
  public:
    vector();
    vector(float v);
    vector(float v[]);
    vector(float x, float y, float z, float w = 1.0f);

    vector(const vector &ref);
    virtual vector &operator = (const vector &operand);

    virtual vector &operator += (const vector &operand);
    virtual vector operator + (const vector &operand);

    virtual vector &operator -= (const vector &operand);
    virtual vector operator - (const vector &operand) const;

    virtual vector &operator *= (const vector &operand);
    virtual vector operator * (const vector &operand) const;

    virtual vector &operator /= (const vector &operand);
    virtual vector operator / (const vector &operand) const;

    virtual vector &operator %= (const vector &operand);
    virtual vector operator % (const vector &operand) const;

    vector &rotate_x(float rad);

    vector rotate_y(float rad);

    vector rotate_z(float rad);

    float dot();

    float length();
    vector unit();

    void unproject(vector mouse, const matrix &model, const matrix &projection, int width, int height);

  public:
    float x;
    float y;
    float z;
    float w;
  };

  class matrix {
  public:
    matrix();
    matrix(const matrix &m);
    matrix(float d[]);

    matrix &identity();

    virtual matrix &operator *= (const matrix &operand);
    matrix operator * (const matrix &operand) const;
    matrix &scale(const float &operand);
    matrix &operator *= (const float &operand);
    matrix operator * (const float &operand) const;

    matrix &rotate_x(float angle);
    matrix &rotate_y(float angle);
    matrix &rotate_z(float angle);

    matrix &translate(const vector &v);
    matrix &translate(const float &x, const float &y, const float &z, const float &w = 1.0f);
    matrix &translate(const vector &eye, const vector &center, const vector &up);

    matrix &perspective(float fov, float aspect, float n, float f);
    matrix &ortho(float left, float right, float bottom, float top);

    matrix &lookat(const vector &eye, const vector &center, const vector &up);

    matrix &invert();

  public:
    float r[4][4];
  };

  class position {
  public:
    position(void);
    position(const position &ref);

    operator matrix();

    void identity(void);

    void viewable(bool toggle);

    void move(float t);
    void vertical(float t);

    void strafe(float t);

    void rotate(float x, float y, float z = 0.0f);

    void project(const vector &offset, const vector &projection);

  public:
    bool view;

    vector rotation;

    vector eye;
    vector center;
    vector up;
  };

  class quaternion : public vector {
  public:

    quaternion();
    quaternion(const matrix &m);

    ~quaternion();

    operator matrix();
    void euler(const float &x, const float &y, const float &z, const float &degrees);

    quaternion operator *(const quaternion &operand);
  };
}

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
