/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
#include "API/Sound/SoundProviders/soundprovider_recorder.h"
#include "API/Sound/SoundProviders/soundprovider_session.h"
#include "soundprovider_recorder_impl.h"
#ifdef WIN32
#include "Win32/soundprovider_recorder_directsound.h"
#else
#ifdef __APPLE__
// #include "MacOSX/soundprovider_osx.h"
#include <assert.h>
#else
#include "Unix/soundprovider_recorder_oss.h"
#endif
#endif

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// SoundProvider_Recorder construction:

SoundProvider_Recorder::SoundProvider_Recorder(
	int frequency) : impl(new SoundProvider_Recorder_Impl)
{
	impl->frequency = frequency;
}

SoundProvider_Recorder::~SoundProvider_Recorder()
{
}

/////////////////////////////////////////////////////////////////////////////
// SoundProvider_Recorder operations:

SoundProvider_Session *SoundProvider_Recorder::begin_session()
{
#ifdef WIN32
	return new SoundProvider_Recorder_DirectSound_Session(impl->frequency);
#else
#ifdef __APPLE__
	assert(false); // not implemented.
	return 0;
#else
	return new SoundProvider_Recorder_OSS_Session(impl->frequency);
#endif
#endif
}

void SoundProvider_Recorder::end_session(SoundProvider_Session *session)
{
	delete session;
}

/////////////////////////////////////////////////////////////////////////////
// SoundProvider_Recorder implementation:

}
