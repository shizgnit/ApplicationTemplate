#include "mylo.hpp"

#ifndef __IMAGE_HPP
#define __IMAGE_HPP

__MYLO_NAMESPACE_BEGIN

class __MYLO_DLL_EXPORT image: public datatype<image> {
public:
  struct {
    unsigned int type;
    unsigned int width;
    unsigned int height;
    unsigned char bpp;
    unsigned char id;
    unsigned int x_offset;
    unsigned int y_offset;
    unsigned char descriptor;
    unsigned int colormap;
  } header;

  my::shared_ptr<unsigned char, allocation_array> colormap;
  my::shared_ptr<unsigned char, allocation_array> raster;

  my::buffer buffer;
};

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
