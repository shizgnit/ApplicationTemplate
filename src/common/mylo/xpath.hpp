#include "mylo.hpp"

#ifndef __XPATH_HPP
#define __XPATH_HPP

__MYLO_NAMESPACE_BEGIN

/*

  http://www.w3.org/TR/xpath

  This implementation is incomplete.  Node index specification is not
  supported... maybe if I ever write an xpath that uses one I'll finish
  it.  Also, not all axes have been tested.

*/

namespace xml {
namespace xpath {

  struct pivot {
    my::string axis;
    my::string type;
    my::string name;
    my::string args;

    my::vector<my::string> filter;
  };

  __MYLO_DLL_EXPORT pivot resolve(my::string segment);

  __MYLO_DLL_EXPORT dom::node::NodeList execute(dom::node &context, my::string path);
  __MYLO_DLL_EXPORT my::map<my::string, dom::node> traverse(dom::node &context, my::vector<my::string> segments);
  __MYLO_DLL_EXPORT dom::node::NodeList list(dom::node &node, my::string axis, my::string type);
  __MYLO_DLL_EXPORT bool match(dom::node &node, my::vector<my::string> &filter);

}}

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:

