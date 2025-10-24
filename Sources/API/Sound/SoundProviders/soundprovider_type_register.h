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

//! clanSound="Sound Providers"
//! header=sound.h

#ifndef header_soundprovider_type_register
#define header_soundprovider_type_register

#if _MSC_VER > 1000
#pragma once
#endif

#include "soundprovider_type.h"

//: Class template to register a sound provider type.
//- !group=Sound/Sound Providers!
//- !header=sound.h!
template<class SoundProviderClass>
class CL_SoundProviderType_Register : public CL_SoundProviderType
{
//! Construction:
public:
	//: Registers sound provider type in the CL_SoundProviderFactory.
	CL_SoundProviderType_Register(const std::string &type) : CL_SoundProviderType(type)
	{
	}

//! Operations:
public:
	//: Called to load static with this sound provider type.
	virtual CL_SoundProvider *load(
		const std::string &filename,
		CL_InputSourceProvider *input_provider,
		bool stream)
	{
		return new SoundProviderClass(filename, input_provider, stream);
	}
};

#endif
