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

#ifndef __TRACE_HPP
#define __TRACE_HPP

__MYLO_NAMESPACE_BEGIN

namespace trace {

  class device;
  class scope;

  class environment {
  public:
    void add(device *instance);
    void remove(device *instance);

    void add(scope *instance);
    void remove(scope *instance);

    void SetContext(string label, string value) {
      _session[label] = value;
    }
    string context(string label) {
      return(_session[label]);
    }
    bool exists(string key) {
      return(_session.find(key) != _session.end());
    }
    vector<string> keys() {
      vector<string> list;
      map<string, string>::iterator it = _session.begin();
      for (; it != _session.end(); it++) {
        list.push_back(it->first);
      }
      return(list);
    }
    void clear() {
      _session.clear();
    }

    int depth() {
      return(_scopes.size());
    }

    void output(vector<string> contents);

  private:
    map<string, string> _session;

    vector<scope *> _scopes;
    map<string, device *> _devices;
  };

  environment *manager();

  class scope {
  public:
    scope(const char *file, int line, const char *function = NULL) {
      _file = file;
      _line = line;
      _function = function;
      manager()->add(this);
    }

    ~scope() {
      manager()->remove(this);
    }

    virtual string file() {
      return(_file);
    }
    virtual int line() {
      return(_line);
    }
    virtual string function() {
      return(_function);
    }

  protected:
    string _file;
    int _line;
    string _function;
  };

  class device {
  public:
    device(string name) {
      _name = name;
      open();
      manager()->add(this);
    }

    virtual ~device() {
      manager()->remove(this);
      close();
    }

    virtual string name() {
      return(_name);
    }

    virtual void open() { /*NULL*/ }
    virtual void close() { /*NULL*/ }

    virtual void enter(scope *instance) { /*NULL*/ }
    virtual void exit(scope *instance) { /*NULL*/ }

    virtual void output(vector<string> content) = 0;

  protected:
    string _name;
  };

  class writer {
  public:
    writer &operator << (int i) {
      _contents.push_back(type_cast<string>(i));
      return(*this);
    }

    writer &operator << (string text) {
      _contents.push_back(text);
      return(*this);
    }

    writer &operator << (manipulator object) {
      _contents.push_back("\n");
      return(*this);
    }

    writer(environment *instance = NULL) {
      if (instance == NULL) {
        _manager = manager();
      }
      else {
        _manager = instance;
      }
    }

    ~writer() {
      _manager->output(_contents);
    }

  private:
    environment *_manager;
    vector<string> _contents;
  };
  
  class file : public device {
  public:
    file(string filename) : device(filename) {}
    void output(vector<string> content);
  };

  class console : public device {
  public:
    console() : device("console") {}
    void output(vector<string> content);
  };
  
  class buffer : public device {
  public:
    buffer() : device("buffer"), maxlines(1024) {}
    void output(vector<string> content);
    
    int maxlines;
    list<string> contents;
  };
}

__MYLO_NAMESPACE_END

#endif
