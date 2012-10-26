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
**    (if your name is missing here, please add it)
*/

#include "precomp.h"

#include "soundprovider_mikmod_session.h"
#include "soundprovider_mikmod_impl.h"
#include "module_reader.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_MikMod_Session construction:

CL_SoundProvider_MikMod_Session::CL_SoundProvider_MikMod_Session(CL_SoundProvider_MikMod &source) :
	source(source), num_samples(0), position(0), stream_eof(false)
{
	CL_UniquePtr<CL_IODevice_Memory> input_autoptr(new CL_IODevice_Memory(source.impl->buffer));
	CL_IODevice_Memory *input = input_autoptr.get();

	MREADER *reader = new_clanlib_reader((void *) input);
	if (reader == 0) throw CL_Exception("new_clanlib_reader failed!");

	module = Player_LoadGeneric(reader,CLANLIB_READER_CHANNELS,0);
	if (module == 0)
	{
		delete_clanlib_reader(reader);
		throw CL_Exception(MikMod_strerror(MikMod_errno));
	}

	module->wrap = false;
	module->loop = false;
	module->fadeout = true;

	delete_clanlib_reader(reader);

	frequency = md_mixfreq;
	format = (md_mode & DMODE_16BITS) ? sf_16bit_signed : sf_8bit_unsigned;
	num_channels = (md_mode&DMODE_STEREO) ? 2 : 1;
}

CL_SoundProvider_MikMod_Session::~CL_SoundProvider_MikMod_Session()
{
	Player_Stop();
	Player_Free(module);
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_MikMod_Session attributes:

int CL_SoundProvider_MikMod_Session::get_num_samples() const
{
	return num_samples;
}

int CL_SoundProvider_MikMod_Session::get_frequency() const
{
	return frequency;
}

int CL_SoundProvider_MikMod_Session::get_num_channels() const
{
	return num_channels;
}

int CL_SoundProvider_MikMod_Session::get_position() const
{
	return position;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_MikMod_Session operations:

bool CL_SoundProvider_MikMod_Session::set_looping(bool loop)
{
	module->wrap = loop;
	module->loop = loop;

	return true;
}

bool CL_SoundProvider_MikMod_Session::eof() const
{
	return !Player_Active();
}

void CL_SoundProvider_MikMod_Session::stop()
{
	Player_Stop();
}

bool CL_SoundProvider_MikMod_Session::play()
{
	Player_Start (module);
	return true;
}
	
bool CL_SoundProvider_MikMod_Session::set_position(int pos)
{
	Player_SetPosition(pos);
	position = pos;
	if (num_samples < position) num_samples = position;
	return true;
}

int CL_SoundProvider_MikMod_Session::get_data(float **channels, int data_requested)
{

	int total_written = 0;
	int bytes_per_sample = (format == sf_16bit_signed) ? 2 : 1;
	bytes_per_sample *= num_channels;

	while (data_requested > 0)
	{
		MikMod_Update();
		int written = VC_WriteBytes(buffer, ((data_requested > 16*1024) ? 16*1024 : data_requested) * bytes_per_sample);
		written /= bytes_per_sample;

		if (format == sf_16bit_signed)
		{
			short *src = (short *) buffer;
			if (num_channels == 2)
			{
				float *temp_data_ptr[2];
				temp_data_ptr[0] = channels[0] + total_written;
				temp_data_ptr[1] = channels[1] + total_written;
				CL_SoundSSE::unpack_16bit_stereo(src, written*2, temp_data_ptr);
			}
			else
			{
				CL_SoundSSE::unpack_16bit_mono(src, written*2, channels[0] + total_written);
			}
		}
		else if (format == sf_8bit_unsigned)
		{
			unsigned char *src = (unsigned char *) buffer;
			if (num_channels == 2)
			{
				float *temp_data_ptr[2];
				temp_data_ptr[0] = channels[0] + total_written;
				temp_data_ptr[1] = channels[1] + total_written;

				CL_SoundSSE::unpack_8bit_stereo(src, written, temp_data_ptr);
			}
			else
			{
				CL_SoundSSE::unpack_8bit_mono(src, written, channels[0] + total_written);
			}
		}

		position += written;
		total_written += written;
		data_requested -= written;

		// Check if we reached the end of the song:
		if (module->sngpos >= module->numpos)
		{
			stop();
			break;
		}

		// If no more bytes to write from input stream:
		// (This never happens with this version of mikmod, but its a good check anyway)
		if (written < 16*1024) break;
	}

	if (num_samples < position) num_samples = position;
	return total_written;

}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_MikMod_Session implementation:
