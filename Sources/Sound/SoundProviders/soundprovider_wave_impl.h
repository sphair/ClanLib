/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

#include "API/Sound/soundformat.h"

class CL_InputSourceProvider;
class CL_IODevice;

class CL_SoundProvider_Wave_Impl
{
public:
	CL_SoundProvider_Wave_Impl()
	: data(0)
	{
	}

	~CL_SoundProvider_Wave_Impl()
	{
		delete[] data;
	}

	void load(CL_IODevice &source);

/// \name Attributes
/// \{

public:
	char *data;
	CL_SoundFormat format;
	int num_channels;
	int num_samples;
	int frequency;
/// \}

private:
	cl_uint32 find_subchunk(const char *chunk, CL_IODevice &source, cl_uint32 file_offset, cl_uint32 max_offset );

};


