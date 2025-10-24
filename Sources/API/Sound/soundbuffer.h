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
#include "../Core/System/sharedptr.h"
#include "soundbuffer_session.h"

class CL_ResourceManager;
class CL_SoundOutput;
class CL_SoundProvider;
class CL_SoundBuffer_Session;
class CL_SoundFilter;
class CL_SoundBuffer_Impl;
class CL_IODevice;
class CL_VirtualDirectory;

/// \brief Sample interface in ClanLib.
///
///    <p>The CL_SoundBuffer class represents a sample in ClanLib. It can
///    either be static or streamed. The soundbuffer gets its sample data from
///    a soundprovider, that is passed during construction.</p>
/// \xmlonly !group=Sound/Audio Mixing! !header=sound.h! \endxmlonly
class CL_API_SOUND CL_SoundBuffer
{
/// \name Construction
/// \{

public:
	/// \brief Construct a null instance
	CL_SoundBuffer();

	/// \brief Construct sound buffer.
	/** <p>A sound buffer can be constructed either as static or
	    streamed. If the sound buffer is loaded from resources, the
	    buffer type is determined by the resource option 'stream'
	    associated with the resource.</p>
	    -
	    <p>CL_SoundBuffer's internals are reference counted, so the copy
	    constructor will create a new soundbuffer object which shares
	    the same buffer as the original one. This means that if the copy
	    is modified, the original is affected as well.</p>
	    -
	    <p>If <i>delete_provider</i> is true, the provider will be
	    deleted when the soundbuffer is deleted.</p>*/

	CL_SoundBuffer(
		const CL_String &res_id,
		CL_ResourceManager *manager);

	CL_SoundBuffer(
		CL_SoundProvider *provider);

	CL_SoundBuffer(
		const CL_String &fullname,
		bool streamed = false,
		const CL_String &format = "");

	CL_SoundBuffer(
		const CL_String &filename,
		bool streamed,
		const CL_VirtualDirectory &directory,
		const CL_String &type = "");

	CL_SoundBuffer(
		CL_IODevice &file,
		bool streamed,
		const CL_String &type);

	virtual ~CL_SoundBuffer();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the sound provider to be used for playback.
	CL_SoundProvider *get_provider() const;

	/// \brief Returns the start/default volume used when the buffer is played.
	float get_volume() const;

	/// \brief Returns the default panning position when the buffer is played.
	float get_pan() const;

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Sets the volume of the sound buffer in a relative measure (0->1)
	/** <p>A value of 0 will effectively mute the sound (although it will
	    still be sampled), and a value of 1 will set the volume to "max".</p>
	    \param new_volume New volume of sound buffer. */
	void set_volume(float new_volume);

	/// \brief Sets the panning of the sound buffer played in measures from -1 -> 1
	/** <p>Setting the pan with a value of -1 will pan the sound buffer to the
	    extreme left (left speaker only), 1 will pan the sound buffer to the
	    extreme right (right speaker only).</p>
	    \param new_pan New pan of the sound buffer played.*/
	void set_pan(float new_pan);

	/// \brief Adds the sound filter to the sound buffer.
	///
	/// \param filter Sound filter to pass sound through.
	void add_filter(CL_SoundFilter &filter);

	/// \brief Remove the sound filter from the sound buffer.
	void remove_filter(CL_SoundFilter &filter);

	/// \brief Plays the soundbuffer on the specified soundcard.
	///
	/// \param looping looping
	/// \param output Sound output to be used - NULL means use the current selected sound output (CL_Sound::get_selected_output().
	///
	/// \return The playback session.
	CL_SoundBuffer_Session play(bool looping = false, CL_SoundOutput *output = 0);

	/// \brief Prepares the soundbuffer for playback on the specified soundcard.
	///
	/// \param output Sound output to be used - NULL means use the current selected sound output (CL_Sound::get_selected_output().
	///
	/// \return The playback session.
	CL_SoundBuffer_Session prepare(bool looping = false, CL_SoundOutput *output = 0);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_SoundBuffer_Impl> impl;
/// \}
};

/// \}
