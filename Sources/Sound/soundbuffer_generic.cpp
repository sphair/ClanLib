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
#include "soundbuffer_generic.h"
#include "API/Sound/soundprovider.h"
#include "API/Sound/soundfilter.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SoundBuffer_Generic construction:

CL_SoundBuffer_Generic::CL_SoundBuffer_Generic() :
	provider(0), delete_provider(false),
	volume(1.0f), pan(0.0f)
{
}
	
CL_SoundBuffer_Generic::~CL_SoundBuffer_Generic()
{
	if(delete_provider)
		delete provider;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundBuffer_Generic attributes:

	
/////////////////////////////////////////////////////////////////////////////
// CL_SoundBuffer_Generic operations:

/////////////////////////////////////////////////////////////////////////////
// CL_SoundBuffer_Generic implementation:
