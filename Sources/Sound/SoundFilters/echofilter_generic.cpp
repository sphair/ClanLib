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

#ifdef _MSC_VER
#pragma warning (disable:4786)
#endif

#include "echofilter_generic.h"
#include <memory.h>

CL_EchoFilter::CL_EchoFilter(int buffer_size, int shift_factor)
{
	impl = new CL_EchoFilter_Generic;
	impl->buffer_size = buffer_size;
	impl->shift_factor = shift_factor;
	impl->pos = 0;

	impl->buffer[0] = new int[buffer_size];
	memset(impl->buffer[0], 0, sizeof(int)*buffer_size);

	impl->buffer[1] = new int[buffer_size];
	memset(impl->buffer[1], 0, sizeof(int)*buffer_size);
}

CL_EchoFilter::~CL_EchoFilter()
{
	delete[] impl->buffer[0];
	delete[] impl->buffer[1];
	delete impl;
}

void CL_EchoFilter::filter(int **sample_data, int num_samples, int channels)
{
	int start_pos = impl->pos;

	for (int c=0; c<2; c++)
	{
		if (c == channels) break;

		int *data = sample_data[c];
		int *buffer = impl->buffer[c];
		int &pos = impl->pos;
		int &buffer_size = impl->buffer_size;
		int &shift_factor = impl->shift_factor;

		pos = start_pos;

		for (int i=0; i<num_samples; i++)
		{
			buffer[pos] /= shift_factor;
			buffer[pos] += data[i];
			data[i] = buffer[pos];
			pos++;
			if (pos == buffer_size) pos = 0;
		}
	}
}
