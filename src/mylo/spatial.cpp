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

#include "spatial.hpp"

__MYLO_NAMESPACE_BEGIN

namespace spatial {

  vector::vector() {
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
    this->w = 1.0f;
  }

  vector::vector(float v) {
    this->x = v;
    this->y = v;
    this->z = v;
    this->w = v;
  }

  vector::vector(float v[]) {
    this->x = v[0];
    this->y = v[1];
    this->z = v[2];
    this->w = v[3];
  }

  vector::vector(float x, float y, float z, float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  }

  vector::vector(const vector &ref) {
    this->x = ref.x;
    this->y = ref.y;
    this->z = ref.z;
    this->w = ref.w;
  }

  vector &vector::operator = (const vector &operand) {
    this->x = operand.x;
    this->y = operand.y;
    this->z = operand.z;
    this->w = operand.w;
    return(*this);
  }

  vector &vector::operator += (const vector &operand) {
    return (*this = (*this) + operand);
  }
  vector vector::operator + (const vector &operand) {
    vector result;
    result.x = x + operand.x;
    result.y = y + operand.y;
    result.z = z + operand.z;
    return result;
  }

  vector &vector::operator -= (const vector &operand) {
    return (*this = (*this) - operand);
  }
  vector vector::operator - (const vector &operand) const {
    vector result;
    result.x = x - operand.x;
    result.y = y - operand.y;
    result.z = z - operand.z;
    return result;
  }

  vector &vector::operator *= (const vector &operand) {
    return (*this = (*this) * operand);
  }
  vector vector::operator * (const vector &operand) const {
    vector result;
    result.x = x * operand.x;
    result.y = y * operand.y;
    result.z = z * operand.z;
    return result;
  }

  vector &vector::operator /= (const vector &operand) {
    return (*this = (*this) / operand);
  }
  vector vector::operator / (const vector &operand) const {
    vector result;
    result.x = x / (operand.x ? operand.x : 1.0f);
    result.y = y / (operand.y ? operand.y : 1.0f);
    result.z = z / (operand.z ? operand.z : 1.0f);
    return result;
  }

  vector &vector::operator %= (const vector &operand) {
    return (*this = (*this) % operand);
  }
  vector vector::operator % (const vector &operand) const {
    vector result;
    result.x = (y * operand.z) - (z * operand.y);
    result.y = (z * operand.x) - (x * operand.z);
    result.z = (x * operand.y) - (y * operand.x);
    return result;
  }

  vector &vector::rotate_x(float rad) {
    vector result;
    result.x = (x * 1) + (y * 0) + (z * 0);
    result.y = (x * 0) + (y * cosf(rad)) + (z * sinf(rad));
    result.z = (x * 0) + (y * -sinf(rad)) + (z * cosf(rad));
    return (*this = result);
  }

  vector vector::rotate_y(float rad) {
    vector result;
    result.x = (x * cosf(rad)) + (y * 0) + (z * sinf(rad));
    result.y = (x * 0) + (y * 1) + (z * 0);
    result.z = (x * -sinf(rad)) + (y * 0) + (z * cosf(rad));
    return (*this = result);
  }

  vector vector::rotate_z(float rad) {
    vector result;
    result.x = (x * cosf(rad)) + (y * sinf(rad)) + (z * 0);
    result.y = (x * -sinf(rad)) + (y * cosf(rad)) + (z * 0);
    result.z = (x * 0) + (y * 0) + (z * 1);
    return (*this = result);
  }

  float vector::dot() {
    return(x * x + y * y + z * z);
  }

  float vector::length() {
    return((float)sqrt(dot()));
  }

  vector vector::unit() {
    return(*this / (vector)length());
  }

  void vector::unproject(vector mouse, const matrix &model, const matrix &projection, int width, int height) {
    matrix inverted = model * projection;
    inverted.invert();

    mouse.x /= width;
    mouse.y /= height;

    mouse.x = (mouse.x / width) * 2 - 1;
    mouse.y = (mouse.y / height) * 2 - 1;
    mouse.z = mouse.z * 2 - 1;

    // don't have a matrix vector multiply yet
    //*this = inverted * mouse;

    mouse /= mouse.z;
  }

  matrix::matrix() {
    identity();
  }
  matrix::matrix(const matrix &m) {
    memcpy(r, m.r, sizeof(float) * 16);
  }
  matrix::matrix(float d[]) {
    memcpy(r, d, sizeof(float) * 16);
  }

