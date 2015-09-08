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
#include "sound_format_conversion.h"

namespace clan
{
	void SoundFormatConversion::from_ieee_float(SoundMixingBuffersData &output, void *input, SpeakerPositionMask input_speakers, unsigned int sample_count)
	{
		float *output_speakers[32];
		unsigned int num_input_speakers = find_output_speakers(output, input_speakers, output_speakers);
		clear_unused_speakers(output, input_speakers, sample_count);

		unsigned int bytes_per_sample = sizeof(float)*num_input_speakers;
		char *input_pos = reinterpret_cast<char*>(input);
		for (unsigned int i = 0; i < sample_count; i++, input_pos += bytes_per_sample)
		{
			float *input_sample = reinterpret_cast<float*>(input_pos);
			for (unsigned int j = 0; j < num_input_speakers; j++)
			{
				if (output_speakers[j])
					output_speakers[j][i] = input_sample[j];
			}
		}
	}

	void SoundFormatConversion::from_16bit_le(SoundMixingBuffersData &output, void *input, SpeakerPositionMask input_speakers, unsigned int sample_count)
	{
		float *output_speakers[32];
		unsigned int num_input_speakers = find_output_speakers(output, input_speakers, output_speakers);
		clear_unused_speakers(output, input_speakers, sample_count);

		unsigned int bytes_per_sample = sizeof(short)*num_input_speakers;
		char *input_pos = reinterpret_cast<char*>(input);
		for (unsigned int i = 0; i < sample_count; i++, input_pos += bytes_per_sample)
		{
			short *input_sample = reinterpret_cast<short*>(input_pos);
			for (unsigned int j = 0; j < num_input_speakers; j++)
			{
				if (output_speakers[j])
					output_speakers[j][i] = input_sample[j] / 32768.0f;
			}
		}
	}

	void SoundFormatConversion::from_16bit_be(SoundMixingBuffersData &output, void *input, SpeakerPositionMask input_speakers, unsigned int sample_count)
	{
		throw Exception("16 bit big endian samples not implemented");
	}

	void SoundFormatConversion::from_8bit_signed(SoundMixingBuffersData &output, void *input, SpeakerPositionMask input_speakers, unsigned int sample_count)
	{
		float *output_speakers[32];
		unsigned int num_input_speakers = find_output_speakers(output, input_speakers, output_speakers);
		clear_unused_speakers(output, input_speakers, sample_count);

		unsigned int bytes_per_sample = sizeof(char)*num_input_speakers;
		char *input_pos = reinterpret_cast<char*>(input);
		for (unsigned int i = 0; i < sample_count; i++, input_pos += bytes_per_sample)
		{
			char *input_sample = reinterpret_cast<char*>(input_pos);
			for (unsigned int j = 0; j < num_input_speakers; j++)
			{
				if (output_speakers[j])
					output_speakers[j][i] = input_sample[j] / 128.0f;
			}
		}
	}

	void SoundFormatConversion::from_8bit_unsigned(SoundMixingBuffersData &output, void *input, SpeakerPositionMask input_speakers, unsigned int sample_count)
	{
		float *output_speakers[32];
		unsigned int num_input_speakers = find_output_speakers(output, input_speakers, output_speakers);
		clear_unused_speakers(output, input_speakers, sample_count);

		unsigned int bytes_per_sample = sizeof(char)*num_input_speakers;
		char *input_pos = reinterpret_cast<char*>(input);
		for (unsigned int i = 0; i < sample_count; i++, input_pos += bytes_per_sample)
		{
			unsigned char *input_sample = reinterpret_cast<unsigned char*>(input_pos);
			for (unsigned int j = 0; j < num_input_speakers; j++)
			{
				if (output_speakers[j])
					output_speakers[j][i] = (input_sample[j] / 255.0f) - 0.5f;
			}
		}
	}

	void SoundFormatConversion::to_ieee_float(void **output, SpeakerPositionMask output_speakers, const SoundMixingBuffersData &input, unsigned int sample_count)
	{
		float * input_speakers[32];
		unsigned int num_output_speakers = find_input_speakers(input, output_speakers, input_speakers);

		unsigned int bytes_per_sample = sizeof(char)*num_output_speakers;
		char *output_pos = reinterpret_cast<char*>(output);
		for (unsigned int i = 0; i < sample_count; i++, output_pos += bytes_per_sample)
		{
			unsigned char *output_sample = reinterpret_cast<unsigned char*>(output_pos);
			for (unsigned int j = 0; j < num_output_speakers; j++)
			{
				if (input_speakers[j])
				{
					float v = (input_speakers[j][i] + 0.5f)*255.0f;
					if (v < 0.0f)
						v = 0.0f;
					else if (v > 255.0f)
						v = 255.0f;
					output_sample[j] = (unsigned char)v;
				}
			}
		}
	}

