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

#ifndef __WINAPI_HPP
#define __WINAPI_HPP

__PLATFORM_NAMESPACE_BEGIN

namespace winapi {

  class asset : public asset_interface {
  public:
    void init(void *instance);
    void search(my::string path);
    my::buffer retrieve(my::string request, my::string path = "");
  };

  class filesystem : public filesystem_interface {
  public:
    bool rm(my::string filename);
    bool mv(my::string src, my::string dest);
    bool cp(my::string src, my::string dest);

    bool mkdir(my::string path, my::string mask);
    bool rmdir(my::string path);

    my::string pwd(my::string path = "");

    my::vector<unsigned long> stat(my::string path);
    my::vector<unsigned long> lstat(my::string path);

    bool exists(my::string path);

    my::string filetype(my::string path);
  };

}

__PLATFORM_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
