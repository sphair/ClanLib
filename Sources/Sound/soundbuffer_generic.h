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

#ifndef header_soundbuffer_generic
#define header_soundbuffer_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include <vector>
#include "API/Core/System/mutex.h"

class CL_SoundProvider;
class CL_SoundFilter;

class CL_SoundBuffer_Generic
{
//! Construction:
public:
	CL_SoundBuffer_Generic();
	
	virtual ~CL_SoundBuffer_Generic();

//! Attributes:
public:
	CL_SoundProvider *provider;
	
	bool delete_provider;
	
	float volume;
	
	float pan;

	std::vector<CL_SoundFilter *> filters;
	
	std::vector<bool> delete_filters;
	
	mutable CL_Mutex mutex;
	
//! Operations:
public:
	
//! Implementation:
private:
};

#endif
