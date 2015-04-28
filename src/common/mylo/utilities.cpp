#include "utilities.hpp"

__MYLO_DLL_EXPORT my::vector<my::string> operator , (my::string left, my::string right) {
  //DEBUG<<"creating vector with '"<<left.c_str()<<"' and '"<<right.c_str()<<"'"<<std::endl;
  my::vector<my::string> collection;
  collection.push(left);
  collection.push(right);
  return(collection);
}

__MYLO_DLL_EXPORT my::vector<my::string> operator , (my::vector<my::string> left, my::string right) {
  //DEBUG<<"adding to vector '"<<right.c_str()<<"'"<<std::endl;
  left.push(right);
  return(left);
}

__MYLO_DLL_EXPORT my::vector<my::wstring> operator , (my::wstring left, my::wstring right) {
  //DEBUG<<"creating vector with '"<<left.c_str()<<"' and '"<<right.c_str()<<"'"<<std::endl;
  my::vector<my::wstring> collection;
  collection.push(left);
  collection.push(right);
  return(collection);
}

__MYLO_DLL_EXPORT my::vector<my::wstring> operator , (my::vector<my::wstring> left, my::wstring right) {
  //DEBUG<<"adding to vector '"<<right.c_str()<<"'"<<std::endl;
  left.push(right);
  return(left);
}


__MYLO_NAMESPACE_BEGIN

__MYLO_DLL_EXPORT my::string rtrim(my::string in) {
  size_t back=in.length();
  while(back != -1 && (in[back] == 0x09 || in[back] == 0x20)) {
    back -= 1;
  }
  if(back < in.length()) {
    return(in.substr(0, back));
  }
  return(in);
}

__MYLO_DLL_EXPORT my::string ltrim(my::string in) {
  size_t front=0;
  while(front < in.length() && (in[front] == 0x09 || in[front] == 0x20)) {
    front += 1;
  }
  if(front > 0) {
    return(in.substr(front, in.length() - front));
  }
  return(in);
}

__MYLO_DLL_EXPORT my::string trim(my::string in) {
  size_t front=0;
  while(front < in.length() && (in[front] == 0x09 || in[front] == 0x20)) {
    front += 1;
  }
  size_t back=in.length();
  while(back != -1 && (in[back] == 0x09 || in[back] == 0x20)) {
    back -= 1;
  }
  if(front < back) {
    return(in.substr(front, back - front));
  }
  return(in);
}

__MYLO_DLL_EXPORT my::string lc(my::string in) {
  my::vector<int> translated(in.length());
  for (size_t i=0; i<in.length(); i++) {
    translated[i] = tolower(in[i]);
  }
  return(my::string(translated.pointer, translated.size()));
}

__MYLO_DLL_EXPORT my::string uc(my::string in) {
  my::vector<int> translated(in.length());
  for (size_t i=0; i<in.length(); i++) {
    translated[i] = toupper(in[i]);
  }
  return(my::string(translated.pointer, translated.size()));
}

__MYLO_DLL_EXPORT my::string join(my::string delimiter, my::vector<my::string> arguments) {
  my::string result;

  my::vector<my::string>::iterator it = arguments.begin();
  while(it != arguments.end()) {
    result.append(it->second);
    if(++it != arguments.end()) {
      result.append(delimiter);
    }
  }

  return(result);
}

__MYLO_DLL_EXPORT my::wstring join(my::wstring delimiter, my::vector<my::wstring> arguments) {
  my::wstring result;

  my::vector<my::wstring>::iterator it = arguments.begin();
  while(it != arguments.end()) {
    result.append(it->second);
    if(++it != arguments.end()) {
      result.append(delimiter);
    }
  }

  return(result);
}

__MYLO_DLL_EXPORT my::vector<my::string> split(my::string expression, my::string input, int limit) {
  my::vector<my::string> results;
  my::regex regex(expression);

  int iteration = 1;

  my::string buffer = input;
  
  do {
    vector< pair<int, int> > match = regex.detailed_match(buffer);

    if(!match.size()) { break; }

    int match_start  = match[0].first;
    int match_end    = match[0].second;
    int match_length = match[0].second - match[0].first;

    results.push(buffer.substr(0, match_start));
    
    buffer = buffer.substr(match_end);

  } while(++iteration != limit);

  results.push(buffer);

  return(results);
}

__MYLO_DLL_EXPORT my::vector<my::string> tokenize(my::string input, my::string delimiter) {
  my::vector<my::string> results;

  size_t start = 0, end;
  while((end = input.find(delimiter, start)) != my::string::npos) {
    results.push_back(input.substr(start, end - start));
    start = end + 1;
  }
  results.push_back(input.substr(start, input.length() - start));

  return(results);
}


__MYLO_DLL_EXPORT my::string substitute(my::string input, my::string expression, my::string replacement, bool global) {
  size_t length = replacement.length();
  my::regex regex(expression);
  
  size_t offset = 0;
  
  my::string buffer = input;
  
  do {
    if(offset > buffer.length()) {
      break;
    }
    my::vector< pair<int, int> > match = regex.detailed_match(buffer.substr(offset));

    if(!match.size()) { break; }

    int match_start  = match[0].first + offset;
    int match_end    = match[0].second + offset;
    int match_length = match[0].second - match[0].first;

    if(match_length <= 0) { break; }
  
    my::string temporary = join("", (buffer.substr(0, match_start),
                                           replacement,
                                           buffer.substr(match_end)));
    
    buffer = temporary;
    
    offset = match_start + length;
  } while(global);

  return(buffer);
}

