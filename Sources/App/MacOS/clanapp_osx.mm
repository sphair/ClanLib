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

#include "clanapp_osx.h"

#include "API/Display/System/run_loop.h"
#include "API/Core/System/exception.h"
#include "API/App/clanapp.h"

namespace clan
{
	static ApplicationInstancePrivate *app_instance = 0;
	static bool enable_catch_exceptions = false;
	static int timing_timeout = 0;
	
	static std::vector<std::string> command_line_args;
	
	static std::unique_ptr<Application> app_obj;
	static NSTimer *app_update_timer = nil;

	ApplicationInstancePrivate::ApplicationInstancePrivate(bool catch_exceptions)
	{
		app_instance = this;
		enable_catch_exceptions = catch_exceptions;
	}
	
	const std::vector<std::string> &Application::main_args()
	{
		return command_line_args;
	}

	void Application::use_timeout_timing(int timeout)
	{
		timing_timeout = timeout;
	}
}

int main(int argc, char **argv)
{
	using namespace clan;
	
	if (app_instance == 0)
	{
		NSLog(@"ClanLib: No global Application instance!");
		return 255;
	}
	
	for (int i = 0; i < argc; i++)
		command_line_args.push_back(argv[i]);
	
	AppDelegate *appDelegate = [[AppDelegate alloc] init];
	
	NSApplication *app = [NSApplication sharedApplication];
	[app setDelegate: appDelegate];
	[app run];
	
	app_obj.reset();

	return 0;
}

@implementation AppDelegate

- (void) applicationDidFinishLaunching:(NSNotification *)aNotification
{
	using namespace clan;
	
	NSTimeInterval interval = timing_timeout/1000.0;
	if (interval == 0.0)
		interval = 0.01;
	
	app_obj = app_instance->create();
	app_update_timer = [NSTimer scheduledTimerWithTimeInterval:interval target:self selector:@selector(updateTimeout:) userInfo:nil repeats:YES];
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
	return NO;
}

- (void) updateTimeout:(NSTimer *)timer
{
	using namespace clan;
	
	if (!app_obj->update())
	{
		[[NSApplication sharedApplication] terminate: self];
	}
}

@end
