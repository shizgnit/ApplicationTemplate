#include "mylo.hpp"

#ifndef __UTILITIES_HPP
#define __UTILITIES_HPP

__MYLO_DLL_EXPORT my::vector<my::string> operator , (my::string left, my::string right);
__MYLO_DLL_EXPORT my::vector<my::string> operator , (my::vector<my::string> left, my::string right);

__MYLO_DLL_EXPORT my::vector<my::wstring> operator , (my::wstring left, my::wstring right);
__MYLO_DLL_EXPORT my::vector<my::wstring> operator , (my::vector<my::wstring> left, my::wstring right);

__MYLO_NAMESPACE_BEGIN

__MYLO_DLL_EXPORT my::string rtrim(my::string in);
__MYLO_DLL_EXPORT my::string ltrim(my::string in);
__MYLO_DLL_EXPORT my::string trim(my::string in);

__MYLO_DLL_EXPORT my::string lc(my::string in);
__MYLO_DLL_EXPORT my::string uc(my::string in);
  
__MYLO_DLL_EXPORT my::string join(my::string, my::vector<my::string> arguments);
__MYLO_DLL_EXPORT my::wstring join(my::wstring, my::vector<my::wstring> arguments);
__MYLO_DLL_EXPORT my::vector<my::string> split(my::string expression, my::string buffer, int limit=0);

__MYLO_DLL_EXPORT my::vector<char *> segment(char *input, char delimiter='\0');
__MYLO_DLL_EXPORT my::vector<my::string> tokenize(my::string input, my::string delimiter="");

__MYLO_DLL_EXPORT my::string substitute(my::string input, my::string expression, my::string replacement, bool global=true);
__MYLO_DLL_EXPORT my::string substitute(my::string input, my::vector<my::string> &replacement);

__MYLO_DLL_EXPORT my::string base(my::string in, int target_ordinal, int current_ordinal=10);
__MYLO_DLL_EXPORT my::string base64(my::string in);

__MYLO_DLL_EXPORT my::string hostname();
__MYLO_DLL_EXPORT my::string ip(my::string hostname);

__MYLO_DLL_EXPORT my::string mac();

__MYLO_DLL_EXPORT unsigned long pid();

__MYLO_NAMESPACE_END

#endif
  
// Local Variables:
// mode:C++
// End:
