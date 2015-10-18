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

#include "directory.hpp"

__MYLO_NAMESPACE_BEGIN

class directory_handle {
  public:
#if defined __PLATFORM_WINDOWS
  HANDLE handle;
  WIN32_FIND_DATA *last;
#else
  DIR *handle;
  struct dirent *last;
#endif
};

__MYLO_DLL_EXPORT directory::directory(my::string path) {
  m_path = "";

  m_dha = new directory_handle;
  m_dha->last   = NULL;
  m_dha->handle = NULL;
  if(!path.empty()) {
    this->open(path);
  }
}

__MYLO_DLL_EXPORT directory::~directory(void) {
  this->close();
  if(m_dha) {
    delete(m_dha);
    m_dha = NULL;
  }
}

__MYLO_DLL_EXPORT directory::operator my::vector<my::string> (void) {
  my::vector<my::string> results;
  my::string entry;
  while(1) {
    my::string entry = this->next();
    if(entry.empty()) {
      break;
    }
    results.push_back(entry);
  }
  return(results);
}

#if defined __PLATFORM_WINDOWS

__MYLO_DLL_EXPORT bool directory::open(my::string path) {
  this->close();
  
  if(!path.empty()) {
    if(path[path.length()-1] == L'\\') {
      path.append("*");
    }
    m_path = path;
  }
  else {
    return(false);
  }
  return(true);
}

#else

bool directory::open(my::string path) {
  this->close();
  if(!path) {
    return(false);
  }

  m_path = path;
  if(m_path) {
    m_dha->handle = opendir(m_path);
    if(m_dha->handle == NULL) {
      return(false);
    }
  }
  else {
    return(false);
  }

  return(true);
}

#endif

#if defined __PLATFORM_WINDOWS

__MYLO_DLL_EXPORT bool directory::close() {
  if(m_dha->last) {
    delete m_dha->last;
  }
  if(m_dha->handle) {
    FindClose(m_dha->handle);
  }

  return(true);
}

#else

bool directory::close() {
  if(m_dha->last) {
    delete m_dha->last;
  }
  if(m_dha->handle) {
    closedir(m_dha->handle);
  }

  return(true);
}

#endif


#if defined __PLATFORM_WINDOWS

__MYLO_DLL_EXPORT my::string directory::next() {
  my::string value;
  if(!m_dha->last) {
    m_dha->last = new WIN32_FIND_DATA;
#if defined WIDE
    m_dha->handle = FindFirstFile(type_cast<my::wstring>(m_path).c_str(), m_dha->last);
#else
    m_dha->handle = FindFirstFile(type_cast<my::string>(m_path).c_str(), m_dha->last);
#endif
    if (m_dha->handle == INVALID_HANDLE_VALUE) {
      //ERROR(UNDEFINED_ERROR);
      return(value);
    }
    value = type_cast<my::string>(m_dha->last->cFileName);
  }
  else {
    if(FindNextFile(m_dha->handle, m_dha->last)) {
      value = type_cast<my::string>(m_dha->last->cFileName);
    }
    else {
      if (GetLastError() == ERROR_NO_MORE_FILES) {
        return(value);
      }
      else {
        //ERROR(UNDEFINED_ERROR);
        return(value);
      }
    }
  }
  return(value);
}

#else

my::string directory::next() {
  my::string value;

  if(m_dha->handle) {
    if(m_dha->last = readdir(m_dha->handle)) {
      value = m_dha->last->d_name;
    }
  }

  return(value);
}

#endif

__MYLO_NAMESPACE_END

// Local Variables:
// mode:C++
// End:
