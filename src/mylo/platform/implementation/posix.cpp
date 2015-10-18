/*
================================================================================

Copyright (c) 2012, Dee E. Abbott
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.
* Neither the name of the organization nor the names of its contributors may
be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

================================================================================
*/

#include "posix.hpp"

__PLATFORM_NAMESPACE_BEGIN

bool posix::filesystem::cp(my::string src, my::string dest) {
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

bool posix::filesystem::rm(my::string filename) {
  remove(filename);
}

bool posix::filesystem::mv(my::string src, my::string dest) {
  rename(src, dest);
}

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

bool posix::filesystem::mkdir(my::string path, my::string mask) {
  return(::mkdir(path, _mask(mask)));
}

bool posix::filesystem::rmdir(my::string path) {
  return(::rmdir(path));
}

my::string posix::filesystem::pwd(my::string path) {
  my::string result;
  static char current[2048];

  if(path) {
    chdir(path);
  }
  getcwd(current, 2048);
  result = current;
  return(result);
}

my::vector<unsigned long> posix::filesystem::stat(my::string path) {
  my::vector<unsigned long> results;
  struct stat sst;

  if(!path) {
    return(results);
  }

  if(::stat(path, &sst) < 0) {
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

my::vector<unsigned long> posix::filesystem::lstat(my::string path) {
  my::vector<unsigned long> results;
  struct stat sst;

  if(!path) {
    return(results);
  }

  if(::lstat(path, &sst) < 0) {
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

bool posix::filesystem::exists(my::string path) {
  struct stat sst;
  return(::stat(path, &sst) == 0);
}


my::string posix::filesystem::filetype(my::string path) {
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

__PLATFORM_NAMESPACE_END

// Local Variables:
// mode:C++
// End:
