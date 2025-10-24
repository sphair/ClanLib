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
*/

/// \addtogroup clanSound_Sound_Providers clanSound Sound Providers
/// \{

#pragma once

#include "soundprovider_type.h"

/// \brief Class template to register a sound provider type.
///
/// \xmlonly !group=Sound/Sound Providers! !header=sound.h! \endxmlonly
template<class SoundProviderClass>
class CL_SoundProviderType_Register : public CL_SoundProviderType
{
/// \name Construction
/// \{

public:
	/// \brief Registers sound provider type in the CL_SoundProviderFactory.
	CL_SoundProviderType_Register(const CL_String &type) : CL_SoundProviderType(type)
	{
	}

/// \}
/// \name Operations
/// \{

public:
	/// \brief Called to load static with this sound provider type.
	virtual CL_SoundProvider *load(
		const CL_String &filename,
		bool stream,
		const CL_VirtualDirectory &directory)
	{
		return new SoundProviderClass(filename, directory, stream);
	}

	/// \brief Called to load static with this sound provider type.
	virtual CL_SoundProvider *load(
		CL_IODevice &file,
		bool stream)
	{
		return new SoundProviderClass(file, stream);
	}

/// \}
};

/// \}
