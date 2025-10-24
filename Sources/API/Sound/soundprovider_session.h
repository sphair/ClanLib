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

#ifndef header_soundprovider_session
#define header_soundprovider_session

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
#include "soundformat.h"

class CL_SoundProvider_Session_Generic;

//: Sound provider playback session.
//- !group=Sound/Audio Mixing!
//- !header=sound.h!
//- <p>Interface used by ClanLib during playback of sound providers.</p>
class CL_API_SOUND CL_SoundProvider_Session
{
//! Construction:
public:
	CL_SoundProvider_Session();

	CL_SoundProvider_Session(const CL_SoundProvider_Session &copy);

	virtual ~CL_SoundProvider_Session();

//! Attributes:
public:
	//: Returns the number of samples in the soundbuffer.
	virtual int get_num_samples() const = 0;

	//: Returns the playback frequency of the input data.
	//return: Playback frequency.
	virtual int get_frequency() const = 0;

	//: Returns the current position in the playback stream.
	virtual int get_position() const = 0;

	//: Returns the playback sample format.
	//return: The playback Sample format.
	virtual CL_SoundFormat get_format() const = 0;

	//: Returns the number of channels filled when get_data is called.
	virtual int get_num_channels() const = 0;

//! Operations:
public:
	//: Enable/disable session looping.
	//- <p>If this function returns false (default), the clanSound mixer will manually
	//- try to simulate looping by setting the position to 0 when eof is encountered.</p>
	virtual bool set_looping(bool loop) { return false; }

	//: Returns true if no more input data is available.
	//return: True if end of input data. False otherwise.
	virtual bool eof() const = 0;
	
	//: Stops the current stream.
	virtual void stop() = 0;

	//: Start/continue playing of the stream.
	//return: True for success. False otherwise.
	virtual bool play() = 0;
		
	//: Sets the position within the current stream.
	//param pos: Position to seek to.
	//return: True for success. False otherwise.
	virtual bool set_position(int pos) = 0;
	
	//: Called when a playback session needs more sample data.
	//param data_ptr: Points to a buffer that should be filled with sample data.
	//param data_requested: Samples of data requested.
	//return: Number of samples actually filled with sample data.
	virtual int get_data(void **data_ptr, int data_requested) = 0;

//! Implementation:
private:
	CL_MutexSharedPtr<CL_SoundProvider_Session_Generic> impl;
};

#endif
