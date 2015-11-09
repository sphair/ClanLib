/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
**    James Lammlein
**    Magnus Norddahl
*/

#include "GL/precomp.h"
#include "API/Display/Window/input_event.h"

#include "opengl_window_provider_osx.h"
#include "opengl_window_provider_osx_impl.h"
#include "input_device_provider_osxkeyboard.h"
#include "input_device_provider_osxmouse.h"

#import "cocoa_window.h"

namespace clan
{
	OpenGLWindowProvider::OpenGLWindowProvider(OpenGLContextDescription &opengl_desc)
	{
		impl.reset(new OpenGLWindowProvider_Impl(this, opengl_desc));

		keyboard = InputDevice(new InputDeviceProvider_OSXKeyboard(this));
		mouse = InputDevice(new InputDeviceProvider_OSXMouse(this));
	}

	OpenGLWindowProvider::~OpenGLWindowProvider()
	{
		get_keyboard_provider()->dispose();
		get_mouse_provider()->dispose();
	}

	ProcAddress *OpenGLWindowProvider::get_proc_address(const std::string& function_name) const
	{
		static CFBundleRef bundle = 0;
		if (bundle == 0)
		{
			bundle = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.opengl"));
			if (bundle == 0)
				throw Exception("Unable to find com.apple.opengl bundle");
		}

		CFStringRef name = CFStringCreateWithCString(NULL, function_name.c_str(), CFStringGetSystemEncoding());
		auto ptr = (ProcAddress *)CFBundleGetFunctionPointerForName(bundle, name);
		CFRelease(name);
		return ptr;
	}

	Rect OpenGLWindowProvider::get_geometry() const
	{
		NSRect frame = impl->window.frame;
		NSRect screen_frame = [[NSScreen mainScreen] frame];
		
		// Make frame relative to mainScreen:
		frame.origin.x -= screen_frame.origin.x;
		frame.origin.y -= screen_frame.origin.y;
		
		return from_cocoa_rect(frame, screen_frame) * get_pixel_ratio();
	}

	Rect OpenGLWindowProvider::get_viewport() const
	{
		NSRect client = [impl->window.contentView frame];
		return Rectf::xywh(0.0f, 0.0f, client.size.width, client.size.height) * get_pixel_ratio();
	}
	
	bool OpenGLWindowProvider::is_fullscreen() const
	{
		return false;
	}

	bool OpenGLWindowProvider::has_focus() const
	{
		return impl->window.isKeyWindow == YES;
	}

	bool OpenGLWindowProvider::is_minimized() const
	{
		return impl->window.isMiniaturized == YES;
	}

	bool OpenGLWindowProvider::is_maximized() const
	{
		return impl->window.isZoomed == YES;
	}

	bool OpenGLWindowProvider::is_visible() const
	{
		return impl->window.isVisible == YES;
	}

	Size OpenGLWindowProvider::get_minimum_size(bool client_area) const
	{
		return Size();
	}

	Size OpenGLWindowProvider::get_maximum_size(bool client_area) const
	{
		return Size(32768, 32768);
	}

	GraphicContext& OpenGLWindowProvider::get_gc()
	{
		return impl->gc;
	}
	
	InputDevice &OpenGLWindowProvider::get_keyboard()
	{
		return keyboard;
	}
	
	InputDevice &OpenGLWindowProvider::get_mouse()
	{
		return mouse;
	}
	
	std::vector<InputDevice> &OpenGLWindowProvider::get_game_controllers()
	{
		static std::vector<InputDevice> empty;
		return empty;
	}

	std::string OpenGLWindowProvider::get_title() const
	{
		return [impl->window.title UTF8String];
	}

	bool OpenGLWindowProvider::is_clipboard_text_available() const
	{
		return false;
	}

	bool OpenGLWindowProvider::is_clipboard_image_available() const
	{
		return false;
	}

	void OpenGLWindowProvider::make_current() const
	{
		[impl->opengl_context makeCurrentContext];
	}

	Point OpenGLWindowProvider::client_to_screen(const Point &client_pos)
	{
		Rectf client_box = Rectf::xywh(client_pos.x / get_pixel_ratio(), client_pos.y / get_pixel_ratio(), 1.0f, 1.0f);
		
		NSRect client = to_cocoa_rect(client_box, [impl->window.contentView bounds]);
		NSRect window = [impl->window.contentView convertRect:client toView:nil];
		
		NSRect screen_frame = [[NSScreen mainScreen] frame];
		NSRect screen = [impl->window convertRectToScreen: window];
		screen.origin.x -= screen_frame.origin.x;
		screen.origin.y -= screen_frame.origin.y;
		
		return Rect(from_cocoa_rect(screen, screen_frame) * get_pixel_ratio()).get_top_left();
	}

	Point OpenGLWindowProvider::screen_to_client(const Point &screen_pos)
	{
		NSRect screen_frame = [[NSScreen mainScreen] frame];
		NSRect screen = to_cocoa_rect(Rectf::xywh(screen_pos.x / get_pixel_ratio(), screen_pos.y / get_pixel_ratio(), 1.0f, 1.0f), screen_frame);
		screen.origin.x += screen_frame.origin.x;
		screen.origin.y += screen_frame.origin.y;
		
		NSRect window = [impl->window convertRectFromScreen:screen];
		NSRect client = [impl->window.contentView convertRect:window fromView:nil];
		
		return Rect(from_cocoa_rect(client, [impl->window.contentView bounds]) * get_pixel_ratio()).get_top_left();
	}

	void OpenGLWindowProvider::create(DisplayWindowSite *new_site, const DisplayWindowDescription &desc)
	{
		impl->site = new_site;

		impl->window = [[CocoaWindow alloc] initWithDescription:desc provider:impl.get()];
		if (impl->window == nil)
			throw Exception("Could not create the window.");

		[impl->window setTitle:[NSString stringWithUTF8String:desc.get_title().c_str()]];

		std::vector<NSOpenGLPixelFormatAttribute> attributes;

		attributes.push_back(NSOpenGLPFAOpenGLProfile);
		attributes.push_back(NSOpenGLProfileVersion3_2Core);

		attributes.push_back(NSOpenGLPFADoubleBuffer);
		attributes.push_back(NSOpenGLPFAColorSize);
		attributes.push_back(24);
		attributes.push_back(NSOpenGLPFAAlphaSize);
		attributes.push_back(8);
		attributes.push_back(NSOpenGLPFADepthSize);
		attributes.push_back(desc.get_depth_size());
		attributes.push_back(NSOpenGLPFAStencilSize);
		attributes.push_back(desc.get_stencil_size());
		attributes.push_back(NSOpenGLPFAMinimumPolicy); // Color and depth must minimum match what we specified

		if (desc.get_multisampling() > 0)
		{
			attributes.push_back(NSOpenGLPFAMultisample);
			attributes.push_back(NSOpenGLPFASampleBuffers);
			attributes.push_back(1);
			attributes.push_back(NSOpenGLPFASamples);
			attributes.push_back(desc.get_multisampling());
		}
		attributes.push_back(0);

		NSOpenGLPixelFormat *pixel_format = [[NSOpenGLPixelFormat alloc] initWithAttributes:&attributes[0]];
		if (pixel_format == nil)
			throw Exception("Could not create the requested OpenGL pixel format");

		impl->opengl_context = [[NSOpenGLContext alloc] initWithFormat:pixel_format shareContext:impl->get_share_context()];
		if (impl->opengl_context == nil)
			throw Exception("Could not create OpenGL context");

		[impl->window.contentView setWantsBestResolutionOpenGLSurface:true];
		[impl->opengl_context setView:impl->window.contentView];
		
		if (desc.is_popup())
		{
			// Make window transparent:
			[impl->window setOpaque:FALSE];
			impl->window.backgroundColor = [NSColor clearColor];
			
			// The view transparent:
			[[impl->window.contentView layer] setOpaque: FALSE];
			
			// And the OpenGL context transparent:
			GLint opaque = 0;
			[impl->opengl_context setValues:&opaque forParameter:NSOpenGLCPSurfaceOpacity];
		}

		impl->gc = GraphicContext(new GL3GraphicContextProvider(this));

		[impl->window setDelegate:impl->window];
		
		[impl->window setOneShot:NO]; // Do not destroy the 'window device' when window is hidden
		
		if (desc.is_visible())
		{
			[impl->window makeMainWindow];
			[impl->window makeKeyAndOrderFront:impl->window];
		}
		/*else
		{
			[impl->window orderOut:impl->window];
		}*/
	}

