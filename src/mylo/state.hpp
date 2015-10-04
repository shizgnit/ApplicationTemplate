#include "mylo.hpp"

#ifndef __STATE_HPP
#define __STATE_HPP

__MYLO_NAMESPACE_BEGIN

class event {};

template<class T, class E = my::event> class __MYLO_INTERNAL_DLL_EXPORT state {
public:
  typedef my::functor<void (*)(T*, E*)> cmd;

  virtual ~state() {}

  void callback(unsigned long index, my::string label, void (*fp)(T*, E*)) {
    m_callbacks[index] = my::create_functor(label, label, fp);
  }

  virtual void raise() {
    if(m_callbacks[m_current]) {
      m_callbacks[m_current]->exec((T*)this, NULL);
    }
  }

  virtual void raise(E *in) {
    if(m_callbacks[m_current]) {
      m_callbacks[m_current]->exec((T*)this, in);
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

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
