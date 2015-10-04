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

#ifndef __TIMER_HPP
#define __TIMER_HPP

__MYLO_NAMESPACE_BEGIN

class __MYLO_DLL_EXPORT Timer {
  public:
    Timer(void) {
      m_start = 0;
    }

    time_t start(void) {
      m_start = current();
      m_reference = m_offset;
      return(m_start);
    }

    time_t elapse(void) {
      if(m_start == 0 || m_reference != m_offset) {
        return(0);
      }
      return(current() - m_start);
    }

    static time_t current(void) {
      timeb timebuffer;
      ftime(&timebuffer);
      if(m_offset == 0) {
        m_offset = timebuffer.time;
      }
      time_t diff = (timebuffer.time - m_offset);
      if(diff >= 4294967) {
        m_offset = timebuffer.time;
      }
      return((diff * 1000) + timebuffer.millitm);
    }

  private:
    static time_t m_offset;
    time_t m_start;
    time_t m_reference;
};

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
