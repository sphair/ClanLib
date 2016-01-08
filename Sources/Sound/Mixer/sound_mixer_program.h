/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "channel_buffers_data.h"

namespace clan
{
	class SoundMixingInput
	{
	public:
		SoundMixingBuffersData data;
		unsigned int buffer_size;
		float read_cursor;
		unsigned int write_cursor;
		float volumes[32];
		float speed;

		virtual void get_more_data() = 0;
	};

	class SoundMixerProgram
	{
	public:
		virtual void mix(SoundMixingBuffersData &output, std::vector<SoundMixingInput*> inputs, unsigned int sample_count) = 0;
	};

	class StandardSoundMixerProgram : public SoundMixerProgram
	{
	public:
		void mix(SoundMixingBuffersData &output, std::vector<SoundMixingInput*> inputs, unsigned int sample_count)
		{
			for (unsigned int i = 0; i < inputs.size(); i++)
			{
				SoundMixingInput *input = inputs[i];
				for (unsigned int s = 0; s < sample_count; s++)
				{
					for (int c = 0; c < 32; c++)
					{
						if (output.channels[c])
							if (input->data.channels[c])
								output.channels[c][s] += input->data.channels[c][(unsigned int)input->read_cursor] * input->volumes[c];
					}

					input->read_cursor += input->speed;
					if (input->read_cursor >= input->write_cursor)
						input->get_more_data();
				}
			}
		}
	};
}
