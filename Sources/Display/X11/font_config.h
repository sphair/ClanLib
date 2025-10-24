/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    Kevin Bluck
*/


#pragma once


#include "fontconfig/fontconfig.h"
#include "API/Core/IOData/virtual_directory.h"

class CL_FontDescription;

class CL_FontConfig
{
/// \name Construction
/// \{

private:
	CL_FontConfig();
	~CL_FontConfig();
	CL_FontConfig(const CL_FontConfig &);
	CL_FontConfig operator=(const CL_FontConfig &);


/// \}
/// \name Attributes
/// \{

public:
	static CL_FontConfig &instance();


/// \}
/// \name Operations
/// \{

public:
	CL_String match_font( const CL_FontDescription &desc) const;

/// \}
/// \name Implementation
/// \{

private:
	FcConfig * fc_config;
/// \}
};



