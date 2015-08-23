#include "stream.hpp"

__MYLO_NAMESPACE_BEGIN

__MYLO_DLL_EXPORT stream::stream(const serializable *lval, const serializable *rval) {
  m_chain = new my::vector<serializable *>();
  m_chain->push_back(const_cast<serializable *>(lval));
  m_chain->push_back(const_cast<serializable *>(rval));
  serializable *a = (*m_chain)[0];
  serializable *b = (*m_chain)[1];
}

__MYLO_DLL_EXPORT stream::stream(const stream &in) {
  m_chain = in.m_chain;
}

__MYLO_DLL_EXPORT stream::~stream() {
  if(m_chain.count() > 1) {
    return;
  }
  unsigned char temporary[stream::alloc];
  off_t bytes_available;
  off_t bytes_read;
  off_t bytes_written;
  off_t bytes_expected;
  off_t bytes_offset;
  do {
    bytes_read = bytes_written = 0;
    for(int i=0, end=m_chain->size()-1; i<end; i++) {
      do {
        bytes_offset = (*m_chain)[i+1]->offset();
        if(bytes_offset && (*m_chain)[i]->seek((*m_chain)[i]->location() + bytes_offset)) {
          bytes_read = bytes_offset;
          (*m_chain)[i+1]->write(NULL, bytes_offset);
        }
        else {
          bytes_expected = (*m_chain)[i+1]->expect();
          bytes_read += (bytes_available = (*m_chain)[i]->read(temporary, bytes_expected));
          bytes_written += (*m_chain)[i+1]->write(temporary, bytes_available);
        }
      } while((*m_chain)[i]->cached());
    }
  } while(bytes_read || bytes_written);
}

__MYLO_DLL_EXPORT stream &stream::operator << (const serializable &rval) {
  m_chain->resize(m_chain->size()+1);
  for(int i=m_chain->size()-1; i>0; i--) {
    serializable *ptr = (*m_chain)[i-1];
    (*m_chain)[i] = (*m_chain)[i-1];
  }
  (*m_chain)[0] = const_cast<serializable *>(&rval);
  return(*this);
}

__MYLO_DLL_EXPORT stream &stream::operator >> (const serializable &rval) {
  m_chain->push_back(const_cast<serializable *>(&rval));
  return(*this);
}

__MYLO_DLL_EXPORT serializable::serializable() {
}

__MYLO_DLL_EXPORT serializable::~serializable() {
}

__MYLO_DLL_EXPORT off_t serializable::cached() {
  return(m_cache ? m_cache->size() : 0);
}

__MYLO_DLL_EXPORT my::string serializable::getline(void) {
  char *characters = NULL;
  off_t length = 0;
  
  unsigned char temporary[stream::alloc];
  off_t bytes_available = 0;
  
  if(m_cache && m_cache->size()) {
    memset(temporary, 0, stream::alloc);
    bytes_available = m_cache->read(temporary, stream::alloc);
    characters = new char[bytes_available + 1];
    memcpy(characters, temporary, bytes_available);
    characters[bytes_available] = 0;
  }

  while(1) {
    if(bytes_available == 0) {
      memset(temporary, 0, stream::alloc);
      bytes_available = read(temporary, stream::alloc);
      if(bytes_available == 0) {
        break;
      }
    }
    
    int i=0, newline=0;
    for(; i<bytes_available; i++) {
      if(temporary[i] == 0x0A || temporary[i] == 0x0D) {
        newline = i;
        break;
      }
    }
    i += 1;

    if(bytes_available < stream::alloc && !newline) {
      newline = bytes_available;
    }

    char *accumulation = new char[length+i+1];
    memcpy(accumulation, characters, length);
    memcpy(accumulation + length, temporary, i);
    
    length += i;
    accumulation[length] = 0;
    
    delete [] (characters);
    characters = accumulation;
    
    if(i<=bytes_available) {
      while(i < bytes_available && (temporary[i] == 0x0A || temporary[i] == 0x0D)) {
        i += 1;
      }
      memcpy(temporary, temporary+i, bytes_available - i);
      bytes_available -= i;
    }
    else {
      bytes_available = 0;
    }
    
    if(newline) {
      break;
    }
  }

  if(characters == NULL) {
    return(my::string());
  }
  
  my::string results = characters;
  
  if(bytes_available) {
    m_cache = new my::buffer();
    m_cache->write(temporary, bytes_available);
  }

  delete [] (characters);
  return(results);
}

__MYLO_DLL_EXPORT serializable::operator my::string (void) {
  DEBUG_SCOPE;

  char *characters = NULL;
  off_t length = 0;
  
  unsigned char temporary[stream::alloc];
  off_t bytes_available = 0;

  memset(temporary, 0, stream::alloc);

  if(m_cache && m_cache->size()) {
    bytes_available = m_cache->read(temporary, stream::alloc);
    characters = new char[bytes_available + 1];
    memcpy(characters, temporary, bytes_available);
    characters[bytes_available] = 0;
    DEBUG_TRACE << "Termination of serializable characters at " << bytes_available << endl;
  }

  while(1) {
    bytes_available = read(temporary, stream::alloc);
    if(bytes_available == 0) {
      break;
    }
    
    char *accumulation = new char[length+bytes_available+1];
    memcpy(accumulation, characters, length);
    memcpy(accumulation + length, temporary, bytes_available);
    
    length += bytes_available;
    accumulation[length] = 0;
    
    DEBUG_TRACE << "Further termination of serializable characters at " << length << endl;

    delete [] (characters);
    characters = accumulation;
  }

  if(characters == NULL) {
    return(my::string());
  }
  
  my::string results = characters;
  
  delete [] (characters);
  return(results);
}

__MYLO_DLL_EXPORT serializable::operator my::vector<my::string> (void) {
  return(tokenize(*this));
}

__MYLO_DLL_EXPORT off_t serializable::write(my::string &in) {
  return(write((unsigned char *)in.c_str(), in.size()));
}

__MYLO_DLL_EXPORT off_t serializable::write(my::vector<my::string> &in) {
  off_t total = 0;
  my::vector<my::string>::iterator it = in.begin();
  for(;it != in.end(); it++) {
    total += write((unsigned char *)it->second.c_str(), it->second.size());
  }
  return(total);
}

__MYLO_DLL_EXPORT void serializable::path(my::string rpath) {
  search_path = rpath;
}


__MYLO_NAMESPACE_END
