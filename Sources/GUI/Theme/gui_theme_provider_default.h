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

namespace clan
{

class GUIComponent;
class GUIThemePart;
class Canvas;

class GUIThemeProvider_Default : public GUIThemeProvider
{
/// \name Construction
/// \{
public:
	GUIThemeProvider_Default();
	~GUIThemeProvider_Default();

/// \}
/// \name Attributes
/// \{
public:
	std::string get_property(
		const GUIThemePart &part,
		const std::string &name,
		const std::string &css_value);

	ResourceManager get_resources() const { return resources; }

/// \}
/// \name Operations
/// \{
public:
	void add_resources(ResourceManager new_resources);
	void set_resources(ResourceManager new_resources);
	void render_box(Canvas &canvas, GUIThemePart &part, const Rect &rect, const Rect &clip_rect);
	void component_destroyed(GUIComponent *component);
	Rect render_text(
		Canvas &canvas,
		GUIThemePart &part,
		Font &font,
		const std::string &text,
		const Rect &content_rect,
		const Rect &clip_rect);

/// \}
/// \name Implementation
/// \{
public:
	ResourceManager resources;

	struct ComponentCache
	{
		GUIComponent *component;
		std::map<std::string, Image> images;
	};

	struct ImagePart
	{
		Rect source_rect;
		Rect dest_rect;
	};

	std::vector<ImagePart> calc_stretch_image_parts(
		const Rect &rect,
		const Image &sprite,
		GUIThemePart &part);

	ComponentCache *get_component_cache(GUIComponent *component);
	Image &get_image(Canvas &canvas, GUIComponent *component, const std::string &name);

	bool is_no_draw(GUIThemePart &part);
	bool is_image_background(GUIThemePart &part);
	Image &get_background_image(Canvas &canvas, GUIThemePart &part);
	Rect get_background_rect(GUIThemePart &part, const Rect &rect);
	Rect get_inner_border_rect(GUIThemePart &part, const Rect &rect);
	Rect get_left_border(const Rect &outer_rect, const Rect &inner_rect);
	Rect get_top_border(const Rect &outer_rect, const Rect &inner_rect);
	Rect get_right_border(const Rect &outer_rect, const Rect &inner_rect);
	Rect get_bottom_border(const Rect &outer_rect, const Rect &inner_rect);

	void render_background(Canvas &canvas, GUIThemePart &part, const Rect &rect, const Rect &clip_rect);
	void render_image_background(Canvas &canvas, GUIThemePart &part, const Rect &rect, const Rect &clip_rect);
	void render_image_tile_background(Canvas &canvas, GUIThemePart &part, const Rect &rect, const Rect &clip_rect);
	void render_image_fixed_background(Canvas &canvas, GUIThemePart &part, const Rect &rect, const Rect &clip_rect, const std::string &size_mode);
	void render_image_repeat_background(Canvas &canvas, GUIThemePart &part, const Rect &rect, const Rect &clip_rect);
	void render_image_repeat_horizontally(Canvas &canvas, ImagePart &ip, Image &image);
	void render_image_repeat_vertically(Canvas &canvas, ImagePart &ip, Image &image);
	void render_image_stretch_background(Canvas &canvas, GUIThemePart &part, const Rect &rect, const Rect &clip_rect);
	void debug_colorize_parts(Canvas &canvas, const std::vector<ImagePart> &parts);
	void render_solid_background(Canvas &canvas, GUIThemePart &part, const Rect &rect, const Rect &clip_rect);
	void render_border(Canvas &canvas, GUIThemePart &part, const Rect &rect, const Rect &clip_rect);

	std::map<GUIComponent *, ComponentCache *> cache;

	GUIThemePartProperty prop_no_draw; // "no-draw"
	GUIThemePartProperty prop_bg_image; // "bg-image"
	GUIThemePartProperty prop_bg_image_sizing; // "bg-image-sizing"
	GUIThemePartProperty prop_bg_image_sizing_left; // "bg-image-sizing-left"
	GUIThemePartProperty prop_bg_image_sizing_top; // "bg-image-sizing-top"
	GUIThemePartProperty prop_bg_image_sizing_right; // "bg-image-sizing-right"
	GUIThemePartProperty prop_bg_image_sizing_bottom; // "bg-image-sizing-bottom"
	GUIThemePartProperty prop_margin_top; // "margin-top", "0"
	GUIThemePartProperty prop_margin_left; // "margin-left", "0"
	GUIThemePartProperty prop_margin_right; // "margin-right", "0"
	GUIThemePartProperty prop_margin_bottom; // "margin-bottom", "0"
	GUIThemePartProperty prop_border_top; // "border-top", "0"
	GUIThemePartProperty prop_border_left; // "border-left", "0"
	GUIThemePartProperty prop_border_right; // "border-right", "0"
	GUIThemePartProperty prop_border_bottom; // "border-bottom", "0"
	GUIThemePartProperty prop_bg_color; // "bg-color"
	GUIThemePartProperty prop_border_color; // "border-color"
	GUIThemePartProperty prop_text_color; // CssStr::text_color, "black"
	GUIThemePartProperty prop_align; // CssStr::align, CssStr::left
	GUIThemePartProperty prop_vertical_align; // CssStr::vertical_align, CssStr::center

/// \}
};

}
