/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#include "API/Display/Font/path_font.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Core/IOData/file_system.h"
#include "path_font_impl.h"
#include "API/Display/2D/canvas.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/utf8_reader.h"
#include "API/Core/IOData/path_help.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// PathFont Construction:

PathFont::PathFont()
{
}

PathFont::PathFont(const std::string &typeface_name, int height)
{
	FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_height(height);
	*this = PathFont(desc);
}

PathFont::PathFont(const FontDescription &desc)
{
	*this = PathFont(desc, "");
}

PathFont::PathFont(const std::string &typeface_name, int height, const std::string &ttf_filename)
{
	FontDescription desc;
	desc.set_typeface_name(typeface_name);
	desc.set_height(height);
	*this = PathFont(desc, ttf_filename);
}

PathFont::PathFont(const FontDescription &desc, const std::string &ttf_filename) : impl(std::make_shared<PathFont_Impl>())
{
	std::string path = PathHelp::get_fullpath(ttf_filename, PathHelp::path_type_file);
	std::string new_filename = PathHelp::get_filename(ttf_filename, PathHelp::path_type_file);
	FileSystem vfs(path);
	impl->load_font(desc, new_filename, vfs);
}

PathFont::PathFont(const FontDescription &desc, const std::string &ttf_filename, FileSystem fs) : impl(std::make_shared<PathFont_Impl>())
{
	impl->load_font(desc, ttf_filename, fs);
}

PathFont::~PathFont()
{

}

/////////////////////////////////////////////////////////////////////////////
// PathFont Attributes:

bool PathFont::is_null() const
{
	return !impl;
}

GlyphMetrics PathFont::get_metrics(Canvas &canvas, unsigned int glyph)
{
	if (impl)
		return impl->get_metrics(canvas, glyph);
	return GlyphMetrics();
}

GlyphMetrics PathFont::measure_text(Canvas &canvas, const std::string &string)
{
	if (impl)
		return impl->measure_text(canvas, string);
	return GlyphMetrics();
}

FontMetrics PathFont::get_font_metrics()
{
	if (impl)
		return impl->get_font_metrics();
	return FontMetrics();
}

/////////////////////////////////////////////////////////////////////////////
// PathFont Operations:

void PathFont::draw_text(Canvas &canvas, const Pointf &position, const std::string &text, const Brush &brush)
{
	if (impl)
		impl->draw_text(canvas, position, text, brush);
}

/////////////////////////////////////////////////////////////////////////////
// PathFont Implementation:

}
