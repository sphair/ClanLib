
#pragma once
#ifdef WIN32
#include "giflib/gif_lib.h"
#else
#include "gif_lib.h"
#endif

class GIFProvider
{
public:
	static clan::PixelBuffer load(clan::IODevice &device);

private:
	static int on_read_input(GifFileType *filetype, GifByteType *buffer, int length);

//	static const int interlaced_offset[];
//	static const int interlaced_jumps[];
};
