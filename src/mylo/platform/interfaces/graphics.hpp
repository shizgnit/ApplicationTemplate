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

#ifndef __GRAPHICS_INTERFACE_HPP
#define __GRAPHICS_INTERFACE_HPP

__PLATFORM_NAMESPACE_BEGIN

class graphics_interface {
  public: 
    virtual void init(void) = 0;
    virtual void clear(void) = 0;

    virtual void compile(my::shader &shader, unsigned int type) = 0;
    virtual void compile(my::program &program) = 0;
    virtual void compile(my::object &object) = 0;
    virtual void compile(my::objects &objects) = 0;
    virtual void compile(my::font &font) = 0;

    virtual void draw(my::object &object, my::program &shader, const my::spatial::matrix &model, const my::spatial::matrix &view, const my::spatial::matrix &projection) = 0;
    virtual void draw(my::objects &objects, my::program &shader, const my::spatial::matrix &model, const my::spatial::matrix &view, const my::spatial::matrix &projection) = 0;
    virtual void draw(my::string text, my::font &font, my::program &shader, const my::spatial::matrix &model, const my::spatial::matrix &view, const my::spatial::matrix &projection) = 0;
};

__PLATFORM_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:

