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
**    Mark Page
*/

#pragma once

#include "Core/precomp.h"

#include "API/Core/System/thread_local_storage.h"

// This class controls the destruction order of clanCore global variables
// This class MUST be a singleton, created in core_global.cpp

class CL_ThreadLocalStorage;
class CL_CoreGlobal;
extern CL_CoreGlobal cl_core_global;

class CL_CoreGlobal
{
public:
	CL_CoreGlobal();
	~CL_CoreGlobal();

private:
	friend class CL_ThreadLocalStorage;

	// These cl_tls_ variables are used by System/thread_local_storage.cpp
#ifdef WIN32
	CL_Mutex cl_tls_mutex;
	DWORD cl_tls_index;
#elif !defined(HAVE_TLS)
	CL_Mutex cl_tls_mutex;
	bool cl_tls_index_created;
	pthread_key_t cl_tls_index;
#else
	static __thread CL_ThreadLocalStorage_Impl *cl_tls_impl;
#endif

	CL_ThreadLocalStorage *cl_tls ;

protected:
	void destroy_tls();
};



