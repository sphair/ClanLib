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

#include "../api_sound.h"
#include <map>

class CL_SoundProvider;
class CL_InputSourceProvider;
class CL_SoundProviderType;
class CL_VirtualDirectory;
class CL_IODevice;

/// \brief Sound Provider Factor class.
///
/// \xmlonly !group=Sound/Sound Providers! !header=sound.h! \endxmlonly
class CL_API_SOUND CL_SoundProviderFactory
{
/// \name Attributes
/// \{

public:
	/// \brief Map of the class factories for each sound provider type.
	static std::map<CL_String, CL_SoundProviderType *> types;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Loads an sample file from 'filename', using the provider identified by 'type'.
	/** <p>If the type is an empty string, it
	    uses the extension of the filename to determine what type
	    it is.</p>
	    <p>The input provider will be used as a the source for filename,
	    if specified.</p>*/
	static CL_SoundProvider *load(
		const CL_String &filename,
		bool streamed,
		const CL_VirtualDirectory &directory,
		const CL_String &type = CL_String()
		);

	/// \brief Loads an sample file from 'filename', using the provider identified by 'type'.
	static CL_SoundProvider *load(
		const CL_String &fullname,
		bool streamed,
		const CL_String &type = CL_String());

	/// \brief Loads an sample file from 'file', using the provider identified by 'type'.
	static CL_SoundProvider *load(
		CL_IODevice &file,
		bool streamed,
		const CL_String &type);
/// \}
};

/// \}
