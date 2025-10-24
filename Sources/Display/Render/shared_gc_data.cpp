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
**    Harry Storbacka
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Core/System/mutex.h"
#include "API/Core/System/disposable_object.h"
#include "API/Display/Render/shared_gc_data.h"
#include <algorithm>

#include "shared_gc_data_impl.h"

CL_SharedGCData::CL_SharedGCData(): impl(new CL_SharedGCData_Impl)
{
}

CL_SharedGCData::~CL_SharedGCData()
{
}

CL_Signal_v0 &CL_SharedGCData::func_gc_destruction_imminent()
{
	CL_SharedGCData *self = get_instance();
	return self->impl->func_gc_destruction_imminent();
}

CL_Texture CL_SharedGCData::load_texture(CL_GraphicContext &gc, const CL_String &filename, const CL_VirtualDirectory &virtual_directory, const CL_ImageImportDescription &import_desc)
{
	CL_SharedGCData *self = get_instance();
	return self->impl->load_texture(gc, filename, virtual_directory, import_desc);
}

void CL_SharedGCData::unload_texture(const CL_String &filename, const CL_VirtualDirectory &virtual_directory, const CL_ImageImportDescription &import_desc)
{
	CL_SharedGCData *self = get_instance();
	self->impl->unload_texture(filename, virtual_directory, import_desc);
}

void CL_SharedGCData::unload_all_textures()
{
	CL_SharedGCData *self = get_instance();
	self->impl->unload_all_textures();
}


#ifdef WIN32
	static DWORD cl_sharedgc_tls_index = TLS_OUT_OF_INDEXES;
	CL_Mutex cl_sharedgc_tls_mutex;
	class CL_SharedGCData_ImplTLSCleanUpHandler
	{
	public:
		~CL_SharedGCData_ImplTLSCleanUpHandler()
		{
			if (cl_sharedgc_tls_index != TLS_OUT_OF_INDEXES)
			{
				TlsFree(cl_sharedgc_tls_index);
				cl_sharedgc_tls_index = TLS_OUT_OF_INDEXES;
			}
		}
	} cl_sharedgc_tls_cleanup;
#elif defined(__APPLE__)
	static bool cl_sharedgc_tls_index_created = false;
	static pthread_key_t cl_sharedgc_tls_index;
	CL_Mutex cl_sharedgc_tls_mutex;
#else
	__thread CL_SharedGCData *cl_tls_sharedgc = 0;
#endif


void CL_SharedGCData::add_ref()
{
#ifdef WIN32
	if (cl_sharedgc_tls_index == TLS_OUT_OF_INDEXES)
	{
		CL_MutexSection mutex_lock(&cl_sharedgc_tls_mutex);
		cl_sharedgc_tls_index = TlsAlloc();
	}

	CL_SharedGCData *cl_sharedgc = (CL_SharedGCData *) TlsGetValue(cl_sharedgc_tls_index);
	if (cl_sharedgc == 0)
	{
		cl_sharedgc = new CL_SharedGCData;
		TlsSetValue(cl_sharedgc_tls_index, cl_sharedgc);
	}
	cl_sharedgc->impl->reference_count++;
#elif defined(__APPLE__)
	if (!cl_sharedgc_tls_index_created)
	{
		CL_MutexSection mutex_lock(&cl_sharedgc_tls_mutex);
		pthread_key_create(&cl_sharedgc_tls_index, 0);
		cl_sharedgc_tls_index_created = true;
	}
	
	CL_ThreadTempStackPool *cl_sharedgc = (CL_SharedGCData *) pthread_getspecific(cl_sharedgc_tls_index);
	if (cl_sharedgc == 0)
	{
		cl_sharedgc = new CL_SharedGCData;
		pthread_setspecific(cl_sharedgc_tls_index, cl_sharedgc);
	}
	cl_sharedgc->impl->reference_count++;
#else
	if (cl_tls_sharedgc == 0)
		cl_tls_sharedgc = new CL_SharedGCData;
	cl_tls_sharedgc->impl->reference_count++;
#endif
}

void CL_SharedGCData::release_ref()
{
#ifdef WIN32
	CL_SharedGCData *cl_sharedgc = (CL_SharedGCData *) TlsGetValue(cl_sharedgc_tls_index);
	if (cl_sharedgc && --cl_sharedgc->impl->reference_count == 0)
	{
		cl_sharedgc->func_gc_destruction_imminent().invoke();
		cl_sharedgc->unload_all_textures(); // Need to do this here because we have a vector of graphic contexts, and if the vector gets cleared on destruction before the texture map is cleared - bad things happen.
		delete cl_sharedgc;
		TlsSetValue(cl_sharedgc_tls_index, 0);
	}
#elif defined(__APPLE__)
	CL_SharedGCData *cl_sharedgc = (CL_SharedGCData *) pthread_getspecific(cl_sharedgc_tls_index);
	if (cl_sharedgc && --cl_sharedgc->impl->reference_count == 0)
	{
		cl_sharedgc->func_gc_destruction_imminent().invoke();
		cl_sharedgc->unload_all_textures();
		delete cl_sharedgc;
		pthread_setspecific(cl_sharedgc_tls_index, 0);
	}
#else
	if (cl_tls_sharedgc && --cl_tls_sharedgc->impl->reference_count == 0)
	{
		cl_tls_sharedgc->func_gc_destruction_imminent().invoke();
		cl_tls_sharedgc->unload_all_textures();
		delete cl_tls_sharedgc;
		cl_tls_sharedgc = 0;
	}
#endif
}

CL_SharedGCData *CL_SharedGCData::get_instance()
{
#ifdef WIN32
	CL_SharedGCData *cl_sharedgc = (CL_SharedGCData *) TlsGetValue(cl_sharedgc_tls_index);
	return cl_sharedgc;
#elif defined(__APPLE__)
	CL_SharedGCData *cl_sharedgc = (CL_SharedGCData *) pthread_getspecific(cl_sharedgc_tls_index);
	return cl_sharedgc;
#else
	return cl_tls_sharedgc;
#endif
}

std::vector<CL_GraphicContextProvider*> &CL_SharedGCData::get_gc_providers()
{
	CL_SharedGCData *self = get_instance();
	return self->impl->get_gc_providers();
}

void CL_SharedGCData::dispose_objects()
{
	CL_SharedGCData *self = get_instance();
	self->impl->dispose_objects();
}

void CL_SharedGCData::add_disposable(CL_DisposableObject *disposable)
{
	CL_SharedGCData *self = get_instance();
	self->impl->add_disposable(disposable);
}

void CL_SharedGCData::remove_disposable(CL_DisposableObject *disposable)
{
	CL_SharedGCData *self = get_instance();
	if (self)
		self->impl->remove_disposable(disposable);
}