	void OpenGLWindowProvider::show_system_cursor()
	{
	}

	CursorProvider *OpenGLWindowProvider::create_cursor(const CursorDescription &cursor_description)
	{
		throw Exception("Custom cursors not supported yet by OS X target");
	}

	void OpenGLWindowProvider::set_cursor(CursorProvider *cursor)
	{
	}

	void OpenGLWindowProvider::set_cursor(StandardCursor type)
	{
	}

	void OpenGLWindowProvider::hide_system_cursor()
	{
	}

	void OpenGLWindowProvider::set_title(const std::string &new_title)
	{
		[impl->window setTitle:[NSString stringWithUTF8String:new_title.c_str()]];
	}

	void OpenGLWindowProvider::set_position(const Rect &pos, bool client_area)
	{
		if (client_area)
		{
			NSRect frame = [impl->window frameRectForContentRect:to_cocoa_rect(Rectf(pos) * (1.0f / get_pixel_ratio()), impl->window.frame)];
			[impl->window setFrame:frame display:NO animate:NO];
		}
		else
		{
			NSRect screen_frame = [[NSScreen mainScreen] frame];
			
			NSRect frame = to_cocoa_rect(Rectf(pos) * (1.0f / get_pixel_ratio()), screen_frame);
			frame.origin.x += screen_frame.origin.x;
			frame.origin.y += screen_frame.origin.y;

			[impl->window setFrame:frame display:NO animate:NO];
		}
	}

	void OpenGLWindowProvider::set_size(int width, int height, bool client_area)
	{
		if (client_area)
		{
			auto box = get_geometry();
			box.right = box.left + width;
			box.bottom = box.top + height;
			set_position(box, false);
		}
		else
		{
			auto box = get_viewport();
			box.right = box.left + width;
			box.bottom = box.top + height;
			set_position(box, true);
		}
	}

	void OpenGLWindowProvider::set_minimum_size( int width, int height, bool client_area )
	{
		if (client_area)
		{
			[impl->window setContentMinSize:NSMakeSize(width/ get_pixel_ratio(), height/ get_pixel_ratio())];
		}
		else
		{
			[impl->window setMinSize:NSMakeSize(width/ get_pixel_ratio(), height/ get_pixel_ratio())];
		}
	}

	void OpenGLWindowProvider::set_maximum_size( int width, int height, bool client_area )
	{
		if (client_area)
		{
			[impl->window setContentMaxSize:NSMakeSize(width/ get_pixel_ratio(), height/ get_pixel_ratio())];
		}
		else
		{
			[impl->window setMaxSize:NSMakeSize(width/ get_pixel_ratio(), height/ get_pixel_ratio())];
		}
	}

	void OpenGLWindowProvider::set_enabled(bool enable)
	{
	}

	void OpenGLWindowProvider::minimize()
	{
		[impl->window miniaturize:impl->window];
	}

	void OpenGLWindowProvider::restore()
	{
		[impl->window deminiaturize:impl->window];
	}

	void OpenGLWindowProvider::maximize()
	{
		[impl->window performZoom:impl->window];
	}

	void OpenGLWindowProvider::toggle_fullscreen()
	{
		// FIXME: real implementation
	}

	void OpenGLWindowProvider::show(bool activate)
	{
		if (activate)
			[impl->window makeKeyAndOrderFront:impl->window];
		else
			[impl->window orderFront:impl->window];
	}

	void OpenGLWindowProvider::hide()
	{
		[impl->window orderOut:impl->window];
	}

	void OpenGLWindowProvider::bring_to_front()
	{
		[impl->window makeKeyAndOrderFront:impl->window];
	}

	void OpenGLWindowProvider::flip(int interval)
	{
		OpenGL::set_active(get_gc());
		OpenGL::check_error();
		[impl->opengl_context flushBuffer];
	}

	void OpenGLWindowProvider::capture_mouse(bool capture)
	{
	}

	void OpenGLWindowProvider::set_clipboard_text(const std::string &text)
	{
	}

	std::string OpenGLWindowProvider::get_clipboard_text() const
	{
		return std::string();
	}

	void OpenGLWindowProvider::request_repaint()
	{
		[impl->window.contentView setNeedsDisplay:TRUE];
	}

	void OpenGLWindowProvider::set_large_icon(const PixelBuffer &image)
	{
	}

	void OpenGLWindowProvider::set_small_icon(const PixelBuffer &image)
	{
	}

	void OpenGLWindowProvider::enable_alpha_channel(const Rect &blur_rect)
	{
	}

	void OpenGLWindowProvider::extend_frame_into_client_area(int left, int top, int right, int bottom)
	{
	}

	void OpenGLWindowProvider::set_clipboard_image(const PixelBuffer &buf)
	{
	}

	PixelBuffer OpenGLWindowProvider::get_clipboard_image() const
	{
		return PixelBuffer();
	}

	bool OpenGLWindowProvider::is_double_buffered() const
	{
		return true;
	}
		
	float OpenGLWindowProvider::get_pixel_ratio() const
	{
		return [impl->window backingScaleFactor];
	}
		
	void OpenGLWindowProvider::set_pixel_ratio(float ratio)
	{
	}

	InputDeviceProvider_OSXKeyboard *OpenGLWindowProvider::get_keyboard_provider()
	{
		return static_cast<InputDeviceProvider_OSXKeyboard*>(keyboard.get_provider());
	}

	InputDeviceProvider_OSXMouse *OpenGLWindowProvider::get_mouse_provider()
	{
		return static_cast<InputDeviceProvider_OSXMouse*>(mouse.get_provider());
	}

	/////////////////////////////////////////////////////////////////////

	OpenGLWindowProvider_Impl::OpenGLWindowProvider_Impl(OpenGLWindowProvider *self, OpenGLContextDescription &opengl_desc)
	: self(self), site(0), opengl_desc(opengl_desc)
	{
	}

	NSOpenGLContext *OpenGLWindowProvider_Impl::get_share_context()
	{
		NSOpenGLContext *share_context = nil;

		std::unique_ptr<std::unique_lock<std::recursive_mutex>> mutex_section;
		GraphicContextProvider* gc_providers = SharedGCData::get_provider(mutex_section);
		if (gc_providers)
		{
			GL3GraphicContextProvider *gl3_provider = dynamic_cast<GL3GraphicContextProvider*>(gc_providers);
			if (gl3_provider)
			{
				const OpenGLWindowProvider *render_window_wgl = dynamic_cast<const OpenGLWindowProvider*>(&gl3_provider->get_render_window());
				if (render_window_wgl)
					share_context = render_window_wgl->impl->opengl_context;
			}
		}

		return share_context;
	}

	void OpenGLWindowProvider_Impl::on_input_event(NSEvent *theEvent)
	{
		NSEventType type = [theEvent type];
		switch (type)
		{
			// Keyboard events:
			case NSKeyDown:
			case NSKeyUp:
			case NSFlagsChanged:
				on_keyboard_event(theEvent);
				break;

			// Mouse movement events:
			case NSMouseEntered: // see: NSTrackingArea
				// TODO:
				break;

			// Mouse events:
			case NSLeftMouseDown:
			case NSLeftMouseUp:
			case NSRightMouseDown:
			case NSRightMouseUp:
			case NSOtherMouseDown:
			case NSOtherMouseUp:
			case NSScrollWheel:
			case NSMouseMoved: // requires setAcceptsMouseMovedEvents: to be called first
				on_mouse_event(theEvent);
				break;

			default:
				break;
		}
	}

	void OpenGLWindowProvider_Impl::on_keyboard_event(NSEvent *theEvent)
	{
		NSEventType type = [theEvent type];

		// Is the message a down or up event?
		bool keydown = false;
		if (type == NSKeyDown)
		{
			keydown = true;
		}

		// Prepare event to be emitted:
		clan::InputEvent key;
		if (keydown)
		{
			key.type = clan::InputEvent::pressed;
		}
		else
		{
			key.type = clan::InputEvent::released;
		}

		// Handle modifier flags
		NSUInteger flags = [NSEvent modifierFlags];

		key.alt = flags & NSAlternateKeyMask;
		key.shift = flags &  NSShiftKeyMask;
		key.ctrl = flags & NSControlKeyMask;

		// TODO: Finish implementing.
		// Map the Cocoa key code to the appropriate ClanLib key code.
		switch ([theEvent keyCode])
		{
			// Miscellaneous keys.
			case 0x24: key.id = keycode_return; break;
			case 0x30: key.id = keycode_tab; break;
			case 0x31: key.id = keycode_space; break;
			case 0x33: key.id = keycode_backspace; break;
			case 0x35: key.id = keycode_escape; break;
			case 0x73: key.id = keycode_home; break;
			case 0x77: key.id = keycode_end; break;
			case 0x18: key.id = keycode_add; break;
			case 0x1B: key.id = keycode_subtract; break;
			case 0x2C: key.id = keycode_divide; break;
			case 0x2F: key.id = keycode_decimal; break;

			case 0x37: key.id = keycode_control; break;
			case 0x38: key.id = keycode_shift; break;
			case 0x3C: key.id = keycode_rshift; break;
			case 0x3E: key.id = keycode_rcontrol; break;

			// Arrow keys.
			case 0x7B: key.id = keycode_left; break;
			case 0x7C: key.id = keycode_right; break;
			case 0x7D: key.id = keycode_down; break;
			case 0x7E: key.id = keycode_up; break;

			// Number keys.
			case 0x1D: key.id = keycode_0; break;
			case 0x12: key.id = keycode_1; break;
			case 0x13: key.id = keycode_2; break;
			case 0x14: key.id = keycode_3; break;
			case 0x15: key.id = keycode_4; break;
			case 0x17: key.id = keycode_5; break;
			case 0x16: key.id = keycode_6; break;
			case 0x1A: key.id = keycode_7; break;
			case 0x1C: key.id = keycode_8; break;
			case 0x19: key.id = keycode_9; break;

			// Function keys.
			case 0x7A: key.id = keycode_f1; break;
			case 0x78: key.id = keycode_f2; break;
			case 0x63: key.id = keycode_f3; break;
			case 0x76: key.id = keycode_f4; break;
			case 0x60: key.id = keycode_f5; break;
			case 0x61: key.id = keycode_f6; break;
			case 0x62: key.id = keycode_f7; break;
			case 0x64: key.id = keycode_f8; break;
			case 0x65: key.id = keycode_f9; break;
			case 0x6D: key.id = keycode_f10; break;

			// Character keys.
			case 0x00: key.id = keycode_a; break;
			case 0x0B: key.id = keycode_b; break;
			case 0x08: key.id = keycode_c; break;
			case 0x02: key.id = keycode_d; break;
			case 0x0E: key.id = keycode_e; break;
			case 0x03: key.id = keycode_f; break;
			case 0x05: key.id = keycode_g; break;
			case 0x04: key.id = keycode_h; break;
			case 0x22: key.id = keycode_i; break;
			case 0x26: key.id = keycode_j; break;
			case 0x28: key.id = keycode_k; break;
			case 0x25: key.id = keycode_l; break;
			case 0x2E: key.id = keycode_m; break;
			case 0x2D: key.id = keycode_n; break;
			case 0x1F: key.id = keycode_o; break;
			case 0x23: key.id = keycode_p; break;
			case 0x0C: key.id = keycode_q; break;
			case 0x0F: key.id = keycode_r; break;
			case 0x01: key.id = keycode_s; break;
			case 0x11: key.id = keycode_t; break;
			case 0x20: key.id = keycode_u; break;
			case 0x09: key.id = keycode_v; break;
			case 0x0D: key.id = keycode_w; break;
			case 0x07: key.id = keycode_x; break;
			case 0x10: key.id = keycode_y; break;
			case 0x06: key.id = keycode_z; break;

			default: key.id = keycode_unknown; break;
		}

		static InputEvent prevInput;
		key.repeat_count = 0;  // TODO: Implement.
		if(type == NSKeyDown || type == NSKeyUp) // Can only call this on NSKeyUp or NSKeyDown type events
		{
			NSString* text = [theEvent charactersIgnoringModifiers];
			key.str = [text UTF8String];
			
			if (type == NSKeyDown)
				self->keyboard.sig_key_down()(key);
			else
				self->keyboard.sig_key_up()(key);
		}
		else if(type == NSFlagsChanged)
		{
			// Translate flag changes of shift and ctrl into keypresses / releases
			if(prevInput.shift != key.shift)
			{
				key.id = keycode_shift;
				key.type = key.shift ? clan::InputEvent::pressed : clan::InputEvent::released;
				if (key.shift)
					self->keyboard.sig_key_down()(key);
				else
					self->keyboard.sig_key_up()(key);
			}
			if(prevInput.ctrl != key.ctrl)
			{
				key.id = keycode_control;
				key.type = key.ctrl ? clan::InputEvent::pressed : clan::InputEvent::released;
				if (key.ctrl)
					self->keyboard.sig_key_down()(key);
				else
					self->keyboard.sig_key_up()(key);
			}
		}

		// Update our internal keyboard state
		self->get_keyboard_provider()->on_key_event(key.id, key.type);

		prevInput = key;
	}

	void OpenGLWindowProvider_Impl::on_mouse_event(NSEvent *theEvent)
	{
		NSEventType type = [theEvent type];

		InputCode id;
		bool up = false;
		bool down = false;
		bool dblclk = false;

		switch (type)
		{
			case NSLeftMouseDown: id = mouse_left; down = true; dblclk = theEvent.clickCount == 2; break;
			case NSLeftMouseUp: id = mouse_left; up = true; break;
			case NSRightMouseDown: id = mouse_right; down = true; dblclk = theEvent.clickCount == 2; break;
			case NSRightMouseUp: id = mouse_right; up = true; break;
			case NSOtherMouseDown: id = mouse_middle; down = true; dblclk = theEvent.clickCount == 2; break;
			case NSOtherMouseUp: id = mouse_middle; up = true; break;
			case NSScrollWheel: id = ([theEvent deltaY] > 0) ? mouse_wheel_up : mouse_wheel_down; up = true; down = true; break;
			case NSMouseMoved: break;
			default:
				return;
		}

		NSPoint content_mouse_pos = [window.contentView convertPoint:[theEvent locationInWindow] fromView:nil];
		
		// Prepare event to be emitted:
		InputEvent key;
		key.mouse_pos = clan::Pointf(content_mouse_pos.x, [window.contentView bounds].size.height - content_mouse_pos.y);
		key.id = id;

		if (dblclk)
		{
			key.type = InputEvent::doubleclick;

			// Update our internal mouse state
			self->get_mouse_provider()->on_mouse_event(key.id, key.type, key.mouse_pos);

			self->mouse.sig_key_dblclk()(key);
		}
		else if (down)
		{
			key.type = InputEvent::pressed;

			// Update our internal mouse state
			self->get_mouse_provider()->on_mouse_event(key.id, key.type, key.mouse_pos);

			self->mouse.sig_key_down()(key);
		}

		// It is possible for 2 events to be called when the wheelmouse is used.
		if (up)
		{
			key.type = InputEvent::released;

			// Update our internal mouse state
			self->get_mouse_provider()->on_mouse_event(key.id, key.type, key.mouse_pos);

			self->mouse.sig_key_up()(key);
		}
		
		if (type == NSMouseMoved)
		{
			key.type = InputEvent::pointer_moved;
			
			// Update our internal mouse state
			self->get_mouse_provider()->on_mouse_event(key.id, key.type, key.mouse_pos);
			
			self->mouse.sig_pointer_move()(key);
		}
	}
}