  matrix &matrix::identity() {
    memset(r, 0, sizeof(float) * 16);

    r[0][0] = 1.0f;
    r[1][1] = 1.0f;
    r[2][2] = 1.0f;
    r[3][3] = 1.0f;

    return *this;
  }

  matrix &matrix::operator *= (const matrix &operand) {
    return (*this = (*this) * operand);
  }
  matrix matrix::operator * (const matrix &operand) const {
    //matrix result = *this;

    /*
    result.r[0][0] = r[0][0] * operand.r[0][0] + r[1][0] * operand.r[0][1] + r[2][0] * operand.r[0][2] + r[3][0] * operand.r[0][3];
    result.r[0][1] = r[0][1] * operand.r[0][0] + r[1][1] * operand.r[0][1] + r[2][1] * operand.r[0][2] + r[3][1] * operand.r[0][3];
    result.r[0][2] = r[0][2] * operand.r[0][0] + r[1][2] * operand.r[0][1] + r[2][2] * operand.r[0][2] + r[3][2] * operand.r[0][3];
    result.r[0][3] = r[0][3] * operand.r[0][0] + r[1][3] * operand.r[0][1] + r[2][3] * operand.r[0][2] + r[3][3] * operand.r[0][3];

    result.r[1][0] = r[0][0] * operand.r[1][0] + r[1][0] * operand.r[1][1] + r[2][0] * operand.r[1][2] + r[3][0] * operand.r[1][3];
    result.r[1][1] = r[0][1] * operand.r[1][0] + r[1][1] * operand.r[1][1] + r[2][1] * operand.r[1][2] + r[3][1] * operand.r[1][3];
    result.r[1][2] = r[0][2] * operand.r[1][0] + r[1][2] * operand.r[1][1] + r[2][2] * operand.r[1][2] + r[3][2] * operand.r[1][3];
    result.r[1][3] = r[0][3] * operand.r[1][0] + r[1][3] * operand.r[1][1] + r[2][3] * operand.r[1][2] + r[3][3] * operand.r[1][3];

    result.r[2][0] = r[0][0] * operand.r[2][0] + r[1][0] * operand.r[2][1] + r[2][0] * operand.r[2][2] + r[3][0] * operand.r[2][3];
    result.r[2][1] = r[0][1] * operand.r[2][0] + r[1][1] * operand.r[2][1] + r[2][1] * operand.r[2][2] + r[3][1] * operand.r[2][3];
    result.r[2][2] = r[0][2] * operand.r[2][0] + r[1][2] * operand.r[2][1] + r[2][2] * operand.r[2][2] + r[3][2] * operand.r[2][3];
    result.r[2][3] = r[0][3] * operand.r[2][0] + r[1][3] * operand.r[2][1] + r[2][3] * operand.r[2][2] + r[3][3] * operand.r[2][3];

    result.r[3][0] = r[0][0] * operand.r[3][0] + r[1][0] * operand.r[3][1] + r[2][0] * operand.r[3][2] + r[3][0] * operand.r[3][3];
    result.r[3][1] = r[0][1] * operand.r[3][0] + r[1][1] * operand.r[3][1] + r[2][1] * operand.r[3][2] + r[3][1] * operand.r[3][3];
    result.r[3][2] = r[0][2] * operand.r[3][0] + r[1][2] * operand.r[3][1] + r[2][2] * operand.r[3][2] + r[3][2] * operand.r[3][3];

    //result.r[3][3] = r[0][3] * operand.r[3][0] + r[1][3] * operand.r[3][1] + r[2][3] * operand.r[3][2] + r[3][3] * operand.r[3][3];

    */

    matrix result;
    int k, r, c;
    for (c = 0; c<4; ++c) for (r = 0; r<4; ++r) {
      result.r[c][r] = 0.f;
      for (k = 0; k<4; ++k)
        result.r[c][r] += this->r[k][r] * operand.r[c][k];
    }

    return result;
  }

  matrix &matrix::scale(const float &operand) {
    return(*this *= operand);
  }
  matrix &matrix::operator *= (const float &operand) {
    return (*this = (*this) * operand);
  }
  matrix matrix::operator * (const float &operand) const {
    matrix result = *this;

    result.r[0][0] *= operand;
    result.r[0][1] *= operand;
    result.r[0][2] *= operand;
    result.r[0][3] *= operand;

    result.r[1][0] *= operand;
    result.r[1][1] *= operand;
    result.r[1][2] *= operand;
    result.r[1][3] *= operand;

    result.r[2][0] *= operand;
    result.r[2][1] *= operand;
    result.r[2][2] *= operand;
    result.r[2][3] *= operand;

    result.r[3][0] *= operand;
    result.r[3][1] *= operand;
    result.r[3][2] *= operand;
    //result.r[3][3] *= operand;

    return result;
  }

