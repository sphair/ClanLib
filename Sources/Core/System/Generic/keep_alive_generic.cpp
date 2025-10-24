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
#include <list>
#include <map>

#include "API/Core/System/keep_alive.h"
#include "API/Core/System/system.h"
#include "API/Core/System/event_listener.h"
#include "API/Core/System/thread.h"
#include "API/Core/System/mutex.h"

unsigned int CL_System::susp_start;
unsigned int CL_System::susp_cnt = 0;
unsigned int CL_System::susp_accum = 0;

void CL_System::suspend_time()
{
	++susp_cnt;
	if (susp_cnt == 1)
		susp_start = sys_time();
}

void CL_System::resume_time()
{
	if (susp_cnt != 0) {
		--susp_cnt;
		if (susp_cnt == 0) {
			susp_accum += sys_time() - susp_start;
		}
	}
}

unsigned int CL_System::get_time()
{
	if (susp_cnt == 0)
		return sys_time() - susp_accum;
	else
		return susp_start - susp_accum;
}

CL_Mutex mutex_keep_alive;
std::map<CL_ThreadId, std::list<CL_KeepAlive*>* > keep_alives;

CL_KeepAlive::CL_KeepAlive()
{
	create_thread_id = CL_Thread::get_current_id();
	CL_MutexSection mutex_lock(&mutex_keep_alive);
	std::map<CL_ThreadId, std::list<CL_KeepAlive*>* >::iterator it;
	it = keep_alives.find(create_thread_id);
	if (it == keep_alives.end())
	{
		keep_alives[create_thread_id] = new std::list<CL_KeepAlive*>;
	}
	
	keep_alives[create_thread_id]->push_back(this);
}

CL_KeepAlive::~CL_KeepAlive()
{
	CL_MutexSection mutex_lock(&mutex_keep_alive);
	std::list<CL_KeepAlive*> *list = keep_alives[create_thread_id];
	list->remove(this);
	if (list->empty())
	{
		delete list;
		keep_alives.erase(keep_alives.find(create_thread_id));
	}
}

void CL_System::keep_alive()
{
	CL_ThreadId cur_thread_id = CL_Thread::get_current_id();
	CL_MutexSection mutex_lock(&mutex_keep_alive);
	std::map<CL_ThreadId, std::list<CL_KeepAlive*>* >::iterator it_map;
	it_map = keep_alives.find(cur_thread_id);
	if (it_map == keep_alives.end()) return;

	std::list<CL_KeepAlive*> *list = it_map->second;

	for (
		std::list<CL_KeepAlive*>::iterator it = list->begin();
		it != list->end();
		it++)
	{
		(*it)->keep_alive();
	}
}

void CL_System::keep_alive(int millis)
{
	CL_EventListener events;
	keep_alive(events, millis);
}

bool CL_System::keep_alive(CL_EventListener &events, int timeout)
{
	// TODO: Add triggers for keep alive objects.

	if (timeout == -1)
	{
		do CL_System::keep_alive(); while (!events.wait(10));

		return true;
	}
	else
	{
		while (timeout > 0)
		{
			CL_System::keep_alive();
			if (events.wait(timeout > 10 ? 10 : timeout)) return true;
			timeout -= 10;
		}

		return false;
	}
}


