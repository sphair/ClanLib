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

//! clanDisplay="Sprites"
//! header=display.h

#ifndef header_sprite_packer
#define header_sprite_packer

#ifdef CL_API_DLL
#ifdef CL_DISPLAY_EXPORT
#define CL_API_DISPLAY __declspec(dllexport)
#else
#define CL_API_DISPLAY __declspec(dllimport)
#endif
#else
#define CL_API_DISPLAY
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning( disable : 4786)
#endif

#include "surface.h"
#include "../Core/Math/size.h"
#include "../Core/Math/rect.h"

#include <vector>

class CL_SpriteDescription;
class CL_SpritePacker_Generic;

//: Packs sprite into one or more textures.
//- !group=Display/Sprites!
//- !header=display.h!
class CL_API_DISPLAY CL_SpritePacker
{ 
//! Construction:
public:
	//: Constructs a CL_SpritePacker.
	CL_SpritePacker(const CL_SpriteDescription &description);

	~CL_SpritePacker();

//! Attributes:
public:
	//: Returns a list over all the textures needed for the packed-frames.
	//- <p> The get_frames() method returns a list consisting of many TexturePair, where each pair
	//- has an index into this list. </p>
	const std::vector<CL_Size> &get_texture_sizes() const;

	typedef std::pair<int, CL_Rect> TexturePair;	// int texture, CL_Rect frame_position
	//: Returns a list over all frames - which texture it is packed in and where within the
	//: texture it is located.
	//- <p> This list is arranged exactly the same order as the spritedescription frames. </p>
	const std::list<TexturePair> &get_frames() const;

//! Operations:
public:
	//: Packs all frames into one or more textures.
	//: Returns false if it was unable to pack all frames.
	bool pack(
		int max_width = 256, int max_height = 256,
		int min_width = 16, int min_height = 16,
		int max_textures = -1);

//! Implementation:
private:
	// Disallow copy construction.
	CL_SpritePacker(const CL_SpritePacker &copy);

	//: Disallow copy assignment.
	CL_SpritePacker &operator =(const CL_SpritePacker &copy);

	//: CL_SpritePacker implementation.
	CL_SpritePacker_Generic *impl;
};

#endif


