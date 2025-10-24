/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Harry Storbacka
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

#ifndef header_outline_provider_bitmap_generic
#define header_outline_provider_bitmap_generic

#include "API/Display/Collision/contour.h"

class CL_PixelBuffer;

class CL_OutlineProviderBitmap_Generic
{
//!Construction
 public:
	CL_OutlineProviderBitmap_Generic(CL_PixelBuffer pbuf, int alpha_limit=128, bool get_insides=true);
	~CL_OutlineProviderBitmap_Generic();

//!Attributes
	std::vector<CL_Contour> contours;
	int width, height;

 private:
	// find the start point of the next outline
	CL_Pointf find_next_contour_start(unsigned int x, unsigned int y);

	// add the next point
	void get_next_point(std::vector<CL_Pointf> &points);

	// test if a point is already included in some outline
	CL_Contour *point_in_outline(unsigned int x, unsigned int y);

	// test if a pixel is alpha or opaque
	bool is_opaque(int x, int y);

	// test if a corner is part of an edge
	bool is_edge(int x, int y);
	inline unsigned char &get_corner(int x, int y)
	{
		return data[y*(width+1)+x];
	}

	// Find the contours
	void find_contours();

	unsigned char *data;
	unsigned char last_corner;

	bool get_insides;
	
	int alpha_limit;

	int alpha_pixel;

	CL_PixelBuffer &pb;

	CL_Point last_point;
	
	enum MoveDir
	{
		DIR_LEFT=0,
		DIR_RIGHT,
		DIR_UP,
		DIR_DOWN
	} last_dir;

};

#endif
