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
**    Mark Page
*/

/// \addtogroup clanDisplay_Window clanDisplay Window
/// \{


#pragma once


#include "../api_display.h"
#include "../../Core/System/sharedptr.h"

class CL_DisplayWindowMessage_Impl;

#ifndef WIN32
#include <X11/Xlib.h>
typedef struct _CL_XEvent
{
	union _XEvent xevent;	// Contains XEvent structure.
} CL_XEvent;
#endif

/// \brief Windowing-system message.
///
/// \xmlonly !group=Display/Window! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_DisplayWindowMessage
{
/// \name Construction
/// \{

public:
	CL_DisplayWindowMessage();

	~CL_DisplayWindowMessage();

	static CL_DisplayWindowMessage null();

/// \}
/// \name Attributes
/// \{

public:
#ifdef WIN32
	MSG get_msg() const;
#else
	CL_XEvent get_msg() const;
#endif

	bool is_null() const;

/// \}
/// \name Operations
/// \{

public:
	void set_handled_message(bool handled);

#ifdef WIN32
	void set_win32_params(const MSG &msg);
#else
	void set_x11_params(const CL_XEvent &src_event);
#endif


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_DisplayWindowMessage_Impl> impl;
/// \}
};



/// \}
