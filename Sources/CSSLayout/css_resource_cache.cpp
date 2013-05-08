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
*/

#include "CSSLayout/precomp.h"
#include "css_resource_cache.h"
#include "API/CSSLayout/ComputedValues/css_computed_values.h"
#include "Layout/LayoutTree/css_used_value.h"

namespace clan
{

CSSResourceCache::CSSResourceCache()
: dpi(96.0f)
{
#ifdef WIN32
	HDC screen_dc = GetDC(0);
	LOGFONT logfont = { 0 };
	logfont.lfFaceName[0] = 0;
	logfont.lfCharSet = DEFAULT_CHARSET;
	logfont.lfPitchAndFamily = DEFAULT_PITCH;
	BOOL result = EnumFontFamiliesEx(screen_dc, &logfont, &static_enum_font_families_callback, (LPARAM)this, 0);
	ReleaseDC(0, screen_dc);
#endif
}

CSSResourceCache::~CSSResourceCache()
{
}

void CSSResourceCache::set_dpi(float new_dpi)
{
	dpi = new_dpi;
}

#ifdef WIN32
int CSSResourceCache::enum_font_families_callback(const LOGFONTW *fontinfo, const TEXTMETRICW *textmetrics, DWORD font_type)
{
	std::string name = StringHelp::ucs2_to_utf8(fontinfo->lfFaceName);
	std::string search_name = StringHelp::text_to_lower(name);
	font_families[search_name] = true;
	return 1;
}

int CSSResourceCache::static_enum_font_families_callback(const LOGFONTW *fontinfo, const TEXTMETRICW *textmetrics, DWORD font_type, LPARAM lparam)
{
	return reinterpret_cast<CSSResourceCache*>(lparam)->enum_font_families_callback(fontinfo, textmetrics, font_type);
}
#endif

Font &CSSResourceCache::get_font(Canvas &canvas, const CSSComputedValues &properties)
{
	const CSSComputedFont &font_values = properties.get_font();
	int font_size = used_to_actual(font_values.font_size.length.value);
	std::string font_name;
	for (size_t i = 0; i < font_values.font_family.names.size(); i++)
	{
		bool matched = false;
		std::string search_name;
		switch (font_values.font_family.names[i].type)
		{
		case CSSValueFontFamilyName::type_family_name:
			search_name = StringHelp::text_to_lower(font_values.font_family.names[i].name);
			if (font_families.find(search_name) != font_families.end())
			{
				font_name = font_values.font_family.names[i].name;
				matched = true;
			}
			break;
		default:
		case CSSValueFontFamilyName::type_serif:
		case CSSValueFontFamilyName::type_cursive:
		case CSSValueFontFamilyName::type_fantasy:
			font_name = "Times New Roman"; // Ugliest font on the planet.
			matched = true;
			break;
		case CSSValueFontFamilyName::type_sans_serif:
			font_name = "Arial";
			matched = true;
			break;
		case CSSValueFontFamilyName::type_monospace:
			font_name = "Courier New";
			matched = true;
			break;
		}
		if (matched)
			break;
	}
	if (font_name.empty())
		font_name = "Times New Roman";

	int font_weight = 400;
	switch (font_values.font_weight.type)
	{
	case CSSValueFontWeight::type_100: font_weight = 100; break;
	case CSSValueFontWeight::type_200: font_weight = 200; break;
	case CSSValueFontWeight::type_300: font_weight = 300; break;
	case CSSValueFontWeight::type_400: font_weight = 400; break;
	case CSSValueFontWeight::type_500: font_weight = 500; break;
	case CSSValueFontWeight::type_600: font_weight = 600; break;
	case CSSValueFontWeight::type_700: font_weight = 700; break;
	case CSSValueFontWeight::type_800: font_weight = 800; break;
	case CSSValueFontWeight::type_900: font_weight = 900; break;
	case CSSValueFontWeight::type_normal: font_weight = 400; break;
	case CSSValueFontWeight::type_bold: font_weight = 700; break;
	case CSSValueFontWeight::type_bolder: font_weight = 900; break;
	case CSSValueFontWeight::type_lighter: font_weight = 300; break;
	}
	bool italic = false;
	switch (font_values.font_style.type)
	{
	case CSSValueFontStyle::type_normal: italic = false; break;
	case CSSValueFontStyle::type_italic: italic = true; break;
	case CSSValueFontStyle::type_oblique: italic = true; break;
	}
	std::string font_cache_name = string_format("%1+++%2+%3", font_name, font_size, font_weight);
	if (italic) font_cache_name += "i";
	std::map<std::string, Font>::iterator it = font_cache.find(font_cache_name);
	if (it == font_cache.end())
	{
		FontDescription font_desc;
		font_desc.set_typeface_name(font_name);
		font_desc.set_height(-font_size);
		font_desc.set_weight(font_weight);
		font_desc.set_italic(italic);
		font_cache[font_cache_name] = Font(canvas, font_desc);
		return font_cache[font_cache_name];
	}
	else
	{
		return it->second;
	}
}

Image &CSSResourceCache::get_image(Canvas &canvas, const std::string &url)
{
	std::map<std::string, Image>::iterator it = image_cache.find(url);
	if (it == image_cache.end())
	{
		Image image;
		if (!cb_get_image.is_null())
			image = cb_get_image.invoke(canvas, url);

		image_cache[url] = image;
		return image_cache[url];
	}
	else
	{
		return it->second;
	}
}

CSSLength CSSResourceCache::compute_length(const CSSLength &length, float em_size, float ex_size)
{
	float px_size = dpi / 96.0f;
/*
	if (is_printer)
	{
		if (printer.dpi == 300)
		{
			dpi = 300.0f;
			px_size = 3.0f;
		}
		else if (printer.dpi == 600)
		{
			dpi = 600.0f;
			px_size = 5.0f;
		}
		else
		{
			dpi = printer.dpi;
			px_size = (float)(int)(0.20f * dpi / 25.4f + 0.5f); // Find the closest pixel size matching 0.20mm
		}
	}
*/
	CSSLength new_length;
	new_length.type = CSSLength::type_computed_px;
	switch (length.type)
	{
	case CSSLength::type_computed_px:
		new_length.value = length.value;
		break;
	case CSSLength::type_mm:
		new_length.value = length.value * dpi / 25.4f;
		break;
	case CSSLength::type_cm:
		new_length.value = length.value * dpi / 2.54f;
		break;
	case CSSLength::type_in:
		new_length.value = length.value * dpi;
		break;
	case CSSLength::type_pt:
		new_length.value = length.value * dpi / 72.0f;
		break;
	case CSSLength::type_pc:
		new_length.value = length.value * dpi * 12.0f / 72.0f;
		break;
	case CSSLength::type_em:
		new_length.value = length.value * em_size;
		break;
	case CSSLength::type_ex:
		new_length.value = length.value * ex_size;
		break;
	case CSSLength::type_px:
		new_length.value = length.value * px_size;
		break;
	default:
		throw Exception("Unknown css length dimension!");
	}
	return new_length;
}

std::vector<std::string> CSSResourceCache::get_default_quotes()
{
	std::vector<std::string> values;
	values.push_back("\"");
	values.push_back("\"");
	return values;
}

Colorf CSSResourceCache::get_default_color()
{
	return Colorf::black;
}

CSSLength CSSResourceCache::get_font_table_size(int size)
{
	// To do: Return 13px for Monospace font type (so defaults match Firefox)
	return CSSLength(16.0f, CSSLength::type_px);
}

CSSLength CSSResourceCache::get_font_table_smaller(float em_size)
{
	return CSSLength(14.0f, CSSLength::type_px);
}

CSSLength CSSResourceCache::get_font_table_larger(float em_size)
{
	return CSSLength(18.0f, CSSLength::type_px);
}

}
