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

#include "channel_buffers_data.h"

namespace clan
{
	class SoundMixingBuffersContainer : public SoundMixingBuffersData
	{
	public:
		SoundMixingBuffersContainer(SpeakerPositionMask input_speakers, int sample_count)
		{
			for (int i = 0; input_speakers != 0; (input_speakers >>= 1), i++)
			{
				if (input_speakers & 1)
					channels[i] = new float[sample_count];
				else
					channels[i] = 0;
			}
		}

		~SoundMixingBuffersContainer()
		{
			for (int i = 0; i < 32; i++)
				delete[] channels[i];
		}

	private:
		SoundMixingBuffersContainer(const SoundMixingBuffersContainer &source) { }
		SoundMixingBuffersContainer &operator =(const SoundMixingBuffersContainer &source) { }
	};
}
