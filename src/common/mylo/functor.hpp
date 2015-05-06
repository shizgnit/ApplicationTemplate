#include "mylo.hpp"

#ifndef __FUNCTOR_HPP
#define __FUNCTOR_HPP

__MYLO_NAMESPACE_BEGIN

class abstract_functor {
public:
  virtual void exec(my::vector<my::variant> params, my::variant &rval) {}

  abstract_functor(my::string name, my::string desc): _name(name), _desc(desc) {}
  
protected:
  my::string _name;
  my::string _desc;
};

template <class T> struct functor : public abstract_functor {
};



template <class R> struct functor< R (*)() > : public abstract_functor {
  typedef type_traits<R> rtype;
  
  functor(my::string name, my::string desc, R (*fp)()): abstract_functor(name, desc), _fp(fp) {}
  
  R exec() {
    return(_fp());
  }
  
  void exec(my::vector<my::variant> params, my::variant &rval) {
//    rval = _fp();
  }
  
  R (*_fp)();
};

template <class P1> struct functor< void (*)(P1) > : public abstract_functor {
  typedef type_traits<void> rtype;
  typedef type_traits<P1> first;
  
  functor(my::string name, my::string desc, void (*fp)(P1)): abstract_functor(name, desc), _fp(fp) {}
  
  void exec(P1 var1) {
    _fp(var1);
  }
  
  void exec(my::vector<my::variant> params) {
    _fp(params[0]);
  }
  
  void (*_fp)(P1);
};

template <class R, class P1> struct functor< R (*)(P1) > : public abstract_functor {
  typedef type_traits<R> rtype;
  typedef type_traits<P1> first;
  
  functor(my::string name, my::string desc, R (*fp)(P1)): abstract_functor(name, desc), _fp(fp) {}
  
  R exec(P1 var1) {
    return(_fp(var1));
  }
  
  void exec(my::vector<my::variant> params, my::variant &rval) {
    //rval = _fp(params[0]);
  }
  
  R (*_fp)(P1);
};

template <class P1, class P2> struct functor< void (*)(P1, P2) > : public abstract_functor {
  typedef type_traits<void> rtype;
  typedef type_traits<P1> first;
  typedef type_traits<P2> second;
  
  functor(my::string name, my::string desc, void (*fp)(P1, P2)): abstract_functor(name, desc), _fp(fp) {}
  
  void exec(P1 var1, P2 var2) {
    _fp(var1, var2);
  }
  
  void exec(my::vector<my::variant> params) {
    _fp(params[0], params[1]);
  }
  
  void (*_fp)(P1, P2);
};

template <class R, class P1, class P2> struct functor< R (*)(P1, P2) > : public abstract_functor {
  typedef type_traits<R> rtype;
  typedef type_traits<P1> first;
  typedef type_traits<P2> second;
  
  functor(my::string name, my::string desc, R (*fp)(P1, P2)): abstract_functor(name, desc), _fp(fp) {}
  
  R exec(P1 var1, P2 var2) {
    return(_fp(var1, var2));
  }
  
  void exec(my::vector<my::variant> params, my::variant &rval) {
//    rval = _fp(params[0], params[1]);
  }
  
  R (*_fp)(P1, P2);
};

template <class P1, class P2, class P3> struct functor< void (*)(P1, P2, P3) > : public abstract_functor {
  typedef type_traits<void> rtype;
  typedef type_traits<P1> first;
  typedef type_traits<P2> second;
  typedef type_traits<P3> third;
  
  functor(my::string name, my::string desc, void (*fp)(P1, P2, P3)): abstract_functor(name, desc), _fp(fp) {}
  
  void exec(P1 var1, P2 var2, P3 var3) {
    _fp(var1, var2, var3);
  }
  
  void exec(my::vector<my::variant> params) {
    _fp(params[0], params[1], params[2]);
  }
  
  void (*_fp)(P1, P2, P3);
};

template <class R, class P1, class P2, class P3> struct functor< R (*)(P1, P2, P3) > : public abstract_functor {
  typedef type_traits<R> rtype;
  typedef type_traits<P1> first;
  typedef type_traits<P2> second;
  typedef type_traits<P3> third;
  
  functor(my::string name, my::string desc, R (*fp)(P1, P2, P3)): abstract_functor(name, desc), _fp(fp) {}
  
  R exec(P1 var1, P2 var2, P3 var3) {
    return(_fp(var1, var2, var3));
  }
  
  void exec(my::vector<my::variant> params, my::variant &rval) {
//    rval = _fp(params[0], params[1], params[2]);
  }
  
  R (*_fp)(P1, P2, P3);
};


template <class R> functor< R (*)() > *create_functor(my::string name, my::string desc, R (*ffp)()) {
  typedef R (*ffp_t)();
//  typedef typename functor< ffp_t > cmd;
//  return new cmd(name, desc, ffp);
  return new functor< ffp_t > (name, desc, ffp);
}

template <class P1> functor< void (*)(P1) > *create_functor(my::string name, my::string desc, void (*ffp)(P1)) {
  typedef void (*ffp_t)(P1);
//  typedef typename functor< ffp_t > cmd;
//  return new cmd(name, desc, ffp);
  return new functor< ffp_t > (name, desc, ffp);
}

template <class R, class P1> functor< R (*)(P1) > *create_functor(my::string name, my::string desc, R (*ffp)(P1)) {
  typedef R (*ffp_t)(P1);
//  typedef typename functor< ffp_t > cmd;
//  return new cmd(name, desc, ffp);
  return new functor< ffp_t > (name, desc, ffp);
}

template <class P1, class P2> functor< void (*)(P1, P2) > *create_functor(my::string name, my::string desc, void (*ffp)(P1, P2)) {
  typedef void (*ffp_t)(P1, P2);
//  typedef typename functor< ffp_t > cmd;
//  return new cmd(name, desc, ffp);
  return new functor< ffp_t > (name, desc, ffp);
}

template <class R, class P1, class P2> functor< R (*)(P1, P2) > *create_functor(my::string name, my::string desc, R (*ffp)(P1, P2)) {
  typedef R (*ffp_t)(P1, P2);
//  typedef typename functor< ffp_t > cmd;
//  return new cmd(name, desc, ffp);
  return new functor< ffp_t > (name, desc, ffp);
}

template <class P1, class P2, class P3> functor< void (*)(P1, P2, P3) > *create_functor(my::string name, my::string desc, void (*ffp)(P1, P2, P3)) {
  typedef void (*ffp_t)(P1, P2, P3);
//  typedef typename functor< ffp_t > cmd;
//  return new cmd(name, desc, ffp);
  return new functor< ffp_t > (name, desc, ffp);
}

template <class R, class P1, class P2, class P3> functor< R (*)(P1, P2, P3) > *create_functor(my::string name, my::string desc, R (*ffp)(P1, P2, P3)) {
  typedef R (*ffp_t)(P1, P2, P3);
//  typedef typename functor< ffp_t > cmd;
//  return new cmd(name, desc, ffp);
  return new functor< ffp_t > (name, desc, ffp);
}


__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End: