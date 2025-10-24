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
**    Harry Storbacka
*/

#include "Core/precomp.h"
#include "API/Core/System/thread_local_storage.h"
#include "thread_local_storage_impl.h"

#ifdef WIN32
// This doesnt work if you use ClanLib in a DLL that is LoadLibrary'ed.. -- mbn
//	static __declspec(thread) CL_ThreadLocalStorage_Impl *cl_tls_impl = 0;
	static DWORD cl_tls_index = TLS_OUT_OF_INDEXES;
	CL_Mutex cl_tls_mutex;
	class CL_TLSCleanUpHandler
	{
	public:
		~CL_TLSCleanUpHandler()
		{
			if (cl_tls_index != TLS_OUT_OF_INDEXES)
			{
				TlsFree(cl_tls_index);
				cl_tls_index = TLS_OUT_OF_INDEXES;
			}
		}
	} cl_tls_cleanup;
#elif !defined(HAVE_TLS)
	static bool cl_tls_index_created = false;
	static pthread_key_t cl_tls_index = 0;
	CL_Mutex cl_tls_mutex;
	class CL_TLSCleanUpHandler
	{
	public:
		~CL_TLSCleanUpHandler()
		{
			if (cl_tls_index_created)
			{
				pthread_key_delete(cl_tls_index);
				cl_tls_index_created = false;
				cl_tls_index = 0;
			}
		}
	} cl_tls_cleanup;
#else
	__thread CL_ThreadLocalStorage_Impl *cl_tls_impl = 0;
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_ThreadLocalStorage Construction:

CL_ThreadLocalStorage::CL_ThreadLocalStorage()
{
	CL_System::alloc_thread_temp_pool();

#ifdef WIN32
	if (cl_tls_index == TLS_OUT_OF_INDEXES)
	{
		CL_MutexSection mutex_lock(&cl_tls_mutex);
		cl_tls_index = TlsAlloc();
	}

	CL_ThreadLocalStorage_Impl *cl_tls_impl = (CL_ThreadLocalStorage_Impl *) TlsGetValue(cl_tls_index);
#elif !defined(HAVE_TLS)
	if (!cl_tls_index_created)
	{
		CL_MutexSection mutex_lock(&cl_tls_mutex);
		pthread_key_create(&cl_tls_index, 0);
		cl_tls_index_created = true;
	}

	CL_ThreadLocalStorage_Impl *cl_tls_impl = (CL_ThreadLocalStorage_Impl *) pthread_getspecific(cl_tls_index);
#endif
	if (!cl_tls_impl)
	{
		cl_tls_impl = new CL_ThreadLocalStorage_Impl;
#ifdef WIN32
		TlsSetValue(cl_tls_index, cl_tls_impl);
#elif !defined(HAVE_TLS)
		pthread_setspecific(cl_tls_index, cl_tls_impl);
#endif
	}
	else
	{
		cl_tls_impl->add_reference();
	}
}

CL_ThreadLocalStorage::~CL_ThreadLocalStorage()
{
#ifdef WIN32
	if (cl_tls_index == TLS_OUT_OF_INDEXES)
		return;
	CL_ThreadLocalStorage_Impl *cl_tls_impl = (CL_ThreadLocalStorage_Impl *) TlsGetValue(cl_tls_index);
#elif !defined(HAVE_TLS)
	if (!cl_tls_index_created)
		return;
	CL_ThreadLocalStorage_Impl *cl_tls_impl = (CL_ThreadLocalStorage_Impl *) pthread_getspecific(cl_tls_index);
#endif
	cl_tls_impl->release_reference();

	CL_System::free_thread_temp_pool();
}

/////////////////////////////////////////////////////////////////////////////
// CL_ThreadLocalStorage Attributes:

CL_UnknownSharedPtr CL_ThreadLocalStorage::get_variable(const CL_StringRef &name)
{
#ifdef WIN32
	if (cl_tls_index == TLS_OUT_OF_INDEXES)
		throw CL_Exception(cl_text("No CL_ThreadLocalStorage object created for this thread."));
	CL_ThreadLocalStorage_Impl *cl_tls_impl = (CL_ThreadLocalStorage_Impl *) TlsGetValue(cl_tls_index);
#elif !defined(HAVE_TLS)
	if (!cl_tls_index_created)
		throw CL_Exception(cl_text("No CL_ThreadLocalStorage object created for this thread."));
	CL_ThreadLocalStorage_Impl *cl_tls_impl = (CL_ThreadLocalStorage_Impl *) pthread_getspecific(cl_tls_index);
#endif
	if (cl_tls_impl == 0)
		throw CL_Exception(cl_text("No CL_ThreadLocalStorage object created for this thread."));
	return cl_tls_impl->get_variable(name);
}

/////////////////////////////////////////////////////////////////////////////
// CL_ThreadLocalStorage Operations:

void CL_ThreadLocalStorage::set_variable(const CL_StringRef &name, CL_UnknownSharedPtr ptr)
{
#ifdef WIN32
	if (cl_tls_index == TLS_OUT_OF_INDEXES)
		throw CL_Exception(cl_text("No CL_ThreadLocalStorage object created for this thread."));
	CL_ThreadLocalStorage_Impl *cl_tls_impl = (CL_ThreadLocalStorage_Impl *) TlsGetValue(cl_tls_index);
#elif !defined(HAVE_TLS)
	if (!cl_tls_index_created)
		throw CL_Exception(cl_text("No CL_ThreadLocalStorage object created for this thread."));
	CL_ThreadLocalStorage_Impl *cl_tls_impl = (CL_ThreadLocalStorage_Impl *) pthread_getspecific(cl_tls_index);
#endif
	if (cl_tls_impl == 0)
		throw CL_Exception(cl_text("No CL_ThreadLocalStorage object created for this thread."));
	cl_tls_impl->set_variable(name,ptr);
}

/////////////////////////////////////////////////////////////////////////////
// CL_ThreadLocalStorage Implementation:
