/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

#include "Core/precomp.h"
#include "API/Core/System/thread_local_storage.h"
#include "thread_local_storage_impl.h"
#include "setup_core.h"
#include "tls_instance.h"

namespace clan
{
	ThreadLocalStorage_Instance *ThreadLocalStorage::instance = nullptr;

	ThreadLocalStorage::ThreadLocalStorage()
	{
		init_core();

#ifdef WIN32
		if (instance->cl_tls_index == TLS_OUT_OF_INDEXES)
		{
			std::unique_lock<std::recursive_mutex> mutex_lock(instance->cl_tls_mutex);
			instance->cl_tls_index = TlsAlloc();
		}

		ThreadLocalStorage_Impl *tls_impl = static_cast<ThreadLocalStorage_Impl *>(TlsGetValue(instance->cl_tls_index));

		if (!tls_impl)
		{
			tls_impl = new ThreadLocalStorage_Impl;
			TlsSetValue(instance->cl_tls_index, tls_impl);
		}
		else
		{
			tls_impl->add_reference();
		}

#elif !defined(HAVE_TLS)
		if (!instance->cl_tls_index_created)
		{
			std::unique_lock<std::recursive_mutex> mutex_lock(instance->cl_tls_mutex);
			pthread_key_create(&instance->cl_tls_index, 0);
			instance->cl_tls_index_created = true;
		}

		ThreadLocalStorage_Impl *tls_impl = (ThreadLocalStorage_Impl *) pthread_getspecific(instance->cl_tls_index);

		if (!tls_impl)
		{
			tls_impl = new ThreadLocalStorage_Impl;
			pthread_setspecific(instance->cl_tls_index, tls_impl);
		}
		else
		{
			tls_impl->add_reference();
		}
#else

		if (!instance->cl_tls_impl)
		{
			instance->cl_tls_impl = new ThreadLocalStorage_Impl;
		}
		else
		{
			instance->cl_tls_impl->add_reference();
		}
#endif
	}

	ThreadLocalStorage::~ThreadLocalStorage()
	{
		if (!instance)
			return;
#ifdef WIN32
		if (instance->cl_tls_index == TLS_OUT_OF_INDEXES)
			return;
		ThreadLocalStorage_Impl *tls_impl = (ThreadLocalStorage_Impl *)TlsGetValue(instance->cl_tls_index);
		if (tls_impl)
			tls_impl->release_reference();
#elif !defined(HAVE_TLS)
		if (!instance->cl_tls_index_created)
			return;
		ThreadLocalStorage_Impl *tls_impl = (ThreadLocalStorage_Impl *) pthread_getspecific(instance->cl_tls_index);
		if (tls_impl)
			tls_impl->release_reference();
#else
		if (instance->cl_tls_impl)
			instance->cl_tls_impl->release_reference();
#endif
	}

	void ThreadLocalStorage::init_core()
	{
		if (!instance)
			SetupCore::start();
		if (!instance)
			throw Exception("No ThreadLocalStorage instance");
	}

	std::shared_ptr<ThreadLocalStorageData> ThreadLocalStorage::get_variable(const std::string &name)
	{
		init_core();

#ifdef WIN32
		if (instance->cl_tls_index == TLS_OUT_OF_INDEXES)
			throw Exception("No ThreadLocalStorage object created for this thread.");
		ThreadLocalStorage_Impl *tls_impl = (ThreadLocalStorage_Impl *)TlsGetValue(instance->cl_tls_index);
#elif !defined(HAVE_TLS)
		if (!instance->cl_tls_index_created)
			throw Exception("No ThreadLocalStorage object created for this thread.");
		ThreadLocalStorage_Impl *tls_impl = (ThreadLocalStorage_Impl *) pthread_getspecific(instance->cl_tls_index);
#else
		ThreadLocalStorage_Impl *tls_impl = instance->cl_tls_impl;
#endif
		if (tls_impl == nullptr)
			throw Exception("No ThreadLocalStorage object created for this thread.");
		return tls_impl->get_variable(name);
	}

	void ThreadLocalStorage::set_variable(const std::string &name, std::shared_ptr<ThreadLocalStorageData> ptr)
	{
		init_core();
#ifdef WIN32
		if (instance->cl_tls_index == TLS_OUT_OF_INDEXES)
			throw Exception("No ThreadLocalStorage object created for this thread.");
		ThreadLocalStorage_Impl *tls_impl = static_cast<ThreadLocalStorage_Impl *>(TlsGetValue(instance->cl_tls_index));
#elif !defined(HAVE_TLS)
		if (!instance->cl_tls_index_created)
			throw Exception("No ThreadLocalStorage object created for this thread.");
		ThreadLocalStorage_Impl *tls_impl = static_cast<ThreadLocalStorage_Impl *>(pthread_getspecific(instance->cl_tls_index));
#else
		ThreadLocalStorage_Impl *tls_impl = instance->cl_tls_impl;
#endif
		if (tls_impl == nullptr)
			throw Exception("No ThreadLocalStorage object created for this thread.");
		tls_impl->set_variable(name, ptr);
	}
}
