#include "mylo.hpp"

#ifndef __IDENTITY_HPP__
#define __IDENTITY_HPP__

#define FNV32_PRIME ((unsigned long)0x01000193)

__MYLO_NAMESPACE_BEGIN

__MYLO_DLL_EXPORT unsigned long FNV32A(char *buffer, unsigned long length, unsigned long seed=0);

__MYLO_DLL_EXPORT unsigned long CRC32(my::string filename);
__MYLO_DLL_EXPORT unsigned long CRC32(unsigned char *buffer, unsigned long length);

__MYLO_DLL_EXPORT my::string guid(void);
__MYLO_DLL_EXPORT my::string uuid(void);

__MYLO_NAMESPACE_END

extern "C" {

extern void _GenerateUUID(char *);

}

#endif

// Local Variables:
// mode:C++
// End:
