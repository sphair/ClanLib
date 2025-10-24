
#pragma once

#include <uxtheme.h>

class WindowsTheme : public CL_GUIThemeProvider
{
public:
	WindowsTheme(CL_GUITheme *fallback_theme);
	~WindowsTheme();

	void destroy();

	CL_StringRef get_property(const CL_GUIThemePart &part, const CL_StringRef &name, const CL_StringRef &css_value);
	CL_ResourceManager get_resources() const;

	void set_resources(CL_ResourceManager resources);
	void render_box(CL_GraphicContext &gc, CL_GUIThemePart &part, const CL_Rect &rect, const CL_Rect &clip_rect);
	CL_Rect render_text(CL_GraphicContext &gc, CL_GUIThemePart &part, CL_Font &font, const CL_StringRef &text, const CL_Rect &content_rect, const CL_Rect &clip_rect);

	void component_destroyed(CL_GUIComponent *component);

/*	CL_Size get_minimum_size(const CL_GUIThemePart &part) const;
	CL_Size get_maximum_size(const CL_GUIThemePart &part) const;
	CL_Size get_preferred_size(const CL_GUIThemePart &part) const;
	CL_Rect get_content_box(const CL_GUIThemePart &part, const CL_Rect &render_box_rect) const;
	CL_Rect get_content_shrink_box(const CL_GUIThemePart &part) const;
	CL_Rect get_render_box(const CL_GUIThemePart &part, const CL_Rect &content_box_rect) const;
	
*/
private:
	struct UxPart
	{
		UxPart() : theme(0), part_id(0), state_id(0) { }

		HTHEME theme;
		int part_id;
		int state_id;
	};

	UxPart get_ux_part(const CL_GUIThemePart &part) const;

	CL_GUITheme *fallback_theme;
	HWND hwnd;
};
