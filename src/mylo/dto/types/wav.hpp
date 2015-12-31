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

#ifndef __WAV_HPP
#define __WAV_HPP

__MYLO_NAMESPACE_BEGIN

class __MYLO_DLL_EXPORT wav: public my::audio {
public:
  wav() {
    callback_write("WAV", write);
  }

private:

  static off_t write(audio *context, unsigned char *buffer, off_t bytes) { DEBUG_SCOPE;
    if(bytes) {
      context->buffer.write(buffer, bytes);
      return(bytes);
    }

    // parse the header
    struct {
      char chunk[4];
      unsigned int chunk_size;
      char format[4];
      char subchunk[4];
      unsigned int subchunk_size;
      unsigned short audio_format;
      unsigned short channels;
      unsigned int sample_rate;
      unsigned int byte_rate;
      unsigned short block_align;
      unsigned short bits_per_sample;
      char data[4];
      unsigned int data_size;
    } metadata;

    size_t size = context->buffer.size();

	DEBUG_TRACE << size << " bytes" << my::endl;
	
    unsigned char *allocation = new unsigned char[size];
    context->buffer.read(allocation, size);
	
    memcpy(&metadata, allocation, sizeof(metadata));

    context->data = allocation;
    context->size = size;

    context->header.sample_rate = metadata.sample_rate;
    context->header.bits_per_sample = metadata.bits_per_sample;
    context->header.block_align = metadata.block_align;
    context->header.channels = metadata.channels;
    context->header.size = metadata.data_size;

    context->buffer.reset();

    return(0);
  }

};

__MYLO_NAMESPACE_END

#endif