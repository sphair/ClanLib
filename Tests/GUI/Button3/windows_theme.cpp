
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gui.h>
using namespace clan;
#include "windows_theme.h"
#include <vsstyle.h>
#include <vssym32.h>

#pragma comment(lib, "uxtheme.lib")

WindowsTheme::WindowsTheme(GUITheme *fallback_theme)
: fallback_theme(fallback_theme), hwnd(0)
{
	WNDCLASS wndclass;

	wndclass.style = 0;
	wndclass.lpfnWndProc = DefWindowProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = GetModuleHandle(0);
	wndclass.hIcon = 0;
	wndclass.hCursor = 0;
	wndclass.hbrBackground = 0;
	wndclass.lpszMenuName = 0;
	wndclass.lpszClassName = TEXT("WindowsTheme");

	RegisterClass(&wndclass);

	hwnd = CreateWindow(
		TEXT("WindowsTheme"),
		TEXT("WindowsTheme"),
		0, // WS_POPUP,
		0,
		0,
		1,
		1,
		NULL,
		NULL,
		GetModuleHandle(0),
		NULL);
	SetWindowTheme(hwnd, 0, 0);
}

WindowsTheme::~WindowsTheme()
{
	DestroyWindow(hwnd);
}

void WindowsTheme::destroy()
{
	delete this;
}

std::string WindowsTheme::get_property(const GUIThemePart &part, const std::string &name, const std::string &css_value)
{
	return fallback_theme->get_property(part, name, css_value);
}

DisplayCache WindowsTheme::get_resources() const
{
	return fallback_theme->get_resources();
}

void WindowsTheme::set_resources(DisplayCache resources)
{
	fallback_theme->set_resources(resources);
}

void WindowsTheme::render_box(GraphicContext &gc, GUIThemePart &part, const Rect &rect, const Rect &clip_rect)
{
	UxPart uxpart = get_ux_part(part); 
	if (uxpart.theme)
	{
		HDC screen_dc = GetDC(hwnd);
		HDC bitmap_dc = CreateCompatibleDC(screen_dc);
		HBITMAP bitmap = CreateCompatibleBitmap(screen_dc, rect.get_width(), rect.get_height());
		HBITMAP old_bitmap = reinterpret_cast<HBITMAP>(SelectObject(bitmap_dc, bitmap));
		RECT rect_win32 = { 0, 0, rect.right-rect.left, rect.bottom-rect.top };
		RECT clip_rect_win32 = { clip_rect.left-rect.left, clip_rect.top-rect.top, clip_rect.right-rect.left, clip_rect.bottom-rect.top };
		HRESULT result = DrawThemeBackground(uxpart.theme, bitmap_dc, uxpart.part_id, uxpart.state_id, &rect_win32, &clip_rect_win32);
		if (SUCCEEDED(result))
		{
			PixelBuffer buffer(rect.get_width(), rect.get_height(), cl_argb8);
			BITMAPV5HEADER bmp_header;
			memset(&bmp_header, 0, sizeof(BITMAPV5HEADER));
			bmp_header.bV5Size = sizeof(BITMAPV5HEADER);
			bmp_header.bV5Width = rect.get_width();
			bmp_header.bV5Height = -rect.get_height();
			bmp_header.bV5Planes = 1;
			bmp_header.bV5BitCount = 32;
			bmp_header.bV5Compression = BI_RGB;
			GetDIBits(bitmap_dc, bitmap, 0, rect.get_height(), buffer.get_data(), (BITMAPINFO*)&bmp_header, DIB_PAL_COLORS);
			gc.draw_pixels((float)rect.left, (float)rect.top, buffer, buffer.get_size());
		}

		SelectObject(bitmap_dc, old_bitmap);
		DeleteDC(bitmap_dc);
		ReleaseDC(hwnd, screen_dc);
		CloseThemeData(uxpart.theme);
	}
	else
	{
		fallback_theme->render_box(gc, part, rect, clip_rect);
	}
}

