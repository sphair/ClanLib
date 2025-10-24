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

#ifndef header_soundoutput_macosx
#define header_soundoutput_macosx

#if _MSC_VER > 1000
#pragma once
#endif

#include "../soundoutput_generic.h"

#include <CoreServices/CoreServices.h>
#include <CoreAudio/AudioHardware.h>

class CL_SoundOutput_MacOSX : public CL_SoundOutput_Generic
{
//! Construction:
public:
	CL_SoundOutput_MacOSX(int mixing_frequency);
	
	~CL_SoundOutput_MacOSX();

//! Attributes:
public:

//! Operations:
public:
	//: Called when we have no samples to play - and wants to tell the soundcard
	//: about this possible event.
	virtual void silence();

	//: Returns the buffer size used by device (returned as num [stereo] samples).
	virtual int get_fragment_size();

	//: Writes a fragment to the soundcard.
	virtual void write_fragment(short *data);

	//: Waits until output source isn't full anymore.
	virtual void wait();

//! Implementation:
private:
	// Callback handler for CoreAudio HAL.
	static OSStatus audio_handler(
		AudioDeviceID in_device,
		const AudioTimeStamp *in_now,
		const AudioBufferList *in_input_data,
		const AudioTimeStamp *in_input_time,
		AudioBufferList *out_output_data,
		const AudioTimeStamp *in_output_time,
		void *user_data);

	// Device ID for the output we are using.
	AudioDeviceID device;

	// Buffer size returned by kAudioDevicePropertyBufferSize
	UInt32 device_buffer_size;

	// Format description for the device.
	AudioStreamBasicDescription device_format;
	
	// True if CoreAudio HAL successfully was put to play audio.
	bool playing;
	
	// Ring buffer for audio fragments
	std::vector<float*> fragment_buffer;
	unsigned int fragment_insert_position;
	unsigned int fragment_play_position;
};

#endif
