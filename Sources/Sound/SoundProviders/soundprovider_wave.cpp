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
#include "API/Sound/SoundProviders/soundprovider_wave.h"
#include "API/Core/IOData/inputsource_provider.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/cl_endian.h"
#include "API/Core/System/clanstring.h"
#include "API/Core/System/log.h"
#include "soundprovider_wave_generic.h"
#include "soundprovider_wave_session.h"
#include <cstring>

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Wave construction:

CL_SoundProvider_Wave::CL_SoundProvider_Wave(
	const std::string &filename,
	CL_InputSourceProvider *provider,
	bool stream) : impl(new CL_SoundProvider_Wave_Generic)
{
	if (provider == 0) provider = CL_InputSourceProvider::create_file_provider(".");
	else provider = provider->clone();

	impl->provider = provider;
	impl->filename = filename;
	impl->stream = stream;

	CL_InputSource *source = provider->open_source(filename);
	source->set_little_endian_mode();

	// Check to see if this is really a .wav-file
	char temp[12];
	source->read(temp, 4);
	source->seek(4, CL_InputSource::seek_cur);
	source->read(&temp[4], 8);
	source->seek(4, CL_InputSource::seek_cur);

	if (memcmp(temp, "RIFFWAVEfmt ", 12) != 0)
	{
		delete impl;
		throw CL_Error("Invalid RIFF WAVE header!");
	}

	CL_SoundProvider_Wave_Generic::WAVE_FORMAT format;
	format.formatTag = source->read_short16();
	format.nChannels = source->read_ushort16();
	format.nSamplesPerSec = source->read_uint32();
	format.nAvgBytesPerSec = source->read_uint32();
	format.nBlockAlign = source->read_ushort16();

	// FIXME: find a better way to handle format struct padding
	source->read(temp, 2);

	// Another sanity check
	source->read(temp, 4);
	temp[4] = 0;
	if (memcmp(temp, "data", 4) != 0)
	{
		delete impl;
		throw CL_Error("Invalid RIFF data chunk!");
	}

	int data_size = source->read_uint32();
	int bytes_per_sample = format.nAvgBytesPerSec / format.nSamplesPerSec;

	impl->frequency = format.nSamplesPerSec;
	impl->num_channels = format.nChannels;
	impl->num_samples = data_size / bytes_per_sample;

	if (bytes_per_sample / format.nChannels == 2) impl->format = sf_16bit_signed;
	else if (bytes_per_sample / format.nChannels == 1) impl->format = sf_8bit_signed;
	else
	{
		delete impl;
		throw CL_Error(
			CL_String::format(
				"Unsupported wave file format (channels=%1, bytes per sample=%2)",
				format.nChannels,
				bytes_per_sample));
	}

	impl->data = new char[data_size];
	source->read(impl->data, data_size);

	// 8 bit wave is unsigned, while 16 bit wave is signed!
	if (impl->format == sf_8bit_signed)
	{
		for (int i=0; i<data_size; i++)
			((unsigned char *) impl->data)[i] -= 128;
	}
	else if (impl->format == sf_16bit_signed && CL_Endian::is_system_big())
	{
		unsigned char *data = (unsigned char *) impl->data;
		for (int i=1; i<data_size; i+=2)
		{
			unsigned char tmp = data[i];
			data[i] = data[i-1];
			data[i-1] = tmp;
		}
	}

	delete source;
}

CL_SoundProvider_Wave::~CL_SoundProvider_Wave()
{
	delete[] impl->data;
	delete impl->provider;
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Wave operations:

CL_SoundProvider_Session *CL_SoundProvider_Wave::begin_session()
{
	return new CL_SoundProvider_Wave_Session(impl);
}

void CL_SoundProvider_Wave::end_session(CL_SoundProvider_Session *session)
{
	delete session;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Wave implementation:

/*
#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include "API/Core/IOData/cl_endian.h"
#include "API/Core/IOData/inputsource_provider.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/System/cl_assert.h"
#include "API/Core/System/error.h"
#include "API/Sound/SoundProviders/static_provider_raw.h"
#include "API/Sound/SoundProviders/static_provider_wave.h"

CL_SoundBuffer *CL_Sample::create(const std::string &sample_id, CL_InputSourceProvider *provider)
{
	return new CL_SoundBuffer(new CL_Sample(sample_id, provider), true);
}

CL_Sample::CL_Sample(const std::string &s_id, CL_InputSourceProvider *_provider) : sample_id(s_id)
{
	if (_provider == NULL)
	{
		provider = CL_InputSourceProvider::create_file_provider(".");
	}
	else
	{
		provider = _provider->clone();
	}
	sample_data = NULL;
}

CL_Sample::~CL_Sample()
{
	delete[] sample_data;
	delete provider;
}

void CL_Sample::load_data()
{
	CL_InputSource *source = provider->open_source(sample_id.c_str());
	cl_assert(source != NULL);

	source->set_little_endian_mode();

	// Check to see if this is really a .wav-file
	char temp[12];
	source->read(temp, 4);
	source->seek(4, CL_InputSource::seek_cur);
	source->read(&temp[4], 8);
	source->seek(4, CL_InputSource::seek_cur);

	if (memcmp(temp, "RIFFWAVEfmt ", 12) != 0)
	{
		throw CL_Error("Invalid RIFF WAVE header!");
	}

	WAVE_FORMAT format;
	format.formatTag = source->read_short16();
	format.nChannels = source->read_ushort16();
	format.nSamplesPerSec = source->read_uint32();
	format.nAvgBytesPerSec = source->read_uint32();
	format.nBlockAlign = source->read_ushort16();

	// FIXME: find a better way to handle format struct padding
	source->read(temp, 2);

	// Another sanity check
	source->read(temp, 4);
	temp[4] = 0;
	if (memcmp(temp, "data", 4) != 0)
	{
		throw CL_Error("Invalid RIFF data chunk!");
	}

	sample_size = source->read_uint32();
	sample_freq = format.nSamplesPerSec;

	int bytes_per_sample = format.nAvgBytesPerSec / format.nSamplesPerSec;

	if (format.nChannels == 2 && bytes_per_sample == 4) sample_format = sf_16bit_signed_stereo;
	else if (format.nChannels == 2 && bytes_per_sample == 2) sample_format = sf_8bit_signed_stereo;
	else if (format.nChannels == 1 && bytes_per_sample == 2) sample_format = sf_16bit_signed;
	else if (format.nChannels == 1 && bytes_per_sample == 1) sample_format = sf_8bit_signed;
	else
	{
		CL_Log::log("debug", "Invalid wave file format");
		CL_Log::log("debug", "Sample size: %1", sample_size);
		CL_Log::log("debug", "Sample frequency: %1", sample_freq);
		CL_Log::log("debug", "Number of channels: %1", format.nChannels);
		CL_Log::log("debug", "Number of bytes pr. sample: %1", bytes_per_sample);

		throw CL_Error("Invalid wave file format");
	}

	sample_data = new signed char[sample_size];
	if (sample_format == sf_8bit_signed || sample_format == sf_8bit_signed_stereo)
	{
		for (int i=0;i<sample_size;i++)
			sample_data[i] = source->read_uchar8()-128;
	}
	else	// 16 bit
	{
		for (int i=0;i<sample_size;i+=2)
			*(short *)&sample_data[i] = source->read_short16();
	}

	delete source;
}

void CL_Sample::lock()
{
	if (sample_data != NULL) return;

	load_data();
}

void CL_Sample::unlock()
{
	delete[] sample_data;
	sample_data = NULL;
}

SoundFormat CL_Sample::get_format() const
{
	return sample_format;
}

int CL_Sample::data_size() const
{
	return sample_size;
}

void *CL_Sample::get_data() const
{
	return sample_data;
}

int CL_Sample::get_frequency() const
{
	return sample_freq;
}
*/
