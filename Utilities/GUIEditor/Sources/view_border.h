
#ifndef _header_view_border_
#define _header_view_border_

class ViewBorder : public CL_GUIComponent
{
public:
	ViewBorder(CL_GUIComponent *parent);

	void on_resized();
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);

	CL_GUIThemePart theme_part;
};

#endif
