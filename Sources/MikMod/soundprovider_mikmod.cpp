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
**    (if your name is missing here, please add it)
*/

#include "Sound/precomp.h"
#include "API/MikMod/soundprovider_mikmod.h"
#include "API/Core/IOData/iodevice.h"
#include "soundprovider_mikmod_generic.h"
#include "soundprovider_mikmod_session.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_MikMod construction:

CL_SoundProvider_MikMod::CL_SoundProvider_MikMod(
	const CL_String &filename,
	CL_VirtualDirectory directory,
	bool stream) : impl(new CL_SoundProvider_MikMod_Generic)
{
	impl->filename = filename;
	impl->directory = directory;
	impl->stream = stream;

	CL_IODevice input = directory.open_file(filename, CL_File::open_existing, CL_File::access_read, CL_File::share_read);
	int size = input.get_size();
	impl->buffer = CL_DataBuffer(size);
	int bytes_read = input.read(impl->buffer.get_data(), impl->buffer.get_size());
	impl->buffer.set_size(bytes_read);
}

CL_SoundProvider_MikMod::~CL_SoundProvider_MikMod()
{
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
