/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
*/

#include "Display/precomp.h"
#include "freetype_font_provider.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/System/databuffer.h"

/////////////////////////////////////////////////////////////////////////////
// CL_FreetypeFontProvider Construction:

CL_FreetypeFontProvider::CL_FreetypeFontProvider()
{
	FT_Error error = FT_Init_FreeType( &library );
	if ( error )
	{
		throw CL_Exception(cl_text("CL_FreetypeFontProvider: Initializing FreeType library failed."));
	} 
}

CL_FreetypeFontProvider::~CL_FreetypeFontProvider()
{
	FT_Error error = FT_Done_FreeType(library);
	if ( error )
	{
		throw CL_Exception(cl_text("CL_FreetypeFontProvider: Denitializing FreeType library failed."));
	} 
}

/////////////////////////////////////////////////////////////////////////////
// CL_FreetypeFontProvider Attributes:

CL_FreetypeFontProvider &CL_FreetypeFontProvider::instance()
{
	static CL_FreetypeFontProvider provider;
	return provider;
}


/////////////////////////////////////////////////////////////////////////////
// CL_FreetypeFontProvider Operations:

CL_FreetypeFont *CL_FreetypeFontProvider::load_font(CL_IODevice io_dev, double height, double average_width)
{
	FT_Face face;

	if (average_width<0.0)
	{
		throw CL_Exception(cl_text("Freetype error: average_width is invalid"));
	}
	if (height<=0.0)
	{
		throw CL_Exception(cl_text("Freetype error: height is invalid"));
	}
	
	CL_DataBuffer databuf(io_dev.get_size());
	io_dev.read(databuf.get_data(), databuf.get_size());

	FT_Error error = FT_New_Memory_Face( library, (FT_Byte*)databuf.get_data(), databuf.get_size(), 0, &face);

	if ( error == FT_Err_Unknown_File_Format )
	{
		throw CL_Exception(cl_text("Freetype error: The font file could be opened and read, but it appears  that its font format is unsupported"));
	}
	else if ( error )
	{
		throw CL_Exception(cl_text("Freetype error: Font file could not be opened or read, or is corrupted."));
	}

	FT_Set_Char_Size( face, (int)(average_width*64.0), (int)(height*64.0), 0, 0 );

	return new CL_FreetypeFont(face, databuf);
}

/////////////////////////////////////////////////////////////////////////////
// CL_FreetypeFontProvider Implementation:



