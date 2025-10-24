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

#include "API/Core/IOData/iodevice_provider.h"

class CL_IODevice_Impl
{
/// \name Construction
/// \{

public:
	CL_IODevice_Impl() : little_endian_mode(true), provider(0)
	{
	}

	~CL_IODevice_Impl()
	{
		delete provider;
	}


/// \}
/// \name Attributes
/// \{

public:
	bool little_endian_mode;

	CL_IODeviceProvider *provider;


/// \}
/// \name Operations
/// \{

public:


/// \}
/// \name Implementation
/// \{

private:
/// \}
};


