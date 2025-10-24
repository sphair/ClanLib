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
**    Harry Storbacka
**    Kenneth Gangstoe
**    Mark Page
*/

#pragma once

#include "API/Core/Resources/resource_manager.h"
#include "API/Display/2D/image.h"
#include "API/Core/Math/rect.h"
#include "API/GUI/gui_theme_part_property.h"
#include "API/GUI/Providers/gui_theme_provider.h"

#include <map>

class CL_GUIComponent;
class CL_GUIThemePart;
class CL_GraphicContext;

class CL_GUIThemeProvider_Default : public CL_GUIThemeProvider
{
/// \name Construction
/// \{
public:
	CL_GUIThemeProvider_Default();
	~CL_GUIThemeProvider_Default();

/// \}
/// \name Attributes
/// \{
public:
	CL_StringRef get_property(
		const CL_GUIThemePart &part,
		const CL_StringRef &name,
		const CL_StringRef &css_value);

	CL_ResourceManager get_resources() const {return resources;}

/// \}
/// \name Operations
/// \{
public:
	void destroy();

	void set_resources(CL_ResourceManager new_resources) {resources = new_resources;}

	void render_box(CL_GraphicContext &gc, CL_GUIThemePart &part, const CL_Rect &rect, const CL_Rect &clip_rect);

	void component_destroyed(CL_GUIComponent *component);

	CL_Rect render_text(
		CL_GraphicContext &gc,
		CL_GUIThemePart &part,
		CL_Font &font,
		const CL_StringRef &text,
		const CL_Rect &content_rect,
		const CL_Rect &clip_rect);

/// \}
/// \name Implementation
/// \{
public:
	CL_ResourceManager resources;

	struct ComponentCache
	{
		CL_GUIComponent *component;
		std::map<CL_String, CL_Image> images;
	};

	struct ImagePart
	{
		CL_Rect source_rect;
		CL_Rect dest_rect;
	};

	std::vector<ImagePart> calc_stretch_image_parts(
		const CL_Rect &rect,
		const CL_Image &sprite,
		int sizing_left,
		int sizing_top,
		int sizing_right,
		int sizing_bottom);

	ComponentCache *get_component_cache(CL_GUIComponent *component);

	CL_Image &get_image(CL_GraphicContext &gc, CL_GUIComponent *component, const CL_StringRef &name);

	std::map<CL_GUIComponent *, ComponentCache *> cache;

	CL_GUIThemePartProperty prop_no_draw; // cl_text("no-draw")
	CL_GUIThemePartProperty prop_bg_image; // cl_text("bg-image")
	CL_GUIThemePartProperty prop_bg_image_sizing; // cl_text("bg-image-sizing")
	CL_GUIThemePartProperty prop_bg_image_sizing_left; // cl_text("bg-image-sizing-left")
	CL_GUIThemePartProperty prop_bg_image_sizing_top; // cl_text("bg-image-sizing-top")
	CL_GUIThemePartProperty prop_bg_image_sizing_right; // cl_text("bg-image-sizing-right")
	CL_GUIThemePartProperty prop_bg_image_sizing_bottom; // cl_text("bg-image-sizing-bottom")
	CL_GUIThemePartProperty prop_margin_top; // cl_text("margin-top"), cl_text("0")
	CL_GUIThemePartProperty prop_margin_left; // cl_text("margin-left"), cl_text("0")
	CL_GUIThemePartProperty prop_margin_right; // cl_text("margin-right"), cl_text("0")
	CL_GUIThemePartProperty prop_margin_bottom; // cl_text("margin-bottom"), cl_text("0")
	CL_GUIThemePartProperty prop_border_top; // cl_text("border-top"), cl_text("0")
	CL_GUIThemePartProperty prop_border_left; // cl_text("border-left"), cl_text("0")
	CL_GUIThemePartProperty prop_border_right; // cl_text("border-right"), cl_text("0")
	CL_GUIThemePartProperty prop_border_bottom; // cl_text("border-bottom"), cl_text("0")
	CL_GUIThemePartProperty prop_bg_color; // cl_text("bg-color")
	CL_GUIThemePartProperty prop_border_color; // cl_text("border-color")
	CL_GUIThemePartProperty prop_text_color; // CssStr::text_color, cl_text("black")
	CL_GUIThemePartProperty prop_align; // CssStr::align, CssStr::left
	CL_GUIThemePartProperty prop_vertical_align; // CssStr::vertical_align, CssStr::center

/// \}
};
