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
#include "soundoutput_directsound.h"
#include "API/Core/System/error.h"
#include "API/Core/System/cl_assert.h"
#include "API/Core/System/log.h"
#include "Core/System/Win32/init_win32.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput_DirectSound construction:

CL_SoundOutput_DirectSound::CL_SoundOutput_DirectSound(int mixing_frequency) :
	CL_SoundOutput_Generic(mixing_frequency),
	directsound(0), soundbuffer(0),
	frag_size(0), buffer_size(0), bytes_per_sample(0),
	sleep_event(0), notify(0), last_write_pos(-1)
{
	HRESULT err;
	err = DirectSoundCreate(NULL, &directsound, NULL);
	
	if (FAILED(err))
	{
		// throw CL_Error("Cannot open sound device.");
		frag_size = mixing_frequency/2;
		has_sound = false;
		return;
	}

	// We need a hwnd for our directsound interface:
	HWND hwnd = NULL;
//	if (CL_Display::cards.size() > 0) hwnd = ((CL_DisplayCard_Win32Compatible *) CL_Display::cards[0])->get_hwnd();
	if (hwnd == NULL)
	{
		WNDCLASS wndclass;

		wndclass.style = 0;
		wndclass.lpfnWndProc = DefWindowProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = CL_System_Win32::hInstance;
		wndclass.hIcon = NULL;
		wndclass.hCursor = LoadCursor (NULL,IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		wndclass.lpszMenuName = "ClanSound";
		wndclass.lpszClassName = "ClanSound";

		RegisterClass(&wndclass);

		hwnd = CreateWindow(
			"ClanSound",
			"ClanSound",
			0, // WS_POPUP,
			0,
			0,
			1,
			1,
			NULL,
			NULL,
			CL_System_Win32::hInstance,
			NULL);
	}
	cl_assert(hwnd != NULL);

	err = directsound->SetCooperativeLevel(hwnd, DSSCL_NORMAL);
	cl_assert(SUCCEEDED(err));


	// Create mixing buffer.
	WAVEFORMATEX format;
	format.wFormatTag = WAVE_FORMAT_PCM; 
	format.nChannels = 2;
	format.nSamplesPerSec = mixing_frequency;
	format.wBitsPerSample = 16;
	format.nBlockAlign = format.wBitsPerSample * format.nChannels / 8; 
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign; 
	format.cbSize = 0;

	bytes_per_sample = format.nBlockAlign;

	frag_size = 3072;  //changed from 2048 to fix buffer overrun problems that happen with some drivers,
	//including an Audigy2 under Vista64 -SAR 2-13-2008
	
	int num_fragments = 8;

	DSBUFFERDESC desc;
	desc.dwSize = sizeof(DSBUFFERDESC); 
	desc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GLOBALFOCUS | DSBCAPS_GETCURRENTPOSITION2;

	desc.dwBufferBytes = frag_size * bytes_per_sample * num_fragments;
	desc.dwReserved = 0;
	desc.lpwfxFormat = &format; 

	err = directsound->CreateSoundBuffer(&desc, &soundbuffer, NULL);
	if (FAILED(err)) throw CL_Error("Cannot get primary sound buffer.");

	// Find size of buffer:

	DSBCAPS caps;
	memset(&caps, 0, sizeof(DSBCAPS));
	caps.dwSize = sizeof(DSBCAPS);
	
	err = soundbuffer->GetCaps(&caps);
	cl_assert(SUCCEEDED(err));

	buffer_size = caps.dwBufferBytes;
	cl_assert(buffer_size == frag_size * bytes_per_sample * num_fragments);
	
	// Clear buffer at beginning (good for debugging):
	DWORD size1, size2;
	void *ptr1;
	void *ptr2;
	err = soundbuffer->Lock(0, buffer_size, &ptr1, &size1, &ptr2, &size2, 0);
	cl_assert(SUCCEEDED(err));

	memset(ptr1, 0, size1);
	if (ptr2 != NULL) memset(ptr2, 0, size2);

	soundbuffer->Unlock(ptr1, size1, ptr2, size2);

	// Setup some sleeping :)
	sleep_event = CreateEvent(NULL, TRUE, FALSE, NULL);

	err = soundbuffer->QueryInterface(IID_IDirectSoundNotify, (void **) &notify);
//	cl_assert(SUCCEEDED(err));
	if (FAILED(err)) // directsoundnotify doesnt exist on nt4 with dx3. No sound avail.
	{
		// throw CL_Error("Cannot open sound device.");
		frag_size = mixing_frequency/2;
		has_sound = false;
		return;
	}

	DSBPOSITIONNOTIFY *notify_pos = new DSBPOSITIONNOTIFY[num_fragments];
	for (int i=0; i<num_fragments; i++)
	{
		notify_pos[i].dwOffset = i*frag_size*bytes_per_sample;
		notify_pos[i].hEventNotify = sleep_event;
	}
	notify->SetNotificationPositions(num_fragments, notify_pos);
	delete[] notify_pos;

	start_mixer_thread();

	err = soundbuffer->Play(0, 0, DSBPLAY_LOOPING);
	cl_assert(SUCCEEDED(err));
}
	
CL_SoundOutput_DirectSound::~CL_SoundOutput_DirectSound()
{
	if (has_sound) stop_mixer_thread();

	if (notify) notify->Release();
	if (soundbuffer) soundbuffer->Release();
	if (directsound) directsound->Release();
	if (sleep_event) CloseHandle(sleep_event);
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput_DirectSound attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput_DirectSound operations:

void CL_SoundOutput_DirectSound::silence()
{
}

int CL_SoundOutput_DirectSound::get_fragment_size()
{
	return frag_size;
}

void CL_SoundOutput_DirectSound::write_fragment(short *data)
{
	if (!has_sound) return;
	
	HRESULT err;

	DWORD play, write;
	err = soundbuffer->GetCurrentPosition(&play, &write);
	cl_assert(SUCCEEDED(err));

	int frag_bytes = frag_size*bytes_per_sample;

	int pos = (write / frag_bytes + 1) * frag_bytes;
	if (pos >= buffer_size) pos -= buffer_size;

	DWORD size1, size2;
	void *ptr1 = NULL, *ptr2 = NULL;
	err = soundbuffer->Lock(
		pos,
		frag_bytes,
		(void **) &ptr1,
		&size1,
		(void **) &ptr2,
		&size2,
		0);
	cl_assert(SUCCEEDED(err));
/*
	CL_Log::log(
		"debug", "#%1 play ptr: %2, write ptr: %3, pos: %4, size1: %5, size2: %6",
		pos / frag_bytes, play, write, pos, size1, size2);
*/
	char *_data = (char *) data;
	if (ptr1 != NULL) memcpy(ptr1, _data, size1);
	if (ptr2 != NULL) memcpy(ptr2, _data+size1, size2);

	err = soundbuffer->Unlock(ptr1, size1, ptr2, size2);
	cl_assert(SUCCEEDED(err));
}

void CL_SoundOutput_DirectSound::wait()
{
	if(!has_sound)
	{
		CL_System::sleep(100);
		return;
	}

	int pos;
	int frag_bytes = frag_size*bytes_per_sample;

	do
	{
		WaitForSingleObject(sleep_event, INFINITE);

		// Ensure that we can write a new fragment without
		// overwriting the last one. WaitForSingleObject seems
		// to return too early sometimes.
		HRESULT err;
		DWORD write;
		err = soundbuffer->GetCurrentPosition(NULL, &write);
		cl_assert(SUCCEEDED(err));

		pos = (write / frag_bytes + 1) * frag_bytes;
		if (pos >= buffer_size) pos -= buffer_size;

		ResetEvent(sleep_event);
	} while(pos == last_write_pos);

	last_write_pos = pos;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput_DirectSound implementation:
