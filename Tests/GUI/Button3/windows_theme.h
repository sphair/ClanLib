
#pragma once

#include <uxtheme.h>

class WindowsTheme : public GUIThemeProvider
{
public:
	WindowsTheme(GUITheme *fallback_theme);
	~WindowsTheme();

	void destroy();

	std::string get_property(const GUIThemePart &part, const std::string &name, const std::string &css_value);
	ResourceManager get_resources() const;

	void set_resources(ResourceManager resources);
	void render_box(GraphicContext &gc, GUIThemePart &part, const Rect &rect, const Rect &clip_rect);
	Rect render_text(GraphicContext &gc, GUIThemePart &part, Font &font, const std::string &text, const Rect &content_rect, const Rect &clip_rect);

	void component_destroyed(GUIComponent *component);

/*	Size get_minimum_size(const GUIThemePart &part) const;
	Size get_maximum_size(const GUIThemePart &part) const;
	Size get_preferred_size(const GUIThemePart &part) const;
	Rect get_content_box(const GUIThemePart &part, const Rect &render_box_rect) const;
	Rect get_content_shrink_box(const GUIThemePart &part) const;
	Rect get_render_box(const GUIThemePart &part, const Rect &content_box_rect) const;
	
*/
private:
	struct UxPart
	{
		UxPart() : theme(0), part_id(0), state_id(0) { }

		HTHEME theme;
		int part_id;
		int state_id;
	};

	UxPart get_ux_part(const GUIThemePart &part) const;

	GUITheme *fallback_theme;
	HWND hwnd;
};
