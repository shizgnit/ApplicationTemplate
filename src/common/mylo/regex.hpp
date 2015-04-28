#include "mylo.hpp"

#ifndef __REGEX_HPP
#define __REGEX_HPP

__MYLO_NAMESPACE_BEGIN
  
#define __RE_BUFFERSIZE 129

#define __RE_CHARSET_UNSET ' '
#define __RE_CHARSET_VALUE 'X'

#define ___ __RE_CHARSET_UNSET
#define _X_ __RE_CHARSET_VALUE

/*
    0   1   2   3   4   5   6   7   8   9
 0 NUL SOH STX ETX EOT ENQ ACK BEL BS  HT
 1  LF  VT  FF  CR  SO  SI DLE DC1 DC2 DC3
 2 DC4 NAK SYN ETB CAN EM  SUB ESC  FS  GS
 3  RS  US  SP  !   "   #   $   %   &   '
 4  (   )   *   +   ,   -   .   /   0   1
 5  2   3   4   5   6   7   8   9   :   ;
 6  <   =   >   ?   @   A   B   C   D   E
 7  F   G   H   I   J   K   L   M   N   O
 8  P   Q   R   S   T   U   V   W   X   Y
 9  Z   [   \   ]   ^   _   `   a   b   c
10  d   e   f   g   h   i   j   k   l   m
11  n   o   p   q   r   s   t   u   v   w
12  x   y   z   {   |   }   ~  DEL
*/

static char __RE_CHARSET_ESCAPE[__RE_BUFFERSIZE] = {
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,_X_,___,___,___,_X_,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,'\0'
};

static char __RE_CHARSET_ANY[__RE_BUFFERSIZE] = {
   ___,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,'\0'
};

static char __RE_CHARSET_SPACE[__RE_BUFFERSIZE] = {
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,_X_,___,___,___,___,___,___,
   ___,___,_X_,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,'\0'
};

static char __RE_CHARSET_CONTROL[__RE_BUFFERSIZE] = {
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,_X_,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,_X_,___,___,___,_X_,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   _X_,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,_X_,___,___,___,_X_,
   ___,___,___,___,___,___,___,___,'\0'
};

static char __RE_CHARSET_LITERAL[__RE_BUFFERSIZE] = {
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,___,_X_,_X_,_X_,
   ___,___,___,___,___,___,___,___,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,___,___,
   _X_,_X_,_X_,___,___,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,___,___,___,___,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,___,_X_,_X_,_X_,'\0'
};

static char __RE_CHARSET_BLANK[__RE_BUFFERSIZE] = {
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,'\0'
};

static char __RE_CHARSET_NUMERIC[__RE_BUFFERSIZE] = {
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,'\0'
};

static char __RE_CHARSET_ALPHA[__RE_BUFFERSIZE] = {
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,___,___,___,___,___,___,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,___,___,___,___,___,'\0'
};

static char __RE_CHARSET_ALPHANUMERIC[__RE_BUFFERSIZE] = {
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,___,___,
   ___,___,___,___,___,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,___,___,___,___,___,___,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,___,___,___,___,___,'\0'
};

#undef ___
#undef _X_


class __MYLO_DLL_EXPORT regex {
  public:
  enum types {
    START   = 0x01,
    STOP    = 0x02,
    SUBEXP  = 0x03,
    LITERAL = 0x04,
    CHARSET = 0x05,
  };

  class state {
    friend class regex;

  private:

    state(enum types type);
    state(char literal, bool negate=false);
    state(char *charset=NULL, bool negate=false);

    ~state(void);

    void init(void);

    state *transition(state *next);
    void boundary(bool greedy, int max, int min);

  private:

    int m_subexp_id;

    int m_type;
    char m_literal;
    char *m_charset;

    bool m_negate;

    bool m_greedy;
    int  m_max;
    int  m_min;

    int  m_count;
    state **m_transitions;

    state *m_root;
  };

  class node {
    friend class regex;

  public:

    ~node(void) { }

  private:

    node(state *current, int index, int count=0, int transition=0) {
      m_current = current;
      m_index = index;
      m_count = count;
      m_transition = transition;
    }

    node(node *instance) {
      m_current = instance->m_current;
      m_index = instance->m_index;
      m_count = instance->m_count;
      m_transition = instance->m_transition;
    }

  private:

    state *m_current;
    int m_index;
    int m_count;
    int m_transition;
  };

  class stack {
    friend class regex;

  private:

    stack(void);

    ~stack(void);

    node *push(node *value);
    node *pop(void);
    node *last(void);

  private:
    node **m_data;
    int m_size;
    int m_count;
  };

public:

  regex(const char *parameter=NULL);

  ~regex(void) { cleanup(m_root); }

  int compile(const char *parameter, state *root=NULL);
  stack *execute(const char *reference);

  vector< string > match(const char *reference);
  vector< pair<int, int> > detailed_match(const char *reference);

private:

  void cleanup(state *root=NULL);

  int m_subexp_count;

  bool m_anchor_front;
  bool m_anchor_back;

  state *m_root;
};

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
