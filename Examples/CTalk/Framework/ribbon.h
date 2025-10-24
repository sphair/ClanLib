
#pragma once

class RibbonPage;
class RibbonSection;
class RibbonButton;

class Ribbon : public CL_GUIComponent
{
public:
	Ribbon(CL_GUIComponent *container);
	~Ribbon();

	CL_Size get_preferred_size() const;

	void add_page(const RibbonPage &page);

private:
	void on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect);
	void on_resized() { request_repaint(); }

/* Why is this here?
	bool on_mouse_move(WPARAM wparam, int mouse_x, int mouse_y);
	bool on_lbutton_down(WPARAM wparam, int mouse_x, int mouse_y);
*/
	bool on_mouse_leave();
	void paint_tabs(CL_GraphicContext &gc, const CL_Rect &clip_rect);
	void paint_sections(CL_GraphicContext &gc, const CL_Rect &clip_rect);
	void paint_section_buttons(CL_GraphicContext &gc, RibbonSection &section, const CL_Rect &section_rect, const CL_Rect &clip_rect);
	void track_mouse_leave() { }

	bool hot;
	std::vector<RibbonPage> pages;
	CL_Font font_tab;
	CL_GUIThemePart part_background;
	CL_GUIThemePart part_tab;
	CL_GUIThemePart part_section;
	unsigned int current_page_index;
};

class RibbonPage
{
public:
	RibbonPage(const CL_String &text) : text(text) { }

	CL_String text;
	std::vector<RibbonSection> sections;
	CL_Rect position;
};

class RibbonSection
{
public:
	RibbonSection(const CL_String &text, int size) : text(text), size(size) { }

	CL_String text;
	int size;
	CL_Rect position;
	std::vector<RibbonButton> buttons;
};

class RibbonButton
{
public:
	RibbonButton(const CL_Image &icon, const CL_String &text) : icon(icon), text(text) { }

	CL_Image icon;
	CL_String text;
	CL_Rect position;
	CL_Callback_v0 cb_clicked;
};
