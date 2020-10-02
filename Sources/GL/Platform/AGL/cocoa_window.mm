/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
**    Magnus Norddahl
*/

#include "GL/precomp.h"
#include "API/Core/Math/rect.h"
#include "API/Core/Math/point.h"
#include "API/Core/Text/logger.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/memory_device.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/display.h"
#include "API/Display/2D/color.h"
#include "API/Display/display_target.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Window/keys.h"
#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/ImageProviders/png_provider.h"
#include "API/Display/screen_info.h"
#include "Display/Window/input_context_impl.h"
#include "cocoa_window.h"
#include "cocoa_mouse_input_device_provider.h"

namespace clan
{

CocoaWindow::CocoaWindow()
: site(0), window(0), controller(0)
{
    mouse = InputDevice(new CocoaMouseInputDeviceProvider());
    ic.add_mouse(mouse);
}

CocoaWindow::~CocoaWindow()
{
    if (controller)
        [controller release];
}

Rect CocoaWindow::get_geometry() const
{
    CGRect box = [window frame];
    return Rect(box.origin.x, box.origin.y, box.origin.x + box.size.width, box.origin.y + box.size.height);
}

Rect CocoaWindow::get_viewport() const
{
    CGRect box = [window bounds];
    return Rect(box.origin.x, box.origin.y, box.origin.x + box.size.width, box.origin.y + box.size.height);
}

bool CocoaWindow::has_focus() const
{
    return true;
}

bool CocoaWindow::is_minimized() const
{
    return false;
}

bool CocoaWindow::is_maximized() const
{
    return false;
}

bool CocoaWindow::is_fullscreen() const
{
    return true;
}

bool CocoaWindow::is_visible() const
{
    return true;
}

Size CocoaWindow::get_minimum_size(bool client_area) const
{
    return get_viewport().get_size();
}

Size CocoaWindow::get_maximum_size(bool client_area) const
{
    return get_viewport().get_size();
}

std::string CocoaWindow::get_title() const
{
    return std::string();
}

bool CocoaWindow::is_clipboard_text_available() const
{
    return false;
}

bool CocoaWindow::is_clipboard_image_available() const
{
    return false;
}

CAEAGLLayer *CocoaWindow::get_drawable()
{
    return (CAEAGLLayer*)[[controller view] layer];
}

void CocoaWindow::create(DisplayWindowSite *new_site, const DisplayWindowDescription &description)
{
    site = new_site;
    if (!window)
    {
        window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
        controller = [[CocoaController alloc] init];
        window.rootViewController = controller;
        [controller setClanLibWindow:this];
        [window makeKeyAndVisible];
    }
}

Point CocoaWindow::client_to_screen(const Point &client)
{
    return client;
}

Point CocoaWindow::screen_to_client(const Point &screen)
{
    return screen;
}

void CocoaWindow::show_system_cursor()
{

}

void CocoaWindow::set_cursor(CursorProvider_Cocoa *cursor)
{

}

void CocoaWindow::set_cursor(enum StandardCursor type)
{

}

void CocoaWindow::hide_system_cursor()
{

}

void CocoaWindow::set_large_icon(const PixelBuffer &image)
{

}

void CocoaWindow::set_small_icon(const PixelBuffer &image)
{

}

void CocoaWindow::set_title(const std::string &new_title)
{

}

void CocoaWindow::set_enabled(bool enable)
{

}

void CocoaWindow::set_position(const Rect &pos, bool client_area)
{

}

void CocoaWindow::set_size(int width, int height, bool client_area)
{

}

void CocoaWindow::set_minimum_size(int width, int height, bool client_area)
{

}

void CocoaWindow::set_maximum_size(int width, int height, bool client_area)
{

}

void CocoaWindow::minimize()
{

}

void CocoaWindow::restore()
{

}

void CocoaWindow::maximize()
{

}

void CocoaWindow::show(bool activate)
{

}

void CocoaWindow::hide()
{

}

void CocoaWindow::bring_to_front()
{

}

void CocoaWindow::capture_mouse(bool capture)
{

}

void CocoaWindow::set_clipboard_text(const std::string &text)
{

}

std::string CocoaWindow::get_clipboard_text() const
{
    return std::string();
}

PixelBuffer CocoaWindow::get_clipboard_image() const
{
    return PixelBuffer();
}

void CocoaWindow::set_clipboard_image(const PixelBuffer &image)
{

}

void CocoaWindow::request_repaint(const Rect &rect)
{
    CGRect box;
    box.origin.x = rect.left;
    box.origin.y = rect.top;
    box.size.width = rect.get_width();
    box.size.height = rect.get_height();
    [window setNeedsDisplayInRect:box];
}

void CocoaWindow::set_modifier_keys(InputEvent &key)
{

}

void CocoaWindow::on_touches_began(const std::vector<Vec2f> &touches)
{
    get_mouse()->touches = touches;
    get_mouse()->down = true;

    InputEvent e;
    e.type = InputEvent::pressed;
    e.id = GL_MOUSE_LEFT;
    e.mouse_pos = Point(touches[0]);
    e.device = mouse;
    get_mouse()->sig_provider_event->invoke(e);
    ic.process_messages();
}

void CocoaWindow::on_touches_moved(const std::vector<Vec2f> &touches)
{
    get_mouse()->touches = touches;

    InputEvent e;
    e.type = InputEvent::pointer_moved;
    e.mouse_pos = Point(touches[0]);
    e.device = mouse;
    get_mouse()->sig_provider_event->invoke(e);
    ic.process_messages();
}

void CocoaWindow::on_touches_ended(const std::vector<Vec2f> &touches)
{
    get_mouse()->touches = touches;
    get_mouse()->down = false;

    InputEvent e;
    e.type = InputEvent::released;
    e.id = GL_MOUSE_LEFT;
    e.mouse_pos = Point(touches[0]);
    e.device = mouse;
    get_mouse()->sig_provider_event->invoke(e);
    ic.process_messages();
}

void CocoaWindow::on_touches_cancelled(const std::vector<Vec2f> &touches)
{
    get_mouse()->touches = touches;
    get_mouse()->down = false;

    InputEvent e;
    e.type = InputEvent::released;
    e.id = GL_MOUSE_LEFT;
    e.mouse_pos = Point(touches[0]);
    e.device = mouse;
    get_mouse()->sig_provider_event->invoke(e);
    ic.process_messages();
}

CocoaMouseInputDeviceProvider *CocoaWindow::get_mouse()
{
    return dynamic_cast<CocoaMouseInputDeviceProvider*>(mouse.get_provider());
}

}
