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

#ifndef header_resourcedata_sprite_description
#define header_resourcedata_sprite_description

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Core/Resources/resourcedata.h"
#include "API/Core/Resources/resource.h"
#include "API/Display/sprite_description.h"

class CL_ResourceData_SpriteDescription : public CL_ResourceData
{
//! Construction:
public:
	CL_ResourceData_SpriteDescription(CL_Resource &resource, const std::string &type = "sprite_description");
	
	virtual ~CL_ResourceData_SpriteDescription();
	
//! Attributes:
public:

//! Operations:
public:
	//: Loads a sprite description based on the resource description.
	CL_SpriteDescription load_description();

//! Implementation:
private:
	CL_PixelBuffer load_image(const std::string &filename);

	void add_images(CL_SpriteDescription &desc, CL_Resource &resource);
};

#endif
