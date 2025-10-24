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
**    Magnus Norddahl
*/

#pragma once

#include "API/CSSLayout/css_box_properties.h"
#include "css_used_value.h"

class CL_CSSLayoutGraphics;
class CL_CSSResourceCache;
class CL_CSSBoxElement;

class CL_CSSBackgroundRenderer
{
public:
	CL_CSSBackgroundRenderer(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resource_cache, CL_CSSBoxElement *element_node);
	void set_border_box(CL_Rect border_box);
	void set_padding_box(CL_Rect padding_box);
	void set_content_box(CL_Rect content_box);
	void set_initial_containing_box(CL_Rect initial_containing_box);
	void set_is_root(bool is_root);
	void render();

private:
	CL_CSSActualValue get_start_x(size_t index, const CL_Rect &clip_box, const CL_Rect &origin_box, const CL_Size &image_size);
	CL_CSSActualValue get_start_y(size_t index, const CL_Rect &clip_box, const CL_Rect &origin_box, const CL_Size &image_size);
	CL_Size get_image_size(size_t index, CL_Image &image, CL_Rect origin_box);
	CL_Rect get_clip_box(size_t index);
	CL_Rect get_origin_box(size_t index);
	CL_CSSBoxProperties &computed_properties();
	CL_CSSBoxBackgroundClip::ClipType get_layer_clip(size_t index);
	CL_CSSBoxBackgroundOrigin::OriginType get_layer_origin(size_t index);
	CL_CSSBoxBackgroundSize::Size get_layer_size(size_t index);
	CL_CSSBoxBackgroundPosition::Position get_layer_position(size_t index);
	CL_CSSBoxBackgroundAttachment::Attachment get_layer_attachment(size_t index);
	CL_CSSBoxBackgroundRepeat::RepeatStyle get_layer_repeat_x(size_t index);
	CL_CSSBoxBackgroundRepeat::RepeatStyle get_layer_repeat_y(size_t index);

	CL_CSSLayoutGraphics *graphics;
	CL_CSSResourceCache *resource_cache;
	CL_CSSBoxElement *element_node;
	CL_Rect border_box;
	CL_Rect padding_box;
	CL_Rect content_box;
	CL_Rect initial_containing_box;
	bool is_root;
};
