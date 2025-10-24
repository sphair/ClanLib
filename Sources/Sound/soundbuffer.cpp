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

#include "Sound/precomp.h"
#include "API/Sound/soundbuffer.h"
#include "API/Sound/soundbuffer_session.h"
#include "API/Sound/soundfilter.h"
#include "API/Sound/soundoutput.h"
#include "API/Sound/sound.h"
#include "API/Sound/SoundProviders/soundprovider_factory.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/Resources/resource.h"
#include "API/Core/IOData/inputsource_provider_file.h"
#include "soundbuffer_generic.h"
#include "soundbuffer_session_generic.h"
#include "resourcetype_sample.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SoundBuffer construction:

CL_SoundBuffer::CL_SoundBuffer()
{
}
	
CL_SoundBuffer::CL_SoundBuffer(
	const std::string &res_id,
	CL_ResourceManager *manager)
{
	resource = manager->get_resource(res_id);
	resource.load();

	CL_ResourceData_Sample *data =
		(CL_ResourceData_Sample *) resource.get_data("sample");

	if (!data)
		throw CL_Error("Resource '" + res_id + "' is not of type 'sample'");

	impl = data->soundbuffer->impl;
}

CL_SoundBuffer::CL_SoundBuffer(
	CL_SoundProvider *provider,
	bool delete_provider) : impl(new CL_SoundBuffer_Generic)
{
	impl->provider = provider;
	impl->delete_provider = delete_provider;
}

CL_SoundBuffer::CL_SoundBuffer(
	const std::string &filename,
	bool streamed,
	const std::string &sound_format) : impl(new CL_SoundBuffer_Generic)
{
	CL_InputSourceProvider_File *inputprovider = new CL_InputSourceProvider_File(".");
	impl->provider = CL_SoundProviderFactory::load(
		filename,
		streamed,
		sound_format,
		inputprovider);
	impl->delete_provider = true;
	delete inputprovider;
}

CL_SoundBuffer::CL_SoundBuffer(const CL_SoundBuffer &copy) : impl(copy.impl)
{
	resource = copy.resource;
	resource.load();
}

CL_SoundBuffer::~CL_SoundBuffer()
{
	resource.unload();
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundBuffer attributes:

CL_SoundProvider *CL_SoundBuffer::get_sound_provider() const
{
	CL_MutexSection mutex_lock(&impl->mutex);
	return impl->provider;
}

float CL_SoundBuffer::get_volume() const
{
	CL_MutexSection mutex_lock(&impl->mutex);
	return impl->volume;
}

float CL_SoundBuffer::get_pan() const
{
	CL_MutexSection mutex_lock(&impl->mutex);
	return impl->pan;
}

bool CL_SoundBuffer::is_playing() const
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundBuffer operations:

CL_SoundBuffer &CL_SoundBuffer::operator =(const CL_SoundBuffer &copy)
{
	resource.unload();
	impl = copy.impl;
	resource = copy.resource;
	resource.load();
	return *this;
}

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

void CL_SoundBuffer::add_filter(CL_SoundFilter *filter, bool delete_filter)
{
	CL_MutexSection mutex_lock(&impl->mutex);
	impl->filters.push_back(filter);
	impl->delete_filters.push_back(delete_filter);
}

void CL_SoundBuffer::remove_filter(CL_SoundFilter *filter)
{
	CL_MutexSection mutex_lock(&impl->mutex);
	for (unsigned int i=0; i<impl->filters.size(); i++)
	{
		if (impl->filters[i] == filter)
		{
			if (impl->delete_filters[i]) delete impl->filters[i];
			impl->filters.erase(impl->filters.begin()+i);
			impl->delete_filters.erase(impl->delete_filters.begin()+i);
			break;
		}
	}
}

void CL_SoundBuffer::stop()
{
	CL_MutexSection mutex_lock(&impl->mutex);
}

CL_SoundBuffer_Session CL_SoundBuffer::play(bool looping, CL_SoundOutput *output)
{
	CL_SoundBuffer_Session session = prepare(looping, output);
	session.play();
	return session;
}

CL_SoundBuffer_Session CL_SoundBuffer::prepare(bool looping, CL_SoundOutput *output)
{
	if (output == 0) output = CL_Sound::get_current_output();

	CL_MutexSection mutex_lock(&impl->mutex);
	return CL_SoundBuffer_Session(CL_MutexSharedPtr<CL_SoundBuffer_Session_Generic>(new CL_SoundBuffer_Session_Generic(impl, looping, output->impl)));
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundBuffer implementation:
