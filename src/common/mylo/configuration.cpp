#include "configuration.hpp"

__MYLO_NAMESPACE_BEGIN

__MYLO_DLL_EXPORT void configuration::expect(my::string spec, my::string value, my::string alias, bool required) {
  entry specification;

  specification.provided = false;
  specification.alias = alias;
  specification.required = required;
  specification.key = lc(spec);

  if(!value.empty()) {
    specification.source = "default";
    specification.values.push(value);
  }

  m_data.insert(my::pair<my::string, entry>(specification.key, specification));
}

__MYLO_DLL_EXPORT my::vector<my::string> configuration::cmdln(int argc, char **argv, bool passthrough) {
  long offset = 0;

  my::vector<my::string> result;

  for(int i=0; i < argc; i++) {
    if(strlen(argv[i]) > 1 && argv[i][0] == '-') {
      offset = 1;
      if(argv[i][1] == '-') {
        offset = 2;
      }

      if(offset == 2 && strlen(argv[i]) == 2) {
        for(int k=i; k < argc; k++) {
          result.push_back(my::string(argv[k]));
        }
        return(result);
      }

      my::string argument = my::string(argv[i] + offset);
      size_t delimiter = argument.find("=");

      my::string label;
      my::string value;

      if(delimiter == my::string::npos) {
        label = argument;
      }
      else {
        label = argument.substr(0, delimiter);
        value = argument.substr(delimiter+1, argument.length() - delimiter);
      }

      if(!add("cmdln", label, value) || passthrough) {
        result.push_back(my::string(argv[i]));
      }
    }
    else {
      result.push_back(my::string(argv[i]));
    }
  }

  return(result);
}

__MYLO_DLL_EXPORT bool configuration::cgi(void) {
  my::string method = uc(getenv("REQUEST_METHOD"));
  if(!method.empty()) {
    return(false);
  }

  my::string source;

  if(method.compare("GET") == 0) {
    source = getenv("QUERY_STRING");
  }

  if(method.compare("POST") == 0) {
    char *buffer;
    size_t length = my::type_cast<unsigned long>(getenv("CONTENT_LENGTH"));

    buffer = (char *)malloc((length+1) * sizeof(char));
    memset(buffer, 0, (length+1) * sizeof(char));
    fread(buffer, length, 1, stdin);
    source = my::string(buffer);
    free(buffer);
  }

  my::vector<my::string> parameters = my::split("&", source);

  my::vector<my::string>::iterator it = parameters.begin();
  for(; it != parameters.end(); it++) {
    my::string parse = it->second;

    size_t delimiter = parse.find("=");
    if(delimiter != my::string::npos) {
      my::string label = trim(parse.substr(0, delimiter));
      my::string value = trim(parse.substr(delimiter+1, parse.length() - delimiter));

      if(value[0] == '"' && value[value.length()] == '"') {
        value = value.substr(1, value.length()-2);
        continue;
      }

      add("cgi", label, value);
    }
  }

  return(true);
}

__MYLO_DLL_EXPORT bool configuration::ini(my::string filename, my::string category) {
  my::string current;

  my::vector<my::string> contents = my::file(filename);
  my::vector<my::string>::iterator it = contents.begin();
  for(; it != contents.end(); it++) {
    my::string parse = trim(it->second);
    if(parse.empty()) {
      continue;
    }

    if(parse[0] == '[' && parse[parse.length()-1] == ']') {
      current = parse.substr(1, parse.length()-2);
      continue;
    }

    size_t delimiter = parse.find("=");
    if(delimiter != my::string::npos) {
      my::string label = trim(parse.substr(0, delimiter));
      my::string value = trim(parse.substr(delimiter+1, parse.length() - delimiter));

      if(value[0] == '"' && value[value.length()-1] == '"') {
        value = value.substr(1, value.length()-2);
        continue;
      }

      add(join(":", ("file", filename)), label, value);
    }
  }

  return(true);
}