Rect WindowsTheme::render_text(GraphicContext &gc, GUIThemePart &part, Font &font, const std::string &text, const Rect &content_rect, const Rect &clip_rect)
{
	UxPart uxpart = get_ux_part(part); 
	if (uxpart.theme)
	{
		LOGFONTW font_description_win32;
		memset(&font_description_win32, 0, sizeof(LOGFONT));
		HRESULT result = GetThemeFont(uxpart.theme, 0, uxpart.part_id, uxpart.state_id, TMT_BODYFONT, &font_description_win32);
		CloseThemeData(uxpart.theme);
		if (SUCCEEDED(result))
		{
			FontDescription font_description;
			font_description.set_typeface_name(font_description_win32.lfFaceName);
			font_description.set_height(font_description_win32.lfHeight);
			font_description.set_average_width(font_description_win32.lfWidth);
			font_description.set_escapement((float)font_description_win32.lfEscapement);
			font_description.set_orientation((float)font_description_win32.lfOrientation);
			font_description.set_weight(font_description_win32.lfWeight);
			font_description.set_italic(font_description_win32.lfItalic ? true : false);
			font_description.set_underline(font_description_win32.lfUnderline ? true : false);
			font_description.set_strikeout(font_description_win32.lfStrikeOut ? true : false);
			font_description.set_fixed_pitch(font_description_win32.lfPitchAndFamily & FIXED_PITCH);
			// font_description.set_anti_alias(?);
			Font uxfont(gc, font_description);
			uxfont.draw_text(gc, content_rect.left, content_rect.top, text, Colorf::black);
			
			return Rect(content_rect.get_top_left(), uxfont.get_text_size(gc, text));
		}
	}

	return fallback_theme->render_text(gc, part, font, text, content_rect, clip_rect);
}

void WindowsTheme::component_destroyed(GUIComponent *component)
{
	fallback_theme->component_destroyed(component);
}
/*
Size WindowsTheme::get_minimum_size(const GUIThemePart &part) const
{
	return fallback_theme->get_minimum_size(part);
}

Size WindowsTheme::get_maximum_size(const GUIThemePart &part) const
{
	return fallback_theme->get_maximum_size(part);
}

Size WindowsTheme::get_preferred_size(const GUIThemePart &part) const
{
	return fallback_theme->get_preferred_size(part);
}

Rect WindowsTheme::get_content_box(const GUIThemePart &part, const Rect &render_box_rect) const
{
	UxPart uxpart = get_ux_part(part); 
	if (uxpart.theme)
	{
		RECT bounding_rect = { render_box_rect.left, render_box_rect.top, render_box_rect.right, render_box_rect.bottom };
		RECT content_rect = { 0, 0, 0, 0 };
		HRESULT result = GetThemeBackgroundContentRect(uxpart.theme, 0, uxpart.part_id, uxpart.state_id, &bounding_rect, &content_rect);
		CloseThemeData(uxpart.theme);
		return Rect(content_rect.left, content_rect.top, content_rect.right, content_rect.bottom);
	}
	else
	{
		return fallback_theme->get_content_box(part, render_box_rect);
	}
}

Rect WindowsTheme::get_content_shrink_box(const GUIThemePart &part) const
{
	return fallback_theme->get_content_shrink_box(part);
}

Rect WindowsTheme::get_render_box(const GUIThemePart &part, const Rect &content_box_rect) const
{
	UxPart uxpart = get_ux_part(part); 
	if (uxpart.theme)
	{
		RECT content_rect_win32 = { content_box_rect.left, content_box_rect.top, content_box_rect.right, content_box_rect.bottom };
		RECT extent_box_win32 = { 0, 0, 0, 0 };
		HRESULT result = GetThemeBackgroundExtent(uxpart.theme, 0, uxpart.part_id, uxpart.state_id, &content_rect_win32, &extent_box_win32);
		CloseThemeData(uxpart.theme);
		return Rect(extent_box_win32.left, extent_box_win32.top, extent_box_win32.right, extent_box_win32.bottom);
	}
	else
	{
		return fallback_theme->get_render_box(part, content_box_rect);
	}
}
*/

WindowsTheme::UxPart WindowsTheme::get_ux_part(const GUIThemePart &part) const
{
	// To do: Extract type from part.get_element_name()
	UxPart uxpart;
	uxpart.theme = OpenThemeData(hwnd, L"button"); 
	if (uxpart.theme)
	{
		uxpart.part_id = BP_PUSHBUTTON;
		if (part.get_state("pressed"))
			uxpart.state_id = PBS_PRESSED;
		else if (part.get_state("disabled"))
			uxpart.state_id = PBS_DISABLED;
		else if (part.get_state("default"))
			uxpart.state_id = PBS_DEFAULTED;
		else if (part.get_state("hot"))
			uxpart.state_id = PBS_HOT;
		else
			uxpart.state_id = PBS_NORMAL;
	}
	return uxpart;
}
