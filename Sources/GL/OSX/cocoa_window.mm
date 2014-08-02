/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "API/GL/opengl_window_description.h"
#include "opengl_window_provider_osx_impl.h"

#import <CoreFoundation/CoreFoundation.h>
#import <AppKit/AppKit.h>

#import "cocoa_window.h"
#import "opengl_window_provider_osx.h"

@implementation CocoaWindow
{
	clan::OpenGLWindowProvider_Impl *window_provider;
}

- (BOOL) acceptsFirstResponder
{
    NSLog(@"-acceptsFirstResponder:");
    return YES;
}

- (id) initWithDescription:(const clan::DisplayWindowDescription &)desc provider:(clan::OpenGLWindowProvider_Impl*)provider_impl
{
	NSRect frame = NSMakeRect(0, 0, desc.get_size().width, desc.get_size().height);
	NSUInteger styles = NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask;

	self = [self initWithContentRect:frame styleMask:styles backing:NSBackingStoreBuffered defer:NO];
	if (self)
	{
		window_provider = provider_impl;
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
            [super sendEvent:theEvent];
			break;

		default:
            [super sendEvent:theEvent];
            break;
	}
}

- (void) windowDidMiniaturize:(NSNotification *)notification
{
    NSLog(@"-windowDidMiniaturize:");
    // TODO:
}

- (void) windowDidDeminiaturize:(NSNotification *)notification
{
    NSLog(@"-windowDidDeminiaturize:");
    // TODO:
}

- (void) windowDidResize:(NSNotification *)notification
{
    NSLog(@"-windowDidResize:");

    NSRect rect = [window_provider->window.contentView bounds];

    // TODO: Can't actually call this because of the threading issue.  However, this seems pretty close
    //       to where you would actually call something like this.  I am trying to resize the openGL rendering
    //       area when the user manually resizes the window.
    // glViewport(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
}

- (void) windowWillClose:(NSNotification *)notification
{
    NSLog(@"-windowWillClose:");
    // TOOD:
}

@end