	void SoundFormatConversion::to_16bit_le(void **output, SpeakerPositionMask output_speakers, const SoundMixingBuffersData &input, unsigned int sample_count)
	{
		float *input_speakers[32];
		unsigned int num_output_speakers = find_input_speakers(input, output_speakers, input_speakers);

		unsigned int bytes_per_sample = sizeof(char)*num_output_speakers;
		char *output_pos = reinterpret_cast<char*>(output);
		for (unsigned int i = 0; i < sample_count; i++, output_pos += bytes_per_sample)
		{
			short *output_sample = reinterpret_cast<short*>(output_pos);
			for (unsigned int j = 0; j < num_output_speakers; j++)
			{
				if (input_speakers[j])
				{
					float v = input_speakers[j][i] * 32768.0f;
					if (v < -32767.0f)
						v = -32767.0f;
					else if (v > 32768.0f)
						v = 32768.0f;
					output_sample[j] = (short)v;
				}
			}
		}
	}

	void SoundFormatConversion::to_16bit_be(void **output, SpeakerPositionMask output_speakers, const SoundMixingBuffersData &input, unsigned int sample_count)
	{
		throw Exception("16 bit big endian samples not implemented");
	}

	void SoundFormatConversion::to_8bit_signed(void **output, SpeakerPositionMask output_speakers, const SoundMixingBuffersData &input, unsigned int sample_count)
	{
		float *input_speakers[32];
		unsigned int num_output_speakers = find_input_speakers(input, output_speakers, input_speakers);

		unsigned int bytes_per_sample = sizeof(char)*num_output_speakers;
		char *output_pos = reinterpret_cast<char*>(output);
		for (unsigned int i = 0; i < sample_count; i++, output_pos += bytes_per_sample)
		{
			unsigned char *output_sample = reinterpret_cast<unsigned char*>(output_pos);
			for (unsigned int j = 0; j < num_output_speakers; j++)
			{
				if (input_speakers[j])
				{
					float v = input_speakers[j][i] * 128.0f;
					if (v < -127.0f)
						v = -127.0f;
					else if (v > 128.0f)
						v = 128.0f;
					output_sample[j] = (char)v;
				}
			}
		}
	}

	void SoundFormatConversion::to_8bit_unsigned(void **output, SpeakerPositionMask output_speakers, const SoundMixingBuffersData &input, unsigned int sample_count)
	{
		float *input_speakers[32];
		unsigned int num_output_speakers = find_input_speakers(input, output_speakers, input_speakers);

		unsigned int bytes_per_sample = sizeof(char)*num_output_speakers;
		char *output_pos = reinterpret_cast<char*>(output);
		for (unsigned int i = 0; i < sample_count; i++, output_pos += bytes_per_sample)
		{
			unsigned char *output_sample = reinterpret_cast<unsigned char*>(output_pos);
			for (unsigned int j = 0; j < num_output_speakers; j++)
			{
				if (input_speakers[j])
				{
					float v = (input_speakers[j][i] + 0.5f)*255.0f;
					if (v < 0.0f)
						v = 0.0f;
					else if (v > 255.0f)
						v = 255.0f;
					output_sample[j] = (unsigned char)v;
				}
			}
		}
	}

	unsigned int SoundFormatConversion::find_output_speakers(SoundMixingBuffersData &output, SpeakerPositionMask input_speakers, float **output_speakers)
	{
		unsigned int num_output_speakers = 0;
		for (unsigned int i = 0; input_speakers >> i; i++)
		{
			if ((input_speakers >> i) & 1)
				output_speakers[num_output_speakers++] = output.channels[i];
		}
		return num_output_speakers;
	}

	unsigned int SoundFormatConversion::find_input_speakers(const SoundMixingBuffersData &input, SpeakerPositionMask output_speakers, float **input_speakers)
	{
		unsigned int num_input_speakers = 0;
		for (unsigned int i = 0; output_speakers >> i; i++)
		{
			if ((output_speakers >> i) & 1)
				input_speakers[num_input_speakers++] = input.channels[i];
		}
		return num_input_speakers;
	}

	void SoundFormatConversion::clear_unused_speakers(SoundMixingBuffersData &output, SpeakerPositionMask input_speakers, unsigned int sample_count)
	{
		for (unsigned int i = 0; input_speakers >> i; i++)
		{
			if (((input_speakers >> i) & 1) == 0 && output.channels[i])
			{
				for (unsigned int j = 0; j < sample_count; j++)
					output.channels[i][j] = 0.0f;
			}
		}
	}
}
