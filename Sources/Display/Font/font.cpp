/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    Magnus Norddahl
*/

#include "Display/precomp.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/TargetProviders/font_provider.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/Font/font_system.h"
#include "API/Core/Text/string_help.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Font_Impl Class:

class CL_Font_Impl
{
public:
	CL_Font_Impl() : provider(0) { }

	~CL_Font_Impl()
	{
		if (provider)
			provider->destroy();
	}

	CL_FontProvider *provider;
};

/////////////////////////////////////////////////////////////////////////////
// CL_Font Construction:

CL_Font::CL_Font()
{
}

CL_Font::CL_Font( CL_GraphicContext &context, const CL_StringRef &typeface_name, int height)
: impl(new CL_Font_Impl)
{
	CL_FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_height(height);

	CL_Font_System new_font(context, typeface_name, height);
	*this = new_font;
}

CL_Font::CL_Font( CL_GraphicContext &context,const CL_FontDescription &desc)
: impl(new CL_Font_Impl)
{
	CL_Font_System new_font(context, desc);
	*this = new_font;
}

CL_Font::CL_Font(CL_FontProvider *provider)
: impl(new CL_Font_Impl)
{
	impl->provider = provider;
}

CL_Font::CL_Font(const CL_Font &copy)
{
	impl = copy.impl;
}

CL_Font::~CL_Font()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Font Attributes:

bool CL_Font::is_null()
{
	return impl.is_null();
}

CL_FontProvider *CL_Font::get_provider() const
{
	if (impl.is_null())
		return 0;
	return impl->provider;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Font Operations:

void CL_Font::draw_text(CL_GraphicContext &gc, float dest_x, float dest_y, const CL_StringRef &text, const CL_Colorf &color)
{
	if (!impl.is_null())
	{
		CL_FontMetrics fm = get_font_metrics(gc);
		int line_spacing = fm.get_height() + fm.get_external_leading();
		std::vector<CL_String> lines = CL_StringHelp::split_text(text, "\n", false);
		for (std::vector<CL_String>::size_type i=0; i<lines.size(); i++)
		{
			get_provider()->draw_text(gc, dest_x, dest_y, lines[i], color);
			dest_y += line_spacing;
		}
	}
}

void CL_Font::draw_text(CL_GraphicContext &gc, int dest_x, int dest_y, const CL_StringRef &text, const CL_Colorf &color)
{
	draw_text(gc, (float) dest_x, (float) dest_y, text, color);
}

void CL_Font::draw_text(CL_GraphicContext &gc, const CL_Pointf &position, const CL_StringRef &text, const CL_Colorf &color)
{
	draw_text(gc, position.x, position.y, text, color);
}

CL_Size CL_Font::get_text_size(CL_GraphicContext &gc, const CL_StringRef &text)
{
	CL_Size total_size;

	if (!impl.is_null())
	{
		CL_FontMetrics fm = get_font_metrics(gc);
		int line_spacing = fm.get_external_leading();
		std::vector<CL_String> lines = CL_StringHelp::split_text(text, "\n", false);
		for (std::vector<CL_String>::size_type i=0; i<lines.size(); i++)
		{
			CL_Size line_size = get_provider()->get_text_size(gc, lines[i]);

			if ((line_size.width == 0) && (line_size.height == 0) && (lines.size() > 1)) // blank line
				line_size.height = fm.get_descent() + fm.get_ascent(); 

			if ((i+1) != lines.size())	// Do not add the line spacing on the last line
				line_size.height += line_spacing;

			if (total_size.width < line_size.width)	// Find the widest line
				total_size.width = line_size.width;

			total_size.height += line_size.height;
		}
	}
	
	return total_size;
}

CL_FontMetrics CL_Font::get_font_metrics(CL_GraphicContext &gc)
{
	if (!impl.is_null())
		return get_provider()->get_font_metrics(gc);
	return CL_FontMetrics();
}

int CL_Font::get_character_index(CL_GraphicContext &gc, const CL_String &text, const CL_Point &point)
{
	if (!impl.is_null())
		return get_provider()->get_character_index(gc, text, point);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Font Implementation:


