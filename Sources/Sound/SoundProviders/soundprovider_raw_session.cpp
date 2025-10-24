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
#include "soundprovider_raw_session.h"
#include "soundprovider_raw_generic.h"
#include "API/Sound/soundformat.h"
#include <cstring>

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Raw_Session construction:

CL_SoundProvider_Raw_Session::CL_SoundProvider_Raw_Session(CL_SoundProvider_Raw_Generic *data) :
	data(data), position(0), reached_end(false)
{
}

CL_SoundProvider_Raw_Session::~CL_SoundProvider_Raw_Session()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Raw_Session attributes:

int CL_SoundProvider_Raw_Session::get_num_samples() const
{
	return data->num_samples;
}

int CL_SoundProvider_Raw_Session::get_frequency() const
{
	return data->frequency;
}

CL_SoundFormat CL_SoundProvider_Raw_Session::get_format() const
{
	switch (data->bytes_per_sample)
	{
	case 1:
		return sf_8bit_signed;

	case 2:
		return sf_16bit_signed;

	default:
		throw CL_Error("Unsupported sample format");
	}
}

int CL_SoundProvider_Raw_Session::get_num_channels() const
{
	return (data->stereo) ? 2 : 1;
}

int CL_SoundProvider_Raw_Session::get_position() const
{
	return position;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Raw_Session operations:

bool CL_SoundProvider_Raw_Session::eof() const
{
	return (position >= get_num_samples());
}

void CL_SoundProvider_Raw_Session::stop()
{
}

bool CL_SoundProvider_Raw_Session::play()
{
	return true;
}
	
bool CL_SoundProvider_Raw_Session::set_position(int pos)
{
	position = pos;
	return true;
}

int CL_SoundProvider_Raw_Session::get_data(void **data_ptr, int data_requested)
{
	int num_samples = get_num_samples();
	if (position + data_requested > num_samples)
	{
		data_requested = num_samples - position;
		if (data_requested < 0) return 0;
	}

	if (data->bytes_per_sample == 2)
	{
		short **channels = (short **) data_ptr;

		if (data->stereo)
		{
			short *src = (short *) data->sound_data + position * 2;
			for (int i=0; i<data_requested; i++)
			{
				channels[0][i] = *(src++);
				channels[1][i] = *(src++);
			}
		}
		else
		{
			memcpy(channels[0], data->sound_data+position*data->bytes_per_sample, data_requested*data->bytes_per_sample);
		}
	}
	else if (data->bytes_per_sample == 1)
	{
		char **channels = (char **) data_ptr;

		if (data->stereo)
		{
			char *src = (char *) data->sound_data + position * 2;
			for (int i=0; i<data_requested; i++)
			{
				channels[0][i] = *(src++);
				channels[1][i] = *(src++);
			}
		}
		else
		{
			memcpy(channels[0], data->sound_data+position*data->bytes_per_sample, data_requested*data->bytes_per_sample);
		}
	}

	position += data_requested;
	return data_requested;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Raw_Session implementation:
