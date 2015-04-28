#include "archive.hpp"

__MYLO_NAMESPACE_BEGIN

__MYLO_DLL_EXPORT archive::archive() {
  bytes_offset = 0;
  m_contents.push(new file());
  m_contents.last()->position = 0;
}

__MYLO_DLL_EXPORT off_t archive::expect() {
  return(m_contents.last()->expect());
}

__MYLO_DLL_EXPORT off_t archive::offset() {
  return(m_contents.last()->offset_content());
}

__MYLO_DLL_EXPORT off_t archive::read(unsigned char *buffer, off_t bytes) {
  return(m_contents.last()->read(buffer, bytes));
}

__MYLO_DLL_EXPORT off_t archive::write(unsigned char *buffer, off_t bytes) {
  m_contents.last()->write(buffer, bytes);
  bytes_offset += bytes;
  if(m_contents.last()->active() == file::parse_complete) {
    m_lookup[m_contents.last()->filename] = m_contents.last();
    m_contents.push(new file());
    m_contents.last()->position = bytes_offset;
  }
  return(bytes);
}

__MYLO_DLL_EXPORT archive::file &archive::select(my::string filename) {
  my::map< my::string, shared_ptr<archive::file> >::iterator it = m_lookup.find(filename);
  if(it != m_lookup.end()) {
    return *(archive::file *)it->second;
  }
  return(archive::file());
}


__MYLO_DLL_EXPORT off_t archive::file::expect() {
  switch(active()) {
  case(parse_header):    return(sizeof(header));
  case(parse_text):  return(header.flength + header.elength);
  case(parse_contents):  return(header.csize);
  case(parse_complete):  return(0);
  };
  return(0);
}

__MYLO_DLL_EXPORT off_t archive::file::offset() {
  switch(active()) {
  case(parse_header):  return(position);
  };
  return(0);
}

__MYLO_DLL_EXPORT off_t archive::file::offset_content() {
  switch(active()) {
  case(parse_contents):  return(header.csize);
  };
  return(0);
}

__MYLO_DLL_EXPORT off_t archive::file::read(unsigned char *buffer, off_t bytes) {
  return(0);
}

__MYLO_DLL_EXPORT off_t archive::file::write(unsigned char *buffer, off_t bytes) {
  off_t bytes_required = expect();
  if(bytes < bytes_required) {
    return(0);
  }

  switch(active()) {
    case(parse_header): {
      memcpy(&header, buffer, bytes_required);
      transition(parse_text);
      return(bytes_required);
    }
    case(parse_text): {
      char *filename_temporary = new char[header.flength];
      char *extra_temporary = new char[header.elength];

      memcpy(filename_temporary, buffer, header.flength);
      memcpy(extra_temporary, buffer+header.flength, header.elength);

      filename = filename_temporary;
      extra = extra_temporary;

      delete [] filename_temporary;
      delete [] extra_temporary;

      transition(parse_contents);
      return(bytes_required);
    }
    case(parse_contents): {
      if(buffer != NULL) {
        contents.write(buffer, bytes_required);
      }
      transition(parse_complete);
      return(bytes_required);
    }
  };

  return(0);
}

__MYLO_NAMESPACE_END

// Local Variables:
// mode:C++
// End:
