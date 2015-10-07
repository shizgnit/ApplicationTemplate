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

#ifndef __WINDOWS_HPP
#define __WINDOWS_HPP

#include <winsock2.h>
#include <rpc.h>
#include <windows.h>
#include <lmerr.h>
#define fseeko fseek
#define ftello ftell

#include <stdio.h>

#define GLEW_STATIC
#include <GL\glew.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <shellapi.h>

#include "mylo.hpp"

#include "interfaces/graphics.hpp"
#include "implementation/opengl.hpp"

#include "interfaces/audio.hpp"
#include "implementation/openal.hpp"

__PLATFORM_NAMESPACE_BEGIN

static platform::graphics_interface *graphics = new platform::opengl();
static platform::audio_interface *audio = new platform::openal();

__PLATFORM_NAMESPACE_END

#include "ApplicationTemplate.hpp"

#endif

// Local Variables:
// mode:C++
// End:

