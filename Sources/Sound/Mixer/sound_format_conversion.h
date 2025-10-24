/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

struct CL_SoundFormatConversion
{
	static void from_ieee_float(CL_SoundMixingBuffersData &output, void *input, CL_SpeakerPositionMask input_speakers, unsigned int sample_count);
	static void from_16bit_le(CL_SoundMixingBuffersData &output, void *input, CL_SpeakerPositionMask input_speakers, unsigned int sample_count);
	static void from_16bit_be(CL_SoundMixingBuffersData &output, void *input, CL_SpeakerPositionMask input_speakers, unsigned int sample_count);
	static void from_8bit_signed(CL_SoundMixingBuffersData &output, void *input, CL_SpeakerPositionMask input_speakers, unsigned int sample_count);
	static void from_8bit_unsigned(CL_SoundMixingBuffersData &output, void *input, CL_SpeakerPositionMask input_speakers, unsigned int sample_count);

	static void to_ieee_float(void **output, CL_SpeakerPositionMask output_speakers, const CL_SoundMixingBuffersData &input, unsigned int sample_count);
	static void to_16bit_le(void **output, CL_SpeakerPositionMask output_speakers, const CL_SoundMixingBuffersData &input, unsigned int sample_count);
	static void to_16bit_be(void **output, CL_SpeakerPositionMask output_speakers, const CL_SoundMixingBuffersData &input, unsigned int sample_count);
	static void to_8bit_signed(void **output, CL_SpeakerPositionMask output_speakers, const CL_SoundMixingBuffersData &input, unsigned int sample_count);
	static void to_8bit_unsigned(void **output, CL_SpeakerPositionMask output_speakers, const CL_SoundMixingBuffersData &input, unsigned int sample_count);

//	static void resample(CL_SoundMixingBuffersData &output, int sample_count_output, const CL_SoundMixingBuffersData &input, unsigned int sample_count_input);
	
private:
	static unsigned int find_output_speakers(CL_SoundMixingBuffersData &output, CL_SpeakerPositionMask input_speakers, float **output_speakers);
	static unsigned int find_input_speakers(const CL_SoundMixingBuffersData &input, CL_SpeakerPositionMask output_speakers, float **input_speakers);
	static void clear_unused_speakers(CL_SoundMixingBuffersData &output, CL_SpeakerPositionMask input_speakers, unsigned int sample_count);
};
