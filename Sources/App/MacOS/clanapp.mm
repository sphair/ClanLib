/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
#include "API/Core/System/setup_core.h"
#include "API/Core/System/keep_alive.h"
#include "API/App/clanapp.h"
#import <UIKit/UIKit.h>

void cl_runloop_callout();
void *cl_app_on_thread_id();
void cl_app_on_awake_thread(void *thread_id);

@interface CL_ClanAppDelegate : NSObject <UIApplicationDelegate> 
{
}
@end

@implementation CL_ClanAppDelegate

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

static std::vector<CL_String> cl_main_args;

int main(int argc, char **argv)
{
	if (CL_ClanApplication::main == 0)
	{
		std::cout << "ClanLib: No global CL_ClanApplication instance!" << std::endl;
		return 255;
	}

	for (int i = 0; i < argc; i++)
		cl_main_args.push_back(argv[i]);
    
    CL_KeepAlive::func_thread_id().set(&cl_app_on_thread_id);
    CL_KeepAlive::func_awake_thread().set(&cl_app_on_awake_thread);
    
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    int result = UIApplicationMain(argc, argv, nil, @"CL_ClanAppDelegate");
    [pool release];
    return result;
}

void cl_runloop_callout()
{
	CL_ClanApplication::main(cl_main_args);
}

void *cl_app_on_thread_id()
{
    return CFRunLoopGetCurrent();
}

void cl_app_on_awake_thread(void *thread_id)
{
    CFRunLoopRef runloop = (CFRunLoopRef)thread_id;
    CFRunLoopPerformBlock(runloop, kCFRunLoopCommonModes, ^(){ CL_KeepAlive::process(); });
    CFRunLoopWakeUp(runloop);
}

/////////////////////////////////////////////////////////////////////////////
// CL_ClanApplication:

CL_ClanApplication::MainFunction *CL_ClanApplication::main = 0;
