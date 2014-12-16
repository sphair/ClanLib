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
*/

#include "Display/precomp.h"
#include "API/Display/Font/font_face.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Font/font_description.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/utf8_reader.h"
#include "API/Core/XML/dom_element.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/Resources/display_cache.h"
#include "font_face_impl.h"

namespace clan
{

	FontFace::FontFace() : impl(std::make_shared<FontFace_Impl>())
	{

	}

	void FontFace::add(const std::string &typeface_name, int height)
	{
		FontDescription desc;
		desc.set_typeface_name(typeface_name);
		desc.set_height(height);
		FileSystem fs;
		impl->load_font(desc, "", fs);
	}

	void FontFace::add(const FontDescription &desc)
	{
		FileSystem fs;
		impl->load_font(desc, "", fs);
	}

	void FontFace::add(const FontDescription &desc, const std::string &ttf_filename)
	{
		std::string path = PathHelp::get_fullpath(ttf_filename, PathHelp::path_type_file);
		std::string new_filename = PathHelp::get_filename(ttf_filename, PathHelp::path_type_file);
		FileSystem fs(path);
		impl->load_font(desc, new_filename, fs);
	}

	void FontFace::add(const FontDescription &desc, const std::string &ttf_filename, FileSystem fs)
	{
		impl->load_font(desc, ttf_filename, fs);
	}

	void FontFace::add(Canvas &canvas, Sprite &sprite, const std::string &glyph_list, int spacelen, bool monospace, const FontMetrics &metrics)
	{
		impl->load_font(canvas, sprite, glyph_list, spacelen, monospace, metrics);
	}

}
