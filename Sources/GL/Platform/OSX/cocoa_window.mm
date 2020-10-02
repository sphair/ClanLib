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
#include "API/GL/opengl_context_description.h"
#include "opengl_window_provider_osx_impl.h"

#import <CoreFoundation/CoreFoundation.h>
#import <AppKit/AppKit.h>

#import "cocoa_window.h"
#import "cocoa_view.h"
#import "opengl_window_provider_osx.h"

@implementation CocoaWindow
{
	clan::OpenGLWindowProvider_Impl *window_provider;
}

- (id) initWithDescription:(const clan::DisplayWindowDescription &)desc provider:(clan::OpenGLWindowProvider_Impl*)provider_impl
{
	NSRect frame = NSMakeRect(desc.get_position().left, desc.get_position().top, desc.get_size().width, desc.get_size().height);

	NSScreen *screen = [NSScreen mainScreen];
	
	if (desc.get_position().left == -1 && desc.get_position().top == -1)
	{
		NSRect workarea = [screen visibleFrame];
		frame.origin.x = workarea.origin.x + (workarea.size.width - frame.size.width) * 0.5f;
		frame.origin.y = workarea.origin.y + (workarea.size.height - frame.size.height) * 0.5f;
	}
	else
	{
		NSRect screen_frame = [screen frame];
		frame.origin.x = screen_frame.origin.x + frame.origin.x;
		frame.origin.y = screen_frame.origin.y + screen_frame.size.height - frame.origin.y - frame.size.height;
	}
	
	NSUInteger styles = 0;
	
	if (desc.has_caption())
		styles |= NSTitledWindowMask;
	if (desc.has_minimize_button())
		styles |= NSMiniaturizableWindowMask;
	if (desc.has_sysmenu())
		styles |= NSClosableWindowMask;
	if (desc.get_allow_resize())
		styles |= NSResizableWindowMask;
	if (desc.is_fullscreen())
		styles |= NSFullScreenWindowMask;
	
	if (!desc.get_position_client_area())
	{
		frame = [NSWindow contentRectForFrameRect:frame styleMask:styles];
	}

	self = [self initWithContentRect:frame styleMask:styles backing:NSBackingStoreBuffered defer:NO];
	if (self)
	{
		window_provider = provider_impl;
		[self setAcceptsMouseMovedEvents:YES];
		self.contentView = [[CocoaView alloc] initWithProvider:provider_impl];
		
		if (!desc.get_owner().is_null())
		{
			// To do: Cocoa doesn't have owner window ordering (only z-order levels are supported).
			//        We need to set the level back to NSNormalWindowLevel whenever the parent window is not key.
			
			//auto parent_provider = static_cast<clan::OpenGLWindowProvider*>(desc.get_owner().get_provider());
			
			self.level = NSFloatingWindowLevel;
		}
	}
	return self;
}

- (void) sendEvent:(NSEvent *)theEvent
{
	NSEventType type = [theEvent type];
	switch (type)
	{
		// Keyboard events:
		case NSKeyDown:
		case NSKeyUp:
		case NSFlagsChanged:
            window_provider->on_input_event(theEvent);
            break;

		// Mouse events:
		case NSMouseEntered: // see: NSTrackingArea
		case NSLeftMouseDown:
		case NSLeftMouseUp:
		case NSRightMouseDown:
		case NSRightMouseUp:
		case NSOtherMouseDown:
		case NSOtherMouseUp:
		case NSMouseMoved: // requires setAcceptsMouseMovedEvents: to be called first
		case NSScrollWheel:
			window_provider->on_input_event(theEvent);
			break;

		default:
            [super sendEvent:theEvent];
            break;
	}
}

- (BOOL) acceptsFirstResponder
{
	return YES;
}

- (BOOL)canBecomeMainWindow
{
	return YES;
}

- (BOOL)canBecomeKeyWindow
{
	return YES;
}

- (void) windowDidMiniaturize:(NSNotification *)notification
{
	window_provider->site->sig_window_minimized();
}

- (void) windowDidDeminiaturize:(NSNotification *)notification
{
	window_provider->site->sig_window_restored();
}

// - (NSSize)windowWillResize:(NSWindow *)sender toSize:(NSSize)frameSize

- (void)windowDidResize:(NSNotification *)notification
{
	float width = ((NSView*)self.contentView).frame.size.width;
	float height = ((NSView*)self.contentView).frame.size.height;
	[window_provider->opengl_context update];
	window_provider->site->sig_resize(width, height);
	
	clan::GL3GraphicContextProvider *gl_provider = dynamic_cast<clan::GL3GraphicContextProvider*>(window_provider->gc.get_provider());
	if (gl_provider)
		gl_provider->on_window_resized();
}

//- (void)windowWillMove:(NSNotification *)notification

-(void)windowDidMove:(NSNotification *)notification
{
	window_provider->site->sig_window_moved();
}

- (void)windowDidBecomeKey:(NSNotification *)notification
{
	window_provider->site->sig_got_focus();
}

-(void)windowDidResignKey:(NSNotification *)notification
{
	window_provider->site->sig_lost_focus();
}

- (BOOL)windowShouldClose:(id)sender
{
	window_provider->site->sig_window_close();
	return NO;
}

//- (void)windowDidBecomeMain:(NSNotification *)notification
//- (void)windowDidResignMain:(NSNotification *)notification
//- (void) windowWillClose:(NSNotification *)notification

@end
