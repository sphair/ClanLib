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
**    Sean Heber - implemented ring buffer
**    (if your name is missing here, please add it)
*/

#include "Sound/precomp.h"
#include "soundoutput_macosx.h"
#include "API/Core/System/error.h"
#include "API/Core/System/cl_assert.h"
#include "API/Core/System/system.h"
#include "API/Core/System/log.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput_MacOSX construction:

CL_SoundOutput_MacOSX::CL_SoundOutput_MacOSX(int frequency) :
	CL_SoundOutput_Generic(frequency), playing(false)
{
	UInt32 size;
	OSStatus result;
	
	// Find default output device:
	device = kAudioDeviceUnknown;
	size = sizeof(AudioDeviceID);
	result = AudioHardwareGetProperty(kAudioHardwarePropertyDefaultOutputDevice, &size, &device);
	if (result != noErr)
		return;

	// Get buffer size for device:
	device_buffer_size = 0;
	size = sizeof(UInt32);
	result = AudioDeviceGetProperty(device, 0, false, kAudioDevicePropertyBufferSize, &size, &device_buffer_size);
	if (result != noErr)
		return;

	// Get device format description:
	size = sizeof(AudioStreamBasicDescription);
	memset(&device_format, 0, size);
	result = AudioDeviceGetProperty(device, 0, false, kAudioDevicePropertyStreamFormat, &size, &device_format);
	if (result != noErr)
		return;
/*
	printf("sampleRate %g\n", device_format.mSampleRate);
	printf("mFormatFlags %08X\n", device_format.mFormatFlags);
	printf("mBytesPerPacket %d\n", device_format.mBytesPerPacket);
	printf("mFramesPerPacket %d\n", device_format.mFramesPerPacket);
	printf("mChannelsPerFrame %d\n", device_format.mChannelsPerFrame);
	printf("mBytesPerFrame %d\n", device_format.mBytesPerFrame);
	printf("mBitsPerChannel %d\n", device_format.mBitsPerChannel);
*/
	if (device_format.mFormatID != kAudioFormatLinearPCM)
		return;

	mixing_frequency = int(device_format.mSampleRate);

	// configure the fragment ring buffer
	fragment_buffer.resize(10);		// <--- this number affects audio lag (must be >= 2!)
	fragment_insert_position = 0;
	fragment_play_position = 1;
	int frag_size = get_fragment_size();
	for (unsigned int i=0; i<fragment_buffer.size(); i++ )
	{
		fragment_buffer[i] = new float[frag_size*2];
		bzero(fragment_buffer[i],sizeof(float)*frag_size*2);
	}

	result = AudioDeviceAddIOProc(device, &CL_SoundOutput_MacOSX::audio_handler, this);
	if (result != noErr)
		return;
	
	result = AudioDeviceStart(device, &CL_SoundOutput_MacOSX::audio_handler);
	if (result != noErr)
		return;

	playing = true;
	start_mixer_thread();
}
	
CL_SoundOutput_MacOSX::~CL_SoundOutput_MacOSX()
{
	stop_mixer_thread();
	if (playing)
	{
		AudioDeviceStop(device, &CL_SoundOutput_MacOSX::audio_handler);
		AudioDeviceRemoveIOProc(device, &CL_SoundOutput_MacOSX::audio_handler);
		playing = false;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput_MacOSX attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput_MacOSX operations:

void CL_SoundOutput_MacOSX::silence()
{
}

int CL_SoundOutput_MacOSX::get_fragment_size()
{
	int frag_size = device_buffer_size / device_format.mBytesPerFrame;
	return frag_size;
}

void CL_SoundOutput_MacOSX::write_fragment(short *data)
{
	int frag_size = get_fragment_size();
	float *frag_data = fragment_buffer[fragment_insert_position];

	for (int i=0; i<frag_size*2; i++)
		frag_data[i] = data[i]/32768.0f;

	fragment_insert_position++;
	if (fragment_insert_position == fragment_buffer.size() )
		fragment_insert_position = 0;
}

void CL_SoundOutput_MacOSX::wait()
{
	while (true)
	{
		// note: this equality is perhaps dangerous due to threading issue
		// see note down in audio_handler below for why.
		bool buffer_is_full = fragment_insert_position == fragment_play_position;
		if (!buffer_is_full) break;
		CL_System::sleep(10);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput_MacOSX implementation:

OSStatus CL_SoundOutput_MacOSX::audio_handler(
	AudioDeviceID in_device,
	const AudioTimeStamp *in_now,
	const AudioBufferList *in_input_data,
	const AudioTimeStamp *in_input_time,
	AudioBufferList *out_output_data,
	const AudioTimeStamp *in_output_time,
	void *user_data)
{
	CL_SoundOutput_MacOSX *self = (CL_SoundOutput_MacOSX *) user_data;
	float *data = (float *) out_output_data->mBuffers[0].mData;
	
	bcopy(self->fragment_buffer[self->fragment_play_position], data, sizeof(float)*self->get_fragment_size()*2);

	// this is sort of dangerous in that there's no mutex here and the value is compared
	// in another thread above (see above), but I found a mutex introduces a slight lag
	// and sometimes that appears to be problematic (mostly when cpu is pegged) - I guess
	// we shall see if it is an issue or not...
	self->fragment_play_position++;
	if (self->fragment_play_position == self->fragment_buffer.size())
		self->fragment_play_position = 0;

	return noErr;
}
