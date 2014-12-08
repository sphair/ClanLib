/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
**    Harry Storbacka
**    Mark Page
*/

#include "Display/precomp.h"

#include "API/Display/Font/vector_font.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Core/IOData/file_system.h"
#include "API/Display/2D/shape2d.h"
#include "vector_font_impl.h"
#include "API/Display/2D/canvas.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/utf8_reader.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// VectorFont Construction:

VectorFont::VectorFont()
{
}

VectorFont::VectorFont( Canvas &canvas, const std::string &typeface_name, int height, const std::string &filename)
{
	FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_height(height);
	*this = VectorFont(canvas, desc, filename);
}

VectorFont::VectorFont( Canvas &canvas, const FontDescription &desc, const std::string &filename) : impl(std::make_shared<VectorFont_Impl>())
{
	impl->load_font(desc, filename);
}

VectorFont::~VectorFont()
{

}

/////////////////////////////////////////////////////////////////////////////
// VectorFont Attributes:

bool VectorFont::is_null() const
{
	return !impl;
}

GlyphMetrics VectorFont::get_metrics(Canvas &canvas, unsigned int glyph)
{
	if (impl)
		return impl->get_metrics(canvas, glyph);
	return GlyphMetrics();
}

GlyphMetrics VectorFont::get_metrics(Canvas &canvas, const std::string &string)
{
	if (impl)
		return impl->get_metrics(canvas, string);
	return GlyphMetrics();
}

FontMetrics VectorFont::get_font_metrics()
{
	if (impl)
		return impl->get_font_metrics();
	return FontMetrics();
}

/////////////////////////////////////////////////////////////////////////////
// VectorFont Operations:

void VectorFont::draw(Canvas &canvas, const Pointf &position, const std::string &text, const Colorf &color)
{
	if (impl)
		impl->draw(canvas, position, text, color);
}

/////////////////////////////////////////////////////////////////////////////
// VectorFont Implementation:

}
