#include "mylo.hpp"

#ifndef __PNG_HPP
#define __PNG_HPP

#include "png.h"

__MYLO_NAMESPACE_BEGIN

class png: public my::image {
public:
  png() {
    callback_write("PNG", write);
  }


private:
  static void callback(png_structp png_ptr, png_bytep request, png_size_t size) {
    my::buffer *buffer = (my::buffer *)png_get_io_ptr(png_ptr);
    buffer->read(request, size);
  }

  static off_t write(image *context, unsigned char *buffer, off_t bytes) {
    if(bytes) {
      context->buffer.write(buffer, bytes);
      return(bytes);
    }

    int png_data_size = context->buffer.size();
    unsigned char *png_data = context->buffer.raw();

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png_ptr) {
      return 0;
    }

    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr) {
      png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
      return 0;
    }

    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if(!end_info) {
      png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
      return 0;
    }

    // setup the io callback
    png_set_read_fn(png_ptr, &context->buffer, callback);

    // error handlering
    if (setjmp(png_jmpbuf(png_ptr))) {
      png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
      return 0;
    }

    // temporary metadata values
    png_uint_32 width;
    png_uint_32 height;
    int bit_depth;
    int color_type;
    int interlace_method;
    int compession_method;
    int filter_method;

    // read in the initial metadata
    png_read_info(png_ptr, info_ptr);

    png_get_IHDR(
      png_ptr,
      info_ptr,
      &width,
      &height,
      &bit_depth,
      &color_type,
      &interlace_method,
      &compession_method,
      &filter_method
    );

    // post processing
    if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
      png_set_tRNS_to_alpha(png_ptr);
    }
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
      png_set_expand_gray_1_2_4_to_8(png_ptr);
    }
    if(color_type == PNG_COLOR_TYPE_PALETTE) {
      png_set_palette_to_rgb(png_ptr);
    }
    if(color_type == PNG_COLOR_TYPE_PALETTE || color_type == PNG_COLOR_TYPE_RGB) {
      png_set_add_alpha(png_ptr, 0xFF, PNG_FILLER_AFTER);
    }
    if(info_ptr->bit_depth < 8) {
      png_set_packing(png_ptr);
      png_set_expand(png_ptr);
    }
    else if(info_ptr->bit_depth == 16) {
      png_set_strip_16(png_ptr);
    }

    // update the metadata
    png_read_update_info(png_ptr, info_ptr);

    // create the raw data pointers
    png_size_t size = png_get_rowbytes(png_ptr, info_ptr);
//    context->raster = (png_bytep)malloc(size * height);
    context->raster = (png_bytep)new char[size * height];
//    if (context->raster == 0) {
//      png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
//      return 0;
//    }

    png_bytep *rows = (png_bytep *)malloc(sizeof(png_bytep) * height);
    if (rows == NULL) {
      png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
      return 0;
    }

    // setup the row pointers
    for (png_uint_32 i = 0, j=height-1; i < height; i++, j--) {
      rows[i] = &context->raster + (i * size);
    }

    // load the raw image data
    png_read_image(png_ptr, rows);

    // row addressing done
    free(rows);

    // assign metadata
    context->header.width = width;
    context->header.height = height;

    context->buffer.reset();

    return 0;
  }

};

__MYLO_NAMESPACE_END

#endif
