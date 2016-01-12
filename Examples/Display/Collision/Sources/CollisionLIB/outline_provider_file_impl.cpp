/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
**    James Wynn
**    (if your name is missing here, please add it)
*/

#include "precomp.h"
#include "outline_provider_file_impl.h"

using namespace clan;

/////////////////////////////////////////////////////////////////////////////
// OutlineProviderFile_Impl construction:

OutlineProviderFile_Impl::OutlineProviderFile_Impl(
	IODevice &file)
{
	load(file);
}

OutlineProviderFile_Impl::~OutlineProviderFile_Impl()
{
}

/////////////////////////////////////////////////////////////////////////////
// OutlineProviderFile_Impl operations:

void OutlineProviderFile_Impl::load(IODevice &input_source)
{
	// file type & version identifiers
	int type = input_source.read_uint32();
	unsigned char version = input_source.read_uint8();

	if( type != 0x16082004  )
		throw Exception("File is not a collision outline file" );
	if( version != 1 )
		throw Exception(string_format("Unsupported version of outline format: %1. Supported versions: 1.", version) );

	// read in width and height
	width = input_source.read_int32();
	height = input_source.read_int32();

	// x-pos of enclosing disc
	minimum_enclosing_disc.position.x = input_source.read_float();
	// y-pos of enclosing disc
	minimum_enclosing_disc.position.y = input_source.read_float();
	// radius of enclosing disc
	minimum_enclosing_disc.radius = input_source.read_float();
	
	// num contours
	int num_contours = input_source.read_uint32();

	for( int cc=0; cc < num_contours; ++cc )
	{
		Contour contour;

		int num_points = input_source.read_uint32();

		for( int pp=0; pp < num_points; ++pp )
		{
			Pointf point(0,0);
			point.x = input_source.read_float();
			point.y = input_source.read_float();

			contour.get_points().push_back(point);
		}
		
		contours.push_back(contour);
	}
}
