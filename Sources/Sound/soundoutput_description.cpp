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
#include "API/Sound/soundoutput_description.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput_Description_Generic:

class CL_SoundOutput_Description_Generic
{
//! Attributes:
public:
	int mixing_frequency;
};

/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput_Description construction:

CL_SoundOutput_Description::CL_SoundOutput_Description() : impl(new CL_SoundOutput_Description_Generic)
{
	impl->mixing_frequency = 44100;
}

CL_SoundOutput_Description::CL_SoundOutput_Description(const CL_SoundOutput_Description &copy) : impl(new CL_SoundOutput_Description_Generic)
{
	*impl = *copy.impl;
}

CL_SoundOutput_Description::~CL_SoundOutput_Description()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput_Description attributes:

int CL_SoundOutput_Description::get_mixing_frequency() const
{
	return impl->mixing_frequency;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput_Description operations:

CL_SoundOutput_Description &CL_SoundOutput_Description::operator =(const CL_SoundOutput_Description &copy)
{
	*impl = *copy.impl;
	return *this;
}

void CL_SoundOutput_Description::set_mixing_frequency(int frequency)
{
	impl->mixing_frequency = frequency;
}

// CL_SoundOutput_Description implementation:
/////////////////////////////////////////////////////////////////////////////
