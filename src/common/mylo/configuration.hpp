#include "mylo.hpp"

#ifndef __CONFIGURATION_HPP
#define __CONFIGURATION_HPP

__MYLO_NAMESPACE_BEGIN

class __MYLO_DLL_EXPORT configuration {
private:
  struct entry {
    my::string key;
    my::string alias;
    my::string source;
    bool provided;
    bool negated;
    bool required;
    my::vector<my::string> values;
  };

public:
  configuration(void) { /*NULL*/ }
  ~configuration(void) { /*NULL*/ }

  void expect(my::string spec, my::string value="", my::string alias="", bool required=false);

  //
  // The Open Group Base Specifications Issue 6
  // IEEE Std 1003.1, 2004 Edition
  // Copyright (C) 2001-2004 The IEEE and The Open Group, All Rights reserved.
  //
  // 12.1 Utility Argument Syntax
  //
  my::vector<my::string> cmdln(int argc, char **argv, bool passthrough=true);

  //
  // All characters above 127 (7F hex) or below 33 (21 hex) are escaped.
  // This includes the space character, which is escaped as %20.
  // Also, the plus sign (+) needs to be interpreted as a space character.
  //
  bool cgi(void);

  // 
  // Alternative custom sources
  //
  bool ini(my::string filename, my::string category="");
  bool registry(my::string path, bool traverse = false);

  entry &operator[](my::string key);

  my::vector<my::string> undefined(void);
  my::vector<my::string> list(void);

private:
  my::string getenv(my::string label);
  my::string lookup(my::string key, bool &negated, bool negatable=true);
  bool add(my::string source, my::string label, my::string value);

  my::map<my::string, entry> m_data;
};

//template class __MYLO_DLL_EXPORT my::map<my::string, configuration::entry>;

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
