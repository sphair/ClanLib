/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
#include "soundprovider_vorbis_session.h"
#include "soundprovider_vorbis_impl.h"
#include "API/Sound/soundformat.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/IOData/memory_device.h"
#include "API/Core/System/exception.h"

namespace clan
{
	SoundProvider_Vorbis_Session::SoundProvider_Vorbis_Session(SoundProvider_Vorbis &source) :
		source(source), position(0), stream_eof(false), handle(nullptr), stream_byte_offset(0), pcm(nullptr), pcm_position(0), pcm_samples(0)
	{
		int error = 0;
		handle = stb_vorbis_open_pushdata(source.impl->buffer.get_data<unsigned char>(), source.impl->buffer.get_size(), &stream_byte_offset, &error, nullptr);
		if (handle == nullptr)
			throw Exception("Unable to read ogg file");

		stream_info = stb_vorbis_get_info(handle);
	}

	SoundProvider_Vorbis_Session::~SoundProvider_Vorbis_Session()
	{
		if (handle)
			stb_vorbis_close(handle);
	}

	int SoundProvider_Vorbis_Session::get_num_samples() const
	{
		return -1;
	}

	int SoundProvider_Vorbis_Session::get_frequency() const
	{
		return stream_info.sample_rate;
	}

	int SoundProvider_Vorbis_Session::get_num_channels() const
	{
		return stream_info.channels;
	}

	int SoundProvider_Vorbis_Session::get_position() const
	{
		return position;
	}

	bool SoundProvider_Vorbis_Session::eof() const
	{
		return stream_eof;
	}

	void SoundProvider_Vorbis_Session::stop()
	{
	}

	bool SoundProvider_Vorbis_Session::play()
	{
		return true;
	}

	bool SoundProvider_Vorbis_Session::set_position(int pos)
	{
		// Currently only support seeking to beginning of stream.
		if (pos != 0) return false;

		if (handle)
			stb_vorbis_close(handle);
		handle = nullptr;
		stream_byte_offset = 0;

		int error = 0;
		handle = stb_vorbis_open_pushdata(source.impl->buffer.get_data<unsigned char>(), source.impl->buffer.get_size(), &stream_byte_offset, &error, nullptr);
		if (handle == nullptr)
			throw Exception("Unable to read ogg file");

		stream_info = stb_vorbis_get_info(handle);
		stream_eof = false;
		return true;
	}

	int SoundProvider_Vorbis_Session::get_data(float **channels, int data_requested)
	{
		int data_left = data_requested;
		while (!eof() && data_left > 0)
		{
			while (pcm_position == pcm_samples)
			{
				pcm = nullptr;
				pcm_position = 0;
				pcm_samples = 0;
				int bytes_used = stb_vorbis_decode_frame_pushdata(handle, source.impl->buffer.get_data<unsigned char>() + stream_byte_offset, source.impl->buffer.get_size() - stream_byte_offset, nullptr, &pcm, &pcm_samples);
				stream_byte_offset += bytes_used;
				if (bytes_used == 0 || stream_byte_offset == source.impl->buffer.get_size())
				{
					stream_eof = true;
					break;
				}
			}

			int samples = pcm_samples - pcm_position;
			if (samples > data_left) samples = data_left;

			int buffer_pos = data_requested - data_left;
			for (int j = 0; j < stream_info.channels; j++)
			{
				memcpy(&channels[j][buffer_pos], &pcm[j][pcm_position], samples*sizeof(float));
			}

			pcm_position += samples;
			data_left -= samples;

			position += samples;
		}

		return data_requested - data_left;
	}
}
