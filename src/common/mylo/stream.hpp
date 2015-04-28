#include "mylo.hpp"

#ifndef __STREAM_HPP
#define __STREAM_HPP

__MYLO_NAMESPACE_BEGIN

class serializable;

//template class __MYLO_DLL_EXPORT my::shared_ptr< my::vector<my::serializable *> >;

class __MYLO_DLL_EXPORT stream {
public:
  static const size_t alloc = 65536;
  //static const size_t alloc = 16384;
  
  stream() {
    m_chain = new my::vector<my::serializable *>();
  }
  
  stream(const my::serializable *lval, const my::serializable *rval);

  stream(const stream &in);
  
  ~stream();
  
  stream &operator << (const my::serializable &rval);
  stream &operator >> (const my::serializable &rval);
  
private:
  my::shared_ptr< my::vector<my::serializable *> > m_chain;
};

class buffer;

class __MYLO_DLL_EXPORT serializable {
public:
  serializable();
  virtual ~serializable();
  
  stream operator << (const serializable &rval) {
    return(stream(&rval, this));
  }
  stream operator >> (const serializable &rval) {
    return(stream(this, &rval));
  }

  virtual off_t expect(void) {
    return(stream::alloc);
  }

  virtual off_t offset(void) {
    return(0);
  }

  virtual off_t location(void) {
    return(0);
  }
  virtual bool seek(off_t pos) {
    return(false);
  }
  
  virtual off_t read(unsigned char *buffer, off_t bytes) = 0;
  virtual off_t write(unsigned char *buffer, off_t bytes) = 0;
  
  virtual my::string getline(void);

  operator my::string (void);
  operator my::vector<my::string> (void);
  
  virtual off_t write(my::string &line);
  virtual off_t write(my::vector<my::string> &lines);
  
  virtual off_t cached();

protected:
  my::shared_ptr<buffer> m_cache;
};


//template class __MYLO_DLL_EXPORT my::shared_ptr<buffer>;

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
