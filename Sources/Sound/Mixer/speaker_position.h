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

namespace clan
{
	enum SpeakerPosition
	{
		cl_speaker_front_left                = 0x1,
		cl_speaker_front_right               = 0x2,
		cl_speaker_front_center              = 0x4,
		cl_speaker_low_frequency             = 0x8,
		cl_speaker_back_left                 = 0x10,
		cl_speaker_back_right                = 0x20,
		cl_speaker_front_left_of_center      = 0x40,
		cl_speaker_front_right_of_center     = 0x80,
		cl_speaker_back_center               = 0x100,
		cl_speaker_side_left                 = 0x200,
		cl_speaker_side_right                = 0x400,
		cl_speaker_top_center                = 0x800,
		cl_speaker_top_front_left            = 0x1000,
		cl_speaker_top_front_center          = 0x2000,
		cl_speaker_top_front_right           = 0x4000,
		cl_speaker_top_back_left             = 0x8000,
		cl_speaker_top_back_center           = 0x10000,
		cl_speaker_top_back_right            = 0x20000
	};

	typedef unsigned int SpeakerPositionMask;
}
