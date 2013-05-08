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
**    Magnus Norddahl
*/

#include "Core/precomp.h"
#include "thread_unix.h"
#include "API/Core/System/runnable.h"
#include "API/Core/System/thread_local_storage.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Thread_Unix Construction:

Thread_Unix::Thread_Unix()
: handle(0), handle_valid(false)
{
}

Thread_Unix::~Thread_Unix()
{
	if (handle_valid)
		pthread_detach(handle);
}

/////////////////////////////////////////////////////////////////////////////
// Thread_Unix Attributes:

/////////////////////////////////////////////////////////////////////////////
// Thread_Unix Operations:

void Thread_Unix::start(Runnable *runnable)
{
	if (handle_valid)
	{
		pthread_detach(handle);
		handle = 0;
		handle_valid = false;
	}

	int result = pthread_create(&handle, NULL, &Thread_Unix::thread_main, runnable);
	if (result != 0)
		throw Exception("Unable to create new thread");
	handle_valid = true;
}

void Thread_Unix::join()
{
	if (handle_valid)
	{
		pthread_join(handle, NULL);
		handle = 0;
		handle_valid = false;
	}
}

void Thread_Unix::kill()
{
	if (handle_valid)
	{
		pthread_cancel(handle);
		handle = 0;
		handle_valid = false;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Thread_Unix Implementation:

void *Thread_Unix::thread_main(void *data)
{
	// kill thread immediately - no cancellation point
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	Runnable *runnable = (Runnable *)data;
	ThreadLocalStorage tls;
	runnable->run();
	return NULL;
}

}
