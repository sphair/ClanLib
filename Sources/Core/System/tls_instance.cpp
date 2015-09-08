/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#include "Core/precomp.h"
#include "tls_instance.h"
#include "API/Core/Math/cl_math.h"

namespace clan
{

#ifdef WIN32
#elif !defined(HAVE_TLS)
#else
	__thread ThreadLocalStorage_Impl *ThreadLocalStorage_Instance::cl_tls_impl = nullptr;
#endif

	// This class controls the destruction order of clanCore global variables

	ThreadLocalStorage_Instance::ThreadLocalStorage_Instance()
	{
#ifdef WIN32
		cl_tls_index = TLS_OUT_OF_INDEXES;
#elif !defined(HAVE_TLS)
		cl_tls_index_created = false;
		cl_tls_index = 0;
#else
		cl_tls_impl = nullptr;
#endif

		ThreadLocalStorage::instance = this;
		cl_tls = clan::make_unique<ThreadLocalStorage>();	// create initial instance
	}

	ThreadLocalStorage_Instance::~ThreadLocalStorage_Instance()
	{
		cl_tls = std::unique_ptr<ThreadLocalStorage>();
#ifdef WIN32

		if (cl_tls_index != TLS_OUT_OF_INDEXES)
		{
			TlsFree(cl_tls_index);
			cl_tls_index = TLS_OUT_OF_INDEXES;
		}

#elif !defined(HAVE_TLS)

		if (cl_tls_index_created)
		{
			pthread_key_delete(cl_tls_index);
			cl_tls_index_created = false;
			cl_tls_index = 0;
		}
#endif
		ThreadLocalStorage::instance = nullptr;
	}
}
