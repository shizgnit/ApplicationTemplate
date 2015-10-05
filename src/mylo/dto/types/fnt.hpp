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
    if (bytes) {
      context->buffer.write(buffer, bytes);
      return(bytes);
    }

    if (context->buffer.raw()[0] == '<') {
      write_xml(context);
    }
    else {
      write_text(context);
    }

    return(0);
  }

  static void write_text(font *context) {
    /*
    info face="Arial" size=32 bold=0 italic=0 charset="" unicode=1 stretchH=100 smooth=1 aa=1 padding=0,0,0,0 spacing=1,1 outline=0
    common lineHeight=32 base=26 scaleW=256 scaleH=256 pages=1 packed=0 alphaChnl=0 redChnl=0 greenChnl=0 blueChnl=0
    page id=0 file="arial2_0.png"
    chars count=191
    char id=32   x=155   y=75    width=3     height=1     xoffset=-1    yoffset=31    xadvance=8     page=0  chnl=15
    char id=33   x=250   y=116   width=4     height=20    xoffset=2     yoffset=6     xadvance=8     page=0  chnl=15
    ...
    kernings count=91
    kerning first=32  second=65  amount=-2
    kerning first=32  second=84  amount=-1
    ...
    */
    for (int i = 0; i <= 255; i++) {
      font::glyph *current = new font::glyph();
      current->identifier = 0;
      context->glyphs[i] = current;
    }

    my::vector<char *> lines = my::segment((char *)context->buffer.raw(), '\n');
    for (unsigned int i = 0; i < lines.size(); i++) {
      if (lines[i][0] == '#') {
        continue;
      }
      my::vector<char *> line = my::segment(lines[i], ' ');

      if (strcmp(line[0], "info") == 0) {
        for (unsigned int j = 1; j < line.size(); j++) {
          my::vector<char *> parameter = my::segment(line[j], '=');

          if (strcmp(parameter[0], "face") == 0) {
            parameter[1][strlen(parameter[1])] = '\0';
            context->name = parameter[1] + 1;
          }
          if (strcmp(parameter[0], "size") == 0) {
            context->size = type_cast<int>(parameter[1]);
          }
          if (strcmp(parameter[0], "bold") == 0) {
            context->bold = strcmp(parameter[1], "1") == 0;
          }
          if (strcmp(parameter[0], "italic") == 0) {
            context->italic = strcmp(parameter[1], "1") == 0;
          }
          if (strcmp(parameter[0], "padding") == 0) {
            my::vector<char *> padding_fields = my::segment(parameter[1], ',');
            context->padding_top = type_cast<int>(padding_fields[0]);
            context->padding_left = type_cast<int>(padding_fields[1]);
            context->padding_bottom = type_cast<int>(padding_fields[2]);
            context->padding_right = type_cast<int>(padding_fields[3]);
          }
          if (strcmp(parameter[0], "spacing") == 0) {
            my::vector<char *> spacing_fields = my::segment(parameter[1], ',');
            context->spacing_left = type_cast<int>(spacing_fields[0]);
            context->spacing_right = type_cast<int>(spacing_fields[1]);
          }
        }
      }

      if (strcmp(line[0], "page") == 0) {
        my::vector<char *> id_parameter = my::segment(line[1], '=');
        my::vector<char *> file_parameter = my::segment(line[2], '=');

        file_parameter[1][strlen(file_parameter[1])-1] = '\0';
        my::string file = file_parameter[1] + 1;

        my::image *page = new my::png();
        *page << my::asset(file);
        context->pages.push(page);
      }

      if (strcmp(line[0], "char") == 0) {
        font::glyph *glyph = context->glyphs[i];
        for (unsigned int j = 1; j < line.size(); j++) {
          my::vector<char *> parameter = my::segment(line[j], '=');
          if (strcmp(parameter[0], "id") == 0) {
            int id = type_cast<int>(parameter[1]);
            glyph = context->glyphs[id];
            glyph->identifier = id;
          }
          if (strcmp(parameter[0], "x") == 0) {
            glyph->x = type_cast<int>(parameter[1]);
          }
          if (strcmp(parameter[0], "y") == 0) {
            glyph->y = type_cast<int>(parameter[1]);
          }
          if (strcmp(parameter[0], "width") == 0) {
            glyph->width = type_cast<int>(parameter[1]);
          }
          if (strcmp(parameter[0], "height") == 0) {
            glyph->height = type_cast<int>(parameter[1]);
          }
          if (strcmp(parameter[0], "xoffset") == 0) {
            glyph->xoffset = type_cast<int>(parameter[1]);
          }
          if (strcmp(parameter[0], "yoffset") == 0) {
            glyph->yoffset = type_cast<int>(parameter[1]);
          }
          if (strcmp(parameter[0], "xadvance") == 0) {
            glyph->xadvance = type_cast<int>(parameter[1]);
          }
          if (strcmp(parameter[0], "page") == 0) {
            glyph->page = type_cast<int>(parameter[1]);
            glyph->quad = my::primitive::quad((float)glyph->width, (float)glyph->height);
            glyph->quad->xy_projection(context->pages[glyph->page], glyph->x, glyph->y, glyph->width, glyph->height);
          }
          if (strcmp(parameter[0], "channel") == 0) {
            glyph->channel = type_cast<int>(parameter[1]);
          }
        }
      }

      if (strcmp(line[0], "kerning") == 0) {
        font::kerning *kerning = new font::kerning();
        for (unsigned int j = 1; j < line.size(); j++) {
          my::vector<char *> parameter = my::segment(line[j], '=');

          if (strcmp(parameter[0], "first") == 0) {
            kerning->first = type_cast<int>(parameter[1]);
          }
          if (strcmp(parameter[0], "second") == 0) {
            kerning->second = type_cast<int>(parameter[1]);
          }
          if (strcmp(parameter[0], "amount") == 0) {
            kerning->amount = type_cast<int>(parameter[1]);
          }
        }
        context->kernings.push_back(kerning);
      }

    }
  }

  static void write_xml(font *context) {

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
  }

};

__MYLO_NAMESPACE_END

#endif
