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
**    (if your name is missing here, please add it)
*/

#include "Core/precomp.h"
#include "event_trigger_unix.h"

// note: this cannot be replaced by <ctime>! (timeval needs to be defined)
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger_Unix construction:

CL_EventTrigger_Unix::CL_EventTrigger_Unix()
{
	pipe(wait_pipe);
}

CL_EventTrigger_Unix::~CL_EventTrigger_Unix()
{
	close(wait_pipe[0]);
	close(wait_pipe[1]);
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger_Unix attributes:

bool CL_EventTrigger_Unix::get_flag() const
{
	return wait(0);
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger_Unix operations:

void CL_EventTrigger_Unix::reset() const
{
	char v = 0;
	while (wait(0)) read(wait_pipe[0], &v, 1);
}

void CL_EventTrigger_Unix::set_flag()
{
	char v = 1;
	if (!wait(0)) write(wait_pipe[1], &v, 1);
}

bool CL_EventTrigger_Unix::wait(int timeout) const
{
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(wait_pipe[0], &rfds);

	timeval tv;
	tv.tv_sec = timeout/1000;
	tv.tv_usec = (timeout%1000)*1000;

	int result = select(wait_pipe[0]+1, &rfds, NULL, NULL, (timeout == -1) ? NULL : &tv);
	if (result <= 0) return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger_Unix implementation:
