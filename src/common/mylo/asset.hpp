#include "mylo.hpp"

#ifndef __ASSET_HPP
#define __ASSET_HPP

__MYLO_NAMESPACE_BEGIN

class __MYLO_DLL_EXPORT asset : public my::serializable {
public:
  asset(void) { /* NULL */ }
  asset(my::string in) {
    DEBUG_SCOPE;
    open(in);
  }

  ~asset(void) {
    DEBUG_SCOPE;
    close();
  }
  
  bool open() {
    DEBUG_SCOPE;
    return(open(m_name));
  }
  bool open(my::string in);
  
  void close(void);

  off_t read(unsigned char *buffer, off_t bytes);
  off_t write(unsigned char *buffer, off_t bytes);
  
  static void manager(void *ptr);
  static bool exists(my::string file);
  
protected:
  my::string m_name;
  my::buffer m_buffer;
  void *m_ptr;
  
  off_t m_size;
  
  static void *m_manager;
};

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
