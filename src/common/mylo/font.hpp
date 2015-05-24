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