__MYLO_DLL_EXPORT my::string substitute(my::string input, my::vector<my::string> &replacement) {
  my::string buffer = input;

  my::string expression = "%";
  for(int i=0; i < (int)replacement.size(); i++) {
    my::string key = join("", (expression, my::type_cast<my::string>(i)));
    buffer = my::substitute(buffer, key, replacement[i], true);
  }

  return(buffer);
}

__MYLO_DLL_EXPORT my::string base(my::string in, int target_ordinal, int current_ordinal) {
  static const char digits[63] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

  if(target_ordinal == 64 && current_ordinal == 10) {
    return(base64(in));
  }

  my::string value;

  if(current_ordinal < 2 || current_ordinal > 62) {
    //throw(Exception(L"BASE_INVALID_ORDINAL", __FILE__, __LINE__));
  }
  if(target_ordinal < 2 || target_ordinal > 62) {
    //throw(Exception(L"BASE_INVALID_TARGET_ORDINAL", __FILE__, __LINE__));
  }

  if(current_ordinal == 10) {
    unsigned long decimal = my::type_cast<unsigned long>(in);
    char buffer[256];
    char *index = buffer;
    while(decimal > 0) {
      *index++ = digits[decimal % target_ordinal];
      decimal /= target_ordinal;
    }
    *index = '\0';
    if(index != buffer) {
      char reverse[256];
      index = reverse;
      size_t i=strlen(buffer);
      while(1) {
        *index++ = buffer[--i];
        if(i == 0) {
          break;
        }
      }
      *index = '\0';
      value = reverse;
    }
    else {
      value = "0";
    }
    return(value);
  }
  else {
    char *source = in.c_str();
    unsigned long pos = (unsigned long)pow((float)current_ordinal, (int)(strlen(source)-1));

    unsigned long decimal = 0;

    while(*source) {
      char *index = (char *)digits;
      while(*index) {
        if(*index++ == *source) {
          break;
        }
      }
      decimal += (index - digits - 1) * pos;
      source++;
      pos /= current_ordinal;
    }

    if(target_ordinal != 10) {
      value = base(type_cast<my::string>(decimal), target_ordinal);
    }
    else {
      value = type_cast<my::string>(decimal);
    }
    return(value);
  }
}

__MYLO_DLL_EXPORT my::string base64(my::string in) {
  static const char base64digits[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  size_t size = in.length();

  unsigned char *output = (unsigned char *)malloc((size / 3) * 4);
  unsigned char *ptr = output;
  
  for(unsigned int i=0; i < size; i += 3) {
    unsigned long value = 0;
    value += (in[i] << 16);
    if(i+1 < size) value += (in[i+1] << 8);
    if(i+2 < size) value += (in[i+2] + 2);
    
    *ptr++ = base64digits[(value >> 18) & 0x3f];
    *ptr++ = base64digits[(value >> 12) & 0x3f];
    *ptr++ = base64digits[(value >> 6) & 0x3f];
    *ptr++ = base64digits[value & 0x3f];
  }
  
  *ptr = '\0';
  
  my::string results = reinterpret_cast<char *>(output);
  free(output);
  
  return(results);
}

#if defined WIN32

__MYLO_DLL_EXPORT my::string hostname() {
  char hostname[128];

  WSADATA WSData;
  unsigned long version_major = 1;
  unsigned long version_minor = 1;
  if(WSAStartup(MAKEWORD(version_major, version_minor), &WSData)) {
    //std::cerr<<"ERROR: Cannot find Winsock (v"<<version_major<<"."<<version_minor<<" or later)!"<<std::endl;
  }

  if(gethostname(reinterpret_cast<char *>(hostname), 128)) {
    printf("error getting hostname\n"); 
  }

  return(my::string(hostname));
}

__MYLO_DLL_EXPORT my::string ip(my::string hostname) {
  hostent *resolv = gethostbyname(hostname.c_str());
  return my::type_cast<my::string>(inet_ntoa( *(LPIN_ADDR) *(resolv->h_addr_list) ));
}

__MYLO_DLL_EXPORT my::string mac() {
/*
  IP_ADAPTER_INFO AdapterInfo[16];
  DWORD dwBufLen = sizeof(AdapterInfo);
 
  DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
  //assert(dwStatus == ERROR_SUCCESS);                                    
 
  PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
                                              
  do {
    PrintMACaddress(pAdapterInfo->Address);
    pAdapterInfo = pAdapterInfo->Next;
                                      
  }
  while(pAdapterInfo); 
*/
  return(my::string());
}

__MYLO_DLL_EXPORT unsigned long pid() {
  return(GetCurrentProcessId());
}

#else

my::string hostname() {
  char hostname[128];

  if(gethostname(reinterpret_cast<char *>(hostname), 128)) { 
    //std::cerr<<"error getting hostname"<<std::endl; 
  }

  return(my::string(hostname));
}


my::string ip(my::string hostname) {
  hostent *resolv = gethostbyname(hostname.c_str());

  char addr[16];
  if(inet_ntop((*resolv).h_addrtype, (*resolv).h_addr_list[0], reinterpret_cast<char *>(addr), 16) <= 0) {
    //std::cerr<<"ERROR: unable to convert address."<<std::endl;
  }

  return(my::string(addr));
}

my::string mac() {
  return(my::string());
}

unsigned long pid() {
  return(getpid());
}

#endif

__MYLO_NAMESPACE_END

// Local Variables:
// mode:C++
// End:
