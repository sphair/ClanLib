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

#include "Display/display_precomp.h"
#include "API/Display/sprite_description.h"
#include "API/Display/sprite_packer.h"
#include "API/Display/pixel_buffer.h"
#include "API/Core/System/error.h"
#include "API/Core/System/log.h"
#include "sprite_packer_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SpritePacker construction:

CL_SpritePacker::CL_SpritePacker(const CL_SpriteDescription &description)
: impl(new CL_SpritePacker_Generic(description))
{
}

CL_SpritePacker::~CL_SpritePacker()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SpritePacker attributes:

const std::vector<CL_Size> &CL_SpritePacker::get_texture_sizes() const
{
	return impl->texture_sizes;
}

const std::list<CL_SpritePacker::TexturePair> &CL_SpritePacker::get_frames() const
{
	return impl->packed_frames;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SpritePacker operations:

bool CL_SpritePacker::pack(int max_width, int max_height, int min_width, int min_height, int max_textures)
{
	// Fetch all frames from SpriteDescription.
	std::list<CL_SpriteDescription::FramePair>::const_iterator it_frames;
	const std::list<CL_SpriteDescription::FramePair> &frames = impl->description.get_frames();

	std::list<TexturePair>::iterator it_packed_frames;

	// Make a new list with just as many elements as spritedescription.
	// A texture index of -1 means the texture hasn't been packed yet.
	for(int i = frames.size(); i > 0; i--)
		impl->packed_frames.push_back(TexturePair(-1, CL_Rect()));

	// Initial values for texture-size
	int width = min_width;
	int height = min_height;
	impl->texture_sizes.push_back(CL_Size(width, height));

	CL_SpritePacker_Generic::Node root;
	int current_texture = 0;
	bool all_fit;
	do
	{
		root.rect = CL_Rect(0, 0, width, height);
		all_fit = true;
		
		// Pack all frames!
		for(it_frames = frames.begin(), it_packed_frames = impl->packed_frames.begin();
			it_frames != frames.end();
			++it_frames, ++it_packed_frames)
		{
			if((*it_frames).second.get_width() > max_width || (*it_frames).second.get_height() > max_height)
			{
#ifdef _DEBUG
				CL_Log::log("debug", "Error: Trying to pack sprite larger than the maximum");
				CL_Log::log("debug", "Width: %1, Max Width %2", (*it_frames).second.get_width(), max_width);
				CL_Log::log("debug", "Height: %1, Max Height %2", (*it_frames).second.get_height(), max_height);
#endif
				return false;
			}
			
			if((*it_packed_frames).first == -1)	// Frame not yet been packed.
			{
				CL_SpritePacker_Generic::Node *node = root.insert(*it_frames);

				if(node)	// Frame was packed ok.
				{
					(*it_packed_frames).first = current_texture;
					(*it_packed_frames).second = node->rect;
				}
				else	// Frame couldn't fit into current texture.
					all_fit = false;
			}
		}
		
		if(all_fit == false)	// All textures didnt fit.
		{
			// A maximum texture-size ?
			if((width == height && (width * 2) <= max_width) || (height * 2) <= max_height)
			{
				if(width == height && (width * 2) <= max_width)
					width *= 2;
				else
					height *= 2;

				// Clear current texture.
				for(it_packed_frames = impl->packed_frames.begin();
					it_packed_frames != impl->packed_frames.end();
					++it_packed_frames)
				{
					if((*it_packed_frames).first == current_texture)
						(*it_packed_frames).first = -1;
				}
				
				impl->texture_sizes[current_texture] = CL_Size(width, height);
			}
			else
			{
				current_texture++;

				// Check if we still have textures available.
				if(current_texture >= max_textures && max_textures != -1)
					return false;

				// Start a minimum size for next texture.
				width = min_width;
				height = min_height;

				impl->texture_sizes.push_back(CL_Size(width, height));
			}
							
			root.clear();
		}
	} while(all_fit == false);

	return true;
}
