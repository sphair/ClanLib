/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "API/Core/Resources/xml_resource_document.h"
#include "API/Core/Resources/xml_resource_node.h"
#include "API/Core/IOData/file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/XML/dom_element.h"
#include "soundbuffer_impl.h"
#include "soundbuffer_session_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// SoundBuffer construction:

SoundBuffer::SoundBuffer()
{
}
	
SoundBuffer::SoundBuffer(
	const std::string &res_id,
	const XMLResourceDocument &manager)
: impl(new SoundBuffer_Impl)
{
	XMLResourceNode resource = manager.get_resource(res_id);

	DomElement &element = resource.get_element();

	SoundProvider *provider = 0;

	std::string name = resource.get_element().get_attribute("file");
	std::string sound_format = resource.get_element().get_attribute("format");
	bool streamed = (element.get_attribute("stream", "no") == "yes");

	impl->provider = SoundProviderFactory::load(
		PathHelp::combine(resource.get_base_path(), name),
		streamed,
		resource.get_file_system(), sound_format);

	if (!provider)
		throw Exception("Unknown sample format");
}

SoundBuffer::SoundBuffer(
	SoundProvider *provider)
: impl(new SoundBuffer_Impl)
{
	impl->provider = provider;
}

SoundBuffer::SoundBuffer(
	const std::string &fullname,
	bool streamed,
	const std::string &sound_format)
: impl(new SoundBuffer_Impl)
{
	impl->provider = SoundProviderFactory::load(fullname, streamed, sound_format);
}

SoundBuffer::SoundBuffer(
		const std::string &filename,
		bool streamed,
		const FileSystem &fs,
		const std::string &type)
: impl(new SoundBuffer_Impl)
{
	impl->provider = SoundProviderFactory::load(filename, streamed, fs, type);
}

SoundBuffer::SoundBuffer(
		IODevice &file,
		bool streamed,
		const std::string &type)
: impl(new SoundBuffer_Impl)
{
	impl->provider = SoundProviderFactory::load(file, streamed, type);
}

SoundBuffer::~SoundBuffer()
{
}

/////////////////////////////////////////////////////////////////////////////
// SoundBuffer attributes:

SoundProvider *SoundBuffer::get_provider() const
{
	if (!impl)
		return 0;
	return impl->provider;
}


void SoundBuffer::throw_if_null() const
{
	if (!impl)
		throw Exception("SoundBuffer is null");
}

float SoundBuffer::get_volume() const
{
	return impl->volume;
}

float SoundBuffer::get_pan() const
{
	return impl->pan;
}

/////////////////////////////////////////////////////////////////////////////
// SoundBuffer operations:

void SoundBuffer::set_volume(float new_volume)
{
	MutexSection mutex_lock(&impl->mutex);
	impl->volume = new_volume;
}

void SoundBuffer::set_pan(float new_pan)
{
	MutexSection mutex_lock(&impl->mutex);
	impl->pan = new_pan;
}

void SoundBuffer::add_filter(SoundFilter &filter)
{
	MutexSection mutex_lock(&impl->mutex);
	impl->filters.push_back(filter);
}

void SoundBuffer::remove_filter(SoundFilter &filter)
{
	MutexSection mutex_lock(&impl->mutex);
	for (unsigned int i=0; i<impl->filters.size(); i++)
	{
		if (impl->filters[i] == filter)
		{
			impl->filters.erase(impl->filters.begin()+i);
			break;
		}
	}
}

SoundBuffer_Session SoundBuffer::play(bool looping, SoundOutput *output)
{
	SoundBuffer_Session session = prepare(looping, output);
	session.play();
	return session;
}

SoundBuffer_Session SoundBuffer::prepare(bool looping, SoundOutput *output)
{
	SoundOutput current_output = Sound::get_current_output();
	if (output == 0) output = &current_output;

	MutexSection mutex_lock(&impl->mutex);
	return SoundBuffer_Session(*this, looping, *output);
}

/////////////////////////////////////////////////////////////////////////////
// SoundBuffer implementation:

}
