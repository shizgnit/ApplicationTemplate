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

#include "asset.hpp"

#if defined __ANDROID__
#include <android/asset_manager_jni.h>
#endif

__MYLO_NAMESPACE_BEGIN

void* asset::m_manager = NULL;

__MYLO_DLL_EXPORT bool asset::open(my::string in) {
  DEBUG_SCOPE;
#if defined __MYLO_ANDROID
  DEBUG_TRACE << "Attempting to find resource: " << in << endl;
  AAsset* asset = AAssetManager_open((AAssetManager*)m_manager, in, AASSET_MODE_STREAMING);
  m_ptr = asset;
  if(asset) {  
    DEBUG_TRACE << "Found resource " << (int)AAsset_getLength(asset) << " bytes" << endl;
//    DEBUG_TRACE << "Contents: " << (char *)AAsset_getBuffer(asset) << endl;
//    DEBUG_TRACE << "Contents length: " << strlen((char *)AAsset_getBuffer(asset)) << endl;
    m_buffer.write((unsigned char *)AAsset_getBuffer(asset), AAsset_getLength(asset));
  }
  else {
    DEBUG_TRACE << "Failure to load resource" << endl;
  }
#else 
#if defined __MYLO_POSIX
  my::string source = my::string((char *)m_manager);
  source.append(in);
  m_buffer << my::file(source);
#endif
#endif

#if defined __PLATFORM_WINDOWS
  my::string source = my::string((char *)m_manager);
  source.append(my::substitute(in, "/", "\\", true));
  m_buffer << my::file(source);
#endif

  return(true);
}

__MYLO_DLL_EXPORT void asset::close(void) {
  DEBUG_SCOPE;
#if defined __ANDROID__
	AAsset_close((AAsset*)m_ptr);
#endif
}

__MYLO_DLL_EXPORT void asset::manager(void *ptr) {
  DEBUG_SCOPE;
  m_manager = ptr;
}

__MYLO_DLL_EXPORT bool asset::exists(my::string file) {
  DEBUG_SCOPE;
  return(true);
}

__MYLO_DLL_EXPORT off_t asset::read(unsigned char *buffer, off_t bytes) {
  DEBUG_SCOPE;
  return(m_buffer.read(buffer, bytes));
}

__MYLO_DLL_EXPORT off_t asset::write(unsigned char *buffer, off_t bytes) {
  DEBUG_SCOPE;
  return(m_buffer.write(buffer, bytes));
}

__MYLO_NAMESPACE_END

// Local Variables:
// mode:C++
// End:
