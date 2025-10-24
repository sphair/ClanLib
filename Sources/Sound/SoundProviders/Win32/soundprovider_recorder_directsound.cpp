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

#include "Sound/precomp.h"
#include "soundprovider_recorder_directsound.h"
#include "API/Core/System/error.h"
#include "API/Core/System/cl_assert.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DirectSoundRecorder_Session construction:

CL_SoundProvider_Recorder_DirectSound_Session::CL_SoundProvider_Recorder_DirectSound_Session(int frequency)
: frequency(frequency), position(0), dsoundcapture(0), buffer(0), buffer_size(0)
{
	HRESULT result;
	
	// Create sound capture:
	result = DirectSoundCaptureCreate8(0, &dsoundcapture, 0);
	if (FAILED(result)) throw CL_Error("Failed to create DirectSound capture!");

	DSCCAPS caps;
	memset(&caps, 0, sizeof(DSCCAPS));
	caps.dwSize = sizeof(DSCCAPS);
	result = dsoundcapture->GetCaps(&caps);
	if (FAILED(result))
	{
		dsoundcapture->Release();
		throw CL_Error("Failed to query for DirectSound capture capabilities!");
	}

	// Setup sound format and create capture buffer:
	WAVEFORMATEX format;
	format.wFormatTag = WAVE_FORMAT_PCM; 
	format.nChannels = 1;
	format.nSamplesPerSec = frequency;
	format.wBitsPerSample = 16;
	format.nBlockAlign = format.wBitsPerSample * format.nChannels / 8; 
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign; 
	format.cbSize = 0;

	bytes_per_sample = format.nBlockAlign;
	frag_size = 2500; // 0.12 sec latency
	num_fragments = 8;

	DSCBUFFERDESC bufferdesc;
	memset(&bufferdesc, 0, sizeof(DSCBUFFERDESC));
	bufferdesc.dwSize = sizeof(DSCBUFFERDESC);
	bufferdesc.dwBufferBytes = bytes_per_sample * frag_size * num_fragments;
	bufferdesc.lpwfxFormat = &format;

	result = dsoundcapture->CreateCaptureBuffer(&bufferdesc, &capturebuffer, 0);
	if (FAILED(result))
	{
		dsoundcapture->Release();
		throw CL_Error("Failed to create capture buffer!");
	}

	// Setup notify events for each record fragment:
	result = capturebuffer->QueryInterface(IID_IDirectSoundNotify8, (void **) &notify);
	if (FAILED(result))
	{
		capturebuffer->Release();
		dsoundcapture->Release();
		throw CL_Error("Failed to create notify events for capture buffer!");
	}

	notify_event = CreateEvent(NULL, TRUE, FALSE, NULL);

	DSBPOSITIONNOTIFY *notify_pos = new DSBPOSITIONNOTIFY[num_fragments];
	for (int i=0; i<num_fragments; i++)
	{
		notify_pos[i].dwOffset = i*frag_size*bytes_per_sample;
		notify_pos[i].hEventNotify = notify_event;
	}
	result = notify->SetNotificationPositions(num_fragments, notify_pos);
	if (FAILED(result))
	{
		delete[] notify_pos;
		notify->Release();
		capturebuffer->Release();
		dsoundcapture->Release();
		throw CL_Error("Failed to create notification positions for capture buffer!");
	}
	delete[] notify_pos;

	// Start worker thread:
	stop_thread = false;
	thread = CL_Thread(this);
	thread.start();
}

CL_SoundProvider_Recorder_DirectSound_Session::~CL_SoundProvider_Recorder_DirectSound_Session()
{
	// Shut down worker thread.
	stop_thread = true;
	SetEvent(notify_event);
	thread.wait();

	// Clean up.
	delete[] buffer;
	notify->Release();
	capturebuffer->Release();
	dsoundcapture->Release();
	CloseHandle(notify_event);
}

/////////////////////////////////////////////////////////////////////////////
// CL_DirectSoundRecorder_Session attributes:

int CL_SoundProvider_Recorder_DirectSound_Session::get_num_samples() const
{
	return 0;
}

int CL_SoundProvider_Recorder_DirectSound_Session::get_frequency() const
{
	return frequency;
}

CL_SoundFormat CL_SoundProvider_Recorder_DirectSound_Session::get_format() const
{
	return sf_16bit_signed;
}

int CL_SoundProvider_Recorder_DirectSound_Session::get_num_channels() const
{
	return 1;
}

int CL_SoundProvider_Recorder_DirectSound_Session::get_position() const
{
	return position;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Recorder_DirectSound_Session operations:

bool CL_SoundProvider_Recorder_DirectSound_Session::eof() const
{
	return false;
}

void CL_SoundProvider_Recorder_DirectSound_Session::stop()
{
	capturebuffer->Stop();
}

bool CL_SoundProvider_Recorder_DirectSound_Session::play()
{
	HRESULT result = capturebuffer->Start(DSCBSTART_LOOPING);
	return SUCCEEDED(result);
}

bool CL_SoundProvider_Recorder_DirectSound_Session::set_position(int pos)
{
	return false;
}

#define cl_min(a,b) ((a < b) ? a : b)

int CL_SoundProvider_Recorder_DirectSound_Session::get_data(void **data_ptr, int data_requested)
{
	CL_MutexSection mutex_section(&mutex);
	int send_size = cl_min(data_requested*2, buffer_size);
	if (send_size > 0)
	{
		memcpy(data_ptr[0], buffer, send_size);
		buffer_size -= send_size;
		if (buffer_size == 0)
		{
			delete[] buffer;
			buffer = 0;
		}
		else
		{
			char *old_buffer = buffer;
			buffer = new char[buffer_size];
			memcpy(buffer, old_buffer+send_size, buffer_size);
			delete[] old_buffer;
		}
	}
	position += send_size/2;
	return send_size/2;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Recorder_DirectSound_Session implementation:

void CL_SoundProvider_Recorder_DirectSound_Session::run()
{
	HRESULT result;
	DWORD cur_pos = 0;
	DWORD capturebuffer_size = frag_size * num_fragments * 4;

	while (true)
	{
		// Wait for incoming data or quit request:
		WaitForSingleObject(notify_event, INFINITE);
		ResetEvent(notify_event);
		if (stop_thread) break;

		CL_MutexSection mutex_section(&mutex);

		// Figure out what to copy:
		DWORD capture_pos, read_pos;
		result = capturebuffer->GetCurrentPosition(&capture_pos, &read_pos);
		cl_assert(SUCCEEDED(result));

		DWORD avail_bytes = read_pos - cur_pos;
		if (read_pos <= cur_pos) avail_bytes = capturebuffer_size - cur_pos + read_pos;

		LPVOID buf1 = 0;
		LPVOID buf2 = 0;
		DWORD size1 = 0;
		DWORD size2 = 0;

		result = capturebuffer->Lock(
			cur_pos, avail_bytes, &buf1, &size1, &buf2, &size2, 0);
		cl_assert(SUCCEEDED(result));

		// Append new data to buffer:
		char *old_buffer = buffer;
		buffer = new char[buffer_size + avail_bytes];
		if (old_buffer) memcpy(buffer, old_buffer, buffer_size);
		delete[] old_buffer;
		memcpy(buffer+buffer_size, buf1, size1);
		if (buf2) memcpy(buffer+buffer_size+size1, buf2, size2);
		buffer_size += avail_bytes;

		result = capturebuffer->Unlock(buf1, size1, buf2, size2);
		cl_assert(SUCCEEDED(result));

		cur_pos += avail_bytes;
		if (cur_pos >= capturebuffer_size) cur_pos -= capturebuffer_size;
	}
}
