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

#include <cstring>
#include "Sound/precomp.h"
#include "API/Sound/SoundProviders/soundprovider_raw.h"
#include "soundprovider_raw_generic.h"
#include "soundprovider_raw_session.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Raw construction:

CL_SoundProvider_Raw::CL_SoundProvider_Raw(
	void *sound_data,
	int num_samples,
	int bytes_per_sample,
	bool stereo,
	int frequency) : impl(new CL_SoundProvider_Raw_Generic)
{
	int data_size = num_samples * bytes_per_sample;
	if (stereo) data_size *= 2;

	impl->sound_data = new unsigned char[data_size];
	memcpy(impl->sound_data, sound_data, data_size);
	impl->num_samples = num_samples;
	impl->bytes_per_sample = bytes_per_sample;
	impl->stereo = stereo;
	impl->frequency = frequency;
}

CL_SoundProvider_Raw::~CL_SoundProvider_Raw()
{
	delete[] impl->sound_data;
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Raw operations:

CL_SoundProvider_Session *CL_SoundProvider_Raw::begin_session()
{
	return new CL_SoundProvider_Raw_Session(impl);
}

void CL_SoundProvider_Raw::end_session(CL_SoundProvider_Session *session)
{
	delete session;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Raw implementation:
