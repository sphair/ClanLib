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

//! clanSound="Audio Mixing"
//! header=sound.h

#ifndef header_soundfilter
#define header_soundfilter

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

class CL_SoundFilter_Generic;

//: Sound Filter Class
//- !group=Sound/Audio Mixing!
//- !header=sound.h!
class CL_API_SOUND CL_SoundFilter
{
//! Construction:
public:
	//: Sound filter constructor.
	CL_SoundFilter();

	CL_SoundFilter(const CL_SoundFilter &copy);

	//: Sound Filter Destructor
	virtual ~CL_SoundFilter();
	
//! Operations:
public:
	//: Copy assignment operator.
	CL_SoundFilter &operator =(const CL_SoundFilter &copy);

//! Overridables:
	//: Filter callback.
	//- <p>All sound data is passed through this function,
	//- which modifies the sample data accordingly to the function of the
	//- filter.</p>
	//- <p>The format of the sample data is always 16 bit stereo. </p>
	virtual void filter(int **sample_data, int num_samples, int channels)=0;

//! Implementation:
public:
	CL_SoundFilter_Generic *impl;
};

#endif
