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

  class vector {
  public:
    vector() {
      this->x = 0.0f;
      this->y = 0.0f;
      this->z = 0.0f;
      this->w = 1.0f;
    }

    vector(float v) {
      this->x = v;
      this->y = v;
      this->z = v;
      this->w = v;
    }

    vector(float v[]) {
      this->x = v[0];
      this->y = v[1];
      this->z = v[2];
      this->w = v[3];
    }

    vector(float x, float y, float z, float w=1.0f) {
      this->x = x;
      this->y = y;
      this->z = z;
      this->w = w;
    }

    vector(vector &ref) {
      this->x = ref.x;
      this->y = ref.y;
      this->z = ref.z;
      this->w = ref.w;
    }

    virtual vector &operator = (const vector &rval) {
      this->x = rval.x;
      this->y = rval.y;
      this->z = rval.z;
      this->w = rval.w;
      return(*this);
    }

    virtual vector &operator += (const vector &rval) {
      return (*this = (*this) * rval);
    }
    virtual vector operator + (const vector &rval) {
      vector result;
      result.x = x + rval.x;
      result.y = y + rval.y;
      result.z = z + rval.z;
      return result;
    }

    virtual vector &operator -= (const vector &rval) {
      return (*this = (*this) - rval);
    }
    virtual vector operator - (const vector &rval) const {
      vector result;
      result.x = x - rval.x;
      result.y = y - rval.y;
      result.z = z - rval.z;
      return result;
    }

    virtual vector &operator *= (const vector &rval) {
      return (*this = (*this) * rval);
    }
    virtual vector operator * (const vector &rval) const {
      vector result;
      result.x = x * rval.x;
      result.y = y * rval.y;
      result.z = z * rval.z;
      return result;
    }

    virtual vector &operator /= (const vector &rval) {
      return (*this = (*this) / rval);
    }
    virtual vector operator / (const vector &rval) const {
      vector result;
      result.x = x / rval.x;
      result.y = y / rval.y;
      result.z = z / rval.z;
      return result;
    }

    virtual vector &operator %= (const vector &rval) {
      return (*this = (*this) % rval);
    }
    virtual vector operator % (const vector &rval) const {
      vector result;
      result.x = (y * rval.z) - (z * rval.y);
      result.y = (z * rval.x) - (x * rval.z);
      result.z = (x * rval.y) - (y * rval.x);
      return result;
    }

    vector rotate_x(float rad) {
      vector result;
      result.x = (x * 1) + (y * 0) + (z * 0);
      result.y = (x * 0) + (y * cosf(rad)) + (z * sinf(rad));
      result.z = (x * 0) + (y * -sinf(rad)) + (z * cosf(rad));
      return result;
    }

    vector rotate_y(float rad) {
      vector result;
      result.x = (x * cosf(rad)) + (y * 0) + (z * sinf(rad));
      result.y = (x * 0) + (y * 1) + (z * 0);
      result.z = (x * -sinf(rad)) + (y * 0) + (z * cosf(rad));
      return result;
    }

    vector rotate_z(float rad) {
      vector result;
      result.x = (x * cosf(rad)) + (y * sinf(rad)) + (z * 0);
      result.y = (x * -sinf(rad)) + (y * cosf(rad)) + (z * 0);
      result.z = (x * 0) + (y * 0) + (z * 1);
      return result;
    }

    float dot() {
      return(x * x + y * y + z * z);
    }

    float length() {
      return((float)sqrt(dot()));
    }

    vector unit() {
      return(*this / length());
    }

  public:
    float x;
    float y;
    float z;
    float w;
  };

  class matrix {
  public:
    matrix() {
      memset(r, 0, sizeof(float) * 16);
    }
    matrix(const matrix &m) {
      memcpy(r, m.r, sizeof(float) * 16);
    }
    matrix(float d[]) {
      memcpy(r, d, sizeof(float) * 16);
    }

    matrix &identity() {
      memset(r, 0, sizeof(float) * 16);

      r[0][0] = 1.0f;
      r[1][1] = 1.0f;
      r[2][2] = 1.0f;
      r[3][3] = 1.0f;

      return *this;
    }

    virtual matrix &operator *= (const matrix &rval) {
      return (*this = (*this) * rval);
    }
    matrix operator * (const matrix &rval) const {
      matrix result = *this;

      result.r[0][0] = r[0][0] * rval.r[0][0] + r[1][0] * rval.r[0][1] + r[2][0] * rval.r[0][2] + r[3][0] * rval.r[0][3];
      result.r[0][1] = r[0][1] * rval.r[0][0] + r[1][1] * rval.r[0][1] + r[2][1] * rval.r[0][2] + r[3][1] * rval.r[0][3];
      result.r[0][2] = r[0][2] * rval.r[0][0] + r[1][2] * rval.r[0][1] + r[2][2] * rval.r[0][2] + r[3][2] * rval.r[0][3];
      result.r[0][3] = r[0][3] * rval.r[0][0] + r[1][3] * rval.r[0][1] + r[2][3] * rval.r[0][2] + r[3][3] * rval.r[0][3];
                                                                                                                
      result.r[1][0] = r[0][0] * rval.r[1][0] + r[1][0] * rval.r[1][1] + r[2][0] * rval.r[1][2] + r[3][0] * rval.r[1][3];
      result.r[1][1] = r[0][1] * rval.r[1][0] + r[1][1] * rval.r[1][1] + r[2][1] * rval.r[1][2] + r[3][1] * rval.r[1][3];
      result.r[1][2] = r[0][2] * rval.r[1][0] + r[1][2] * rval.r[1][1] + r[2][2] * rval.r[1][2] + r[3][2] * rval.r[1][3];
      result.r[1][3] = r[0][3] * rval.r[1][0] + r[1][3] * rval.r[1][1] + r[2][3] * rval.r[1][2] + r[3][3] * rval.r[1][3];
                                                                                                                
      result.r[2][0] = r[0][0] * rval.r[2][0] + r[1][0] * rval.r[2][1] + r[2][0] * rval.r[2][2] + r[3][0] * rval.r[2][3];
      result.r[2][1] = r[0][1] * rval.r[2][0] + r[1][1] * rval.r[2][1] + r[2][1] * rval.r[2][2] + r[3][1] * rval.r[2][3];
      result.r[2][2] = r[0][2] * rval.r[2][0] + r[1][2] * rval.r[2][1] + r[2][2] * rval.r[2][2] + r[3][2] * rval.r[2][3];
      result.r[2][3] = r[0][3] * rval.r[2][0] + r[1][3] * rval.r[2][1] + r[2][3] * rval.r[2][2] + r[3][3] * rval.r[2][3];
                                                                                                                
      result.r[3][0] = r[0][0] * rval.r[3][0] + r[1][0] * rval.r[3][1] + r[2][0] * rval.r[3][2] + r[3][0] * rval.r[3][3];
      result.r[3][1] = r[0][1] * rval.r[3][0] + r[1][1] * rval.r[3][1] + r[2][1] * rval.r[3][2] + r[3][1] * rval.r[3][3];
      result.r[3][2] = r[0][2] * rval.r[3][0] + r[1][2] * rval.r[3][1] + r[2][2] * rval.r[3][2] + r[3][2] * rval.r[3][3];

      //result.r[3][3] = r[0][3] * rval.r[3][0] + r[1][3] * rval.r[3][1] + r[2][3] * rval.r[3][2] + r[3][3] * rval.r[3][3];

      return result;
    }

    matrix &scale(const float &rval) {
      return(*this *= rval);
    }
    matrix &operator *= (const float &rval) {
      return (*this = (*this) * rval);
    }
    matrix operator * (const float &rval) const {
      matrix result = *this;

      result.r[0][0] *= rval;
      result.r[0][1] *= rval;
      result.r[0][2] *= rval;
      result.r[0][3] *= rval;
                     
      result.r[1][0] *= rval;
      result.r[1][1] *= rval;
      result.r[1][2] *= rval;
      result.r[1][3] *= rval;
                     
      result.r[2][0] *= rval;
      result.r[2][1] *= rval;
      result.r[2][2] *= rval;
      result.r[2][3] *= rval;
                     
      result.r[3][0] *= rval;
      result.r[3][1] *= rval;
      result.r[3][2] *= rval;
      //result.r[3][3] *= rval;

      return result;
    }

    matrix &rotate_x(float angle) {
      matrix rotation;

      rotation.r[0][0] = 1.0f;
      rotation.r[0][1] = 0.0f;
      rotation.r[0][2] = 0.0f;
      rotation.r[0][3] = 0.0f;

      rotation.r[1][0] = 0.0f;
      rotation.r[1][1] = cosf(angle);
      rotation.r[1][2] = sinf(angle);
      rotation.r[1][3] = 0.0f;

      rotation.r[2][0] = 0.0f;
      rotation.r[2][1] = -sinf(angle);
      rotation.r[2][2] = cosf(angle);
      rotation.r[2][3] = 0.0f;

      rotation.r[3][0] = 0.0f;
      rotation.r[3][1] = 0.0f;
      rotation.r[3][2] = 0.0f;
      rotation.r[3][3] = 1.0f;

      return(*this *= rotation);
    }
    matrix &rotate_y(float angle) {
      matrix rotation;

      rotation.r[0][0] = cosf(angle);
      rotation.r[0][1] = 0.0f;
      rotation.r[0][2] = -sinf(angle);
      rotation.r[0][3] = 0.0f;

      rotation.r[1][0] = 0.0f;
      rotation.r[1][1] = 1.0f;
      rotation.r[1][2] = 0.0f;
      rotation.r[1][3] = 0.0f;

      rotation.r[2][0] = sinf(angle);
      rotation.r[2][1] = 0.0f;
      rotation.r[2][2] = cosf(angle);
      rotation.r[2][3] = 0.0f;

      rotation.r[3][0] = 0.0f;
      rotation.r[3][1] = 0.0f;
      rotation.r[3][2] = 0.0f;
      rotation.r[3][3] = 1.0f;

      return(*this *= rotation);
    }
    matrix &rotate_z(float angle) {
      matrix rotation;

      rotation.r[0][0] = cosf(angle);
      rotation.r[0][1] = sinf(angle);
      rotation.r[0][2] = 0.0f;
      rotation.r[0][3] = 0.0f;

      rotation.r[1][0] = -sinf(angle);
      rotation.r[1][1] = cosf(angle);
      rotation.r[1][2] = 0.0f;
      rotation.r[1][3] = 0.0f;

      rotation.r[2][0] = 0.0f;
      rotation.r[2][1] = 0.0f;
      rotation.r[2][2] = 1.0f;
      rotation.r[2][3] = 0.0f;

      rotation.r[3][0] = 0.0f;
      rotation.r[3][1] = 0.0f;
      rotation.r[3][2] = 0.0f;
      rotation.r[3][3] = 1.0f;

      return(*this *= rotation);
    }

    matrix &translate(const vector &v) {
      return(this->translate(v.x, v.y, v.z, v.w));
    }
    matrix &translate(const float &x, const float &y, const float &z, const float &w = 1.0f) {
      matrix current = *this;

      r[3][0] = x * current.r[0][0] + y * current.r[1][0] + z * current.r[2][0] + w * current.r[3][0];
      r[3][1] = x * current.r[0][1] + y * current.r[1][1] + z * current.r[2][1] + w * current.r[3][1];
      r[3][2] = x * current.r[0][2] + y * current.r[1][2] + z * current.r[2][2] + w * current.r[3][2];
      //r[3][3] = x * current.r[0][3] + y * current.r[1][3] + z * current.r[2][3] + w * current.r[3][3];

      return *this;
    }

    matrix &perspective(float fov, float aspect, float n, float f) {
      float const a = 1.0f / (float)tan(fov / 2.0f);

      r[0][0] = a / aspect;
      r[0][1] = 0.0f;
      r[0][2] = 0.0f;
      r[0][3] = 0.0f;

      r[1][0] = 0.0f;
      r[1][1] = a;
      r[1][2] = 0.0f;
      r[1][3] = 0.0f;

      r[2][0] = 0.0f;
      r[2][1] = 0.0f;
      r[2][2] = -((f + n) / (f - n));
      r[2][3] = -1.0f;

      r[3][0] = 0.0f;
      r[3][1] = 0.0f;
      r[3][2] = -((2.0f * f * n) / (f - n));
      r[3][3] = 1.0f;

      return *this;
    }

    matrix &lookat(const vector &eye, const vector &center, const vector &up) {
      vector f = (center - eye).unit();
      vector s = (f % up).unit();
      vector t = s % f;

      r[0][0] = s.x;
      r[0][1] = t.x;
      r[0][2] = -f.x;
      r[0][3] = 0.0f;

      r[1][0] = s.y;
      r[1][1] = t.y;
      r[1][2] = -f.y;
      r[1][3] = 0.0f;

      r[2][0] = s.z;
      r[2][1] = t.z;
      r[2][2] = -f.z;
      r[2][3] = 0.0f;

      r[3][0] = 0.0f;
      r[3][1] = 0.0f;
      r[3][2] = 0.0f;
      r[3][3] = 1.0f;

      return this->translate(eye * -1.0f);
    }

  public:
    float r[4][4];
  };

  class position {
  public:
    position(void) {
      view = false;
      identity();
    }

    position(const position &ref) {
      view = ref.view;

      rotation = ref.rotation;

      eye = ref.eye;
      center = ref.center;
      up = ref.up;
    }

    virtual void viewable(bool toggle = true);

    virtual void identity(void);
    virtual void move(float t);
    virtual void strafe(float t);

    //virtual void place(float x, float y, float z);

    virtual void rotate(float x, float y, float z = 0.0f);

    virtual void project(const vector &offset, const vector &projection);

    virtual void set(void) {}

  public:
    bool view;

    vector rotation;

    vector eye;
    vector center;
    vector up;
  };

}

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
