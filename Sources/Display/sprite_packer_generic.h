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

#ifndef header_sprite_packer_generic
#define header_sprite_packer_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Core/Math/size.h"
#include "API/Core/Math/rect.h"
#include "API/Display/sprite_description.h"
#include "API/Display/surface.h"
#include "API/Display/sprite_packer.h"

// Stuff to improve:
// - Sort the textures after size before inserting - might do wonders

class CL_SpritePacker_Generic
{
//! Construction:
public:
	CL_SpritePacker_Generic(const CL_SpriteDescription &description);

	class Node
	{
	public:
		Node();
		Node(const CL_Rect &rect);
		~Node();

		Node *insert(CL_SpriteDescription::FramePair image);
		void clear();
		
		Node *child[2];
		CL_Rect rect;
	    
	    CL_PixelBuffer image;
	    CL_Rect image_rect;
	};

//! Attributes:
public:
	const CL_SpriteDescription &description;

	std::vector<CL_Size> texture_sizes;
	std::list<CL_SpritePacker::TexturePair> packed_frames;
};

#endif

