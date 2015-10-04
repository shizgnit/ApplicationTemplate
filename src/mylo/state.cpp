#include "mylo.hpp"

#ifndef __STATE_HPP
#define __STATE_HPP

__MYLX_NAMESPACE_BEGIN

class event {};

template<class T, class E> class state {
public:
  typedef my::command<void (*)(T*, E*)> cmd;

  void callback(unsigned long index, my::string label, void (*fp)(T*, E*)) {
    m_callbacks[index] = my::create_command(label, label, fp);
  }

  void raise() {
    if(m_callbacks[m_current]) {
      m_callbacks[m_current]->exec(this, NULL);
    }
  }

  void raise(E *in) {
    if(m_callbacks[m_current]) {
      m_callbacks[m_current]->exec(this, in);
    }
  }

  unsigned long active() {
    return(m_current);
  }

  void transition(unsigned long index) {
    m_current = index;
  }

private:
  my::vector<cmd *> m_callbacks;
  unsigned long m_current;
};

__MYLX_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
