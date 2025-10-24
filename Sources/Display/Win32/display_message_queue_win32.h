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
**    Harry Storbacka
*/

#pragma once


#include "API/Display/TargetProviders/display_message_queue_provider.h"
#include "API/Display/api_display.h"
#include "API/Display/Window/timer.h"
#include <vector>

class CL_Win32Window;

class CL_API_DISPLAY CL_DisplayMessageQueue_Win32 : public CL_DisplayMessageQueue_Provider
{
/// \name Construction
/// \{
public:
	CL_DisplayMessageQueue_Win32();
	~CL_DisplayMessageQueue_Win32();

/// \}
/// \name Operations
/// \{
public:
	int wait(int count, CL_Event const * const * events, int timeout, bool wait_all);
	CL_DisplayWindowMessage get_message();
	CL_DisplayWindowMessage peek_message(bool block);
	void dispatch_message(const CL_DisplayWindowMessage &message);

	void add_client(CL_Win32Window *window);
	void remove_client(CL_Win32Window *window);

 /// \}
 /// \name Implementation
 /// \{

 private:
	struct ThreadData
	{
		ThreadData() : already_flagged(false) { }

		std::vector<CL_Win32Window *> windows;
		bool already_flagged;
	};

	CL_SharedPtr<ThreadData> get_thread_data();
 /// \}
};


