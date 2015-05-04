#ifndef __MYLO_HPP
#define __MYLO_HPP

#define __MYLO_BUILD_MAJOR 0
#define __MYLO_BUILD_MINOR 0
#define __MYLO_BUILD_REV 1

#define __MYLO_RELEASE $(__MYLO_BUILD_MAJOR).$(__MYLO_BUILD_MINOR).$(__MYLO_BUILD_REV)

#if defined WIN32
#define __MYLO_WINDOWS 1
//#define __MYLO_STATIC 1
#define _CRT_SECURE_NO_WARNINGS 1
#else
#define __MYLO_POSIX 1
#endif

#if defined(__MYLO_WINDOWS)
#ifndef _UNICODE
#error Set "Use Unicode Character Set" in the general project settings
#endif
#define WIDE 1
#endif

//
//-[ MACROS ]-----------------------------------------------------------
//
#ifdef _WIN32
#define __MYLO_STATIC 1
//#if defined(__MYLO_WINDOWS)
#  if defined(__MYLO_STATIC)
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

//#if defined(__MYLO_WINDOWS)
#ifdef _WIN32
#  include <winsock2.h>
#  include <rpc.h>
#  include <windows.h>
#  include <lmerr.h>
#  define fseeko fseek
#  define ftello ftell
//#endif

//#if defined(__MYLO_POSIX)
#else
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
#  ifndef MAP_FILE
#    define MAP_FILE 0
#  endif
#  if defined(__MYLO_UUID)
#     include <uuid/uuid.h>
#  endif
#endif

//#include <fstream>
//#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <stdarg.h>
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

#if defined(__MYLO_WINDOWS)
typedef __int64             INT64;
typedef unsigned __int64    UINT64;
#define OMANIP(var) ostream& (__cdecl * var)(ostream&)
#define MANIP(var) ios& (__cdecl * var)(ios&)
#endif

#if defined(__MYLO_POSIX)
typedef long long           INT64;
typedef unsigned long long  UINT64;
#define OMANIP(var) ostream& (*var)(ostream&)
#define MANIP(var) ios& (*var)(ios&)
#endif


__MYLO_NAMESPACE_END

#include "string.hpp"
#include "typetraits.hpp"
#include "shared_ptr.hpp"
#include "iterator.hpp"
#include "pair.hpp"
#include "vector.hpp"
#include "map.hpp"
#include "stream.hpp"
#include "buffer.hpp"
#include "typecast.hpp"
#include "variant.hpp"
#include "regex.hpp"
#include "utilities.hpp"
#include "filesystem.hpp"
#include "asset.hpp"
#include "compression.hpp"
#include "timer.hpp"
#include "identity.hpp"
#include "xml.hpp"
#include "xpath.hpp"
#include "configuration.hpp"
#include "functor.hpp"

#include "datatype.hpp"
//#include "archive.hpp"
#include "image.hpp"
#include "shader.hpp"
#include "material.hpp"
#include "object.hpp"
#include "primitives.hpp"
#include "entity.hpp"
#include "font.hpp"


#if defined(__MYLO_MYSQL) && defined(__MYLO_PTHREAD)
#include "db.hpp"
#endif

#include "tga.hpp"
#include "png.hpp"
#include "mtl.hpp"
#include "obj.hpp"
#include "vert.hpp"
#include "frag.hpp"
#include "program.hpp"
#include "fnt.hpp"

#endif

// Local Variables:
// mode:C++
// End:

