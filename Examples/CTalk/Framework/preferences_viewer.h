
#pragma once

class PreferencesPage;

class PreferencesViewer : public CL_Window
{
public:
	PreferencesViewer(CL_GUIComponent *owner);
	~PreferencesViewer();

	void add_page(PreferencesPage *page);

private:
	static CL_GUITopLevelDescription get_description();

	void on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect);
	void on_resized();
	bool on_input_pressed(const CL_InputEvent &input_event);
	bool on_input_released(const CL_InputEvent &input_event);
	bool on_input_pointer_moved(const CL_InputEvent &input_event);
	void on_ok_clicked();
	void on_cancel_clicked();

	void render_label_area(CL_GraphicContext &gc, const CL_Rect &label_area, const CL_Rect &clip_rect);
	void render_page_area(CL_GraphicContext &gc, const CL_Rect &page_area, const CL_Rect &clip_rect);

	CL_GUIThemePart part_background;
	CL_GUIThemePart part_label_area;
	CL_GUIThemePart part_label;
	CL_GUIThemePart part_page_area;
	CL_PushButton *button_ok;
	CL_PushButton *button_cancel;

	std::vector<PreferencesPage *> pages;
	size_t current_page_index;
};
