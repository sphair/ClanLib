/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#include "Sound/precomp.h"
#include "soundoutput_macosx.h"
#include "API/Core/System/exception.h"
#include "API/Core/System/system.h"

namespace clan
{

inline UInt32 CalculateLPCMFlags (
                                  UInt32 inValidBitsPerChannel,
                                  UInt32 inTotalBitsPerChannel,
                                  bool inIsFloat,
                                  bool inIsBigEndian,
                                  bool inIsNonInterleaved = false
                                  ) {
    return
    (inIsFloat ? kAudioFormatFlagIsFloat : kAudioFormatFlagIsSignedInteger) |
    (inIsBigEndian ? ((UInt32)kAudioFormatFlagIsBigEndian) : 0)             |
    ((!inIsFloat && (inValidBitsPerChannel == inTotalBitsPerChannel)) ?
     kAudioFormatFlagIsPacked : kAudioFormatFlagIsAlignedHigh)           |
    (inIsNonInterleaved ? ((UInt32)kAudioFormatFlagIsNonInterleaved) : 0);
}


SoundOutput_MacOSX::SoundOutput_MacOSX(int frequency, int latency)
: SoundOutput_Impl(frequency, latency), frequency(frequency), latency(latency), fragment_size(0), next_fragment(0), read_cursor(0), fragments_available(0)
{
    fragment_size = frequency * latency / fragment_buffer_count / 1000;
    fragment_size = (fragment_size + 3) & ~3; // Force to be a multiple of 4
    fragments_available = fragment_buffer_count;
    fragment_data = DataBuffer(fragment_size * sizeof(short) * 2 * fragment_buffer_count);

    audio_format = {0};
    start_mixer_thread();
}

SoundOutput_MacOSX::~SoundOutput_MacOSX()
{
	stop_mixer_thread();
}

void SoundOutput_MacOSX::mixer_thread_starting()
{
    audio_format.mSampleRate = frequency;
    audio_format.mFormatID = kAudioFormatLinearPCM;
    audio_format.mFormatFlags = CalculateLPCMFlags(8*sizeof(short),8*sizeof(short),false,false,false);
    audio_format.mBytesPerPacket = 2 * sizeof(short);
    audio_format.mFramesPerPacket = 1;
    audio_format.mBytesPerFrame = 2 * sizeof(short);
    audio_format.mChannelsPerFrame = 2;
    audio_format.mBitsPerChannel = 8 * sizeof (short);
    audio_format.mReserved = 0;

    OSStatus result = AudioQueueNewOutput(&audio_format, &SoundOutput_MacOSX::static_audio_queue_callback, this, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode, 0, &audio_queue);
    if (result != 0)
        throw Exception("AudioQueueNewOutput failed");

    for (int i = 0; i < fragment_buffer_count; i++)
    {
        result = AudioQueueAllocateBuffer(audio_queue, fragment_size * sizeof(short) * 2, &audio_buffers[i]);
        if (result != 0)
            throw Exception("AudioQueueAllocateBuffer failed");
        audio_queue_callback(audio_queue, audio_buffers[i]);
    }
    result = AudioQueuePrime(audio_queue,0,NULL);
    if (result != 0)
        throw Exception("AudioQueuePrime failed");
    result = AudioQueueStart(audio_queue, 0);
    if (result != 0)
        throw Exception("AudioQueueStart failed");
}

void SoundOutput_MacOSX::mixer_thread_stopping()
{
    AudioQueueStop(audio_queue, true);
    AudioQueueDispose(audio_queue, true);
}

void SoundOutput_MacOSX::silence()
{
}

int SoundOutput_MacOSX::get_fragment_size()
{
    return fragment_size;
}

void SoundOutput_MacOSX::write_fragment(float *dataf)
{
    if (fragments_available == 0)
        wait();

    short *databuf = reinterpret_cast<short*>(fragment_data.get_data()) + next_fragment * fragment_size * 2;
    for (int i = 0; i < fragment_size * 2; i++)
    {
        databuf[i] = (short)(dataf[i]*32767);
    }

    next_fragment++;
    if (next_fragment == fragment_buffer_count)
        next_fragment = 0;
    fragments_available--;
}

void SoundOutput_MacOSX::wait()
{
    while (fragments_available == 0)
    {
        CFRunLoopRunInMode(kCFRunLoopDefaultMode, 1, true);
    }
}

void SoundOutput_MacOSX::audio_queue_callback(AudioQueueRef queue, AudioQueueBufferRef buffer)
{
    if (fragments_available != fragment_buffer_count)
    {
        short *databuf = reinterpret_cast<short*>(fragment_data.get_data()) + read_cursor * fragment_size * 2;
        memcpy(buffer->mAudioData, databuf, fragment_size * sizeof(short) * 2);
        buffer->mAudioDataByteSize = fragment_size * sizeof(short) * 2;
        fragments_available++;
        read_cursor++;
        if (read_cursor == fragment_buffer_count)
            read_cursor = 0;
    }
    else
    {
        memset(buffer->mAudioData, 0, fragment_size * sizeof(short) * 2);
        buffer->mAudioDataByteSize = fragment_size * sizeof(short) * 2;
    }

    OSStatus result = AudioQueueEnqueueBuffer(queue, buffer, 0, 0);
    if (result != 0)
        throw Exception("AudioQueueEnqueueBuffer failed");
}

void SoundOutput_MacOSX::static_audio_queue_callback(void *userdata, AudioQueueRef queue, AudioQueueBufferRef buffer)
{
    SoundOutput_MacOSX *self = reinterpret_cast<SoundOutput_MacOSX *>(userdata);
    self->audio_queue_callback(queue, buffer);
}

}
