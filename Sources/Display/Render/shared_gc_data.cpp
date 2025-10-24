/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
*/

#include "Display/precomp.h"
#include "API/Core/System/mutex.h"
#include "API/Display/Render/shared_gc_data.h"


CL_SharedGCData::CL_SharedGCData()
: reference_count(0)
{
}

CL_SharedGCData::~CL_SharedGCData()
{
}

CL_Signal_v0 &CL_SharedGCData::func_gc_destruction_imminent()
{
	CL_SharedGCData *self = get_instance();
	return self->sig_destruction_imminent;
}

CL_Texture CL_SharedGCData::load_texture(CL_GraphicContext &gc, const CL_String &filename, const CL_VirtualDirectory &virtual_directory)
{
	CL_SharedGCData *self = get_instance();

	CL_String key = virtual_directory.get_name() + filename;

	std::vector<SharedTextureMap>::size_type i;
	for (i=0; i<self->textures.size(); i++)
	{
		if (self->textures[i].key == key)
			return self->textures[i].texture;
	}

	CL_Texture texture = CL_Texture(gc, filename, virtual_directory, false);
	self->textures.push_back(SharedTextureMap(key, texture));
	return texture;
}

void CL_SharedGCData::unload_texture(const CL_String &filename, const CL_VirtualDirectory &virtual_directory)
{
	CL_SharedGCData *self = get_instance();

	CL_String key = virtual_directory.get_name() + filename;

	std::vector<SharedTextureMap>::iterator it;
	for (it=self->textures.begin(); it<self->textures.end(); ++it)
	{
		if ((*it).key == key)
		{
			self->textures.erase(it);
			break;
		}
	}
}

void CL_SharedGCData::unload_all_textures()
{
	CL_SharedGCData *self = get_instance();
	self->textures.clear();
}

#ifdef WIN32
	static DWORD cl_sharedgc_tls_index = TLS_OUT_OF_INDEXES;
	CL_Mutex cl_sharedgc_tls_mutex;
	class CL_SharedGCDataTLSCleanUpHandler
	{
	public:
		~CL_SharedGCDataTLSCleanUpHandler()
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
	cl_sharedgc->reference_count++;
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
	cl_sharedgc->reference_count++;
#else
	if (cl_tls_sharedgc == 0)
		cl_tls_sharedgc = new CL_SharedGCData;
	cl_tls_sharedgc->reference_count++;
#endif
}

void CL_SharedGCData::release_ref()
{
#ifdef WIN32
	CL_SharedGCData *cl_sharedgc = (CL_SharedGCData *) TlsGetValue(cl_sharedgc_tls_index);
	if (cl_sharedgc && --cl_sharedgc->reference_count == 0)
	{
		cl_sharedgc->sig_destruction_imminent.invoke();
		cl_sharedgc->unload_all_textures(); // Need to do this here because we have a vector of graphic contexts, and if the vector gets cleared on destruction before the texture map is cleared - bad things happen.
		delete cl_sharedgc;
		TlsSetValue(cl_sharedgc_tls_index, 0);
	}
#elif defined(__APPLE__)
	CL_SharedGCData *cl_sharedgc = (CL_SharedGCData *) pthread_getspecific(cl_sharedgc_tls_index);
	if (cl_sharedgc && --cl_sharedgc->reference_count == 0)
	{
		cl_sharedgc->sig_destruction_imminent.invoke();
		cl_sharedgc->unload_all_textures();
		delete cl_sharedgc;
		pthread_setspecific(cl_sharedgc_tls_index, 0);
	}
#else
	if (cl_tls_sharedgc && --cl_tls_sharedgc->reference_count == 0)
	{
		cl_tls_sharedgc->sig_destruction_imminent.invoke();
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
	return self->graphic_context_providers;
}
