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
#include "API/Display/pixel_buffer.h"
#include "sprite_packer_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SpritePacker_Generic construction:

CL_SpritePacker_Generic::CL_SpritePacker_Generic(const CL_SpriteDescription &desc)
: description(desc)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SpritePacker_Generic::Node implementation:

CL_SpritePacker_Generic::Node::Node()
{
	child[0] = NULL;
	child[1] = NULL;

	image = NULL;
}

CL_SpritePacker_Generic::Node::Node(const CL_Rect &new_rect)
{
	rect = new_rect;

	child[0] = NULL;
	child[1] = NULL;

	image = NULL;
}

CL_SpritePacker_Generic::Node::~Node()
{
	clear();
}

void CL_SpritePacker_Generic::Node::clear()
{
	if(child[0])
	{
		delete child[0];
		child[0] = NULL;
	}
	if(child[1])
	{
		delete child[1];
		child[1] = NULL;
	}
	image = NULL;
}

CL_SpritePacker_Generic::Node *CL_SpritePacker_Generic::Node::insert(CL_SpriteDescription::FramePair new_image)
{
	// If we're not a leaf
	if(child[0] && child[1])
	{
		// Try inserting into first child
		Node *new_node = child[0]->insert(new_image);
		if(new_node != NULL)
			return new_node;
        
		// No room, insert into second
		return child[1]->insert(new_image);
	}
	else
	{
		// If there's already a texture here, return
		if (image)
			return NULL;

		int image_width  = new_image.second.get_width()  + 2;
		int image_height = new_image.second.get_height() + 2;
		
		// If we're too small, return
		if (image_width > rect.get_width() || image_height > rect.get_height())
			return NULL;

		// If we're just right, accept
		if (image_width == rect.get_width() && image_height == rect.get_height())
		{
			image      = new_image.first;
			image_rect = new_image.second;
			image_rect.right  += 2;
			image_rect.bottom += 2;
			return this;
		}
        
		// Otherwise, decide which way to split
		int dw = rect.get_width() - image_width;
		int dh = rect.get_height() - image_height;
        
		if (dw > dh)
		{
			child[0] = new Node(CL_Rect(rect.left, rect.top, rect.left + image_width, rect.bottom));
			child[1] = new Node(CL_Rect(rect.left + image_width, rect.top, rect.right, rect.bottom));
		}
		else
		{
			child[0] = new Node(CL_Rect(rect.left, rect.top, rect.right, rect.top + image_height));
			child[1] = new Node(CL_Rect(rect.left, rect.top + image_height, rect.right, rect.bottom));
		}
        
		// Insert into first child we created
		return child[0]->insert(new_image);
	}
}
