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
**    James Lammlein
**    Magnus Norddahl
*/

#include "clanapp_osx.h"

#include "API/Core/System/setup_core.h"
#include "API/Core/System/keep_alive.h"
#include "API/App/clanapp.h"
#include "API/Core/System/exception.h"
#include "API/Core/System/console_window.h"
#include "API/Core/Text/console.h"

#include <assert.h>
#include <cstdlib>
#include <iostream>
#include <pthread.h>

#import <CoreFoundation/CoreFoundation.h>
#import <Cocoa/Cocoa.h>

@implementation AppDelegate

- (void) applicationDidFinishLaunching:(NSNotification *)aNotification
{
    NSLog(@"-applicationDidFinishLaunching:");

	using namespace clan;
	
	if (Application::enable_catch_exceptions)
	{
		try
		{
            create_main_thread();
		}
		catch(Exception &exception)
		{
			// Create a console window for text-output if not available
			std::string console_name("Console");
			if (!main_args.empty())
				console_name = main_args[0];
			
			ConsoleWindow console(console_name, 80, 160);
			Console::write_line("Exception caught: " + exception.get_message_and_stack_trace());
			console.display_close_message();
		}
	}
	else
	{
        create_main_thread();
	}
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
    return YES;
}

@end

namespace clan
{

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

// TODO: Remove these once the official main function
//       OSX update is rolled out.
void* main_thread_wrapper(void*)
{
    try
    {
        clan::Application::main(clan::main_args);
    }
    catch(...) {}
    
    return nullptr;
}

// TODO: Remove these once the official main function
//       OSX update is rolled out.
void create_main_thread()
{
    pthread_attr_t  attr;
    pthread_t       posixThreadID;
    int             returnVal;
        
    returnVal = pthread_attr_init(&attr);
    assert(!returnVal);
    returnVal = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    assert(!returnVal);
        
    int threadError = pthread_create(&posixThreadID, &attr, &main_thread_wrapper, nullptr);
    assert(!threadError);
        
    returnVal = pthread_attr_destroy(&attr);
    assert(!returnVal);
}
    
Application::MainFunction *Application::main = 0;
bool Application::enable_catch_exceptions = true;

}

int main(int argc, char **argv)
{
	using namespace clan;
	
	if (Application::main == 0)
	{
		std::cout << "ClanLib: No global Application instance!" << std::endl;
		return 255;
	}
    
	for (int i = 0; i < argc; i++)
		main_args.push_back(argv[i]);
    
    KeepAlive::func_thread_id().set(&cl_app_on_thread_id);
    KeepAlive::func_awake_thread().set(&cl_app_on_awake_thread);
    
	AppDelegate *appDelegate = [[AppDelegate alloc] init];
	[NSApplication sharedApplication];
	[NSApp setDelegate: appDelegate];
	[NSApp run];
	return 0;
}
