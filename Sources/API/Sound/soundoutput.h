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

/// \addtogroup clanSound_Audio_Mixing clanSound Audio Mixing
/// \{

#pragma once

#include "api_sound.h"
#include "../Core/Text/string_types.h"
#include "../Core/System/sharedptr.h"
#include "../Core/System/weakptr.h"

class CL_SoundFilter;
class CL_SoundBuffer;
class CL_SoundOutput_Description;
class CL_SoundOutput_Impl;

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
	/// \brief Constructs a null instance
	CL_SoundOutput();

	/// \brief Constructs a SoundOutput
	///
	/// \param mixing_frequency = value
	/// \param latency = value
	CL_SoundOutput(int mixing_frequency, int latency = 50);

	/// \brief Constructs a SoundOutput
	///
	/// \param desc = Sound Output_ Description
	CL_SoundOutput(const CL_SoundOutput_Description &desc);

	virtual ~CL_SoundOutput();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

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
	/// \brief Stops all sample playbacks on the sound output.
	void stop_all();

	/// \brief Sets the main/mixer volume on the sound output.
	void set_global_volume(float volume);

	/// \brief Sets the main panning position on the sound output.
	void set_global_pan(float pan);

	/// \brief Adds the sound filter to the sound output.
	///
	/// \param filter Sound filter to pass sound through.
	void add_filter(CL_SoundFilter &filter);

	/// \brief Remove the sound filter from the session.
	void remove_filter(CL_SoundFilter &filter);

/// \}
/// \name Implementation
/// \{

private:

	/// \brief Constructs a SoundOutput
	///
	/// \param impl = Weak Ptr
	CL_SoundOutput(const CL_WeakPtr<CL_SoundOutput_Impl> impl);

	CL_SharedPtr<CL_SoundOutput_Impl> impl;

	friend class CL_SoundBuffer;
	friend class CL_Sound;
	friend class CL_SoundBuffer_Session;
/// \}
};

/// \}
