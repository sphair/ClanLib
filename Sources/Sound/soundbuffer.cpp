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
*/

#include "Sound/precomp.h"
#include "API/Sound/soundbuffer.h"
#include "API/Sound/soundbuffer_session.h"
#include "API/Sound/soundfilter.h"
#include "API/Sound/soundoutput.h"
#include "API/Sound/sound.h"
#include "API/Sound/SoundProviders/soundprovider_factory.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/Resources/resource.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/IOData/path_help.h"
#include "soundbuffer_impl.h"
#include "soundbuffer_session_impl.h"
#include "resourcetype_sample.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SoundBuffer construction:

CL_SoundBuffer::CL_SoundBuffer()
{
}
	
CL_SoundBuffer::CL_SoundBuffer(
	const CL_String &res_id,
	CL_ResourceManager *manager)
{
	CL_Resource resource = manager->get_resource(res_id);

	CL_ResourceDataSession resource_data_session("sample", resource);
	CL_SharedPtr<CL_ResourceData_Sample> data = cl_dynamic_pointer_cast<CL_ResourceData_Sample>(resource.get_data("sample"));
	if (!data)
	{
		data = CL_SharedPtr<CL_ResourceData_Sample>(new CL_ResourceData_Sample(resource));
		resource.set_data("sample", data);
	}

	impl = data->soundbuffer->impl;
	impl->resource_data_session = resource_data_session;
}

CL_SoundBuffer::CL_SoundBuffer(
	CL_SoundProvider *provider)
: impl(new CL_SoundBuffer_Impl)
{
	impl->provider = provider;
}

CL_SoundBuffer::CL_SoundBuffer(
	const CL_String &fullname,
	bool streamed,
	const CL_String &sound_format)
: impl(new CL_SoundBuffer_Impl)
{
	impl->provider = CL_SoundProviderFactory::load(fullname, streamed, sound_format);
}

CL_SoundBuffer::CL_SoundBuffer(
		const CL_String &filename,
		bool streamed,
		const CL_VirtualDirectory &directory,
		const CL_String &type)
: impl(new CL_SoundBuffer_Impl)
{
	impl->provider = CL_SoundProviderFactory::load(filename, streamed, directory, type);
}

CL_SoundBuffer::CL_SoundBuffer(
		CL_IODevice &file,
		bool streamed,
		const CL_String &type)
: impl(new CL_SoundBuffer_Impl)
{
	impl->provider = CL_SoundProviderFactory::load(file, streamed, type);
}

CL_SoundBuffer::~CL_SoundBuffer()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundBuffer attributes:

CL_SoundProvider *CL_SoundBuffer::get_provider() const
{
	if (!impl)
		return 0;
	return impl->provider;
}


void CL_SoundBuffer::throw_if_null() const
{
	if (!impl)
		throw CL_Exception("CL_SoundBuffer is null");
}

float CL_SoundBuffer::get_volume() const
{
	return impl->volume;
}

float CL_SoundBuffer::get_pan() const
{
	return impl->pan;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundBuffer operations:

void CL_SoundBuffer::set_volume(float new_volume)
{
	CL_MutexSection mutex_lock(&impl->mutex);
	impl->volume = new_volume;
}

void CL_SoundBuffer::set_pan(float new_pan)
{
	CL_MutexSection mutex_lock(&impl->mutex);
	impl->pan = new_pan;
}

void CL_SoundBuffer::add_filter(CL_SoundFilter &filter)
{
	CL_MutexSection mutex_lock(&impl->mutex);
	impl->filters.push_back(filter);
}

void CL_SoundBuffer::remove_filter(CL_SoundFilter &filter)
{
	CL_MutexSection mutex_lock(&impl->mutex);
	for (unsigned int i=0; i<impl->filters.size(); i++)
	{
		if (impl->filters[i] == filter)
		{
			impl->filters.erase(impl->filters.begin()+i);
			break;
		}
	}
}

CL_SoundBuffer_Session CL_SoundBuffer::play(bool looping, CL_SoundOutput *output)
{
	CL_SoundBuffer_Session session = prepare(looping, output);
	session.play();
	return session;
}

CL_SoundBuffer_Session CL_SoundBuffer::prepare(bool looping, CL_SoundOutput *output)
{
	CL_SoundOutput current_output = CL_Sound::get_current_output();
	if (output == 0) output = &current_output;

	CL_MutexSection mutex_lock(&impl->mutex);
	return CL_SoundBuffer_Session(*this, looping, *output);
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundBuffer implementation:
