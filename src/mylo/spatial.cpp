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


void spatial::position::identity(void) {
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

void spatial::position::viewable(bool toggle) {
  view = toggle;
}

void spatial::position::move(float t) {
  vector diff = (eye - center) * t;

  center += diff;
  eye += diff;
}

void spatial::position::strafe(float t) {
  vector normal = eye - center;
  vector cross = (normal % up) + center;
  vector diff = (cross - center) * t;

  center += diff;
  eye += diff;
}

void spatial::position::rotate(float x, float y, float z) {
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

void spatial::position::project(const vector &offset, const vector &projection) {
  vector normal = eye - center;

  vector cross = (normal % up) + center;

  vector diff = (cross - center) * offset;

  diff.x = (cross.x - center.x) * offset.x;
  diff.y = (cross.y - center.y) * offset.y;
  diff.z = (cross.z - center.z) * offset.z;
}

__MYLO_NAMESPACE_END

// Local Variables:
// mode:C++
// End:
