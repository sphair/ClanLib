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
**    Magnus Norddahl
*/

#include "CSSLayout/precomp.h"
#include "API/CSSLayout/ComputedValues/css_computed_background.h"
#include "API/CSSLayout/ComputedValues/css_computed_values.h"

namespace clan
{

void CSSComputedBackground::compute(const CSSComputedValues &parent, CSSResourceCache *layout, float em_size, float ex_size)
{
	if (!parent.is_null())
	{
		const CSSComputedBackground &parent_background = parent.get_background();

		background_attachment.compute(&parent_background.background_attachment, layout, em_size, ex_size);
		background_color.compute(&parent_background.background_color, layout, em_size, ex_size);
		background_image.compute(&parent_background.background_image, layout, em_size, ex_size);
		background_position.compute(&parent_background.background_position, layout, em_size, ex_size);
		background_repeat.compute(&parent_background.background_repeat, layout, em_size, ex_size);
		background_origin.compute(&parent_background.background_origin, layout, em_size, ex_size);
		background_clip.compute(&parent_background.background_clip, layout, em_size, ex_size);
		background_size.compute(&parent_background.background_size, layout, em_size, ex_size);
	}
	else
	{
		background_attachment.compute(0, layout, em_size, ex_size);
		background_color.compute(0, layout, em_size, ex_size);
		background_image.compute(0, layout, em_size, ex_size);
		background_position.compute(0, layout, em_size, ex_size);
		background_repeat.compute(0, layout, em_size, ex_size);
		background_origin.compute(0, layout, em_size, ex_size);
		background_clip.compute(0, layout, em_size, ex_size);
		background_size.compute(0, layout, em_size, ex_size);
	}
}

}
