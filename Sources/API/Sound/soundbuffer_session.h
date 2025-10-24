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

//! clanSound="Audio Mixing"
//! header=sound.h

#ifndef header_soundbuffer_session
#define header_soundbuffer_session

#ifdef CL_API_DLL
#ifdef CL_SOUND_EXPORT
#define CL_API_SOUND __declspec(dllexport)
#else
#define CL_API_SOUND __declspec(dllimport)
#endif
#else
#define CL_API_SOUND
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "../Core/System/mutexsharedptr.h"

class CL_SoundCard;
class CL_SoundFilter;
class CL_SoundBuffer;
class CL_SoundBuffer_Session_Generic;

class CL_API_SOUND CL_SoundBuffer_Session
//: CL_SoundBuffer_Session provides control over a playing soundeffect.
//- !group=Sound/Audio Mixing!
//- !header=sound.h!
//- <p>Whenever a soundbuffer is played, it returns a CL_SoundBuffer_Session
//- class, which can be used to control the sound (its volume, pitch, 
//- pan, position). It can also be used to retrigger the sound
//- or to stop it.</p>
{
//! Construction:
public:
	//: Creates a soundbuffer session.
	CL_SoundBuffer_Session();

	CL_SoundBuffer_Session(const CL_SoundBuffer_Session &copy);

	virtual ~CL_SoundBuffer_Session();

//! Attributes:
public:
	//: Returns the current sample position of the playback.
	int get_position() const;
	
	//: Returns the sample position relative to the full length.
	//- <p>The value returned will be between 0 and 1, where 0
	//- means the session is at the beginning, and 1
	//- means that the soundeffect has reached the end.</p>
	float get_position_relative() const;

	//: Returns the total length (in samples) of the sound buffer played.
	//- <p>Value returned will be -1 if the length is unknown (in
	//- case of non-static soundeffects like streamed sound)</p>
	int get_length() const;

	//: Returns the frequency of the session.
	int get_frequency() const;

	//: Returns the linear relative volume of the soundeffect.
	//- <p>0 means the soundeffect is muted, 1 means the soundeffect
	//- is playing at "max" volume.</p>
	float get_volume() const;
	
	//: Returns the current pan (in a measure from -1 -> 1).
	//- <p>-1 means the soundeffect is only playing in the left speaker, 
	//- and 1 means the soundeffect is only playing in the right speaker.</p>
	float get_pan() const;

	//: Returns the current speedfactor.
	float get_speedfactor() const;

	//: Returns true if the session is playing
	bool is_playing();

//! Operations:
public:
	//: Sets the session position to 'new_pos'.
	//- <p>The new position of the session.</p>
	//return: Returns true if operation completed succesfully.
	bool set_position(int new_pos);

	//: Sets the relative position of the session.
	//- <p>Value must be between 0 and 1, where 0 sets the 
	//- session to the beginning, and 1 sets it
	//- to the end of the sound buffer.</p>
	//param new_pos: New relative position.
	//return: Returns true if operation completed succesfully.
	bool set_position_relative(float new_pos);
	
	//: Sets the frequency of the session.
	//param new_freq: New frequency of session.
	//return: Returns true, if the operation completed succesfully.
	bool set_frequency(int new_freq);
	
	//: Sets the volume of the session in a relative measure (0->1)
	//- <p>A value of 0 will effectively mute the sound (although it will
	//- still be sampled), and a value of 1 will set the volume to "max".</p>
	//param new_volume: New volume of session. 
	//return: Returns true if the operation completed succesfully.
	void set_volume(float new_volume);

	//: Sets the panning of the session played in measures from -1 -> 1
	//- <p>Setting the pan with a value of -1 will pan the session to the
	//- extreme left (left speaker only), 1 will pan the session to the
	//- extreme right (right speaker only).</p>
	//param new_pan: New pan of the session played.
	//return: Returns true if the operation completed sucecsfully.
	void set_pan(float new_pan);

	//: Sets the speedfactor of the session.
	//param new_speedfactor: New speedfactor of session.
	void set_speedfactor(float new_speedfactor);

	//: Starts playback of the session.
	void play();

	//: Stops playback of the session.
	void stop();

	//: Rewinds the session so playback will start from the beginning.
	void rewind() { set_position(0); }

	//: Determines whether this session should loop
	//param loop: true if session should loop, false otherwise
	void set_looping(bool loop);
	
	//: Adds the sound filter to the session. See CL_SoundFilter for details.
	//param filter: Sound filter to pass sound through.
	//param delete_filter: If true, the filter will be deleted when removed or the session is destroyed
	void add_filter(CL_SoundFilter *filter, bool delete_filter = false);

	//: Remove the sound filter from the session. See CL_SoundFilter for details.
	void remove_filter(CL_SoundFilter *filter);

	//: Removes all filters from the session.
	void remove_all_filters();

//! Implementation:
private:
	CL_SoundBuffer_Session(const CL_MutexSharedPtr<CL_SoundBuffer_Session_Generic> &impl);

	CL_MutexSharedPtr<CL_SoundBuffer_Session_Generic> impl;

	friend class CL_SoundBuffer;
};

#endif
