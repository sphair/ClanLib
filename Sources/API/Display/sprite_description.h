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

#ifndef header_sprite_description
#define header_sprite_description

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

#include <string>
#include <map>
#include <list>

#include "../Core/Math/rect.h"
#include "surface.h"

class CL_ResourceManager;
class CL_PixelBuffer;
class CL_SpriteDescription_Generic;

//: This class contains everything to construct a sprite - its data, default settings etc.
//- !group=Display/Sprites!
//- !header=display.h!
//- <p>CL_SpriteDescription is used by CL_Sprite to construct itself based
//- on the description. The description class give a big flexibility
//- creating a sprite, individual frames can come from different image
//- sources, or be cut out from an image source using various different
//- techniques.</p>
class CL_API_DISPLAY CL_SpriteDescription
{ 
//! Construction:
public:
	//: Constructs a sprite description.
	//param resource_id: Resource name of a sprite description resource.
	//param resources: Resource manager used to load resource.
	CL_SpriteDescription(const std::string &resource_id, CL_ResourceManager *resources);

	CL_SpriteDescription();

	CL_SpriteDescription(const CL_SpriteDescription &copy);

	~CL_SpriteDescription();

//! Attributes:
public:
	//: CL_PixelBuffer provider, CL_Rect position.
	typedef std::pair<CL_PixelBuffer, CL_Rect> FramePair;

	//: Returns a list over all available frames.
	const std::list<FramePair> &get_frames() const;

	//: Returns the surface flag to be used.
	int get_surface_flag() const;

//! Operations:
public:
	//: Copy assignment operator.
	CL_SpriteDescription &operator =(const CL_SpriteDescription &copy);

	//: Sets what surface flag CL_Sprite should use for its surfaces.
	void set_surface_flag(int flag);

	//: Adds a single image.
	//param provider: Image source.
	//param delete_provider: If true, deletes provider when sprite description is destroyed.
	void add_frame(CL_PixelBuffer provider);

	//: Adds images formed in a grid.
	//- <p>This function will cut out a grid of frames from one image.</p>
	//param provider: Image source.
	//param xpos, ypos: Position of where image grid starts.
	//param width, height: Size of a frame in the grid.
	//param xarray, yarray: Number of columns and rows in grid.
	//param array_skipframes: Number of frames to skip at last gridline.
	//param xspacing, yspacing: Pixel interspacing between grid frames.
	//param delete_provider: If true, deletes provider when sprite description is destroyed.
	void add_gridclipped_frames(CL_PixelBuffer provider, int xpos, int ypos, int width, int height, int xarray = 1, int yarray = 1, int array_skipframes = 0, int xspacing = 0, int yspacing = 0);

	//: Adds images separated with pure alpha (within trans_limit).
	//- <p>The alpha clipper will cut out frames from an image based on
	//- the transparency in the picture. It first determines the height
	//- of a row by searching for the first line that it considers
	//- completely transparent. Then it finds the width of each frame on
	//- this line by looking for columns that are completely transparency.</p>
	//param provider: Image source.
	//param xpos, ypos: Upper left position where alpha cutting should begin.
	//param trans_limit: Amount of non-transparent alpha allowed before a pixel is not considered transparent.
	//param delete_provider: If true, deletes provider when sprite description is destroyed.
	void add_alphaclipped_frames(CL_PixelBuffer provider, int xpos = 0, int ypos = 0, float trans_limit = 0.05f);

	//: Adds images separated with pure alpha (within trans_limit).
	//- <p>The alpha clipper will cut out frames from an image based on
	//- the transparency in the picture. It scans the lines horizontally
	//- from top to bottom. As soon as a non-transarent pixel is discovered,
	//- the clipper finds the bounding box for that region and then moves on.</p>
	//param provider: Image source.
	//param xpos, ypos: Upper left position where alpha cutting should begin.
	//param trans_limit: Amount of non-transparent alpha allowed before a pixel is not considered transparent.
	//param delete_provider: If true, deletes provider when sprite description is destroyed.
	void add_alphaclipped_frames_free(CL_PixelBuffer provider, int xpos = 0, int ypos = 0, float trans_limit = 0.05f);

	//: Adds images separated with palette-colours defining the boundaries.
	//param provider: Image source.
	//param xpos, ypos: Upper left position where cutting should begin.
	//param delete_provider: If true, deletes provider when sprite description is destroyed.
	void add_paletteclipped_frames(CL_PixelBuffer provider, int xpos = 0, int ypos = 0);
	
//! Implementation:
private:
	//: SpriteDescription implementation.
	CL_SpriteDescription_Generic *impl;
};

#endif


