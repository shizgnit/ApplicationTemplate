#include "asset.hpp"

#if defined __ANDROID__
#include <android/asset_manager_jni.h>
#endif

__MYLO_NAMESPACE_BEGIN

void* asset::m_manager = NULL;

__MYLO_DLL_EXPORT bool asset::open(my::string in) {
#if defined __ANDROID__
  DEBUG_TRACE << "Attempting to find resource: " << in << endl;
  AAsset* asset = AAssetManager_open((AAssetManager*)m_manager, in, AASSET_MODE_STREAMING);
  m_ptr = asset;
  if(asset) {  
    DEBUG_TRACE << "Found resource %d bytes" << (int)AAsset_getLength(asset) << endl;
    DEBUG_TRACE << "Contents: %s" << (char *)AAsset_getBuffer(asset) << endl;
    DEBUG_TRACE << "Contents length: " << strlen((char *)AAsset_getBuffer(asset)) << endl;
    m_buffer.write((unsigned char *)AAsset_getBuffer(asset), AAsset_getLength(asset));
  }
  else {
    DEBUG_TRACE << "Failure to load resource" << endl;
  }
#else
  my::string source = my::string((char *)m_manager);
  source.append(my::substitute(in, "/", "\\", true));
  m_buffer << my::file(source);
#endif
  return(true);
}

__MYLO_DLL_EXPORT void asset::close(void) {
#if defined __ANDROID__
	AAsset_close((AAsset*)m_ptr);
#endif
}

__MYLO_DLL_EXPORT void asset::manager(void *ptr) {
  m_manager = ptr;
}

__MYLO_DLL_EXPORT bool asset::exists(my::string file) {
  return(true);
}

__MYLO_DLL_EXPORT off_t asset::read(unsigned char *buffer, off_t bytes) {
  return(m_buffer.read(buffer, bytes));
}

__MYLO_DLL_EXPORT off_t asset::write(unsigned char *buffer, off_t bytes) {
  return(m_buffer.write(buffer, bytes));
}

__MYLO_NAMESPACE_END

// Local Variables:
// mode:C++
// End:
