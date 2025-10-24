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
#include "API/Vorbis/soundprovider_vorbis.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/inputsource_provider.h"
#include "API/Core/System/clanstring.h"
#include "soundprovider_vorbis_generic.h"
#include "soundprovider_vorbis_session.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Vorbis construction:

CL_SoundProvider_Vorbis::CL_SoundProvider_Vorbis(
	const std::string &filename,
	CL_InputSourceProvider *provider,
	bool stream) : impl(new CL_SoundProvider_Vorbis_Generic)
{
	if (provider == 0) provider = CL_InputSourceProvider::create_file_provider(".");
	else provider = provider->clone();

	try
	{
		impl->provider = provider;
		impl->filename = filename;
		impl->stream = stream;

		CL_InputSource *input = provider->open_source(filename);
		int size = input->size();
		impl->buffer = new unsigned char[size];
		impl->buffer_size = input->read(impl->buffer, size);
		delete input;
	}
	catch (...)
	{
		delete impl->provider;
		throw;
	}
}

CL_SoundProvider_Vorbis::~CL_SoundProvider_Vorbis()
{
	delete[] impl->buffer;
	delete impl->provider;
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Vorbis operations:

CL_SoundProvider_Session *CL_SoundProvider_Vorbis::begin_session()
{
	return new CL_SoundProvider_Vorbis_Session(impl);
}

void CL_SoundProvider_Vorbis::end_session(CL_SoundProvider_Session *session)
{
	delete session;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Vorbis implementation:
