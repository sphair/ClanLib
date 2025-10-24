/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

class CL_Font;
class CL_GraphicContext;
class CL_CSSBoxElement;
class CL_CSSBoxText;
class CL_CSSBoxNode;
class CL_CSSBoxProperties;
class CL_CSSBoxLength;
class CL_CSSLayoutTreeNode;
class CL_CSSInlineLayout;
class CL_CSSLayoutCursor;
class CL_CSSBlockFormattingContext;

class CL_CSSResourceCache
{
public:
	CL_CSSResourceCache();
	~CL_CSSResourceCache();

	CL_Callback_2<CL_Image, CL_GraphicContext &, const CL_String &> cb_get_image;

	CL_CSSBoxLength compute_length(const CL_CSSBoxLength &length, float em_size, float ex_size);
	std::vector<CL_String> get_default_quotes();
	CL_Colorf get_default_color();
	CL_CSSBoxLength get_font_table_size(int size);
	CL_CSSBoxLength get_font_table_smaller(float em_size);
	CL_CSSBoxLength get_font_table_larger(float em_size);

	CL_Font &get_font(CL_GraphicContext &gc, const CL_CSSBoxProperties &properties);
	CL_Image &get_image(CL_GraphicContext &gc, const CL_String &url);

private:
#ifdef WIN32
	int enum_font_families_callback(const LOGFONTW *fontinfo, const TEXTMETRICW *textmetrics, DWORD font_type);
	static int CALLBACK static_enum_font_families_callback(const LOGFONTW *fontinfo, const TEXTMETRICW *textmetrics, DWORD font_type, LPARAM lparam);
#endif

	std::map<CL_String, CL_Font> font_cache;
	std::map<CL_String, CL_Image> image_cache;
	std::map<CL_String, bool> font_families;
};
