
#include "precomp.h"
#include "controls.h"

using namespace clan;

Control::Control()
{
	InitCommonControls();
}

Control::~Control()
{
	if (font_handle)
		DeleteObject(font_handle);
	font_handle = 0;

	if (handle)
		DestroyWindow(handle);
	handle = 0;
}

HWND Control::control_handle() const
{
	if (!handle)
	{
		WindowView *root = const_cast<WindowView*>(dynamic_cast<const WindowView*>(root_view()));
		if (root)
			handle = create_control(root->get_display_window().get_handle().hwnd, GetModuleHandle(0));
	}
	return handle;
}

void Control::layout_subviews(Canvas &canvas)
{
	if (control_handle())
	{
		if (font_handle)
			DeleteObject(font_handle);
		font_handle = 0;

		// To do: implement style_cascade().get_font().get_handle() so the font style gets applied to common controls
		int font_size = (int)std::round(style_cascade().computed_value("font-size").number() * canvas.get_pixel_ratio());
		font_handle = CreateFont(-font_size, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Segoe UI");
		SendMessage(handle, WM_SETFONT, (WPARAM)font_handle, FALSE);


		Pointf pos = canvas.grid_fit(to_root_pos(Pointf()) + root_view()->geometry().content_pos());

		int x = (int)std::round(pos.x * canvas.get_pixel_ratio());
		int y = (int)std::round(pos.y * canvas.get_pixel_ratio());
		int width = (int)std::round(geometry().content_width * canvas.get_pixel_ratio());
		int height = (int)std::round(geometry().content_height * canvas.get_pixel_ratio());

		SetWindowPos(handle, 0, x, y, width, height, SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOACTIVATE);
	}
}

float Control::calculate_preferred_width(Canvas &canvas)
{
	if (style_cascade().computed_value("width").is_keyword("auto"))
		return 100.0f;
	else
		return View::calculate_preferred_width(canvas);
}

float Control::calculate_preferred_height(Canvas &canvas, float width)
{
	if (style_cascade().computed_value("height").is_keyword("auto"))
		return 25.0f;
	else
		return View::calculate_preferred_height(canvas, width);
}

float Control::calculate_first_baseline_offset(Canvas &canvas, float width)
{
	return calculate_preferred_height(canvas, width);
}

float Control::calculate_last_baseline_offset(Canvas &canvas, float width)
{
	return calculate_first_baseline_offset(canvas, width);
}
