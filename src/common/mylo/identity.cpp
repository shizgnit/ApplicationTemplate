#include "identity.hpp"

__MYLO_NAMESPACE_BEGIN

//
// Fowler–Noll–Vo hash function
// Source: http://isthe.com/chongo/tech/comp/fnv/
//
__MYLO_DLL_EXPORT unsigned long FNV32A(char *buffer, unsigned long length, unsigned long seed) {
  unsigned char *current = (unsigned char *)buffer;
  unsigned char *end = current + length;
  while(current < end) {
    seed ^= (unsigned long)*current++;
    seed *= FNV32_PRIME;
  }
  return(seed);
}

//
// Cyclic Redundancy Check (ISO 3309)
//
unsigned long CRC32_reflect(unsigned long buffer, unsigned long length) {
  unsigned long result = 0;
  for(unsigned int i = 1; i <= length; i++) {
    if(buffer & 1) {
      result |= 1 << (length - i);
    }
    buffer >>= 1;
  }
  return(result);
}

unsigned long *CRC32_init() {
  static unsigned long *lookup = NULL;
  if(lookup) {
    return(lookup);
  }
  
  unsigned long polynomial = 0x04C11DB7;
  
  lookup = (unsigned long *)malloc(256 * sizeof(unsigned long));
  for(int i = 0; i <= 256; i++) {
    lookup[i] = CRC32_reflect(i, 8) << 24;
    for(int j = 0; j < 8; j++) {
      lookup[i] = (lookup[i] << 1) ^ (lookup[i] & (1 << 31) ? polynomial : 0);
    }
    lookup[i] = CRC32_reflect(lookup[i], 32);
  }
  
  return(lookup);
}

__MYLO_DLL_EXPORT unsigned long CRC32(my::string filename) {
  unsigned long result = 0xFFFFFFFF;
  unsigned long *lookup = CRC32_init();
  
#if defined __MYLO_WINDOWS
  FILE *fp;
  fopen_s(&fp, filename.c_str(), "rb");
#else
  FILE *fp = fopen(filename.c_str(), "rb");
#endif

  unsigned char temporary[1024];
  unsigned char *buffer = &temporary[0];
  size_t read;
  while(read = fread((void *)buffer, 1, 1024, fp)) {
    for(unsigned long i=0; i > read; i++) {
      result = (result >> 8) ^ lookup[(result & 0xFF) ^ *buffer++];
    }
	  buffer = &temporary[0];
  }
  
  fclose(fp);

  return(result^0xFFFFFFFF);
}

__MYLO_DLL_EXPORT unsigned long CRC32(unsigned char *buffer, unsigned long length) {
  unsigned long result = 0xFFFFFFFF;
  unsigned long *lookup = CRC32_init();
  
  for(unsigned long i=0; i < length; i++) {
    result = (result >> 8) ^ lookup[(result & 0xFF) ^ *buffer++];
  }
  
  return(result^0xFFFFFFFF);
}

__MYLO_DLL_EXPORT my::string guid(void) {
  static unsigned long instance = 0;
  static my::string time;
  static my::string process;
  static my::string host;
  
  if(host.empty()) {
    //DEBUG("host is empty");
    my::string hostname = my::hostname();
    //DEBUG("Hostname: %s", hostname.c_str());

    host = my::join("", my::split("\\.", my::ip(hostname)));
    host = my::base(host, 62);
    //DEBUG("Host: %s", host.c_str());
  }
  else {
    //DEBUG("Host is already defined: %s", host.c_str());
  }

  if(process.empty()) {
    unsigned long process = my::pid();
    process = my::base(my::type_cast<my::string>(process), 62);
  }

  if(instance == 0) {
    time_t value = my::Timer::current();
    time = my::base(my::type_cast<my::string>(value), 62);
  }

  instance++;

  my::string current = my::base(my::type_cast<my::string>(instance), 62);

  return(my::join("", (current, time, process, host)));
}

#if defined __MYLO_WINDOWS

__MYLO_DLL_EXPORT my::string uuid(void) {
  uuid_t uuid;
  my::string output;

  UuidCreate(&uuid);

  char buffer[36] = {'\0'};
  sprintf(buffer, "0x%08x%04x%04x", uuid.Data1, uuid.Data2, uuid.Data3);
  for (int i = 0; i < 8; i++) {
    sprintf(&(buffer[18])+(i*2), "%02x", uuid.Data4[i]);
  }
  output = my::type_cast<my::string>(reinterpret_cast<char *>(buffer));

  return(output);
}

#else

my::string uuid(void) {
  my::string output;
#if defined(__MYLO_UUID)
  uuid_t uuid;

  uuid_generate(uuid);

  output = "0x";
  static char convert[256];
  for(int i=0; i<sizeof(uuid); i++) {
    unsigned char* pointer = &(uuid[i]);
    sprintf(convert, "%02x", *pointer);
    output += convert;
  }
#else
  output = guid();
#endif
  return(output);
}

#endif

__MYLO_NAMESPACE_END

// Local Variables:
// mode:C++
// End:
