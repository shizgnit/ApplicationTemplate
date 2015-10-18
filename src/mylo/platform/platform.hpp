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

#ifndef __PLATFORM_HPP
#define __PLATFORM_HPP

#if defined WIN32
#define __PLATFORM_WINDOWS 1
#endif

#if defined __ANDROID__
#define __PLATFORM_ANDROID 1
#endif

#if defined(__PLATFORM_WINDOWS)
#ifndef _UNICODE
#error Set "Use Unicode Character Set" in the general project settings
#endif
#define WIDE 1
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#if defined __PLATFORM_WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
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
#endif

#if defined __PLATFORM_ANDROID
#  include <unistd.h>
#  include <fcntl.h>
#  include <sys/mman.h>
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <dirent.h>
#  include <utime.h>
#  include <netdb.h>
#  include <resolv.h>
#  include <cstring>
#  ifndef MAP_FILE
#    define MAP_FILE 0
#  endif
#  if defined(__MYLO_UUID)
#     include <uuid/uuid.h>
#  endif
#endif

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <stdarg.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <errno.h>

#if defined(__MOVE_THESE_HEADERS)

#if defined(__MYLO_PTHREAD)
#include <pthread.h>
#endif

#if defined(__MYLO_MYSQL)
#include <mysql/mysql.h>
#endif

#if defined(__MYLO_LUA)
extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
#endif

#endif

#define __PLATFORM_NAMESPACE_BEGIN namespace platform {
#define __PLATFORM_NAMESPACE_END }

#endif

