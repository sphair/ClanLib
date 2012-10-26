
#include "precomp.h"
#include "toplevel_window.h"
#include "window_manager.h"
#include "postprocess_scene.h"

TopLevelWindow::TopLevelWindow(WindowManager *wm, CL_GUITopLevelWindow *owner, CL_GUIComponent *component, CL_GUITopLevelDescription description)
: wm(wm), owner(owner), component(component), visible(description.is_visible()), enabled(true), minimized(false), maximized(false)
{
}

void TopLevelWindow::set_visible(bool show, bool activate)
{
	if (show != visible)
	{
		visible = show;
		if (visible)
			create_texture();
		else
			destroy_texture();
	}
}

void TopLevelWindow::set_geometry(const CL_Rect &box)
{
	bool resized = (box.get_size() != geometry.get_size());
	geometry = box;
	if (visible && resized)
	{
		destroy_texture();
		create_texture();
	}
}

void TopLevelWindow::create_texture()
{
	CL_GraphicContext gc = wm->scene->get_display_window().get_gc();
	texture = CL_Texture(gc, geometry.get_width(), geometry.get_height());
	frame_buffer = CL_FrameBuffer(gc);
	frame_buffer.attach_color_buffer(0, texture);
}

void TopLevelWindow::destroy_texture()
{
	texture = CL_Texture();
	frame_buffer = CL_FrameBuffer();
}
