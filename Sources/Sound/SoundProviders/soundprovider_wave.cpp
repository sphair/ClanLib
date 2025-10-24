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
*/

#include "Sound/precomp.h"
#include "API/Sound/SoundProviders/soundprovider_wave.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/IOData/cl_endian.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/logger.h"
#include "soundprovider_wave_generic.h"
#include "soundprovider_wave_session.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Wave construction:

CL_SoundProvider_Wave::CL_SoundProvider_Wave(
	const CL_String &filename,
	CL_VirtualDirectory virtual_directory,
	bool stream) : impl(new CL_SoundProvider_Wave_Generic)
{
	impl->stream = stream;

	CL_IODevice source = virtual_directory.open_file(filename, CL_File::open_existing, CL_File::access_read, CL_File::share_read);
	source.set_little_endian_mode();

	// Check to see if this is really a .wav-file
	char temp[12];
	memset(temp, 0, 12);
	source.read(temp, 4);
	source.seek(4, CL_IODevice::seek_cur);
	source.read(temp+4, 8);

	cl_uint32 chunk_size = source.read_uint32();
	if (chunk_size & 1) chunk_size++;	// Align RIFF pad byte

	if (memcmp(temp, "RIFFWAVEfmt ", 12) != 0)
	{
		delete impl;
		throw CL_Exception(cl_text("Invalid RIFF WAVE header!"));
	}

	CL_SoundProvider_Wave_Generic::WAVE_FORMAT format;
	format.formatTag = source.read_int16();
	format.nChannels = source.read_uint16();
	format.nSamplesPerSec = source.read_uint32();
	format.nAvgBytesPerSec = source.read_uint32();
	format.nBlockAlign = source.read_uint16();

	source.seek( chunk_size - 14, CL_IODevice::seek_cur );	// Already read 14 bytes, skip the rest

	// Find "data" chunk
	while(true)
	{
		source.read(temp, 4);
		if (!memcmp(temp, "data", 4))
		{
			break;	// Found chunk
		}
		chunk_size = source.read_uint32();
		if (chunk_size & 1) chunk_size++;	// Align RIFF pad byte

		source.seek( chunk_size, CL_IODevice::seek_cur );
	}

	int data_size = source.read_uint32();
	int bytes_per_sample = format.nAvgBytesPerSec / format.nSamplesPerSec;

	impl->frequency = format.nSamplesPerSec;
	impl->num_channels = format.nChannels;
	impl->num_samples = data_size / bytes_per_sample;

	if (bytes_per_sample / format.nChannels == 2) impl->format = sf_16bit_signed;
	else if (bytes_per_sample / format.nChannels == 1) impl->format = sf_8bit_signed;
	else
	{
		delete impl;
		throw CL_Exception(
			cl_format(
				cl_text("Unsupported wave file format (channels=%1, bytes per sample=%2)"),
				format.nChannels,
				bytes_per_sample));
	}

	impl->data = new char[data_size];
	source.read(impl->data, data_size);

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
}

CL_SoundProvider_Wave::~CL_SoundProvider_Wave()
{
	delete[] impl->data;
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
