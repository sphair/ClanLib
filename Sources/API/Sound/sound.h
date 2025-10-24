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

#ifndef header_sound
#define header_sound

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

#include <string>

class CL_SoundOutput;

class CL_API_SOUND CL_Sound
//: Sound interface in ClanLib.
//- !group=Sound/Audio Mixing!
//- !header=sound.h!
//- <p>All the functions that share name with those in CL_SoundOutput have the
//- same functionality. The only difference is, that the ones CL_Sound
//- operate on a selected sound output. This saves the trouble of passing around
//- a pointer to the sound output, when only a single one is used anyway.</p>
//also: CL_SoundOutput - Sound Output interface class.
//also: CL_SoundBuffer - Sample class in ClanLib.
{
//! Selected CL_SoundOutput accessors:
public:
	//: Returns the name of the current sound output:
	static const std::string &get_name();

	//: Returns the mixing frequency of the current sound output.
	static int get_mixing_frequency();

	//: Returns the current selected sound output.
	static CL_SoundOutput *get_current_output();

	//: Change the current selected sound output.
	//param output: The new current selected sound output.
	static void select_output(CL_SoundOutput *output);
};

#endif
