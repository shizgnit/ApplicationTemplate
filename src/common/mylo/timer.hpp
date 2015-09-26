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