__MYLO_DLL_EXPORT bool configuration::registry(my::string path, bool traverse) {

#if defined __MYLO_WINDOWS

#define MAX_BUFFER 4096

#if defined WIDE
  my::wstring wpath = path;
  wchar_t key[MAX_BUFFER];
#else
  my::string wpath = path;
  char key[MAX_BUFFER];
#endif

  HKEY read;
  if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, wpath.c_str(), 0L, KEY_READ , &read) == ERROR_SUCCESS) {
    int index;
    int status;

    if(traverse) {
      index = 0;
      status = RegEnumKey(read, index, key, MAX_BUFFER);
      while(status == ERROR_SUCCESS) {
#if defined WIDE
        registry(my::join(L"\\", (wpath, key)), traverse);
#else
        registry(my::join("\\", (wpath, key)), traverse);
#endif
        status = RegEnumKey(read, ++index, key, MAX_BUFFER);
      }
    }

    DWORD maximum = MAX_BUFFER;
    DWORD type = 0;

    index = 0;
    status = RegEnumValue(read, index, key, &maximum, NULL, &type, NULL, NULL);
    while(status == ERROR_SUCCESS) {
      my::wstring value = L"???";
      HKEY access;
      if(type == REG_DWORD) {
        DWORD contents;
        DWORD size = 4;
        if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, wpath.c_str(), 0L, KEY_ALL_ACCESS, &access) == ERROR_SUCCESS) {
          if(RegQueryValueEx(access, key, NULL, &type, (LPBYTE)&contents, &size) == ERROR_SUCCESS) {
            value = my::type_cast<my::wstring>(contents);
          }
        }
        RegCloseKey(access);
      }
      if(type == REG_SZ || type == REG_EXPAND_SZ) {
        wchar_t contents[MAX_BUFFER];
        if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, wpath.c_str(), 0L, KEY_ALL_ACCESS, &access) == ERROR_SUCCESS) {
          maximum = MAX_BUFFER;
          if(RegQueryValueEx(access, key, NULL, &type, (LPBYTE)&contents, &maximum) == ERROR_SUCCESS) {
            value = contents;
          }
        }
        RegCloseKey(access);
      }
      if(type == REG_BINARY) {
        value = L"binary";
      }

      add(my::join(":", ("registry", path)), my::type_cast<my::string>(key), value);

      maximum = MAX_BUFFER;
      status = RegEnumValue(read, ++index, key, &maximum, NULL, &type, NULL, NULL);
    }
  }
  RegCloseKey(read);

#endif

  return(true);
}

__MYLO_DLL_EXPORT my::string configuration::getenv(my::string label) {
  return(my::string(::getenv(label.c_str())));
}

__MYLO_DLL_EXPORT configuration::entry &configuration::operator[](my::string key) {
  my::string lookfor = lc(key);

  my::map<my::string, entry>::iterator it = m_data.find(lookfor);
  if(it == m_data.end()) {
    static entry empty;

    empty.provided = false;
    empty.negated = false;
    empty.required = false;

    return(empty);
  }
  return(it->second);
}

__MYLO_DLL_EXPORT my::vector<my::string> configuration::undefined(void) {
  my::vector<my::string> results;

  my::map<my::string, entry>::iterator it = m_data.begin();
  for(; it != m_data.end(); it++) {
    if(!it->second.provided) {
      results.push(it->second.key);
    }
  }
  return(results);
}

__MYLO_DLL_EXPORT my::string configuration::lookup(my::string key, bool &negated, bool negatable) {
  my::string lookfor = lc(key);

  my::map<my::string, entry>::iterator it = m_data.find(lookfor);

  if(it != m_data.end()) {
    return(lookfor);
  }
  else {
    for(it = m_data.begin(); it != m_data.end(); it++) {
      if(it->second.alias.compare(lookfor) == 0) {
        return(it->second.key);
      }
    }
    if(negatable) {
      if(lookfor.length() > 2 && lookfor.substr(0, 2) == "no") {
        negated = true;
        return(lookup(lookfor.substr(2, lookfor.length() - 2), negated, false));
      }
    }
  }

  return("");
}

__MYLO_DLL_EXPORT bool configuration::add(my::string source, my::string label, my::string value) {
  bool negated;
  my::string key = lookup(label, negated);
  if(key.empty()) {
    return(false);
  }

  m_data[key].provided = true;
  m_data[key].negated = negated;
  if(!value.empty()) {
    if(m_data[key].source.compare(source) != 0) {
      m_data[key].values.clear();
      m_data[key].source = source;
    }
    m_data[key].values.push_back(value);
  }

  return(true);
}

__MYLO_DLL_EXPORT my::vector<my::string> configuration::list(void) {
  my::vector<my::string> results;

  return(results);
}

__MYLO_NAMESPACE_END

// Local Variables:
// mode:C++
// End:
