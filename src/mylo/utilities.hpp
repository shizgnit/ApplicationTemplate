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

#include "mylo.hpp"

#ifndef __UTILITIES_HPP
#define __UTILITIES_HPP

/// <summary>
/// Syntactic sugar for lists of strings to a vector.
/// </summary>
__MYLO_DLL_EXPORT my::vector<my::string> operator , (my::string left, my::string right);

/// <summary>
/// Syntactic sugar for a vector and string to a vector.
/// </summary>
__MYLO_DLL_EXPORT my::vector<my::string> operator , (my::vector<my::string> left, my::string right);

/// <summary>
/// Syntactic sugar for lists of wide strings to a vector.
/// </summary>
__MYLO_DLL_EXPORT my::vector<my::wstring> operator , (my::wstring left, my::wstring right);

/// <summary>
/// Syntactic sugar for a vector and wide string to a vector.
/// </summary>
__MYLO_DLL_EXPORT my::vector<my::wstring> operator , (my::vector<my::wstring> left, my::wstring right);

__MYLO_NAMESPACE_BEGIN

__MYLO_DLL_EXPORT my::string rtrim(my::string in);
__MYLO_DLL_EXPORT my::string ltrim(my::string in);
__MYLO_DLL_EXPORT my::string trim(my::string in);

__MYLO_DLL_EXPORT my::string lc(my::string in);
__MYLO_DLL_EXPORT my::string uc(my::string in);

__MYLO_DLL_EXPORT my::string format(my::string str, ...);

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
