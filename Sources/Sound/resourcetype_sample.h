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

//! l2_component="Resources"

#pragma once

class CL_SoundBuffer;
class CL_InputSourceProvider;
class CL_OutputSourceProvider;

class CL_ResourceData_Sample
{
/// \name Construction
/// \{

public:
	CL_ResourceData_Sample(class CL_Resource &resource);

	~CL_ResourceData_Sample();


/// \}
/// \name Attributes
/// \{

public:
	// Loaded soundbuffer for this resource.
	// It is IMPORTANT that this is a pointer, else the CL_ResourceDataSession garbage collection will not work
	CL_SoundBuffer *soundbuffer;


/// \}
/// \name Implementation
/// \{

private:
/// \}
};


