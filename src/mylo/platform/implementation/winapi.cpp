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

#include "winapi.hpp"

__PLATFORM_NAMESPACE_BEGIN

void winapi::asset::init(void *instance) {
  search((char *)instance);
}

void winapi::asset::search(my::string path) {
  search_paths[my::substitute(path, "/", "\\", true)] = 1;
}

my::buffer winapi::asset::retrieve(my::string request, my::string path) { DEBUG_SCOPE;
  my::buffer data;
  if (path.empty() == true) {
    my::map<my::string, int>::iterator it = search_paths.begin();
    for (; it != search_paths.end(); it++) {
      my::string path = my::substitute(it->first + "\\" + request, "/", "\\", true);
      if (platform::api::filesystem->exists(path) == false) {
        continue;
      }
      data << my::file(path);
    }
  }
  else {
    path.append("\\");
    path.append(my::substitute(request, "/", "\\", true));
    data << my::file(path);
  }
  return(data);
}

bool winapi::filesystem::cp(my::string srcfile, my::string dest) {
  DWORD dwAttrs;

#if defined WIDE
  if (CopyFile(my::type_cast<my::wstring>(srcfile).c_str(), my::type_cast<my::wstring>(dest).c_str(), FALSE)) {
    dwAttrs = GetFileAttributes(my::type_cast<my::wstring>(srcfile).c_str());
    if (!(dwAttrs & FILE_ATTRIBUTE_READONLY)) {
      SetFileAttributes(my::type_cast<my::wstring>(dest).c_str(), dwAttrs | FILE_ATTRIBUTE_READONLY);
    }
  }
#else
  if (CopyFile(my::type_cast<my::string>(srcfile).c_str(), my::type_cast<my::string>(dest).c_str(), FALSE)) {
    dwAttrs = GetFileAttributes(my::type_cast<my::string>(srcfile).c_str());
    if (!(dwAttrs & FILE_ATTRIBUTE_READONLY)) {
      SetFileAttributes(my::type_cast<my::string>(dest).c_str(), dwAttrs | FILE_ATTRIBUTE_READONLY);
    }
  }
#endif
  else {
    return(false);
  }
  return(true);
}

bool winapi::filesystem::rm(my::string filename) {
#if defined WIDE
  DeleteFile(my::type_cast<my::wstring>(filename).c_str());
#else
  DeleteFile(my::type_cast<my::string>(filename).c_str());
#endif
  return(true);
}

bool winapi::filesystem::mv(my::string srcfile, my::string dest) {
#if defined WIDE
  MoveFile(my::type_cast<my::wstring>(srcfile).c_str(), my::type_cast<my::wstring>(dest).c_str());
#else
  MoveFile(my::type_cast<my::string>(srcfile).c_str(), my::type_cast<my::string>(dest).c_str());
#endif
  return(true);
}

bool winapi::filesystem::mkdir(my::string path, my::string mask) {
  return(CreateDirectory(my::type_cast<my::wstring>(path).c_str(), NULL) ? true : false);
  //return(CreateDirectory(my::type_cast<my::string>(path).c_str(), NULL) ? true : false);
}

bool winapi::filesystem::rmdir(my::string path) {
#if defined WIDE
  return(RemoveDirectory(my::type_cast<my::wstring>(path).c_str()) ? true : false);
#else
  return(RemoveDirectory(my::type_cast<my::string>(path).c_str()) ? true : false);
#endif
}

my::string winapi::filesystem::pwd(my::string path) {
  static wchar_t current[2048];
  //static char current[2048];

  if(!path.empty()) {
    SetCurrentDirectory(my::type_cast<my::wstring>(path).c_str());
    //SetCurrentDirectory(my::type_cast<my::string>(path).c_str());
  }

  GetCurrentDirectory(2048, current);
  return(my::wstring(current));
}


my::vector<unsigned long> winapi::filesystem::stat(my::string path) {
  my::vector<unsigned long> results;

  struct _stat sst;

  if(_stat(path.c_str(), &sst) != 0) {
    return(results);
  }

  results.reserve(13);

  results[12] = 0;
  results[11] = 0;
  results[10] = (unsigned long)sst.st_ctime;
  results[9]  = (unsigned long)sst.st_mtime;
  results[8]  = (unsigned long)sst.st_atime;
  results[7]  = sst.st_size;
  results[6]  = 0;
  results[5]  = 0;
  results[4]  = 0;
  results[3]  = 0;
  results[2]  = sst.st_mode;
  results[1]  = sst.st_dev + 'A';
  results[0]  = 0;

  return(results);
}

my::vector<unsigned long> winapi::filesystem::lstat(my::string path) {
  return(stat(path));
}

bool winapi::filesystem::exists(my::string path) {
  struct _stat sst;
  return(_stat(path.c_str(), &sst) == 0);
}

my::string winapi::filesystem::filetype(my::string path) {
  my::string result = "unknown";

  my::vector<unsigned long> stats = stat(path);
  if(stats) {
    unsigned long mode = stats[2];

    if(mode & _S_IFREG)  { result = "regular"; }
    if(mode & _S_IFDIR)  { result = "directory"; }
  }

  return(result);
}

__PLATFORM_NAMESPACE_END

// Local Variables:
// mode:C++
// End:
