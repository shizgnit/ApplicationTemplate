#include "filesystem.hpp"

__MYLO_NAMESPACE_BEGIN

class directory_handle {
  public:
#if defined __MYLO_WINDOWS
  HANDLE handle;
  WIN32_FIND_DATA *last;
#else
  DIR *handle;
  struct dirent *last;
#endif
};


//------------------------------------------------------------------------------
#if defined __MYLO_WINDOWS

__MYLO_DLL_EXPORT bool cp(my::string srcfile, my::string dest) {
  DWORD dwAttrs;

#if defined WIDE
  if (CopyFile(type_cast<my::wstring>(srcfile).c_str(), type_cast<my::wstring>(dest).c_str(), FALSE)) {
    dwAttrs = GetFileAttributes(type_cast<my::wstring>(srcfile).c_str());
    if (!(dwAttrs & FILE_ATTRIBUTE_READONLY)) {
      SetFileAttributes(type_cast<my::wstring>(dest).c_str(), dwAttrs | FILE_ATTRIBUTE_READONLY);
    }
  }
#else
  if (CopyFile(type_cast<my::string>(srcfile).c_str(), type_cast<my::string>(dest).c_str(), FALSE)) {
    dwAttrs = GetFileAttributes(type_cast<my::string>(srcfile).c_str());
    if (!(dwAttrs & FILE_ATTRIBUTE_READONLY)) {
      SetFileAttributes(type_cast<my::string>(dest).c_str(), dwAttrs | FILE_ATTRIBUTE_READONLY);
    }
  }
#endif
  else {
    return(false);
  }
  return(true);
}

#else

bool cp(my::string src, my::string dest) {
  long fdin, fdout;
  void *sbuf, *dbuf;
  struct stat sst;

  if((fdin = open(src, O_RDONLY)) < 0) {
    printf("error, unable to open source file\n");
    return(false);
  }

  if(fstat(fdin, &sst) < 0) {
    printf("error, unable to stat source file\n");
    return(false);
  }

  if( (fdout = open(dest, O_RDWR|O_CREAT|O_TRUNC, sst.st_mode)) < 0) {
    printf("error, unable to open target file.\n");
    return(false);
  }

  if(lseek(fdout, sst.st_size - 1, SEEK_SET) == -1) {
    printf("error\n");
    return(false);
  }

  if(write(fdout, "", 1) != 1) {
    printf("error\n");
    return(false);
  }

  if( (sbuf = mmap(0, sst.st_size, PROT_READ, MAP_FILE|MAP_SHARED, fdin, 0)) == (caddr_t) -1) {
    printf("error\n");
    return(false);
  }

  if( (dbuf = mmap(0, sst.st_size, PROT_READ|PROT_WRITE, MAP_FILE|MAP_SHARED, fdout, 0)) == (caddr_t) -1) {
    printf("error\n");
    return(false);
  }

  memcpy(dbuf, sbuf, sst.st_size);

  return(true);
}

#endif

//------------------------------------------------------------------------------
#if defined __MYLO_WINDOWS

__MYLO_DLL_EXPORT bool rm(my::string filename) {
#if defined WIDE
  DeleteFile(type_cast<my::wstring>(filename).c_str());
#else
  DeleteFile(type_cast<my::string>(filename).c_str());
#endif
  return(true);
}

#else

bool rm(my::string filename) {
  remove(filename);
}

#endif

//------------------------------------------------------------------------------
#if defined __MYLO_WINDOWS

__MYLO_DLL_EXPORT bool mv(my::string srcfile, my::string dest) {
#if defined WIDE
  MoveFile(type_cast<my::wstring>(srcfile).c_str(), type_cast<my::wstring>(dest).c_str());
#else
  MoveFile(type_cast<my::string>(srcfile).c_str(), type_cast<my::string>(dest).c_str());
#endif
  return(true);
}

#else

bool mv(my::string src, my::string dest) {
  rename(src, dest);
}

#endif

//------------------------------------------------------------------------------
#if defined __MYLO_WINDOWS

__MYLO_DLL_EXPORT bool mkdir(my::string path) {
  return(CreateDirectory(type_cast<my::wstring>(path).c_str(), NULL) ? true : false);
  //return(CreateDirectory(type_cast<my::string>(path).c_str(), NULL) ? true : false);
}

#else

mode_t _mask(my::string &mask) {
  mode_t bytes = 0;

  if(mask.empty()) {
    return(bytes);
  }
  
  char *cmask = mask.c_str();
  int offset=strlen(cmask);

  bool valid = false;

  my::string octal;
  if(offset == 3 && mask <= 511 && mask > 0) {
    octal  = base(mask, 8);
    cmask  = octal.c_str();
    offset = strlen(cmask);
    valid  = true;
  }
  if(offset == 4 && mask <= 7777 && mask > 0) {
    valid = true;
  }

  if(valid) {
    char oth = (int)(cmask[offset-1] - 48);
    char grp = (int)(cmask[offset-2] - 48);
    char usr = (int)(cmask[offset-3] - 48);
    if(offset == 4) {
      char stk = (int)(cmask[offset-4] - 48);
    }
    bytes |= (oth);
    bytes |= (grp << 3);
    bytes |= (usr << 6);
  }

  return(bytes);
}

bool mkdir(my::string path, my::string mask) {
  return(::mkdir(path, _mask(mask)));
}

#endif

//------------------------------------------------------------------------------
#if defined __MYLO_WINDOWS

__MYLO_DLL_EXPORT bool rmdir(my::string path) {
#if defined WIDE
  return(RemoveDirectory(type_cast<my::wstring>(path).c_str()) ? true : false);
#else
  return(RemoveDirectory(type_cast<my::string>(path).c_str()) ? true : false);
#endif
}

#else

bool rmdir(my::string path) {
  return(::rmdir(path));
}

#endif

//------------------------------------------------------------------------------
#if defined __MYLO_WINDOWS

__MYLO_DLL_EXPORT my::string pwd(my::string path) {
  static wchar_t current[2048];
  //static char current[2048];

  if(!path.empty()) {
    SetCurrentDirectory(type_cast<my::wstring>(path).c_str());
    //SetCurrentDirectory(type_cast<my::string>(path).c_str());
  }

  GetCurrentDirectory(2048, current);
  return(my::wstring(current));
}

#else

my::string pwd(my::string path) {
  my::string result;
  static char current[2048];

  if(path) {
    chdir(path);
  }
  getcwd(current, 2048);
  result = current;
  return(result);
}

#endif

//------------------------------------------------------------------------------
#if defined __MYLO_WINDOWS

__MYLO_DLL_EXPORT my::vector<unsigned long> stat(my::string path) {
  my::vector<unsigned long> results;

  struct _stat sst;

  if(_stat(path.c_str(), &sst) != 0) {
    return(results);
  }

  results.reserve(13);

  results[12] = 0;
  results[11] = 0;
  results[10] = (unsigned long)sst.st_ctime;
  results[9]  = (unsigned long)sst.st_mtime;
  results[8]  = (unsigned long)sst.st_atime;
  results[7]  = sst.st_size;
  results[6]  = 0;
  results[5]  = 0;
  results[4]  = 0;
  results[3]  = 0;
  results[2]  = sst.st_mode;
  results[1]  = sst.st_dev + 'A';
  results[0]  = 0;

  return(results);
}

__MYLO_DLL_EXPORT my::vector<unsigned long> lstat(my::string path) {
  return(stat(path));
}

__MYLO_DLL_EXPORT bool exists(my::string path) {
  struct _stat sst;
  return(_stat(path.c_str(), &sst) == 0);
}

#else

my::vector<unsigned long> stat(my::string path) {
  my::vector<unsigned long> results;
  struct stat sst;

  if(!path) {
    return(results);
  }

  if(stat(path, &sst) < 0) {
    return(results);
  }

  results.reserve(13);
  
  results[12] = sst.st_blocks;
  results[11] = sst.st_blksize;
  results[10] = sst.st_ctime;
  results[9]  = sst.st_mtime;
  results[8]  = sst.st_atime;
  results[7]  = sst.st_size;
  results[6]  = sst.st_rdev;
  results[5]  = sst.st_gid;
  results[4]  = sst.st_uid;
  results[3]  = sst.st_nlink;
  results[2]  = sst.st_mode;
  results[1]  = sst.st_dev;
  results[0]  = sst.st_ino;

  return(results);
}

my::vector<unsigned long> lstat(my::string path) {
  my::vector<unsigned long> results;
  struct stat sst;

  if(!path) {
    return(results);
  }

  if(lstat(path, &sst) < 0) {
    return(results);
  }

  results.reserve(13);
  
  results[12] = sst.st_blocks;
  results[11] = sst.st_blksize;
  results[10] = sst.st_ctime;
  results[9]  = sst.st_mtime;
  results[8]  = sst.st_atime;
  results[7]  = sst.st_size;
  results[6]  = sst.st_rdev;
  results[5]  = sst.st_gid;
  results[4]  = sst.st_uid;
  results[3]  = sst.st_nlink;
  results[2]  = sst.st_mode;
  results[1]  = sst.st_dev;
  results[0]  = sst.st_ino;

  return(results);
}

#endif

//------------------------------------------------------------------------------
#if defined __MYLO_WINDOWS

__MYLO_DLL_EXPORT my::string filetype(my::string path) {
  my::string result = "unknown";

  my::vector<unsigned long> stats = stat(path);
  if(stats) {
    unsigned long mode = stats[2];

    if(mode & _S_IFREG)  { result = "regular"; }
    if(mode & _S_IFDIR)  { result = "directory"; }
  }

  return(result);
}

#else

__MYLO_DLL_EXPORT my::string filetype(my::string path) {
  my::string result;

  my::vector<unsigned long> stats = stat(path);
  if(stats) {
    unsigned long mode = stats[2];

    if(S_ISREG(mode))  { result = "regular"; }
    if(S_ISDIR(mode))  { result = "directory"; }
    if(S_ISCHR(mode))  { result = "character"; }
    if(S_ISBLK(mode))  { result = "block"; }
    if(S_ISFIFO(mode)) { result = "fifo"; }

#if defined S_ISLNK
    if(S_ISLNK(mode))  { result = "link"; }
#endif

#if defined S_ISSOCK
    if(S_ISSOCK(mode)) { result = "socket"; }
#endif
  }

  return(result);
}
#endif

//==============================================================================
// directory
//==============================================================================

//------------------------------------------------------------------------------
__MYLO_DLL_EXPORT directory::directory(my::string path) {
  m_path = "";

  m_dha = new directory_handle;
  m_dha->last   = NULL;
  m_dha->handle = NULL;
  if(!path.empty()) {
    this->open(path);
  }
}

//------------------------------------------------------------------------------
__MYLO_DLL_EXPORT directory::~directory(void) {
  this->close();
  if(m_dha) {
    delete(m_dha);
    m_dha = NULL;
  }
}

//------------------------------------------------------------------------------
__MYLO_DLL_EXPORT directory::operator my::vector<my::string> (void) {
  my::vector<my::string> results;
  my::string entry;
  while(1) {
    my::string entry = this->next();
    if(entry.empty()) {
      break;
    }
    results.push_back(entry);
  }
  return(results);
}

//------------------------------------------------------------------------------
#if defined __MYLO_WINDOWS

__MYLO_DLL_EXPORT bool directory::open(my::string path) {
  this->close();
  
  if(!path.empty()) {
    if(path[path.length()-1] == L'\\') {
      path.append("*");
    }
    m_path = path;
  }
  else {
    return(false);
  }
  return(true);
}

#else

bool directory::open(my::string path) {
  this->close();
  if(!path) {
    return(false);
  }

  m_path = path;
  if(m_path) {
    m_dha->handle = opendir(m_path);
    if(m_dha->handle == NULL) {
      return(false);
    }
  }
  else {
    return(false);
  }

  return(true);
}

#endif

//------------------------------------------------------------------------------
#if defined __MYLO_WINDOWS

__MYLO_DLL_EXPORT bool directory::close() {
  if(m_dha->last) {
    delete m_dha->last;
  }
  if(m_dha->handle) {
    FindClose(m_dha->handle);
  }

  return(true);
}

#else

bool directory::close() {
  if(m_dha->last) {
    delete m_dha->last;
  }
  if(m_dha->handle) {
    closedir(m_dha->handle);
  }

  return(true);
}

#endif


//------------------------------------------------------------------------------
#if defined __MYLO_WINDOWS

__MYLO_DLL_EXPORT my::string directory::next() {
  my::string value;
  if(!m_dha->last) {
    m_dha->last = new WIN32_FIND_DATA;
#if defined WIDE
    m_dha->handle = FindFirstFile(type_cast<my::wstring>(m_path).c_str(), m_dha->last);
#else
    m_dha->handle = FindFirstFile(type_cast<my::string>(m_path).c_str(), m_dha->last);
#endif
    if (m_dha->handle == INVALID_HANDLE_VALUE) {
      //ERROR(UNDEFINED_ERROR);
      return(value);
    }
    value = type_cast<my::string>(m_dha->last->cFileName);
  }
  else {
    if(FindNextFile(m_dha->handle, m_dha->last)) {
      value = type_cast<my::string>(m_dha->last->cFileName);
    }
    else {
      if (GetLastError() == ERROR_NO_MORE_FILES) {
        return(value);
      }
      else {
        //ERROR(UNDEFINED_ERROR);
        return(value);
      }
    }
  }
  return(value);
}

#else

my::string directory::next() {
  my::string value;

  if(m_dha->handle) {
    if(m_dha->last = readdir(m_dha->handle)) {
      value = m_dha->last->d_name;
    }
  }

  return(value);
}

#endif

__MYLO_DLL_EXPORT bool file::open(my::string in, file::modes mode) {
  if(m_fp == NULL) {
    m_size = 0;
    m_file = in;
    if(mode & file::wo && !exists(m_file)) {
      FILE *init;
      if(init = fopen(m_file.c_str(), "wb+")) {
        fclose(init);
      }
    }
    if(mode & file::ro) {
      if(mode & file::wo) {
        m_fp = fopen(m_file.c_str(), "rb+");
      }
      if(!m_fp) {
        m_fp = fopen(m_file.c_str(), "rb");
      }
      if(m_fp) {
        fseeko(m_fp, 0, SEEK_END);
        m_size = location();
        fseeko(m_fp, 0, SEEK_SET);
      }
    }
    if(m_fp != NULL) {
#if defined __MYLO_WINDOWS
      m_fd = _fileno(m_fp);
#else
      m_fd = fileno(m_fp);
#endif
    }
  }
  return(m_fp ? true : false);
}

__MYLO_DLL_EXPORT void file::close(void) {
  if(m_fp != NULL) {
    fflush(m_fp);
    fclose(m_fp);
  }
  m_fp = NULL;
}

__MYLO_DLL_EXPORT off_t file::location(void) {
  return(ftello(m_fp));
}

__MYLO_DLL_EXPORT bool file::seek(off_t pos) {
  bool success = (fseeko(m_fp, pos, SEEK_SET) == 0);
  return(success);
}

__MYLO_DLL_EXPORT bool file::exists(my::string file) {
#if defined __MYLO_WINDOWS
  struct _stat sst;
  return(_stat(file.c_str(), &sst) == 0);
#else
  struct stat sst;
  return(stat(file.c_str(), &sst) == 0);
#endif
}

__MYLO_DLL_EXPORT off_t file::read(unsigned char *buffer, off_t bytes) {
  off_t request = bytes + location() > m_size ? m_size - location() : bytes; 
  return(feof(m_fp) ? 0 : fread(buffer, 1, request, m_fp));
}

__MYLO_DLL_EXPORT off_t file::write(unsigned char *buffer, off_t size) {
  return(fwrite(buffer, 1, size, m_fp));
}

__MYLO_NAMESPACE_END

// Local Variables:
// mode:C++
// End:
