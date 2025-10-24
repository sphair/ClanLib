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

#ifndef header_soundoutput
#define header_soundoutput

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

#include <string>

class CL_SoundFilter;
class CL_SoundBuffer;
class CL_SoundOutput_Description;
class CL_SoundOutput_Generic;

//: SoundOutput interface in ClanLib.
//- !group=Sound/Audio Mixing!
//- !header=sound.h!
//- <p>CL_SoundOutput is the interface to a sound output device. It is used to
//- control the main mixer volume and other global settings.</p>
class CL_API_SOUND CL_SoundOutput
{
//! Construction:
public:
	//: Constructs a sound output object.
	CL_SoundOutput();

	CL_SoundOutput(int mixing_frequency);

	CL_SoundOutput(const CL_SoundOutput_Description &desc);

	CL_SoundOutput(const CL_SoundOutput &copy);

	virtual ~CL_SoundOutput();

//! Attributes:
public:
	//: True if the sound system has been properly initialized
	bool has_sound() const;
	
	//: Name of the output device.
	const std::string &get_name() const;

	//: Returns the mixing frequency for the sound output device.
	int get_mixing_frequency() const;

	//: Returns the main volume of the sound output.
	float get_global_volume() const;

	//: Returns the main panning position of the sound output.
	float get_global_pan() const;

//! Operations:
public:
	//: Copy assignment operator.
	CL_SoundOutput &operator =(const CL_SoundOutput &copy);

	//: Stops all sample playbacks on the sound output.
	void stop_all();
	
	//: Sets the main/mixer volume on the sound output.
	void set_global_volume(float volume);

	//: Sets the main panning position on the sound output.
	void set_global_pan(float pan);

	//: Adds the sound filter to the sound output.
	//param filter: Sound filter to pass sound through.
	//param delete_filter: If true, the filter will be deleted when the sound output is destroyed.
	void add_filter(CL_SoundFilter *filter, bool delete_filter = false);

	//: Remove the sound filter from the session.
	void remove_filter(CL_SoundFilter *filter);

//! Implementation:
private:
	CL_SoundOutput_Generic *impl;

	friend class CL_SoundBuffer;
};

#endif
