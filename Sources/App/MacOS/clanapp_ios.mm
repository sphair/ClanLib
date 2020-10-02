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

#include <cstdlib>
#include <iostream>
#include "API/Display/System/run_loop.h"
#include "API/App/clanapp.h"
#include "API/Core/System/exception.h"
#include "API/Core/System/console_window.h"
#include "API/Core/Text/console.h"

#if defined(USE_IOS)

#import <UIKit/UIKit.h>

void cl_runloop_callout();
void *cl_app_on_thread_id();
void cl_app_on_awake_thread(void *thread_id);

@interface ClanAppDelegate : NSObject <UIApplicationDelegate> 
{
}
@end

@implementation ClanAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    CFRunLoopRef runloop = CFRunLoopGetCurrent();
    CFRunLoopPerformBlock(runloop, kCFRunLoopCommonModes, ^(){ cl_runloop_callout(); });
    CFRunLoopWakeUp(runloop);

    return YES;
}

@end

/////////////////////////////////////////////////////////////////////////////
// main:

static std::vector<std::string> cl_main_args;

int main(int argc, char **argv)
{
	if (Application::main == 0)
	{
		std::cout << "ClanLib: No global Application instance!" << std::endl;
		return 255;
	}

	for (int i = 0; i < argc; i++)
		cl_main_args.push_back(argv[i]);
    
    KeepAlive::func_thread_id().set(&cl_app_on_thread_id);
    KeepAlive::func_awake_thread().set(&cl_app_on_awake_thread);
    
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    int result = UIApplicationMain(argc, argv, nil, @"CL_ClanAppDelegate");
    [pool release];
    return result;
}

void cl_runloop_callout()
{
	// Call clanapp main:
	if (clan::Application::enable_catch_exceptions)
	{
		try
		{
			clan::Application::main(cl_main_args);
		}
		catch(clan::Exception &exception)
		{
			// Create a console window for text-output if not available
			std::string console_name("Console");
			if (!cl_main_args.empty())
				console_name = cl_main_args[0];

			clan::ConsoleWindow console(console_name, 80, 160);
			clan::Console::write_line("Exception caught: " + exception.get_message_and_stack_trace());
			console.display_close_message();
		}

	}
	else
	{
		clan::Application::main(cl_main_args);
	}
}

void *cl_app_on_thread_id()
{
    return CFRunLoopGetCurrent();
}

void cl_app_on_awake_thread(void *thread_id)
{
    CFRunLoopRef runloop = (CFRunLoopRef)thread_id;
    CFRunLoopPerformBlock(runloop, kCFRunLoopCommonModes, ^(){ KeepAlive::process(); });
    CFRunLoopWakeUp(runloop);
}

/////////////////////////////////////////////////////////////////////////////
// Application:

Application::MainFunction *Application::main = 0;
bool clan::Application::enable_catch_exceptions = true;

#endif
