/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
#include "API/Display/2D/canvas.h"
#include "API/Display/2D/sprite.h"
#include "API/Display/2D/sprite_description.h"
#include "API/Display/Render/primitives_array.h"
#include "API/Display/Image/pixel_buffer.h"
#include "sprite_impl.h"
#include "API/Display/2D/collidable_sprite.h"
#include "API/Display/Image/pixel_buffer_lock.h"

namespace clan
{

std::vector<CollisionOutline> CollidableSprite::create_collision_outlines(GraphicContext &gc, const std::string &resource_id, ResourceManager *resources, int alpha_limit, OutlineAccuracy accuracy)
{
	std::vector<CollisionOutline> outlines;

	SpriteDescription description(gc, resource_id, resources, ImageImportDescription () );

	// Fetch frames
	const std::vector<SpriteDescriptionFrame> &description_frames = description.get_frames();
	std::vector<SpriteDescriptionFrame>::const_iterator it_frames;

	outlines.reserve(description_frames.size());

	for (it_frames = description_frames.begin(); it_frames != description_frames.end(); ++it_frames)
	{
		SpriteDescriptionFrame description_frame = (*it_frames);

		PixelBuffer target;

		if(description_frame.type == SpriteDescriptionFrame::type_pixelbuffer)
		{
			target = PixelBuffer(description_frame.rect.get_width(), description_frame.rect.get_height(), tf_rgba8);
			target.set_subimage(description_frame.pixelbuffer, Point(0, 0), description_frame.rect);
		}
		else
		{
			target = PixelBuffer(description_frame.rect.get_width(), description_frame.rect.get_height(), tf_rgba8);
			PixelBuffer pbuff = description_frame.texture.get_pixeldata(gc, tf_rgba8).to_cpu(gc);
			target.set_subimage(pbuff, Point(0, 0), description_frame.rect);
		}

		CollisionOutline outline(target, alpha_limit, accuracy);
		outlines.push_back(outline);

	}
	return outlines;

}

CollisionOutline CollidableSprite::create_collision_outline(GraphicContext &gc, const std::string &resource_id, ResourceManager *resources, int alpha_limit, OutlineAccuracy accuracy)
{
	std::vector<CollisionOutline> outlines = CollidableSprite::create_collision_outlines(gc, resource_id, resources, alpha_limit, accuracy);
	if (outlines.empty())
		return CollisionOutline();
	return outlines[0];
}

}
