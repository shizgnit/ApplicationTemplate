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

//  https://www.stack.nl/~dimitri/doxygen/manual/xmlcmds.html

#ifndef __MYLO_HPP
#define __MYLO_HPP

#define __MYLO_BUILD_MAJOR 0
#define __MYLO_BUILD_MINOR 0
#define __MYLO_BUILD_REV 1

#define __MYLO_RELEASE $(__MYLO_BUILD_MAJOR).$(__MYLO_BUILD_MINOR).$(__MYLO_BUILD_REV)

#include "platform/platform.hpp"

#if defined __PLATFORM_WINDOWS
#define __MYLO_STATIC 1
#  if defined __MYLO_STATIC
#    define __MYLO_DLL_EXPORT
#    define __MYLO_INTERNAL_DLL_EXPORT
#    define __MYLO_STL_EXPORT
#  else
#    if defined(__MYLO_EXPORTS)
#      define __MYLO_DLL_EXPORT __declspec(dllexport)
#      define __MYLO_INTERNAL_DLL_EXPORT __declspec(dllexport)
#      define __MYLO_STL_EXPORT
#    else
#      define __MYLO_DLL_EXPORT __declspec(dllimport)
#      define __MYLO_INTERNAL_DLL_EXPORT
#      define __MYLO_STL_EXPORT extern
#    endif
#  endif
#else
#  define __MYLO_DLL_EXPORT
#  define __MYLO_INTERNAL_DLL_EXPORT
#  define __MYLO_STL_EXPORT
#endif

#define __MYLO_ZLIB 1
//#define __MYLO_UUID 1

#define __MYLO_NAMESPACE_BEGIN namespace my {
#define __MYLO_NAMESPACE_END }

__MYLO_NAMESPACE_BEGIN

typedef int                 INT;
typedef char                INT08;
typedef short               INT16;
typedef long                INT32;
typedef unsigned int        UINT;
typedef unsigned char       UINT08;
typedef unsigned short      UINT16;
typedef unsigned long       UINT32;
typedef float               FLT32;
typedef double              FLT64;
typedef char                CHAR;
typedef unsigned char       BYTE;
typedef bool                BOOL;

#if defined __PLATFORM_WINDOWS
typedef __int64             INT64;
typedef unsigned __int64    UINT64;
#define OMANIP(var) ostream& (__cdecl * var)(ostream&)
#define MANIP(var) ios& (__cdecl * var)(ios&)
#endif

#if defined __PLATFORM_ANDROID
typedef long long           INT64;
typedef unsigned long long  UINT64;
#define OMANIP(var) ostream& (*var)(ostream&)
#define MANIP(var) ios& (*var)(ios&)
#endif


__MYLO_NAMESPACE_END

#include "string.hpp"
#include "type_traits.hpp"
#include "shared_ptr.hpp"
#include "iterator.hpp"
#include "pair.hpp"
#include "vector.hpp"
#include "map.hpp"
#include "stream.hpp"
#include "buffer.hpp"
#include "type_cast.hpp"
#include "trace.hpp"
#include "debug.hpp"

#include "variant.hpp"
#include "regex.hpp"
#include "utilities.hpp"
#include "asset.hpp"
#include "compression.hpp"
#include "timer.hpp"
#include "identity.hpp"
#include "xml.hpp"
#include "xpath.hpp"
#include "configuration.hpp"
#include "functor.hpp"
#include "datatype.hpp"
#include "file.hpp"
#include "directory.hpp"
//#include "archive.hpp"

#include "dto/image.hpp"
#include "dto/shader.hpp"
#include "dto/material.hpp"
#include "dto/object.hpp"
#include "dto/primitives.hpp"
#include "dto/entity.hpp"
#include "dto/font.hpp"
#include "dto/audio.hpp"
#include "dto/program.hpp"

#if defined __PLATFORM_WINDOWS
#include "platform/windows.hpp"
#endif

#if defined __PLATFORM_ANDROID
#include "platform/android.hpp"
#endif

#include "dto/types/tga.hpp"
#include "dto/types/png.hpp"
#include "dto/types/mtl.hpp"
#include "dto/types/obj.hpp"
#include "dto/types/vert.hpp"
#include "dto/types/frag.hpp"
#include "dto/types/fnt.hpp"
#include "dto/types/wav.hpp"


#endif

// Local Variables:
// mode:C++
// End:

