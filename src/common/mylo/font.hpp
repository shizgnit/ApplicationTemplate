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

#ifndef __FONT_HPP
#define __FONT_HPP

__MYLO_NAMESPACE_BEGIN

class __MYLO_DLL_EXPORT font: public datatype<font> {
public:
  class glyph {
  public:
    int identifier;
    
    int x;
    int y;
    
    int width;
    int height;
    
    int xoffset;
    int yoffset;
    
    int xadvance;
    
    int page;
    int channel;

    my::shared_ptr<my::object> quad;
  };

  class kerning {
  public:
    int first;
    int second;
    int amount;
  };

	int identifier;
	
	my::string name;
	
	int size;
	
	char bold;
	char italic;
	
	int padding_top;
	int padding_left;
	int padding_bottom;
	int padding_right;
	
	int spacing_left;
	int spacing_right;

  my::vector< my::shared_ptr<my::image> > pages;
  my::vector< my::shared_ptr<glyph> > glyphs;
  my::vector< my::shared_ptr<kerning> > kernings;

  int kern(int left, int right) {
    my::vector< my::shared_ptr<kerning> >::iterator it = kernings.begin();
    for (; it != kernings.end(); it++) {
      if (it->second->first == left && it->second->second == right) {
        return(it->second->amount);
      }
    }
    return(0);
  }

  my::buffer buffer;
};

__MYLO_NAMESPACE_END

#endif
