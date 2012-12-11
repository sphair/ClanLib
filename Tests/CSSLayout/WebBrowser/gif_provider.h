
#pragma once

#include "giflib/gif_lib.h"

class GIFProvider
{
public:
	static CL_PixelBuffer load(CL_IODevice &device);

private:
	static int on_read_input(GifFileType *filetype, GifByteType *buffer, int length);

//	static const int interlaced_offset[];
//	static const int interlaced_jumps[];
};
