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
**    Mark Page
*/

#include "Sound/precomp.h"
#include "echofilter_provider.h"
#include <memory.h>

namespace clan
{
	EchoFilterProvider::EchoFilterProvider(int new_buffer_size, float new_shift_factor) : buffer_size(new_buffer_size), shift_factor(new_shift_factor)
	{
		pos = 0;

		buffer[0] = new float[buffer_size];
		memset(buffer[0], 0, sizeof(float)*buffer_size);

		buffer[1] = new float[buffer_size];
		memset(buffer[1], 0, sizeof(float)*buffer_size);
	}

	EchoFilterProvider::~EchoFilterProvider()
	{
		delete[] buffer[0];
		delete[] buffer[1];
	}

	void EchoFilterProvider::filter(float **sample_data, int num_samples, int channels)
	{
		int start_pos = pos;

		for (int c = 0; c < 2; c++)
		{
			if (c == channels) break;

			float *data = sample_data[c];
			float *work_buffer = buffer[c];

			pos = start_pos;

			for (int i = 0; i < num_samples; i++)
			{
				work_buffer[pos] /= shift_factor;
				work_buffer[pos] += data[i];
				data[i] = work_buffer[pos];
				pos++;
				if (pos == buffer_size) pos = 0;
			}
		}
	}
}
