
#pragma once

#include "API/Core/Math/rect.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/display.h"
#include "API/Display/display_target.h"
#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Render/shared_gc_data.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Window/input_context.h"
#include "API/GL/opengl.h"
#include "API/GL/opengl_window_description.h"
#include "API/Core/Text/logger.h"
#include "../opengl_window_description_impl.h"
#include "../GL3/gl3_graphic_context_provider.h"

#import <CoreFoundation/CoreFoundation.h>
#import <AppKit/AppKit.h>

@class CocoaWindow;

namespace clan
{

class OpenGLWindowProvider_Impl
{
public:
	OpenGLWindowProvider_Impl(OpenGLWindowProvider *self, OpenGLWindowDescription &opengl_desc);
	
	NSOpenGLContext *get_share_context();
	
	void on_input_event(NSEvent *theEvent);
	
	OpenGLWindowProvider *self;
	GraphicContext gc;
	InputContext ic;
	DisplayWindowSite *site;
	OpenGLWindowDescription opengl_desc;
	
	CocoaWindow *window;
	NSOpenGLContext *opengl_context;
};

}
