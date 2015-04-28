#include "xml.hpp"

__MYLO_NAMESPACE_BEGIN

namespace xml {
namespace dom {

__MYLO_DLL_EXPORT node &node::appendChild(node child) {
  child.parentNode() = *this;
  m_instance->childNodes.push(child);
  m_instance->firstChild = m_instance->childNodes[0];
  m_instance->lastChild = m_instance->childNodes[m_instance->childNodes.size()-1];
  return(*this);
}

__MYLO_DLL_EXPORT node &node::setAttribute(my::string name, my::string value) {
  document doc = m_instance->ownerDocument;

  node attribute = doc.createAttribute(name);
  node text = doc.createTextNode(value);

  attribute.appendChild(text);

  m_instance->attributes[name] = attribute;

  return(*this);
}

__MYLO_DLL_EXPORT node &node::setAttributeNode(node attribute) {
  m_instance->attributes[attribute.nodeName()] = attribute;
  return(*this);
}

__MYLO_DLL_EXPORT my::string node::getAttribute(my::string name) {
  if(hasAttribute(name) && m_instance->attributes[name].hasChildNodes()) {
    return(m_instance->attributes[name].firstChild().nodeValue());
  }
  return("");
}

__MYLO_DLL_EXPORT node &node::getAttributeNode(my::string name) {
  return(m_instance->attributes[name]);
}

__MYLO_DLL_EXPORT bool node::hasAttribute(my::string name) {
  return(m_instance->attributes.exists(name));
}

__MYLO_DLL_EXPORT node node::selectSingleNode(my::string path) {
  return(xml::xpath::execute(*this, path)[0]);
}

__MYLO_DLL_EXPORT node::NodeList node::selectNodes(my::string path) {
  return(xml::xpath::execute(*this, path));
}

__MYLO_DLL_EXPORT bool node::hasChildNodes(void) {
  return(m_instance->childNodes.size() ? true : false);
}

__MYLO_DLL_EXPORT void document::load(my::string xml) {
  my::buffer buf;
  buf<<my::file(xml);
  loadXML(buf);
}

__MYLO_DLL_EXPORT void document::loadXML(my::string xml) {
  if(xml.empty()) {
    return;
  }

  char *parse_string = xml.c_str();
  char *reference;

  size_t length;
  char buffer[1024];

  char *dynamic = NULL;

  node current_node;

  while(*parse_string) {
    reference = parse_string;
    while(*parse_string && *parse_string != '<') {
      parse_string++;
    }
    length = parse_string - reference;
    if(length) {
      unsigned long i=0;
      while(i < length && __DOM_CHARSET_TRIM[reference[i]]) { i++; }
      if(i < length) {
        dynamic = (char *)malloc((length-i+1) * sizeof(char));
        memcpy(dynamic, reference+i, length-i);

        dynamic[length-i++] = '\0';
        while(i <= length && __DOM_CHARSET_TRIM[dynamic[length-i]]) {
          dynamic[length-i++] = '\0';
        }

        current_node.appendChild(createTextNode(dynamic));

        free(dynamic);
        dynamic = NULL;
      }
    }

    if(*parse_string == '<') {
      parse_string++;

      bool close_stack = false;
      bool close_current = false;

      while(__DOM_CHARSET_SPACE[*parse_string]) {
        parse_string++;
      }

      if(*parse_string == '/') {
        parse_string++;
        close_stack = true;
        while(__DOM_CHARSET_SPACE[*parse_string]) {
          parse_string++;
        }
      }

      if(*parse_string == '!') {
        parse_string++;
        if(strncmp(parse_string, "[CDATA[", 7) == 0) {
          reference = parse_string;
          while(*parse_string && *parse_string != ']') {
            parse_string++;
          }
          length = parse_string - reference;
          if(length) {
            if(length > 8 && strncmp(reference, "[CDATA[", 7) == 0) {
              reference += 7;
              length -= 8;
              dynamic = (char *)malloc((length+1) * sizeof(char));
              memcpy(dynamic, reference, length);
              dynamic[length] = '\0';

              current_node.appendChild(createCDATASection(dynamic));

              free(dynamic);
              dynamic = NULL;
            }
          }
        }
      }

      reference = parse_string;
      while(__DOM_CHARSET_ELEMENT_CLOSE[*parse_string]) {
        parse_string++;
      }

      length = parse_string - reference;
      memcpy(buffer, reference, length);
      buffer[length] = '\0';

      if(close_stack) {
        if(current_node.parentNode()) {
          current_node = current_node.parentNode();
        }
      }
      else {
        node element_node = createElement(buffer);
        if(current_node) {
          current_node.appendChild(element_node);
        }
        current_node = element_node;
      }

      while(*parse_string && *parse_string != '>') {
        while(__DOM_CHARSET_SPACE[*parse_string]) {
          parse_string++;
        }
        if(*parse_string == '/') {
          close_current = true;
          if(current_node.parentNode()) {
            current_node = current_node.parentNode();
          }
          parse_string++;
          while(__DOM_CHARSET_SPACE[*parse_string]) {
            parse_string++;
          }
        }
        if(*parse_string == '>') {
          break;
        }

        reference = parse_string;
        while(__DOM_CHARSET_ATTRIBUTE_NAME[*parse_string]) {
          parse_string++;
        }
        if(!*parse_string) {
          //std::cerr<<"attribute definition incomplete"<<std::endl;
        }

        length = parse_string - reference;
        memcpy(buffer, reference, length);
        buffer[length] = '\0';

        node attribute_node = createAttribute(buffer);

        bool open = false;
        while(__DOM_CHARSET_ATTRIBUTE_VALUE[*parse_string] && !open) {
          open = *parse_string == '"';
          parse_string++;
        }

        reference = parse_string;
        while(*parse_string && *parse_string != '"') {
          parse_string++;
        }

        length = parse_string - reference;
        memcpy(buffer, reference, length);
        buffer[length] = '\0';

        attribute_node.appendChild(createTextNode(buffer));

        current_node.setAttributeNode(attribute_node);

        parse_string++;
      }

      parse_string++;
    }

  }

  m_instance->documentElement = current_node;
  m_instance->childNodes[0] = current_node;
}

__MYLO_DLL_EXPORT my::string serialize(node &current_node, bool format, int spacing, bool collapse, size_t level) {
  my::string results = "";

  switch(current_node.nodeType()) {
    case(ELEMENT_NODE): {
      if(format) {
        for(size_t i=0; i<level*spacing; i++) {
          results.append(" ");
        }
      }
      results.append("<");
      results.append(current_node.nodeName());
      my::string key;

      node::NamedNodeMap::iterator attributes = current_node.attributes().begin();

      for(; attributes != current_node.attributes().end(); attributes++) {
        node attribute = attributes->second;
        results.append(" ");
        results.append(attribute.nodeName());
        results.append("=\"");
        my::string attr;

        node::NodeList::iterator children = attribute.childNodes().begin();
        for(; children != attribute.childNodes().end(); children++) {
          results.append(children->second.nodeValue());
        }
        results.append("\"");
      }

      if(current_node.childNodes().size() || !collapse) {
        results.append(">");

        bool simple_text = false;
        if(current_node.childNodes().size() == 1 && current_node.firstChild().nodeType() == TEXT_NODE) {
          simple_text = true;
        }

        if(format && !simple_text) {
          results.append("\n");
        }

        node::NodeList::iterator children = current_node.childNodes().begin();
        for(; children != current_node.childNodes().end(); children++) {
          //results.append(children->second.nodeValue());
          results.append(serialize(children->second, format && !simple_text, spacing, collapse, level+1));
        }
        if(format && !simple_text) {
          for(size_t i=0; i<level*spacing; i++) {
            results.append(" ");
          }
        }
        results.append("</");
        results.append(current_node.nodeName());
        results.append(">");
        if(format) {
          results.append("\n");
        }
      }
      else {
        results.append("/>\n");
      }

      break;
    }
    case(TEXT_NODE): {
      if(format) {
        for(size_t i=0; i<level*spacing; i++) {
          results.append(" ");
        }
      }
      results.append(current_node.nodeValue());
      if(format) {
        results.append("\n");
      }
      break;
    }
    case(CDATA_SECTION_NODE): {
      results.append(current_node.nodeValue());
      results.append("\n");
      break;
    }
  };

  return(results);
}

}}

__MYLO_NAMESPACE_END

// Local Variables:
// mode:C++
// End:
