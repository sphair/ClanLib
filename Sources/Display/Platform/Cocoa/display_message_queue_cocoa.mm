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

#include "Display/precomp.h"
#include "display_message_queue_cocoa.h"
#include <CoreFoundation/CoreFoundation.h>
#import <Cocoa/Cocoa.h>

namespace clan
{
	void DisplayMessageQueue_Cocoa::run()
	{
		runloop_nesting_level++;
		try
		{
			CFRunLoopRun();
			runloop_nesting_level--;
		}
		catch (...)
		{
			runloop_nesting_level--;
			throw;
		}
	}

	void DisplayMessageQueue_Cocoa::exit()
	{
		if (runloop_nesting_level > 0)
		{
			CFRunLoopStop(CFRunLoopGetMain());
		}
		else
		{
			[[NSApplication sharedApplication] terminate:nil];
		}
	}

	bool DisplayMessageQueue_Cocoa::process(int timeout_ms)
	{
		SInt32 result = CFRunLoopRunInMode(kCFRunLoopDefaultMode, timeout_ms / 1000.0, false);
		return result == kCFRunLoopRunTimedOut || result == kCFRunLoopRunHandledSource;
	}

	void DisplayMessageQueue_Cocoa::post_async_work_needed()
	{
		CFRunLoopPerformBlock(CFRunLoopGetMain(), kCFRunLoopDefaultMode, ^{
			process_async_work();
		});
		
		CFRunLoopWakeUp(CFRunLoopGetMain());
	}
}
