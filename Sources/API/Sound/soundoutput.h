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
*/

/// \addtogroup clanSound_Audio_Mixing clanSound Audio Mixing
/// \{


#pragma once


#if _MSC_VER > 1000
#pragma once
#endif

#include "api_sound.h"
#include "../Core/Text/string_types.h"

class CL_SoundFilter;
class CL_SoundBuffer;
class CL_SoundOutput_Description;
class CL_SoundOutput_Generic;

/// \brief SoundOutput interface in ClanLib.
///
///   <p>CL_SoundOutput is the interface to a sound output device. It is used to
///    control the main mixer volume and other global settings.</p>
/// \xmlonly !group=Sound/Audio Mixing! !header=sound.h! \endxmlonly
class CL_API_SOUND CL_SoundOutput
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a sound output object.
	CL_SoundOutput();

	CL_SoundOutput(int mixing_frequency, int latency = 50);

	CL_SoundOutput(const CL_SoundOutput_Description &desc);

	CL_SoundOutput(const CL_SoundOutput &copy);

	virtual ~CL_SoundOutput();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Name of the output device.
	const CL_String8 &get_name() const;

	/// \brief Returns the mixing frequency for the sound output device.
	int get_mixing_frequency() const;

	/// \brief Returns the mixing latency in milliseconds.
	int get_mixing_latency() const;

	/// \brief Returns the main volume of the sound output.
	float get_global_volume() const;

	/// \brief Returns the main panning position of the sound output.
	float get_global_pan() const;


/// \}
/// \name Operations
/// \{

public:
	/// \brief Copy assignment operator.
	CL_SoundOutput &operator =(const CL_SoundOutput &copy);

	/// \brief Stops all sample playbacks on the sound output.
	void stop_all();

	/// \brief Sets the main/mixer volume on the sound output.
	void set_global_volume(float volume);

	/// \brief Sets the main panning position on the sound output.
	void set_global_pan(float pan);

	/// \brief Adds the sound filter to the sound output.
	///
	/// \param filter Sound filter to pass sound through.
	/// \param delete_filter If true, the filter will be deleted when the sound output is destroyed.
	void add_filter(CL_SoundFilter *filter, bool delete_filter = false);

	/// \brief Remove the sound filter from the session.
	void remove_filter(CL_SoundFilter *filter);


/// \}
/// \name Implementation
/// \{

private:
	CL_SoundOutput_Generic *impl;

	friend class CL_SoundBuffer;
/// \}
};


/// \}
