#include "mylo.hpp"

#ifndef __TGA_HPP
#define __TGA_HPP

__MYLO_NAMESPACE_BEGIN

class __MYLO_DLL_EXPORT tga: public my::image {
public:
  tga() {
    callback_write("TGA", write);
  }

private:

  static off_t write(image *context, unsigned char *buffer, off_t bytes) {
    if(bytes) {
      context->buffer.write(buffer, bytes);
      return(bytes);
    }

    // parse the header
    unsigned char header[18];

    context->buffer.read(header, 18);

    context->header.type = header[2];
    context->header.width = header[12] | (header[13]<<8);
    context->header.height = header[14] | (header[15]<<8);
    context->header.bpp = static_cast<unsigned char>(header[16]);
    context->header.id = static_cast<unsigned char>(header[0]);
    context->header.x_offset = header[8] | (header[9]<<8);
    context->header.y_offset = header[10] | (header[11]<<8);
    context->header.descriptor = static_cast<unsigned char>(header[17]);

    context->header.colormap = (header[5]<<8) | header[6];

    
    // ignore colormaps for now
    if(context->header.colormap) {
      context->buffer.seek(context->buffer.location() + context->header.id + context->header.colormap);
    }

    // parse the raster data
    unsigned char *raster = context->buffer.raw();
    unsigned char order = (context->header.descriptor & 0x30)>>4;

    unsigned char byte;
    unsigned int Bpp = context->header.bpp/8;
    unsigned int pixel_count = context->header.width * context->header.height;
    unsigned int allocation = pixel_count * (Bpp);

    unsigned char *temporary = new unsigned char[allocation];

    if(context->header.type == 9 || context->header.type == 10) {
	    unsigned int target = 0;
      unsigned int source = 0;
	    unsigned char *pixel = new unsigned char[Bpp];
	    while(target < allocation) {
        if(target >= 766000) {
          target = target;
        }
        byte = raster[source++];
        unsigned int count = (byte & 0x7F) + 1;
	      if(byte & 0x80) {
          memcpy(pixel, buffer + source, Bpp);
          source += Bpp;
	        for(unsigned int i=0; i<count; i++) {
	          memcpy(temporary + target, pixel, Bpp);
            target += Bpp;
	        }
	      }
	      else {
          unsigned int size = count * Bpp;
          memcpy(temporary + target, raster + source, size);
	        target += size;
          source += size;
	      }
	    }
      delete [] pixel;
    }
    else {
      memcpy(temporary, context->buffer.raw(), allocation);          
    }

    // normalize data orientation
    unsigned char *normalization = NULL;
    if(context->header.descriptor & 0x20) {
      normalization = new unsigned char[allocation];
      unsigned int back = allocation;
      for(unsigned int i=0; i < allocation; i++) {
        normalization[--back] = temporary[i];
      }
      delete [] (temporary);
    }
    else {
      normalization = temporary;
    }

    unsigned char *final = new unsigned char[pixel_count*4];

    unsigned char *target;
    unsigned char *source;
    for (unsigned int y = 0; y < context->header.height; y++) {
      for (unsigned int x = 0; x < context->header.width; x++) {
//        target = final + (y * context->header.width * 4) + ((context->header.width - x) * 4);
//        target = final + ((context->header.height - y - 1) * context->header.width * 4) + (x * 4);
        target = final + (y * context->header.width * 4) + (x * 4);
        source = normalization + (y * context->header.width * Bpp) + (x * Bpp);
        target[0] = source[2];
        target[1] = source[1];
        target[2] = source[0];
        target[3] = Bpp == 4 ? source[4] : 255;
      }
    }

    delete [] normalization;

    context->raster = final;

    context->buffer.reset();

    return(0);
  }

};

__MYLO_NAMESPACE_END

#endif