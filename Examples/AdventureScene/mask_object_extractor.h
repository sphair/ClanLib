
#ifndef header_mask_object_extractor
#define header_mask_object_extractor

#if _MSC_VER > 1000
#pragma once
#endif

#include <list>
#include "object.h"

class CL_PixelBuffer;

class MaskObjectExtractor
{
public:
	MaskObjectExtractor(CL_PixelBuffer &background, CL_PixelBuffer &mask);
	
	std::vector<Object *> objects;

private:
	CL_Rect isolate_mask_object(CL_PixelBuffer &mask, int object_col);
	Object *create_object_from_mask(const CL_Rect &rect, CL_PixelBuffer &background, CL_PixelBuffer &mask, int color);
};

#endif
