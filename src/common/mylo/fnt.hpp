#include "mylo.hpp"

//#include <iostream>
//#include <fstream>

#ifndef __FNT_HPP
#define __FNT_HPP

__MYLO_NAMESPACE_BEGIN

using namespace xml::dom;

class __MYLO_DLL_EXPORT fnt: public font {
public:
  fnt() {
    callback_write("FNT", write);
  }

  static off_t write(font *context, unsigned char *buffer, off_t bytes) {
    if(bytes) {
      context->buffer.write(buffer, bytes);
      return(bytes);
    }

    document doc;
    doc.loadXML(join(" ", context->buffer));

    //my::string text = xml::dom::serialize(doc.documentElement(), true);

    //std::ofstream file;
    //file.open("output.txt");
    //file << text.c_str();
    //file.close();

    node info = doc.selectSingleNode("/font/info");

    context->name = info.getAttribute("face");

    context->size = atoi(info.getAttribute("size").c_str());

    context->bold = info.getAttribute("bold").compare("1") == 0;
    context->italic = info.getAttribute("italic").compare("1") == 0;

    my::string padding = info.getAttribute("padding");
    my::vector<my::string> padding_fields = tokenize(padding, ",");
    context->padding_top = type_cast<int>(padding_fields[0]);
    context->padding_left = type_cast<int>(padding_fields[1]);
    context->padding_bottom = type_cast<int>(padding_fields[2]);
    context->padding_right = type_cast<int>(padding_fields[3]);

    my::string spacing = info.getAttribute("spacing");
    my::vector<my::string> spacing_fields = tokenize(spacing, ",");
    context->spacing_left = type_cast<int>(spacing_fields[0]);
    context->spacing_right = type_cast<int>(spacing_fields[1]);

    node::NodeList pages = doc.selectNodes("/font/pages/page");
    node::NodeList::iterator pit = pages.begin();
    for(;pit != pages.end(); pit++) {
      my::image *page = new my::png();
      *page << my::asset(pit->second.getAttribute("file"));
      context->pages.push(page);
    }

    for (int i = 0; i <= 255; i++) {
      font::glyph *current = new font::glyph();
      current->identifier = 0;
      context->glyphs[i] = current;
    }

    node::NodeList chars = doc.selectNodes("/font/chars/char");
    node::NodeList::iterator cit = chars.begin();
    for(;cit != chars.end(); cit++) {
      int i = type_cast<int>(cit->second.getAttribute("id"));

      font::glyph *current = context->glyphs[i];

      current->identifier = i;

      current->x = type_cast<int>(cit->second.getAttribute("x"));
      current->y = type_cast<int>(cit->second.getAttribute("y"));

      current->width = type_cast<int>(cit->second.getAttribute("width"));
      current->height = type_cast<int>(cit->second.getAttribute("height"));

      current->xoffset = type_cast<int>(cit->second.getAttribute("xoffset"));
      current->yoffset = type_cast<int>(cit->second.getAttribute("yoffset"));

      current->xadvance = type_cast<int>(cit->second.getAttribute("xadvance"));

      current->page = type_cast<int>(cit->second.getAttribute("page"));
      current->channel = type_cast<int>(cit->second.getAttribute("channel"));

      current->quad = my::primitive::quad((float)current->width, (float)current->height);
      current->quad->xy_projection(context->pages[current->page], current->x, current->y, current->width, current->height);
    }

    node::NodeList kernings = doc.selectNodes("/font/kernings/kerning");
    node::NodeList::iterator kit = kernings.begin();
    for(;kit != kernings.end(); kit++) {
      font::kerning *current = new font::kerning();

      current->first = type_cast<int>(kit->second.getAttribute("first"));
      current->second = type_cast<int>(kit->second.getAttribute("second"));
      current->amount = type_cast<int>(kit->second.getAttribute("amount"));

      context->kernings.push_back(current);
    }

    context->buffer.reset();

    return(0);
  }

};

__MYLO_NAMESPACE_END

#endif
