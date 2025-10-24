/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "API/Core/Text/logger.h"

#include <algorithm>

/////////////////////////////////////////////////////////////////////////////
// CL_Logger Construction:

CL_Logger::CL_Logger()
{
	enable();
}
	
CL_Logger::~CL_Logger()
{
	disable();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Logger Attributes:

std::vector<CL_Logger*> CL_Logger::instances;

CL_Mutex CL_Logger::mutex;

/////////////////////////////////////////////////////////////////////////////
// CL_Logger Operations:

void CL_Logger::enable()
{
	CL_MutexSection mutex_lock(&CL_Logger::mutex);
	instances.push_back(this);
}
	
void CL_Logger::disable()
{
	CL_MutexSection mutex_lock(&CL_Logger::mutex);
	std::vector<CL_Logger*>::iterator il = std::find(instances.begin(),instances.end(),this);
	if(il != instances.end())
		instances.erase(il);
}

void cl_log_event(const CL_StringRef &type, const CL_StringRef &text)
{
	CL_MutexSection mutex_lock(&CL_Logger::mutex);
	if (CL_Logger::instances.empty())
		return;
	for(std::vector<CL_Logger*>::iterator il = CL_Logger::instances.begin(); il != CL_Logger::instances.end(); il++)
		(*il)->log(type, text);
}

void CL_Logger::log(const CL_StringRef &type, const CL_StringRef &text)
{
	throw CL_Exception("Implement me");
}

/////////////////////////////////////////////////////////////////////////////
// CL_Logger Implementation:
