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

#ifndef __PAIR_HPP
#define __PAIR_HPP

__MYLO_NAMESPACE_BEGIN

template <class T, class S> class pair {
public:
  typedef pair<T, S> type;
  typedef type_traits<T> traits_first;
  typedef type_traits<S> traits_second;
  
public:
  pair() {
    traits_first::initialize(first);
    traits_second::initialize(second);
  }
  
  pair(const pair &in) {
    copy(in);
  }

  pair(const T &_first, const S &_second) {
    first = _first;
    second = _second;
  }

  pair &operator = (const pair &in) {
    copy(in);
    return(*this);
  }

  operator bool() const {
    return(traits_first::has_value(first) || traits_second::has_value(second)); 
  }
  
public:
  T first;
  S second;

protected:

  void copy(const pair &in) {
    if(this == &in) {
      return;
    }
    this->first = in.first;
    this->second = in.second;
  }
};

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
