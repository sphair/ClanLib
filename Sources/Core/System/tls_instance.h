/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
#include <mutex>

namespace clan
{
	// This class MUST be a singleton, created in SetupCore_Impl

	class ThreadLocalStorage;
	class ThreadLocalStorage_Instance;

	class ThreadLocalStorage_Instance
	{
	public:
		ThreadLocalStorage_Instance();
		~ThreadLocalStorage_Instance();

	private:
		friend class ThreadLocalStorage;

		// These cl_tls_ variables are used by System/thread_local_storage.cpp
#ifdef WIN32
		std::recursive_mutex cl_tls_mutex;
		DWORD cl_tls_index;
#elif !defined(HAVE_TLS)
		std::recursive_mutex cl_tls_mutex;
		bool cl_tls_index_created;
		pthread_key_t cl_tls_index;
#else
		static __thread ThreadLocalStorage_Impl *cl_tls_impl;
#endif

	public:
		std::unique_ptr<ThreadLocalStorage> cl_tls;
	};
}
