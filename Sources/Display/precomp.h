/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#pragma once


#ifdef WIN32
#ifdef _MSC_VER
# pragma warning (disable:4786)
#endif
#include <windows.h>
#endif

#include "API/Core/Text/string_types.h"
#include "API/Core/IOData/datatypes.h"
#include "API/Core/System/exception.h"

#if defined(_DEBUG) && !defined(DEBUG)
#define DEBUG
#endif

#ifdef WIN32
#define BREAKPOINT
#else
#define BREAKPOINT asm("int $03");
#endif

#ifdef __APPLE__
#if defined __OBJC__
# import <mach/mach.h>
# import <mach/mach_error.h>
# import <AppKit/AppKit.h>
# import <ApplicationServices/ApplicationServices.h>
# import <Cocoa/Cocoa.h>
# import <CoreAudio/CoreAudio.h>
# import <AudioUnit/AudioUnit.h>
# import <AudioToolbox/AudioToolbox.h>
# import <QuickTime/QuickTime.h>
# import <IOKit/IOKitLib.h>
# import <IOKit/IOCFPlugIn.h>
# import <IOKit/hid/IOHIDLib.h>
# import <IOKit/hid/IOHIDKeys.h>
# import <Kernel/IOKit/hidsystem/IOHIDUsageTables.h>
#endif
#endif


