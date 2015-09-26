#include "mylo.hpp"

#ifndef __FILESYSTEM_HPP
#define __FILESYSTEM_HPP

__MYLO_NAMESPACE_BEGIN

__MYLO_DLL_EXPORT bool rm(my::string filename);
__MYLO_DLL_EXPORT bool mv(my::string src, my::string dest);
__MYLO_DLL_EXPORT bool cp(my::string src, my::string dest);

__MYLO_DLL_EXPORT bool mkdir(my::string path, my::vector<my::string> mask);
__MYLO_DLL_EXPORT bool rmdir(my::string path);

__MYLO_DLL_EXPORT my::string pwd(my::string path="");

__MYLO_DLL_EXPORT my::vector<unsigned long> stat(my::string path);
__MYLO_DLL_EXPORT my::vector<unsigned long> lstat(my::string path);

__MYLO_DLL_EXPORT bool exists(my::string path);

__MYLO_DLL_EXPORT my::string filetype(my::string path);

class directory_handle;

class __MYLO_DLL_EXPORT directory {
public:
  directory(my::string path);
  ~directory(void);

  bool open(my::string path);
  bool close();

  my::string next();

  operator my::vector<my::string> (void);

private:
  my::string m_path;

  directory_handle *m_dha;
};


class __MYLO_DLL_EXPORT file : public my::serializable {
public:
  enum modes {
    ro = 0x01,
    wo = 0x02,
    rw = 0x03,
  };

  file(void) : m_fp(NULL) { /* NULL */ }
  file(my::string in) : m_fp(NULL) {
    open(in);
  }

  ~file(void) {
    close();
  }
  
  bool open() { 
    return(open(m_file)); 
  }
  bool open(my::string in, modes mode = rw);
  
  void close(void);

  off_t location(void);
  bool seek(off_t pos);
  
  off_t read(unsigned char *buffer, off_t bytes);
  off_t write(unsigned char *buffer, off_t bytes);
  
  static bool exists(my::string file);
  
protected:
  my::string m_file;
  
  off_t m_size;
  
  FILE *m_fp;
  int m_fd;
};

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
