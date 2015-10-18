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

#include "art.hpp"

#include <android/asset_manager_jni.h>

__PLATFORM_NAMESPACE_BEGIN

void * platform::asset_interface::manager_instance = NULL;

void art::asset::manager(void *instance) {
  manager_instance = instance;
}

void art::asset::search(my::string path) {
  search_paths[my::substitute(path, "/", "\\", true)] = 1;
}

my::buffer art::asset::retrieve(my::string request, my::string path) { DEBUG_SCOPE;
  my::buffer data;
  
  DEBUG_TRACE << "Attempting to find resource: " << request << my::endl;
  AAsset* asset = AAssetManager_open((AAssetManager*)manager_instance, request, AASSET_MODE_STREAMING);
  //m_ptr = asset;
  if(asset) {
    DEBUG_TRACE << "Found resource " << (int)AAsset_getLength(asset) << " bytes" << my::endl;
//    DEBUG_TRACE << "Contents: " << (char *)AAsset_getBuffer(asset) << my::endl;
//    DEBUG_TRACE << "Contents length: " << strlen((char *)AAsset_getBuffer(asset)) << my::endl;
    data.write((unsigned char *)AAsset_getBuffer(asset), AAsset_getLength(asset));
  }
  else {
    DEBUG_TRACE << "Failure to load resource" << my::endl;
  }
  
  return(data);
}

//void asset::close(void) { DEBUG_SCOPE;
//  AAsset_close((AAsset*)m_ptr);
//}

__PLATFORM_NAMESPACE_END


// Local Variables:
// mode:C++
// End:
