#include "mylo.hpp"

#ifndef __ARCHIVE_HPP
#define __ARCHIVE_HPP

__MYLO_NAMESPACE_BEGIN

class __MYLO_DLL_EXPORT archive : public serializable {
public:
  archive();

  off_t expect();
  off_t offset();

  off_t read(unsigned char *buffer, off_t bytes);
  off_t write(unsigned char *buffer, off_t bytes);

  class __MYLO_DLL_EXPORT file : public serializable, public state<archive::file> {
  public:
    enum {
      parse_header,
      parse_text,
      parse_contents,
      parse_complete
    };

    file() {
      memset(&header, 0, sizeof(header));
      transition(parse_header);
    }

    off_t expect();
    off_t offset();

    off_t offset_content();

    off_t read(unsigned char *buffer, off_t bytes);
    off_t write(unsigned char *buffer, off_t bytes);

  public:
    struct {
      unsigned long signature;
      unsigned int version;
      unsigned int general;
      unsigned int method;
      unsigned int mtime;
      unsigned int mdate;
      unsigned long crc32;
      unsigned long csize;
      unsigned long usize;
      unsigned int flength;
      unsigned int elength;
    } header;

    off_t position;

    my::string filename;
    my::string extra;

    my::buffer contents;
  };

  archive::file &select(my::string filename);

private:
  off_t bytes_offset;
  my::vector< shared_ptr<archive::file> > m_contents;
  my::map< my::string, shared_ptr<archive::file> > m_lookup;
};

//template class __MYLO_DLL_EXPORT my::vector< shared_ptr<archive::file> >;
//template class __MYLO_DLL_EXPORT my::map< my::string, shared_ptr<archive::file> >;


__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:

