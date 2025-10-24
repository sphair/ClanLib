
#pragma once

class View;

class ViewWorkspace : public CL_GUIComponent
{
public:
	ViewWorkspace(CL_GUIComponent *parent);
	~ViewWorkspace();

	void add_view(View *view);
	void remove_view(View *view);
	void show_view(View *view);
	void show_view(int index);
	int find_view_index(View *view) const;
	int get_current_view_index() const;
	void set_view_color(View *view, CL_Colorf &color);

	CL_Callback_v1<View *> cb_view_close;

private:
	void on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect);
	void on_resized();
	void on_process_message(CL_GUIMessage &msg);
	void on_input_message(const CL_GUIMessage_Input &msg);
	void on_pointer_message(const CL_GUIMessage_Pointer &msg);

	void paint_tabs(CL_GraphicContext &gc, const CL_Rect &clip_rect);
	CL_Rect get_workspace_area();

	struct ViewPage
	{
		ViewPage(const CL_String &text, View *view) : text(text), color(CL_Colorf::black), view(view) { }
		bool operator ==(const ViewPage &other) const { return view == other.view; }

		CL_String text;
		CL_Colorf color;
		CL_SpanLayout span;
		CL_Rect position;
		View *view;
	};

	bool hot;
	CL_Font font_tab;
	CL_Image image_cross, image_cross_hot, image_cross_empty;
	CL_GUIThemePart part_background;
	CL_GUIThemePart part_tab;
	std::vector<ViewPage> pages;
	int current_page_index;
};
