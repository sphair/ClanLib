/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
**    Harry Storbacka
*/

#pragma once


#include "API/Display/TargetProviders/display_target_provider.h"

#ifdef WIN32
 #include "Display/Win32/display_message_queue_win32.h"
#else
 #include "Display/X11/display_message_queue_x11.h"
#endif

class CL_GL1TargetProvider : public CL_DisplayTargetProvider
{
/// \name Construction
/// \{

public:
	CL_GL1TargetProvider();

	~CL_GL1TargetProvider();


/// \}
/// \name Attributes
/// \{

public:
	CL_DisplayMessageQueue_Provider *get_message_queue() const;


/// \}
/// \name Operations
/// \{

public:
	void destroy();

	CL_DisplayWindowProvider *alloc_display_window();


/// \}
/// \name Implementation
/// \{

private:
#ifdef WIN32
	static CL_DisplayMessageQueue_Win32 message_queue;
	friend class CL_GL1WindowProvider_WGL;
#else
	static CL_DisplayMessageQueue_X11 message_queue;
	friend class CL_GL1WindowProvider_GLX;
#endif

/// \}
};


