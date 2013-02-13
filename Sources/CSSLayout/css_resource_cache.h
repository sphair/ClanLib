/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

#pragma once

#include <map>
#include "API/Display/Font/font.h"
#include "API/Display/2D/image.h"

namespace clan
{

class Image;
class Canvas;
class Font;
class GraphicContext;
class CSSBoxElement;
class CSSBoxText;
class CSSBoxNode;
class CSSComputedValues;
class CSSLength;
class CSSLayoutTreeNode;
class CSSInlineLayout;
class CSSLayoutCursor;
class CSSBlockFormattingContext;

class CSSResourceCache
{
public:
	CSSResourceCache();
	~CSSResourceCache();

	void set_dpi(float dpi);

	Callback_2<Image, Canvas &, const std::string &> cb_get_image;

	CSSLength compute_length(const CSSLength &length, float em_size, float ex_size);
	std::vector<std::string> get_default_quotes();
	Colorf get_default_color();
	CSSLength get_font_table_size(int size);
	CSSLength get_font_table_smaller(float em_size);
	CSSLength get_font_table_larger(float em_size);

	Font &get_font(Canvas &canvas, const CSSComputedValues &properties);
	Image &get_image(Canvas &canvas, const std::string &url);

private:
#ifdef WIN32
	int enum_font_families_callback(const LOGFONTW *fontinfo, const TEXTMETRICW *textmetrics, DWORD font_type);
	static int CALLBACK static_enum_font_families_callback(const LOGFONTW *fontinfo, const TEXTMETRICW *textmetrics, DWORD font_type, LPARAM lparam);
#endif

	std::map<std::string, Font> font_cache;
	std::map<std::string, Image> image_cache;
	std::map<std::string, bool> font_families;
	float dpi;
};

}
