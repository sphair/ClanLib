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

#pragma once

#include "speaker_position.h"
#include "sound_mixing_buffers_data.h"

namespace clan
{
	struct SoundFormatConversion
	{
		static void from_ieee_float(SoundMixingBuffersData &output, void *input, SpeakerPositionMask input_speakers, unsigned int sample_count);
		static void from_16bit_le(SoundMixingBuffersData &output, void *input, SpeakerPositionMask input_speakers, unsigned int sample_count);
		static void from_16bit_be(SoundMixingBuffersData &output, void *input, SpeakerPositionMask input_speakers, unsigned int sample_count);
		static void from_8bit_signed(SoundMixingBuffersData &output, void *input, SpeakerPositionMask input_speakers, unsigned int sample_count);
		static void from_8bit_unsigned(SoundMixingBuffersData &output, void *input, SpeakerPositionMask input_speakers, unsigned int sample_count);

		static void to_ieee_float(void **output, SpeakerPositionMask output_speakers, const SoundMixingBuffersData &input, unsigned int sample_count);
		static void to_16bit_le(void **output, SpeakerPositionMask output_speakers, const SoundMixingBuffersData &input, unsigned int sample_count);
		static void to_16bit_be(void **output, SpeakerPositionMask output_speakers, const SoundMixingBuffersData &input, unsigned int sample_count);
		static void to_8bit_signed(void **output, SpeakerPositionMask output_speakers, const SoundMixingBuffersData &input, unsigned int sample_count);
		static void to_8bit_unsigned(void **output, SpeakerPositionMask output_speakers, const SoundMixingBuffersData &input, unsigned int sample_count);

		//	static void resample(SoundMixingBuffersData &output, int sample_count_output, const SoundMixingBuffersData &input, unsigned int sample_count_input);

	private:
		static unsigned int find_output_speakers(SoundMixingBuffersData &output, SpeakerPositionMask input_speakers, float **output_speakers);
		static unsigned int find_input_speakers(const SoundMixingBuffersData &input, SpeakerPositionMask output_speakers, float **input_speakers);
		static void clear_unused_speakers(SoundMixingBuffersData &output, SpeakerPositionMask input_speakers, unsigned int sample_count);
	};
}
