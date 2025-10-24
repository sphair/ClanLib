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

#ifndef header_fadefilter
#define header_fadefilter

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

class CL_FadeFilter_Generic;

//: Fade Filter Class
//- !group=Sound/Filters!
//- !header=sound.h!
class CL_API_SOUND CL_FadeFilter : public CL_SoundFilter
{
//! Construction:
public:
	//: Fade Filter Constructor
	CL_FadeFilter(float initial_volume);

	//: Fade Filter Destructor
	virtual ~CL_FadeFilter();

//! Attributes:
public:
	//: Returns the current volume of the fade filter, from 0.0f (no volume) to 1.0f (full volume).
	float get_volume() const;

//! Operations:
public:
	//: Sets the volume instant.
	void set_volume(float new_volume);
	
	//: Fade to volume, from 0.0f (no volume) to 1.0f (full volume).
	void fade_to_volume(float new_volume, int duration = 1000);

//! Overridables:
protected:
	virtual void filter(int **sample_data, int num_samples, int channels);

//! Implementation:
private:
	CL_FadeFilter(const CL_FadeFilter &copy) { return; } // disallow copy construction.

	CL_FadeFilter_Generic *impl;
};

#endif
