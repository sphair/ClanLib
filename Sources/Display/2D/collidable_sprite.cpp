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
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Core/Resources/resource.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/2D/draw.h"
#include "API/Display/2D/sprite.h"
#include "API/Display/2D/sprite_description.h"
#include "API/Display/Render/primitives_array.h"
#include "API/Display/Image/pixel_buffer.h"
#include "sprite_impl.h"
#include "API/Display/2D/collidable_sprite.h"

std::vector<CL_CollisionOutline> CL_CollidableSprite::create_collision_outlines(CL_GraphicContext &gc, const CL_StringRef &resource_id, CL_ResourceManager *resources, int alpha_limit, CL_OutlineAccuracy accuracy)
{
	std::vector<CL_CollisionOutline> outlines;

	CL_SpriteDescription description(gc, resource_id, resources, CL_ImageImportDescription () );

	// Fetch frames
	const std::vector<CL_SpriteDescriptionFrame> &description_frames = description.get_frames();
	std::vector<CL_SpriteDescriptionFrame>::const_iterator it_frames;

	outlines.reserve(description_frames.size());

	for (it_frames = description_frames.begin(); it_frames != description_frames.end(); ++it_frames)
	{
		CL_SpriteDescriptionFrame description_frame = (*it_frames);

		CL_PixelBuffer target;

		if(description_frame.type == CL_SpriteDescriptionFrame::type_pixelbuffer)
		{
			target = CL_PixelBuffer(description_frame.rect.get_width(), description_frame.rect.get_height(), cl_rgba8);
			description_frame.pixelbuffer.convert(target, description_frame.rect.get_size(), description_frame.rect);
		}
		else
		{
			target = CL_PixelBuffer(description_frame.rect.get_width(), description_frame.rect.get_height(), cl_rgba8);
			CL_PixelBuffer pbuff = description_frame.texture.get_pixeldata(cl_rgba8);
			if (pbuff.is_null())
				throw CL_Exception("Display target does not support CL_Texture::get_pixeldata()");

			pbuff.convert(target, description_frame.rect.get_size(), description_frame.rect);
		}

		CL_CollisionOutline outline(target, alpha_limit, accuracy);
		outlines.push_back(outline);

	}
	return outlines;

}

CL_CollisionOutline CL_CollidableSprite::create_collision_outline(CL_GraphicContext &gc, const CL_StringRef &resource_id, CL_ResourceManager *resources, int alpha_limit, CL_OutlineAccuracy accuracy)
{
	std::vector<CL_CollisionOutline> outlines = CL_CollidableSprite::create_collision_outlines(gc, resource_id, resources, alpha_limit, accuracy);
	if (outlines.empty())
		return CL_CollisionOutline();
	return outlines[0];
}