  matrix &matrix::rotate_x(float angle) {
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
  matrix &matrix::rotate_y(float angle) {
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
  matrix &matrix::rotate_z(float angle) {
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

  matrix &matrix::translate(const vector &v) {
    return(this->translate(v.x, v.y, v.z, v.w));
  }
  matrix &matrix::translate(const float &x, const float &y, const float &z, const float &w) {
    matrix current = *this;

    r[3][0] = x * current.r[0][0] + y * current.r[1][0] + z * current.r[2][0] + w * current.r[3][0];
    r[3][1] = x * current.r[0][1] + y * current.r[1][1] + z * current.r[2][1] + w * current.r[3][1];
    r[3][2] = x * current.r[0][2] + y * current.r[1][2] + z * current.r[2][2] + w * current.r[3][2];

    //r[3][3] = x * current.r[0][3] + y * current.r[1][3] + z * current.r[2][3] + w * current.r[3][3];

    return *this;
  }

  matrix &matrix::perspective(float fov, float aspect, float n, float f) {
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

  matrix &matrix::ortho(float left, float right, float bottom, float top) {
    r[0][0] = 2.0f / (right - left);
    r[1][1] = 2.f / (top - bottom);
    r[2][2] = -1.0f;
    r[3][0] = -(right + left) / (right - left);
    r[3][1] = -(top + bottom) / (top - bottom);
    r[3][3] = 1.0f;
    return *this;
  }

  matrix &matrix::lookat(const vector &eye, const vector &center, const vector &up) {
    vector f = (center - eye).unit();
    vector s = (f % up).unit();
    vector t = s % f;

    r[0][0] = s.x;
    r[1][0] = s.y;
    r[2][0] = s.z;

    r[0][1] = t.x;
    r[1][1] = t.y;
    r[2][1] = t.z;

    r[0][2] = -f.x;
    r[1][2] = -f.y;
    r[2][2] = -f.z;

    return this->translate(eye * -1.0f);
  }

  matrix &matrix::translate(const vector &eye, const vector &center, const vector &up) {
    
    vector f = (center - eye).unit();
    vector s = (f % up).unit();
    vector t = s % f;

    r[0][0] = s.x;
    r[1][0] = s.y;
    r[2][0] = s.z;

    r[0][1] = t.x;
    r[1][1] = t.y;
    r[2][1] = t.z;

    r[0][2] = -f.x;
    r[1][2] = -f.y;
    r[2][2] = -f.z;

    return this->translate(center);
  }

  matrix &matrix::invert() {
    float s[6];
    float c[6];

    s[0] = r[0][0] * r[1][1] - r[1][0] * r[0][1];
    s[1] = r[0][0] * r[1][2] - r[1][0] * r[0][2];
    s[2] = r[0][0] * r[1][3] - r[1][0] * r[0][3];
    s[3] = r[0][1] * r[1][2] - r[1][1] * r[0][2];
    s[4] = r[0][1] * r[1][3] - r[1][1] * r[0][3];
    s[5] = r[0][2] * r[1][3] - r[1][2] * r[0][3];

    c[0] = r[2][0] * r[3][1] - r[3][0] * r[2][1];
    c[1] = r[2][0] * r[3][2] - r[3][0] * r[2][2];
    c[2] = r[2][0] * r[3][3] - r[3][0] * r[2][3];
    c[3] = r[2][1] * r[3][2] - r[3][1] * r[2][2];
    c[4] = r[2][1] * r[3][3] - r[3][1] * r[2][3];
    c[5] = r[2][2] * r[3][3] - r[3][2] * r[2][3];

    /* Assumes it is invertible */
    float idet = 1.0f / (s[0] * c[5] - s[1] * c[4] + s[2] * c[3] + s[3] * c[2] - s[4] * c[1] + s[5] * c[0]);

    matrix result;

    result.r[0][0] = (r[1][1] * c[5] - r[1][2] * c[4] + r[1][3] * c[3]) * idet;
    result.r[0][1] = (-r[0][1] * c[5] + r[0][2] * c[4] - r[0][3] * c[3]) * idet;
    result.r[0][2] = (r[3][1] * s[5] - r[3][2] * s[4] + r[3][3] * s[3]) * idet;
    result.r[0][3] = (-r[2][1] * s[5] + r[2][2] * s[4] - r[2][3] * s[3]) * idet;
          
    result.r[1][0] = (-r[1][0] * c[5] + r[1][2] * c[2] - r[1][3] * c[1]) * idet;
    result.r[1][1] = (r[0][0] * c[5] - r[0][2] * c[2] + r[0][3] * c[1]) * idet;
    result.r[1][2] = (-r[3][0] * s[5] + r[3][2] * s[2] - r[3][3] * s[1]) * idet;
    result.r[1][3] = (r[2][0] * s[5] - r[2][2] * s[2] + r[2][3] * s[1]) * idet;
          
    result.r[2][0] = (r[1][0] * c[4] - r[1][1] * c[2] + r[1][3] * c[0]) * idet;
    result.r[2][1] = (-r[0][0] * c[4] + r[0][1] * c[2] - r[0][3] * c[0]) * idet;
    result.r[2][2] = (r[3][0] * s[4] - r[3][1] * s[2] + r[3][3] * s[0]) * idet;
    result.r[2][3] = (-r[2][0] * s[4] + r[2][1] * s[2] - r[2][3] * s[0]) * idet;
          
    result.r[3][0] = (-r[1][0] * c[3] + r[1][1] * c[1] - r[1][2] * c[0]) * idet;
    result.r[3][1] = (r[0][0] * c[3] - r[0][1] * c[1] + r[0][2] * c[0]) * idet;
    result.r[3][2] = (-r[3][0] * s[3] + r[3][1] * s[1] - r[3][2] * s[0]) * idet;
    result.r[3][3] = (r[2][0] * s[3] - r[2][1] * s[1] + r[2][2] * s[0]) * idet;

    return *this = result;
  }


  position::position(void) {
    view = false;
    identity();
  }

  position::position(const position &ref) {
    view = ref.view;

    rotation = ref.rotation;

    eye = ref.eye;
    center = ref.center;
    up = ref.up;
  }

  position::operator matrix() {
    matrix result;
    result.translate(eye, center, up);
    return(result);
  }

  void position::identity(void) {
    eye.x = 0.0f;
    eye.y = 0.0f;
    eye.z = 1.0f;

    center.x = 0.0f;
    center.y = 0.0f;
    center.z = 0.0f;

    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
  }

  void position::viewable(bool toggle) {
    view = toggle;
  }

  void position::move(float t) {
    vector diff = (eye - center).unit() * t;

    center += diff;
    eye += diff;
  }

  void position::vertical(float t) {
    vector diff = up * t;

    center += diff;
    eye += diff;
  }

  void position::strafe(float t) {
    vector normal = eye - center;
    vector cross = (normal % up) + center;
    vector diff = (cross - center) * t;

    center += diff;
    eye += diff;
  }

  void position::rotate(float x, float y, float z) {
    rotation.x += x;
    rotation.y += y;
    rotation.z += z;

    vector offset = center;

    identity();

    float radx = static_cast<float>(rotation.x * (3.1415927 / 180));
    eye.rotate_x(radx);
    up.rotate_x(radx);

    float rady = static_cast<float>(rotation.y * (3.1415927 / 180));
    eye.rotate_y(rady);
    up.rotate_y(rady);

    eye += offset;
    center += offset;
  }

  void position::project(const vector &offset, const vector &projection) {
    vector normal = eye - center;

    vector cross = (normal % up) + center;

    vector diff = (cross - center) * offset;

    diff.x = (cross.x - center.x) * offset.x;
    diff.y = (cross.y - center.y) * offset.y;
    diff.z = (cross.z - center.z) * offset.z;
  }

  quaternion::quaternion() {

  }

  quaternion::quaternion(const matrix &m) {
    float tr, s, q[4];
    int i, j, k;
    int nxt[3] = { 1, 2, 0 };
    tr = m.r[0][0] + m.r[1][1] + m.r[2][2];
    // check the diagonal
    if (tr > 0.0) {
      s = sqrt(tr + 1.0);
      w = s / 2.0;
      s = 0.5 / s;
      x = (m.r[1][2] - m.r[2][1]) * s;
      y = (m.r[2][0] - m.r[0][2]) * s;
      z = (m.r[0][1] - m.r[1][0]) * s;
    }
    else {
      // diagonal is negative
      i = 0;
      if (m.r[1][1] > m.r[0][0]) i = 1;
      if (m.r[2][2] > m.r[i][i]) i = 2;
      j = nxt[i];
      k = nxt[j];
      s = sqrt((m.r[i][i] - (m.r[j][j] + m.r[k][k])) + 1.0);
      q[i] = s * 0.5;
      if (s != 0.0) s = 0.5 / s;
      q[3] = (m.r[j][k] - m.r[k][j]) * s;
      q[j] = (m.r[i][j] + m.r[j][i]) * s;
      q[k] = (m.r[i][k] + m.r[k][i]) * s;
      x = q[0];
      y = q[1];
      z = q[2];
      w = q[3];
    }
  }

  quaternion::~quaternion() {

  }

  quaternion quaternion::operator *(const quaternion &operand){
    quaternion result;

    result.w = w*operand.w - x*operand.x - y*operand.y - z*operand.z;
    result.x = w*operand.x + x*operand.w + y*operand.z - z*operand.y;
    result.y = w*operand.y + y*operand.w + z*operand.x - x*operand.z;
    result.z = w*operand.z + z*operand.w + x*operand.y - y*operand.x;

    return(result);
  }
/*
  void quaternion::euler(const float &x, const float &y, const float &z, const float &degrees) {
    float angle = float((degrees / 180.0f) * M_PI);
    float result = float(sin(angle / 2.0f));
    this->w = float(cos(angle / 2.0f));

    // Calculate the x, y and z of the quaternion
    this->x = float(x * result);
    this->y = float(y * result);
    this->z = float(z * result);
  }
  */
  void quaternion::euler(const float &roll, const float &pitch, const float &yaw, const float &degrees)
  {
    float cr, cp, cy, sr, sp, sy, cpcy, spsy;
    // calculate trig identities
    cr = cos(roll / 2);
    cp = cos(pitch / 2);
    cy = cos(yaw / 2);
    sr = sin(roll / 2);
    sp = sin(pitch / 2);
    sy = sin(yaw / 2);
    cpcy = cp * cy;
    spsy = sp * sy;
    w = cr * cpcy + sr * spsy;
    x = sr * cpcy - cr * spsy;
    y = cr * sp * cy + sr * cp * sy;
    z = cr * cp * sy - sr * sp * cy;
  }

  quaternion::operator matrix() {
    matrix result;

    result.r[0][0] = 1.0f - 2.0f * (y * y + z * z);
    result.r[1][0] = 2.0f * (x * y - w * z);
    result.r[2][0] = 2.0f * (x * z + w * y);
    result.r[3][0] = 0.0f;

    result.r[0][1] = 2.0f * (x * y + w * z);
    result.r[1][1] = 1.0f - 2.0f * (x * x + z * z);
    result.r[2][1] = 2.0f * (y * z - w * x);
    result.r[3][1] = 0.0f;

    result.r[0][2] = 2.0f * (x * z - w * y);
    result.r[1][2] = 2.0f * (y * z + w * x);
    result.r[2][2] = 1.0f - 2.0f * (x * x + y * y);
    result.r[3][2] = 0.0f;

    result.r[0][3] = 0;
    result.r[1][3] = 0;
    result.r[2][3] = 0;
    result.r[3][3] = 1.0f;

    return(result);
  }

  /*
  Slerp(QUAT * from, QUAT * to, float t, QUAT * res)
  {
    float           to1[4];
    double        omega, cosom, sinom, scale0, scale1;
    // calc cosine
    cosom = from->x * to->x + from->y * to->y + from->z * to->z
      + from->w * to->w;
    // adjust signs (if necessary)
    if (cosom <0.0) {
      cosom = -cosom; to1[0] = -to->x;
      to1[1] = -to->y;
      to1[2] = -to->z;
      to1[3] = -to->w;
    }
    else {
      to1[0] = to->x;
      to1[1] = to->y;
      to1[2] = to->z;
      to1[3] = to->w;
    }
    // calculate coefficients
    if ((1.0 - cosom) > DELTA) {
      // standard case (slerp)
      omega = acos(cosom);
      sinom = sin(omega);
      scale0 = sin((1.0 - t) * omega) / sinom;
      scale1 = sin(t * omega) / sinom;
    }
    else {
      // "from" and "to" quaternions are very close 
      //  ... so we can do a linear interpolation
      scale0 = 1.0 - t;
      scale1 = t;
    }
    // calculate final values
    res->x = scale0 * from->x + scale1 * to1[0];
    res->y = scale0 * from->y + scale1 * to1[1];
    res->z = scale0 * from->z + scale1 * to1[2];
    res->w = scale0 * from->w + scale1 * to1[3];
  }
  */
}

__MYLO_NAMESPACE_END

// Local Variables:
// mode:C++
// End:
