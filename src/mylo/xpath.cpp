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

#include "xpath.hpp"

__MYLO_NAMESPACE_BEGIN

namespace xml {
namespace xpath {

__MYLO_DLL_EXPORT pivot resolve(my::string segment) {
  pivot results;

  //
  // Resolve any shortcuts
  //
  if(segment.length() == 0) {
    results.axis = "descendant-or-self";
    results.type = "node";
    results.name = "*";
  }
  if(segment == "*") {
    results.axis = "child";
    results.type = "node";
    results.name = "*";
  }
  if(segment == ".") {
    results.axis = "self";
    results.type = "node";
    results.name = "*";
  }
  if(segment == "..") {
    results.axis = "parent";
    results.type = "node";
    results.name = "*";
  }

  if(!results.axis.empty()) {
    return(results);
  }

  //
  // Parse the fully qualified segments
  //  
  my::vector<my::string> parts = my::regex("^((`w+)::)?(.+)$").match(segment);
  my::vector<my::string> match;

  results.axis = parts[2].empty() ? "child" : my::lc(parts[2]);

  if(match = my::regex("(`w+)`((.*)`)").match(parts[2])) {
    results.type = "function";
    results.name = match[1];
    results.args = match[2];
  }
  if(match = my::regex("(`w+)`[(.+)`]").match(parts[2])) {
    results.type = "node";
    results.name = match[1];
    results.filter = my::split("`s+", match[2]);
  }

  if(results.name.empty()) {
    results.type = "node";
    results.name = parts[3];
  }
  
  return(results);
}

__MYLO_DLL_EXPORT dom::node::NodeList execute(dom::node &context, my::string path) {
  my::vector<my::string> segments = my::split("/", path);

  if(path.length() && path[0] == '/') {
    if(context.nodeName() != "#document") {
      if(!context.ownerDocument()) {
        //ERROR(UNDEFINED_ERROR);
        return(dom::node::NodeList());
      }
      context = context.ownerDocument();
    }
    segments.shift();
  }

  //
  // Make sure only unique nodes are returned in document order
  //
  my::map<my::string, dom::node> set = traverse(context, segments);
  dom::node::NodeList results;

  dom::node::NodeList order;
  if(context.nodeName() != "#document") {
    order = list(context.ownerDocument(), "descendant", "node");
  }
  else {
    order = list(context, "descendant", "node");
  }

  my::vector<dom::node>::iterator node = order.begin();
  for(; node != order.end(); node++) {
    if(set.exists(node->second.nodeId())) {
      results.push(node->second);
    }
  }

  return(results);
}

__MYLO_DLL_EXPORT my::map<my::string, dom::node> traverse(dom::node &context, my::vector<my::string> segments) {
  my::map<my::string, dom::node> results;

  my::vector<my::string>::iterator segment = segments.shift();
  pivot resolved = resolve(segment->second);
  
  if(resolved.type == "function") {
    //ERROR(UNDEFINED_ERROR);
    return(results);
  }

  my::vector<dom::node> sub = list(context, resolved.axis, resolved.type);
  my::vector<dom::node>::iterator node = sub.begin();
  for(; node != sub.end(); node++) {
    if(resolved.name != "*" && node->second.nodeName().compare(resolved.name) != 0) {
      continue;
    }
    if(resolved.filter && !match(node->second, resolved.filter)) {
      continue;
    }
    if(segments.size() == 0) {
      results[node->second.nodeId()] = node->second;
    }
    else {
      my::map<my::string, dom::node> sub = traverse(node->second, segments);
      my::map<my::string, dom::node>::iterator entry = sub.begin();
      for(; entry != sub.end(); entry++) {
        results[entry->second.nodeId()] = entry->second;
      }
    }
  }

  return(results);
}

__MYLO_DLL_EXPORT dom::node::NodeList list(dom::node &node, my::string axis, my::string type) {
  dom::node::NodeList results;

  if(axis == "descendant-or-self") {
    results.push(node);
    axis = "descendant";
  }
  if(axis == "descendant") {
    if(node.childNodes()) {
      my::vector<dom::node>::iterator child = node.childNodes().begin();
      for(; child != node.childNodes().end(); child++) {
        if(type == "node" && child->second.nodeType() == dom::ELEMENT_NODE) {
          results.push(child->second);
        }
        dom::node::NodeList sub = list(child->second, axis, type);
        my::vector<dom::node>::iterator result = sub.begin();
        for(;result != sub.end(); result++) {
          results.push(result->second);
        }
      }
    }
  }

  if(axis == "self") {
    results.push(node);
  }

  if(axis == "child") {
    my::vector<dom::node>::iterator child = node.childNodes().begin();
    for(; child != node.childNodes().end(); child++) {
      if(type == "node" && child->second.nodeType() == dom::ELEMENT_NODE) {
        results.push(child->second);
      }
    }
  }

  if(axis == "parent") {
    if(node.parentNode()) {
      results.push(node.parentNode());
    }
  }

  if(axis == "ancestor-or-self") {
    results.push(node);
    axis = "ancestor";
  }
  if(axis == "ancestor") {
    dom::node current = node;
    while(current.parentNode()) {
      results.push(current.parentNode());
    }
  }

  return(results);
}

__MYLO_DLL_EXPORT bool match(dom::node &node, my::vector<my::string> &filter) {
  bool result = false;
  for(unsigned long i=0; i < filter.size(); i++) {
    if(i%2) { continue; }

    bool iteration = false;

    my::vector<my::string> entry = my::regex("(@)?(`w+)(`s*(!=|=)`s*[\"'](.+)[\"'])?").match(filter[i]);

    //
    // Evaluate the children
    //
    if(!entry[0].empty() && node.childNodes()) {
      my::vector<dom::node>::iterator child = node.childNodes().begin();
      for(; child != node.childNodes().end(); child++) {
        if(child->second.nodeName().compare(entry[1]) == 0) {
          iteration = true;
          break;
        }
      }
    }

    //
    // Evaluate the attributes
    //
    if(entry[0] == "@" && node.hasAttribute(entry[1])) {
      if(!entry[2].empty()) {
        iteration = true;
      }
      else {
        if(node.getAttribute(entry[1]).compare(entry[4]) == 0) {
          iteration = entry[3] == "=" ? true : false;
        }
        else {
          iteration = entry[3] == "=" ? false : true;
        }
      }
    }

    //
    // Determine how this iteration effects the result
    //
    if(i > 0) {
      if(filter[i-1] == "or") {
        result = result || iteration;
      }
      if(filter[i-1] == "and") {
        result = result && iteration;
      }
    }
    else {
      result = iteration;
    }
  }
  return(result);
}

}}

__MYLO_NAMESPACE_END

// Local Variables:
// mode:C++
// End:
