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

//! clanMikMod="Sound Providers"
//! header=mikmod.h

#ifndef header_sound_provider_mikmod
#define header_sound_provider_mikmod

#ifdef CL_API_DLL
#ifdef CL_MIKMOD_EXPORT
#define CL_API_MIKMOD __declspec(dllexport)
#else
#define CL_API_MIKMOD __declspec(dllimport)
#endif
#else
#define CL_API_MIKMOD
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "../Sound/soundprovider.h"
#include <string>

class CL_InputSourceProvider;
class CL_SoundProvider_MikMod_Generic;

//: Module format (.mod, .s3m, .xm, etc) sound provider.
//- !group=MikMod/Sound Providers!
//- !header=mikmod.h!
class CL_API_MIKMOD CL_SoundProvider_MikMod : public CL_SoundProvider
{
//! Construction:
public:
	//: Constructs a sound provider based on a MikMod compatible module file.
	//param filename: Filename of module file.
	//param provider: Input source provider used to retrieve module file.
	//param stream: If true, will stream from disk. If false, will load it to memory.
	CL_SoundProvider_MikMod(
		const std::string &filename,
		CL_InputSourceProvider *provider = 0,
		bool stream = false);

	virtual ~CL_SoundProvider_MikMod();

//! Operations:
public:
	//: Called by CL_SoundBuffer when a new session starts.
	//return: The soundbuffer session to be attached to the newly started session.
	virtual CL_SoundProvider_Session *begin_session();

	//: Called by CL_SoundBuffer when a session has finished. After this call,
	//- <p>CL_SoundBuffer will not access the session anymore. It can safely be deleted
	//- here (and in most cases should be delete here).</p>
	virtual void end_session(CL_SoundProvider_Session *session);

//! Implementation:
private:
	CL_SoundProvider_MikMod_Generic *impl;
};

#endif
