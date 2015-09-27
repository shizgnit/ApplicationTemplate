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

/*

BUGS:

  empty attribute strings causing xpath resolution errors
  doesn't support xml identity strings

*/


#ifndef __XML_HPP
#define __XML_HPP

__MYLO_NAMESPACE_BEGIN

#define __DOM_BUFFERSIZE 129

#define __DOM_CHARSET_UNSET '\0'
#define __DOM_CHARSET_VALUE 'X'

#define ___ __DOM_CHARSET_UNSET
#define _X_ __DOM_CHARSET_VALUE

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

static char __DOM_CHARSET_ATTRIBUTE_VALUE[__DOM_BUFFERSIZE] = {
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,_X_,___,___,___,___,___,___,
   ___,___,_X_,___,_X_,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,_X_,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,___,___,
   ___,___,___,___,___,___,___,___,'\0'
};

static char __DOM_CHARSET_ATTRIBUTE_NAME[__DOM_BUFFERSIZE] = {
   ___,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,___,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,___,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,___,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,'\0'
};

static char __DOM_CHARSET_ELEMENT_CLOSE[__DOM_BUFFERSIZE] = {
   ___,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,___,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,___,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,___,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,___,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,
   _X_,_X_,_X_,_X_,_X_,_X_,_X_,_X_,'\0'
};

static char __DOM_CHARSET_SPACE[__DOM_BUFFERSIZE] = {
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

static char __DOM_CHARSET_TRIM[__DOM_BUFFERSIZE] = {
   ___,___,___,___,___,___,___,___,___,_X_,
   _X_,___,___,_X_,___,___,___,___,___,___,
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


#undef ___
#undef _X_

namespace xml {
namespace dom {

enum NodeType {
  UNINITIALIZED_NODE = 0,
  ELEMENT_NODE = 1,
  ATTRIBUTE_NODE = 2,
  TEXT_NODE = 3,
  CDATA_SECTION_NODE = 4,
  ENTITY_REFERENCE_NODE = 5,
  ENTITY_NODE = 6,
  PROCESSING_INSTRUCTION_NODE = 7,
  COMMENT_NODE = 8,
  DOCUMENT_NODE = 9,
  DOCUMENT_TYPE_NODE = 10,
  DOCUMENT_FRAGMENT_NODE = 11,
  NOTATION_NODE = 12
};

class __MYLO_DLL_EXPORT node {
  friend class document;

public:

  typedef my::vector< node > NodeList;
  typedef my::map< my::string, node > NamedNodeMap;

  public:
    node(void) { }
    node(const node &instance) {
      copy(instance);
    }

    virtual ~node() { }

    node &operator=(const node &instance) {
      copy(instance);
      return(*this);
    }

    node &appendChild(node child);

    //node insertBefore(node in, node ref);
    //node replaceChild(node in, node ref);
    //node removechild(node old);
    //node cloneNode(bool);

    node &setAttribute(my::string name, my::string value);
           
    node &setAttributeNode(node attribute);

    my::string getAttribute(my::string name);

    node &getAttributeNode(my::string name);

    bool hasAttribute(my::string name);

    //node &removeAttributeNode(node attribute);


    bool hasChildNodes(void);

    node selectSingleNode(my::string path);

    NodeList selectNodes(my::string path);
      
  protected:
    template <class T> class __MYLO_DLL_EXPORT _instance {
    public:
      _instance() : nodeType(UNINITIALIZED_NODE) { }
      ~_instance() { }

      my::string nodeId;
      my::string nodeName;
      my::string nodeValue;

      NodeType nodeType;

      T parentNode;

      NodeList childNodes;
      T firstChild;
      T lastChild;

      T previousSibling;
      T nextSibling;

      NamedNodeMap attributes;

      T ownerDocument;
      T documentElement;
    };

    my::shared_ptr< _instance<node> > m_instance;

public:
    _instance<node> &getProperties() {
      initialize(); 
      return(*m_instance);
    }

    operator bool() const {
      return(&m_instance == NULL || m_instance->nodeType == 0 ? false : true);
    }
    
    my::string &nodeId() {
      initialize(); 
      return(m_instance->nodeId);
    }
    my::string &nodeName() {
      initialize(); 
      return(m_instance->nodeName);
    }
    my::string &nodeValue() {
      initialize(); 
      return(m_instance->nodeValue);
    }

    NodeType &nodeType() {
      initialize(); 
      return(m_instance->nodeType);
    }

    node &parentNode() {
      initialize(); 
      return(m_instance->parentNode);
    }

    NodeList &childNodes() {
      initialize(); 
      return(m_instance->childNodes);
    }
    node &firstChild() {
      initialize(); 
      return(m_instance->firstChild);
    }
    node &lastChild() {
      initialize(); 
      return(m_instance->lastChild);
    }

    node &previousSibling() {
      initialize(); 
      return(m_instance->firstChild);
    }
    node &nextSibling() {
      initialize(); 
      return(m_instance->lastChild);
    }

    NamedNodeMap &attributes() {
      initialize(); 
      return(m_instance->attributes);
    }

    node &ownerDocument() {
      initialize(); 
      return(m_instance->ownerDocument);
    }
    node &documentElement() {
      initialize(); 
      return(m_instance->documentElement);
    }

protected:

    void initialize() {
      if(&m_instance == NULL) {
        m_instance = new _instance<node>();
      }
    }

    void copy(const node &instance) {
      if(&instance != this) {
        m_instance = instance.m_instance;
      }
    }

    void release() {
      if(&m_instance != NULL) {
        node::NamedNodeMap::iterator attribute = this->attributes().begin();
        for(; attribute != this->attributes().end(); attribute++) {
          attribute->second.release();
        }
        node::NodeList::iterator child = this->childNodes().begin();
        for(; child != this->childNodes().end(); child++) {
          child->second.release();
        }
        m_instance->firstChild.m_instance = NULL;
        m_instance->lastChild.m_instance = NULL;
        m_instance->parentNode.m_instance = NULL;
        m_instance->ownerDocument.m_instance = NULL;
        m_instance->nextSibling.m_instance = NULL;
        m_instance->previousSibling.m_instance = NULL;
        m_instance = NULL;
      }
    }

};

class __MYLO_DLL_EXPORT document : public node {
  public:
    document(void) {
      this->nodeName() = "#document";
	    this->nodeId() = my::uuid();
    }

    document(const node &element) {
      this->nodeName() = "#document";
	    this->nodeId() = my::uuid();
      this->documentElement() = element;
	    this->childNodes()[0] = element;
    }

    virtual ~document() {
      documentElement().release();
    }

    void load(my::string file);
    void loadXML(my::string xml);

public:
    node createAttribute(my::string name) {
      node instance;
      instance.nodeType() = ATTRIBUTE_NODE;
      instance.nodeName() = name;
	    instance.nodeId() = my::uuid();
      instance.ownerDocument() = *this;
      return(instance);
    }

    node createCDATASection(my::string data) {
      node instance;
      instance.nodeType() = CDATA_SECTION_NODE;
      instance.nodeName() = "#cdata-section";
	    instance.nodeId() = my::uuid();
      instance.nodeValue() = data;
      instance.ownerDocument() = *this;
      return(instance);
    }

    node createComment(my::string data) {
      node instance;
      instance.nodeType() = COMMENT_NODE;
      instance.nodeName() = "#comment";
	    instance.nodeId() = my::uuid();
      instance.nodeValue() = data;
      instance.ownerDocument() = *this;
      return(instance);
    }

    node createElement(my::string name) {
      node instance;
      instance.nodeType() = ELEMENT_NODE;
      instance.nodeName() = name;
	    instance.nodeId() = my::uuid();
      instance.ownerDocument() = *this;
      return(instance);
    }

    node createTextNode(my::string data) {
      node instance;
      instance.nodeType() = TEXT_NODE;
      instance.nodeName() = "#text";
	    instance.nodeId() = my::uuid();
      instance.nodeValue() = data;
      instance.ownerDocument() = *this;
      return(instance);
    }

};

__MYLO_DLL_EXPORT my::string serialize(node &node, bool format=true, int spacing=2, bool collapse=true, size_t level=0);

}}

__MYLO_NAMESPACE_END

__MYLO_NAMESPACE_BEGIN

//template class __MYLO_DLL_EXPORT my::vector< my::xml::dom::node >;
//template class __MYLO_DLL_EXPORT my::map< my::string, my::xml::dom::node >;
//template class __MYLO_DLL_EXPORT my::shared_ptr< my::xml::dom::node::_instance<my::xml::dom::node> >;

__MYLO_NAMESPACE_END


#endif

// Local Variables:
// mode:C++
// End:

