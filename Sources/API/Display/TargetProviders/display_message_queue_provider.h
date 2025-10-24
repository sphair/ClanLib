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

#ifndef display_message_queue_provider
#define display_message_queue_provider

class CL_Event;
class CL_DisplayWindowMessage;

/// \brief Display target provider message queue interface.
///
/// \xmlonly !group=Display/Display Target Interface! !header=display.h! \endxmlonly
class CL_DisplayMessageQueue_Provider
{
/// \name Construction
/// \{
public:
	virtual ~CL_DisplayMessageQueue_Provider() {};

/// \}
/// \name Operations
/// \{

public:
	/// \brief Make thread sleep until a message arrives on the queue or one of the events is triggered.
	/// \return The index of the event that got flagged, -1 if the wait timed out and -2 if a message is available.
	virtual int wait(int count, CL_Event const * const * events, int timeout, bool wait_all) = 0;

	/// \brief Get the first message from the queue.
	virtual CL_DisplayWindowMessage get_message() = 0;

	/// \brief Peek at the first message from the queue.
	virtual CL_DisplayWindowMessage peek_message(bool block) = 0;

	/// \brief Process a message.
	virtual void dispatch_message(const CL_DisplayWindowMessage &message) = 0;

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

#endif
