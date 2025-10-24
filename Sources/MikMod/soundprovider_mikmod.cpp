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
#include "API/MikMod/soundprovider_mikmod.h"
#include "API/Core/IOData/inputsource_provider.h"
#include "API/Core/System/clanstring.h"
#include "soundprovider_mikmod_generic.h"
#include "soundprovider_mikmod_session.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_MikMod construction:

CL_SoundProvider_MikMod::CL_SoundProvider_MikMod(
	const std::string &filename,
	CL_InputSourceProvider *provider,
	bool stream) : impl(new CL_SoundProvider_MikMod_Generic)
{
	if (provider == 0) provider = CL_InputSourceProvider::create_file_provider(".");
	else provider = provider->clone();

	impl->provider = provider;
	impl->filename = filename;
	impl->stream = stream;
}

CL_SoundProvider_MikMod::~CL_SoundProvider_MikMod()
{
	delete impl->provider;
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_MikMod operations:

CL_SoundProvider_Session *CL_SoundProvider_MikMod::begin_session()
{
	return new CL_SoundProvider_MikMod_Session(impl);
}

void CL_SoundProvider_MikMod::end_session(CL_SoundProvider_Session *session)
{
	delete session;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_MikMod implementation:
