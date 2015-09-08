/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "soundoutput_win32.h"
#include "API/Core/System/exception.h"
#include "API/Core/Text/logger.h"
#include "Core/System/Win32/system_win32.h"
#include <mmreg.h>
#include "API/Core/Math/cl_math.h"

// KSDATAFORMAT_SUBTYPE_IEEE_FLOAT is not available on some old headers
#ifndef KSDATAFORMAT_SUBTYPE_IEEE_FLOAT
#ifdef _MSC_VER
struct __declspec(uuid("00000003-0000-0010-8000-00aa00389b71")) KSDATAFORMAT_SUBTYPE_IEEE_FLOAT_STRUCT;
#define KSDATAFORMAT_SUBTYPE_IEEE_FLOAT __uuidof(KSDATAFORMAT_SUBTYPE_IEEE_FLOAT_STRUCT)
#else
#include <initguid.h>
DEFINE_GUID(KSDATAFORMAT_SUBTYPE_IEEE_FLOAT,0x00000003L, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(GUID_NULL,0x00000000L, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
#endif
#endif

namespace clan
{
	SoundOutput_Win32::SoundOutput_Win32(int init_mixing_frequency, int init_mixing_latency)
		: SoundOutput_Impl(init_mixing_frequency, init_mixing_latency), audio_buffer_ready_event(INVALID_HANDLE_VALUE), is_playing(false), fragment_size(0), wait_timeout(mixing_latency * 2), write_pos(0)
	{
		try
		{
			ComPtr<IMMDeviceEnumerator> device_enumerator;
			HRESULT result = CoCreateInstance(__uuidof(MMDeviceEnumerator), 0, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)device_enumerator.output_variable());
			if (FAILED(result))
				throw Exception("Unable to create IMMDeviceEnumerator instance");

			result = device_enumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, mmdevice.output_variable());
			if (FAILED(result))
				throw Exception("IDeviceEnumerator.GetDefaultAudioEndpoint failed");

			result = mmdevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, 0, (void**)audio_client.output_variable());
			if (FAILED(result))
				throw Exception("IMMDevice.Activate failed");

			WAVEFORMATEXTENSIBLE wave_format;
			wave_format.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
			wave_format.Format.nChannels = 2;
			wave_format.Format.nBlockAlign = 2 * sizeof(float);
			wave_format.Format.wBitsPerSample = 8 * sizeof(float);
			wave_format.Format.cbSize = 22;
			wave_format.Samples.wValidBitsPerSample = wave_format.Format.wBitsPerSample;
			wave_format.dwChannelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT;
			wave_format.SubFormat = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;

			wave_format.Format.nSamplesPerSec = mixing_frequency;
			wave_format.Format.nAvgBytesPerSec = wave_format.Format.nSamplesPerSec * wave_format.Format.nBlockAlign;

			WAVEFORMATEX *closest_match = 0;
			result = audio_client->IsFormatSupported(AUDCLNT_SHAREMODE_SHARED, (WAVEFORMATEX*)&wave_format, &closest_match);
			if (FAILED(result))
				throw Exception("IAudioClient.IsFormatSupported failed");

			// We could not get the exact format we wanted. Try to use the frequency that the closest matching format is using:
			if (result == S_FALSE)
			{
				mixing_frequency = closest_match->nSamplesPerSec;
				wait_timeout = mixing_latency * 2;
				wave_format.Format.nSamplesPerSec = mixing_frequency;
				wave_format.Format.nAvgBytesPerSec = wave_format.Format.nSamplesPerSec * wave_format.Format.nBlockAlign;

				CoTaskMemFree(closest_match);
				closest_match = 0;
			}

			/*
					// For debugging what mixing format Windows is using.
					WAVEFORMATEX *device_format = 0; // Note: this points at a WAVEFORMATEXTENSIBLE if cbSize is 22
					result = audio_client->GetMixFormat(&device_format);
					if (SUCCEEDED(result))
					{
					CoTaskMemFree(device_format);
					device_format = 0;
					}
					*/

			result = audio_client->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, mixing_latency * (REFERENCE_TIME)1000, 0, (WAVEFORMATEX*)&wave_format, 0);
			if (FAILED(result))
				throw Exception("IAudioClient.Initialize failed");

			result = audio_client->GetService(__uuidof(IAudioRenderClient), (void**)audio_render_client.output_variable());
			if (FAILED(result))
				throw Exception("IAudioClient.GetService(IAudioRenderClient) failed");

			audio_buffer_ready_event = CreateEvent(0, TRUE, TRUE, 0);
			if (audio_buffer_ready_event == INVALID_HANDLE_VALUE)
				throw Exception("CreateEvent failed");

			result = audio_client->SetEventHandle(audio_buffer_ready_event);
			if (FAILED(result))
				throw Exception("IAudioClient.SetEventHandle failed");

			result = audio_client->GetBufferSize(&fragment_size);
			if (FAILED(result))
				throw Exception("IAudioClient.GetBufferSize failed");

			next_fragment = DataBuffer(sizeof(float) * 2 * fragment_size);

			start_mixer_thread();
		}
		catch (...)
		{
			if (audio_buffer_ready_event != INVALID_HANDLE_VALUE)
				CloseHandle(audio_buffer_ready_event);
			throw;
		}
	}

	SoundOutput_Win32::~SoundOutput_Win32()
	{
		stop_mixer_thread();
		if (is_playing)
			audio_client->Stop();
		audio_render_client.clear();
		audio_client.clear();
		mmdevice.clear();
		CloseHandle(audio_buffer_ready_event);
	}

	void SoundOutput_Win32::silence()
	{
	}

	int SoundOutput_Win32::get_fragment_size()
	{
		return fragment_size;
	}

	void SoundOutput_Win32::write_fragment(float *data)
	{
		memcpy(next_fragment.get_data(), data, next_fragment.get_size());
		write_pos = 0;
	}

	void SoundOutput_Win32::wait()
	{
		while (write_pos < fragment_size)
		{
			WaitForSingleObject(audio_buffer_ready_event, wait_timeout);

			UINT32 num_padding_frames = 0;
			audio_client->GetCurrentPadding(&num_padding_frames);

			UINT32 buffer_available = fragment_size - num_padding_frames;
			UINT32 buffer_needed = fragment_size - write_pos;

			if (buffer_available < buffer_needed)
				ResetEvent(audio_buffer_ready_event);

			UINT32 buffer_size = clan::min(buffer_needed, buffer_available);
			if (buffer_size > 0)
			{
				BYTE *buffer = 0;
				HRESULT result = audio_render_client->GetBuffer(buffer_size, &buffer);
				if (SUCCEEDED(result))
				{
					memcpy(buffer, next_fragment.get_data<float>() + write_pos * 2, sizeof(float) * 2 * buffer_size);
					result = audio_render_client->ReleaseBuffer(buffer_size, 0);

					if (!is_playing)
					{
						result = audio_client->Start();
						if (SUCCEEDED(result))
							is_playing = true;
					}
				}

				write_pos += buffer_size;
			}
		}
	}
}
