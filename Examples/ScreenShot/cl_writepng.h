
#ifndef header_write_png
#define header_write_png

#if _MSC_VER > 1000
#pragma once
#endif

#include "writepng.h"

class WritePNG
{
 public:
	static void write_png( CL_PixelBuffer surface, std::string path );
	static void cleanup();
	
	static mainprog_info wpng_info;
};

#endif // header_write_png
