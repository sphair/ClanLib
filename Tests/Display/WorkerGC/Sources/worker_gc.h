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
**    Mark Page
*/

#pragma once

// This is the Application class (That is instantiated by the Program Class)
class WorkerGC
{
public:
	WorkerGC();
	~WorkerGC();
	void Start(const CL_StringRef &name);
	bool IsReady();
	CL_Sprite Get();
private:
	void worker_thread_loop();

	CL_Thread worker_thread;
	bool worker_thread_started;
	bool worker_thread_processing;
	CL_Event event_stop;
	CL_Event event_start;
	CL_Event event_completed;

	CL_Sprite retrieved_sprite;

	CL_DisplayWindow window;
	CL_String sprite_name;

	bool exception_set;
	CL_Exception exception;
};
