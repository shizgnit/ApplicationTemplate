#include "mylo.hpp"

#ifndef __DATATYPE_HPP
#define __DATATYPE_HPP

__MYLO_NAMESPACE_BEGIN

template<class T> class __MYLO_INTERNAL_DLL_EXPORT datatype : public serializable {
public:
  typedef my::functor<off_t (*)(T*)> output;
  typedef my::functor<bool (*)(T*, off_t)> input;

  typedef my::functor<off_t (*)(T*, unsigned char *, off_t)> transfer;

  datatype():
    m_expect(NULL),
    m_offset(NULL),
    m_location(NULL),
    m_seek(NULL),
    m_read(NULL),
    m_write(NULL) {}

  virtual ~datatype() {}

  void callback_expect(my::string type, off_t (*fp)(T*)) {
    m_expect = my::create_functor(type, "datatype callback", fp);
  }
  void callback_offset(my::string type, off_t (*fp)(T*)) {
    m_offset = my::create_functor(type, "datatype callback", fp);
  }
  void callback_location(my::string type, off_t (*fp)(T*)) {
    m_location = my::create_functor(type, "datatype callback", fp);
  }
  void callback_seek(my::string type, bool (*fp)(T*, off_t)) {
    m_seek = my::create_functor(type, "datatype callback", fp);
  }
  void callback_read(my::string type, off_t (*fp)(T*, unsigned char *, off_t)) {
    m_read = my::create_functor(type, "datatype callback", fp);
  }
  void callback_write(my::string type, off_t (*fp)(T*, unsigned char *, off_t)) {
    m_write = my::create_functor(type, "datatype callback", fp);
  }

  virtual off_t expect(void) {
    if(m_expect) {
      return m_expect->exec((T*)this);
    }
    return 0;
  }

  virtual off_t offset(void) {
    if(m_offset) {
      return m_offset->exec((T*)this);
    }
    return 0;
  }

  virtual off_t location(void) {
    if(m_location) {
      return m_location->exec((T*)this);
    }
    return 0;
  }

  virtual bool seek(off_t pos) {
    if(m_seek) {
      return m_seek->exec((T*)this, pos);
    }
    return false;
  }

  virtual off_t read(unsigned char *buffer, off_t bytes) {
    if(m_read) {
      return m_read->exec((T*)this, buffer, bytes);
    }
    return 0;
  }

  virtual off_t write(unsigned char *buffer, off_t bytes) {
    if(m_write) {
      return m_write->exec((T*)this, buffer, bytes);
    }
    return 0;
  }

private:
  output   *m_expect;
  output   *m_offset;
  output   *m_location;
  input    *m_seek;
  transfer *m_read;
  transfer *m_write;
};

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
