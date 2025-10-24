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

//! clanSound="Filters"
//! header=sound.h

#ifndef header_inverse_echofilter
#define header_inverse_echofilter

#ifdef CL_API_DLL
#ifdef CL_SOUND_EXPORT
#define CL_API_SOUND __declspec(dllexport)
#else
#define CL_API_SOUND __declspec(dllimport)
#endif
#else
#define CL_API_SOUND
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "../soundfilter.h"

class CL_InverseEchoFilter_Generic;

//: Inverse Echo Filter Class
//- !group=Sound/Filters!
//- !header=sound.h!
class CL_API_SOUND CL_InverseEchoFilter : public CL_SoundFilter
{
//! Construction:
public:
	//: Inverse Echo Filter Constructor
	CL_InverseEchoFilter(int buffer_size = 64*1024);

	//: Inverse Echo Filter Destructor
	virtual ~CL_InverseEchoFilter();

//! Operations:
public:
	//: Filter
	virtual void filter(int **sample_data, int num_samples, int channels);

//! Implementation:
private:
	CL_InverseEchoFilter(const CL_InverseEchoFilter &copy) { return; } // disallow copy construction.
	CL_InverseEchoFilter_Generic *impl;
};

#endif
