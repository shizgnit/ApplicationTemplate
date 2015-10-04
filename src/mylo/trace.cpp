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

#include "trace.hpp"

#if defined __MYLO_WINDOWS

#endif
#if defined __MYLO_ANDROID
#include <android/log.h>
#endif

__MYLO_NAMESPACE_BEGIN

trace::environment *trace::manager() {
  static environment *instance = new environment();
  return(instance);
}

void trace::environment::add(device *instance) {
  if (instance != NULL) {
    _devices[instance->name()] = instance;
  }
}
void trace::environment::remove(device *instance) {
  if (instance != NULL) {
    _devices[instance->name()] = NULL;
  }
}

void trace::environment::add(scope *instance) {
  return;
  if (instance != NULL) {
    _scopes.push_back(instance);
  }
  map<string, device *>::iterator device = _devices.begin();
  for (; device != _devices.end(); device++) {
    if (device->second != NULL) {
      device->second->enter(instance);
    }
  }
}

void trace::environment::remove(scope *instance) {
  return;
  map<string, device *>::iterator device = _devices.begin();
  for (; device != _devices.end(); device++) {
    if (device->second != NULL) {
      device->second->exit(instance);
    }
  }
  if (_scopes.size() == 0) {
    return;
  }
  for (int i = 0; i < (int)_scopes.size(); i++) {
    if (_scopes[i] == instance) {
      _scopes[i] = NULL;
    }
  }
  for (int i = (int)_scopes.size() - 1; i >= 0; i--) {
    if (_scopes[i] == NULL) {
      _scopes.pop_back();
    }
    else {
      break;
    }
  }
}

void trace::environment::output(vector<string> content) {
  map<string, device *>::iterator device = _devices.begin();
  for (; device != _devices.end(); device++) {
    if (device->second != NULL) {
      device->second->output(content);
    }
  }
}


void trace::file::output(vector<string> content) {
  my::string text;
  for (unsigned int i = 0; i < content.size(); i++) {
    text.append(content[i].c_str());
  }
#if defined __MYLO_WINDOWS

#endif
#if defined __MYLO_ANDROID
  __android_log_print(ANDROID_LOG_INFO,"ApplicationTemplate", "%s", text.c_str());
#endif
}

void trace::console::output(vector<string> content) {
  my::string text;
  for (unsigned int i = 0; i < content.size(); i++) {
    text.append(content[i].c_str());
  }
#if defined __MYLO_WINDOWS
  printf("%s", text.c_str());
#endif
#if defined __MYLO_POSIX
  printf("%s", text.c_str());
#endif
#if defined __MYLO_ANDROID
  __android_log_print(ANDROID_LOG_INFO, "ApplicationTemplate", "%s", text.c_str());
#endif
}


__MYLO_NAMESPACE_END

