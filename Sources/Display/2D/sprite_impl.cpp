/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
**    Kenneth Gangstoe
**    Mark Page
*/

#include "Display/precomp.h"
#include "sprite_impl.h"
#include "render_batch_triangle.h"
#include "API/Display/2D/sprite_description.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Core/Math/cl_math.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/System/system.h"
#include "Display/Render/graphic_context_impl.h"
#include <list>
#include "API/Display/2D/canvas.h"
#include "canvas_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Sprite_Impl construction:

Sprite_Impl::Sprite_Impl() :
	angle(Angle(0.0f, angle_radians)),
	base_angle(Angle(0.0f, angle_radians)),
	scale_x(1.0f),
	scale_y(1.0f),
	color(1.0f, 1.0f, 1.0f, 1.0f),
	linear_filter(true),
	translation_hotspot(0,0),
	rotation_hotspot(0,0),
	translation_origin(origin_top_left),
	rotation_origin(origin_center),
	current_frame(0),
	delta_frame(1),
	update_time_ms(0),
	id(0),
	finished(false),
	looping(false),
	play_loop(true),
	play_backward(false),
	play_pingpong(false),
	show_on_finish(Sprite::show_blank)
{
}

Sprite_Impl::~Sprite_Impl()
{
}

void Sprite_Impl::init(GraphicContext &gc, const std::string &resource_id, const XMLResourceDocument &resources, const ImageImportDescription &import_desc)
{
//	bool pack_texture = resource.get_element().get_attribute("pack_texture", "yes") == "yes";
	XMLResourceNode resource = resources.get_resource(resource_id);

	// Create sprite from sprite description
	SpriteDescription desc(gc, resource_id, resources, import_desc );
	create_textures(gc, desc);

	// Load base angle
	float work_angle = StringHelp::text_to_float(resource.get_element().get_attribute("base_angle", "0"));
	base_angle= Angle(work_angle, angle_degrees);

	// Load id
	id = StringHelp::text_to_int(resource.get_element().get_attribute("id", "0"));

	// Load play options	
	DomNode cur_node = resource.get_element().get_first_child();
	while (!cur_node.is_null())
	{
		if (!cur_node.is_element())
			continue;

		DomElement cur_element = cur_node.to_element();

		std::string tag_name = cur_element.get_tag_name();

		// <color red="float" green="float" blue="float" alpha="float" />
		if (tag_name == "color")
		{
			color.r = (float)StringHelp::text_to_float(cur_element.get_attribute("red", "1.0"));
			color.g = (float)StringHelp::text_to_float(cur_element.get_attribute("green", "1.0"));
			color.b = (float)StringHelp::text_to_float(cur_element.get_attribute("blue", "1.0"));
			color.a = (float)StringHelp::text_to_float(cur_element.get_attribute("alpha", "1.0"));
		}
		// <animation speed="integer" loop="[yes,no]" pingpong="[yes,no]" direction="[backward,forward]" on_finish="[blank,last_frame,first_frame]"/>
		else if (tag_name == "animation")
		{
			int delay_ms = StringHelp::text_to_int(cur_element.get_attribute("speed", "60"));

			int frame_count = frames.size();
			for(int i=0; i<frame_count; ++i)
				get_frame(i)->delay_ms = delay_ms;

			play_loop = ((cur_element.get_attribute("loop", "yes")) == "yes");
			play_pingpong = ((cur_element.get_attribute("pingpong", "no")) == "yes");
			play_backward = ((cur_element.get_attribute("direction", "forward")) == "backward");

			std::string on_finish = cur_element.get_attribute("on_finish", "blank");
			if (on_finish == "first_frame")
				show_on_finish = Sprite::show_first_frame;
			else if(on_finish == "last_frame")
				show_on_finish = Sprite::show_last_frame;
			else
				show_on_finish = Sprite::show_blank;
		}
		// <scale x="float" y="float />
		else if (tag_name == "scale")
		{
			scale_x = StringHelp::text_to_float(cur_element.get_attribute("x", "1.0"));
			scale_y = StringHelp::text_to_float(cur_element.get_attribute("y", "1.0"));
		}
		// <translation origin="string" x="integer" y="integer" />
		else if (tag_name == "translation")
		{
			std::string hotspot = cur_element.get_attribute("origin", "top_left");
			Origin origin;

			if(hotspot == "center")
				origin = origin_center;
			else if(hotspot == "top_center")
				origin = origin_top_center;
			else if(hotspot == "top_right")
				origin = origin_top_right;
			else if(hotspot == "center_left")
				origin = origin_center_left;
			else if(hotspot == "center_right")
				origin = origin_center_right;
			else if(hotspot == "bottom_left")
				origin = origin_bottom_left;
			else if(hotspot == "bottom_center")
				origin = origin_bottom_center;
			else if(hotspot == "bottom_right")
				origin = origin_bottom_right;
			else
				origin = origin_top_left;

			int xoffset = StringHelp::text_to_int(cur_element.get_attribute("x", "0"));
			int yoffset = StringHelp::text_to_int(cur_element.get_attribute("y", "0"));

			translation_origin = origin;
			translation_hotspot.x = xoffset;
			translation_hotspot.y = yoffset;
		}
		// <rotation origin="string" x="integer" y="integer" />
		else if (tag_name == "rotation")
		{
			std::string hotspot = cur_element.get_attribute("origin", "center");
			Origin origin;

			if(hotspot == "top_left")
				origin = origin_top_left;
			else if(hotspot == "top_center")
				origin = origin_top_center;
			else if(hotspot == "top_right")
				origin = origin_top_right;
			else if(hotspot == "center_left")
				origin = origin_center_left;
			else if(hotspot == "center_right")
				origin = origin_center_right;
			else if(hotspot == "bottom_left")
				origin = origin_bottom_left;
			else if(hotspot == "bottom_center")
				origin = origin_bottom_center;
			else if(hotspot == "bottom_right")
				origin = origin_bottom_right;
			else
				origin = origin_center;

			int xoffset = StringHelp::text_to_int(cur_element.get_attribute("x", "0"));
			int yoffset = StringHelp::text_to_int(cur_element.get_attribute("y", "0"));

			rotation_origin = origin;
			rotation_hotspot.x = xoffset;
			rotation_hotspot.y = yoffset;
		}
		// <frame nr="integer" speed="integer" x="integer" y="integer" />
		else if (tag_name == "frame")
		{
			int nr = StringHelp::text_to_int(cur_element.get_attribute("nr", "0"));

			int yoffset = StringHelp::text_to_int(cur_element.get_attribute("y", "0"));
			int xoffset = StringHelp::text_to_int(cur_element.get_attribute("x", "0"));

			SpriteFrame *sptr = get_frame(nr);
			if (sptr == NULL)
			{
				throw Exception("Invalid sprite frame index specified");
			}

			if (cur_element.has_attribute("speed")) 
			{
				sptr->delay_ms = StringHelp::text_to_int(cur_element.get_attribute("speed", "60"));
			}

			sptr->offset = Point(xoffset, yoffset);
		}

		cur_node = cur_node.get_next_sibling();
	}
}

/////////////////////////////////////////////////////////////////////////////
// Sprite_Impl attributes:

const Sprite_Impl::SpriteFrame *Sprite_Impl::get_frame(unsigned int index) const
{
	if(index >= 0 && index < frames.size())
		return &frames[index];
	else
		return NULL;
}

Sprite_Impl::SpriteFrame *Sprite_Impl::get_frame(unsigned int index) 
{
	if(index >= 0 && index < frames.size())
		return &frames[index];
	else
		return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// Sprite_Impl operations:

Sprite_Impl &Sprite_Impl::operator =(const Sprite_Impl &copy)
{
	angle = copy.angle;
	base_angle = copy.base_angle;
	scale_x = copy.scale_x;
	scale_y = copy.scale_y;
	color = copy.color;
	translation_hotspot = copy.translation_hotspot;
	rotation_hotspot = copy.rotation_hotspot;
	translation_origin = copy.translation_origin;
	rotation_origin = copy.rotation_origin;
	current_frame = copy.current_frame;
	delta_frame = copy.delta_frame;
	update_time_ms = copy.update_time_ms;
	id = copy.id;
	finished = copy.finished;
	play_loop = copy.play_loop;
	play_backward = copy.play_backward;
	play_pingpong = copy.play_pingpong;
	show_on_finish = copy.show_on_finish;

	frames = copy.frames;

	return *this;
}

void Sprite_Impl::draw(Canvas &canvas, float x, float y)
{
	SpriteFrame &frame = frames[current_frame];

	Surface_DrawParams2 params2;
	params2.srcX = frame.position.left;
	params2.srcY = frame.position.top;
	params2.srcWidth = frame.position.get_width();
	params2.srcHeight = frame.position.get_height();
	params2.destX = x;
	params2.destY = y;
	params2.color = color;
	params2.scale_x = scale_x;
	params2.scale_y = scale_y;
	params2.translate_origin = translation_origin;
	params2.translate_x = translation_hotspot.x + frame.offset.x;
	params2.translate_y = translation_hotspot.y + frame.offset.y;
	params2.rotate_angle = angle - base_angle;
	params2.rotate_origin = rotation_origin;
	params2.rotate_x = rotation_hotspot.x + frame.offset.x;
	params2.rotate_y = rotation_hotspot.y + frame.offset.y;
	params2.sub_pixel_accuracy = true;
	draw(canvas, params2);
}

void Sprite_Impl::draw(Canvas &canvas, const Rectf &src, const Rectf &dest)
{
	SpriteFrame &frame = frames[current_frame];

	Surface_DrawParams2 params2;
	params2.srcX = frame.position.left + src.left;
	params2.srcY = frame.position.top + src.top;
	params2.srcWidth = src.get_width();
	params2.srcHeight = src.get_height();
	params2.destX = dest.left;
	params2.destY = dest.top;
	params2.color = color;
	params2.scale_x = dest.get_width() / src.get_width();
	params2.scale_y = dest.get_height() / src.get_height();
	params2.translate_origin = translation_origin;
	params2.translate_x = translation_hotspot.x + frame.offset.x;
	params2.translate_y = translation_hotspot.y + frame.offset.y;
	params2.rotate_angle = angle - base_angle;
	params2.rotate_origin = rotation_origin;
	params2.rotate_x = rotation_hotspot.x + frame.offset.x;
	params2.rotate_y = rotation_hotspot.y + frame.offset.y;
	params2.sub_pixel_accuracy = true;
	draw(canvas, params2);
}

void Sprite_Impl::draw(Canvas &canvas, const Rectf &dest)
{
	SpriteFrame &frame = frames[current_frame];

	Surface_DrawParams2 params2;
	params2.srcX = frame.position.left;
	params2.srcY = frame.position.top;
	params2.srcWidth = frame.position.get_width();
	params2.srcHeight = frame.position.get_height();
	params2.destX = dest.left;
	params2.destY = dest.top;
	params2.color = color;
	params2.scale_x = dest.get_width()/float(frame.position.get_width());
	params2.scale_y = dest.get_height()/float(frame.position.get_height());
	params2.translate_origin = translation_origin;
	params2.translate_x = translation_hotspot.x + frame.offset.x;
	params2.translate_y = translation_hotspot.y + frame.offset.y;
	params2.rotate_angle = angle - base_angle;
	params2.rotate_origin = rotation_origin;
	params2.rotate_x = rotation_hotspot.x + frame.offset.x;
	params2.rotate_y = rotation_hotspot.y + frame.offset.y;
	params2.sub_pixel_accuracy = true;
	draw(canvas, params2);
}

void Sprite_Impl::draw(Canvas &canvas, const Surface_DrawParams1 &params1)
{
	RenderBatchTriangle *batcher = canvas.impl->get_triangle_batcher();
	batcher->draw_sprite(canvas, &params1, frames[current_frame].texture);
}

void Sprite_Impl::draw(Canvas &canvas, const Surface_DrawParams2 &params2)
{
	Surface_TargetDrawParams1 t_params1;
	Surface_DrawParams1 params1;
	draw_calcs_step1(params2, t_params1);
	draw_calcs_step2(params2, &t_params1, params1);
	draw(canvas, params1);
}

void Sprite_Impl::draw_calcs_step1(
	const Surface_DrawParams2 &params2,
	Surface_TargetDrawParams1 &t_params1)
{
	// Find size of surface:
	float size_width  = (float) params2.srcWidth;
	float size_height = (float) params2.srcHeight;

	// Calculate translation hotspot
	Pointf translation_hotspot = calc_hotspot(
		params2.translate_origin,
		(float) params2.translate_x,
		(float) params2.translate_y,
		size_width,
		size_height);

	// Calculate rotation hotspot:
	t_params1.rotation_hotspot = calc_hotspot(
		params2.rotate_origin,
		(float) params2.rotate_x,
		(float) params2.rotate_y,
		size_width,
		size_height);

	if (!params2.sub_pixel_accuracy)
	{
		// Avoid linear smoothing:
		translation_hotspot.x = float(int(translation_hotspot.x));
		translation_hotspot.y = float(int(translation_hotspot.y));
		t_params1.rotation_hotspot.x = float(int(t_params1.rotation_hotspot.x));
		t_params1.rotation_hotspot.y = float(int(t_params1.rotation_hotspot.y));
	}

	// Find top left point of destination rectangle and map rotation hotspot to screen coordinates:
	if (params2.scale_x == 1.0 && params2.scale_y == 1.0)
	{
		t_params1.destWidth = (float)params2.srcWidth;
		t_params1.destHeight = (float)params2.srcHeight;
		t_params1.pixDestX = params2.destX-translation_hotspot.x;
		t_params1.pixDestY = params2.destY-translation_hotspot.y;
		t_params1.rotation_hotspot.x += float(t_params1.pixDestX);
		t_params1.rotation_hotspot.y += float(t_params1.pixDestY);
	}
	else
	{
		t_params1.destWidth = params2.srcWidth * params2.scale_x;
		t_params1.destHeight = params2.srcHeight * params2.scale_y;
		t_params1.pixDestX = params2.destX-translation_hotspot.x * params2.scale_x;
		t_params1.pixDestY = params2.destY-translation_hotspot.y * params2.scale_y;
		t_params1.rotation_hotspot.x = float(t_params1.pixDestX + t_params1.rotation_hotspot.x * params2.scale_x);
		t_params1.rotation_hotspot.y = float(t_params1.pixDestY + t_params1.rotation_hotspot.y * params2.scale_y);
	}
}

void Sprite_Impl::draw_calcs_step2(
	const Surface_DrawParams2 & params2,
	const Surface_TargetDrawParams1 *t_params1,
	Surface_DrawParams1 &params1)
{
	// Calculate unit vectors for rotated surface:
	// (cached for speed reasons)
	static float vect_rotate_x[2] = { 1.0f, 0.0f };
	static float vect_rotate_y[2] = { 0.0f, 1.0f };
	static Angle last_angle(0, angle_radians);

	if (last_angle != params2.rotate_angle)
	{
		if (params2.rotate_angle.to_degrees() == 0.0f)
		{
			vect_rotate_x[0] = 1.0;
			vect_rotate_x[1] = 0.0;
			vect_rotate_y[0] = 0.0;
			vect_rotate_y[1] = 1.0;
		}
		else if (params2.rotate_angle.to_degrees() == 90.0f)
		{
			vect_rotate_x[0] = 0.0;
			vect_rotate_x[1] = 1.0;
			vect_rotate_y[0] = -1.0;
			vect_rotate_y[1] = 0.0;
		}
		else if (params2.rotate_angle.to_degrees() == 180.0f)
		{
			vect_rotate_x[0] = -1.0;
			vect_rotate_x[1] = 0.0;
			vect_rotate_y[0] = 0.0;
			vect_rotate_y[1] = -1.0;
		}
		else if (params2.rotate_angle.to_degrees() == 270.0f)
		{
			vect_rotate_x[0] = 0.0;
			vect_rotate_x[1] = -1.0;
			vect_rotate_y[0] = 1.0;
			vect_rotate_y[1] = 0.0;
		}
		else
		{
			float angle_rad = params2.rotate_angle.to_radians();
			vect_rotate_x[0] = cos(angle_rad);
			vect_rotate_x[1] = sin(angle_rad);
			vect_rotate_y[0] = cos(PI/2+angle_rad);
			vect_rotate_y[1] = sin(PI/2+angle_rad);
		}
	}

	// Calculate final source rectangle points for render:
	const Texture2D &texture = frames[current_frame].texture;
	float texture_width = texture.get_width();
	float texture_height = texture.get_height();

	params1.texture_position[0].x = (((float) params2.srcX) ) / texture_width;
	params1.texture_position[1].x = (((float) params2.srcX+params2.srcWidth) ) / texture_width;
	params1.texture_position[2].x = (((float) params2.srcX) ) / texture_width;
	params1.texture_position[3].x = (((float) params2.srcX+params2.srcWidth) ) / texture_width;

	params1.texture_position[0].y = (((float) params2.srcY) ) / texture_height;
	params1.texture_position[1].y = (((float) params2.srcY) ) / texture_height;
	params1.texture_position[2].y = (((float) params2.srcY+params2.srcHeight) ) / texture_height;
	params1.texture_position[3].y = (((float) params2.srcY+params2.srcHeight) ) / texture_height;

	// Calculate final destination rectangle points for surface rectangle:
	if (params2.rotate_angle.to_radians() == 0.0f)
	{
		params1.dest_position[0].x = t_params1->pixDestX;
		params1.dest_position[1].x = t_params1->pixDestX+t_params1->destWidth;
		params1.dest_position[2].x = t_params1->pixDestX;
		params1.dest_position[3].x = t_params1->pixDestX+t_params1->destWidth;

		params1.dest_position[0].y = t_params1->pixDestY;
		params1.dest_position[1].y = t_params1->pixDestY;
		params1.dest_position[2].y = t_params1->pixDestY+t_params1->destHeight;
		params1.dest_position[3].y = t_params1->pixDestY+t_params1->destHeight;
	}
	else
	{
		// Roll
		params1.dest_position[0].x = calc_rotate_x(t_params1->pixDestX, t_params1->pixDestY, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[0], vect_rotate_y[0]);
		params1.dest_position[1].x = calc_rotate_x(t_params1->pixDestX+t_params1->destWidth, t_params1->pixDestY, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[0], vect_rotate_y[0]);
		params1.dest_position[2].x = calc_rotate_x(t_params1->pixDestX, t_params1->pixDestY+t_params1->destHeight, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[0], vect_rotate_y[0]);
		params1.dest_position[3].x = calc_rotate_x(t_params1->pixDestX+t_params1->destWidth, t_params1->pixDestY+t_params1->destHeight, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[0], vect_rotate_y[0]);

		params1.dest_position[0].y = calc_rotate_y(t_params1->pixDestX, t_params1->pixDestY, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[1], vect_rotate_y[1]);
		params1.dest_position[1].y = calc_rotate_y(t_params1->pixDestX+t_params1->destWidth, t_params1->pixDestY, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[1], vect_rotate_y[1]);
		params1.dest_position[2].y = calc_rotate_y(t_params1->pixDestX, t_params1->pixDestY+t_params1->destHeight, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[1], vect_rotate_y[1]);
		params1.dest_position[3].y = calc_rotate_y(t_params1->pixDestX+t_params1->destWidth, t_params1->pixDestY+t_params1->destHeight, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[1], vect_rotate_y[1]);
	}

	// Pitch
	if (params2.rotate_pitch.to_radians() != 0.0f)
	{
		float pitch_rad = sin(PI/2 + params2.rotate_pitch.to_radians());
		params1.dest_position[0].y = (params1.dest_position[0].y - t_params1->rotation_hotspot.y) * pitch_rad + t_params1->rotation_hotspot.y;
		params1.dest_position[1].y = (params1.dest_position[1].y - t_params1->rotation_hotspot.y) * pitch_rad + t_params1->rotation_hotspot.y;
		params1.dest_position[2].y = (params1.dest_position[2].y - t_params1->rotation_hotspot.y) * pitch_rad + t_params1->rotation_hotspot.y;
		params1.dest_position[3].y = (params1.dest_position[3].y - t_params1->rotation_hotspot.y) * pitch_rad + t_params1->rotation_hotspot.y;
	}
	// Yaw
	if (params2.rotate_yaw.to_radians() != 0.0f)
	{
		float yaw_rad = cos(params2.rotate_yaw.to_radians());
		params1.dest_position[0].x = (params1.dest_position[0].x - t_params1->rotation_hotspot.x) * yaw_rad + t_params1->rotation_hotspot.x;
		params1.dest_position[1].x = (params1.dest_position[1].x - t_params1->rotation_hotspot.x) * yaw_rad + t_params1->rotation_hotspot.x;
		params1.dest_position[2].x = (params1.dest_position[2].x - t_params1->rotation_hotspot.x) * yaw_rad + t_params1->rotation_hotspot.x;
		params1.dest_position[3].x = (params1.dest_position[3].x - t_params1->rotation_hotspot.x) * yaw_rad + t_params1->rotation_hotspot.x;
	}

	params1.color = params2.color;
}

void Sprite_Impl::create_textures(GraphicContext &gc, const SpriteDescription &description)
{
	// Fetch frames
	const std::vector<SpriteDescriptionFrame> &description_frames = description.get_frames();
	std::vector<SpriteDescriptionFrame>::const_iterator it_frames;

	for (it_frames = description_frames.begin(); it_frames != description_frames.end(); ++it_frames)
	{
		SpriteDescriptionFrame description_frame = (*it_frames);

		SpriteFrame frame;
		frame.position = description_frame.rect;
		frame.texture = description_frame.texture;
		frame.delay_ms = 60;
		frame.offset = Point(0, 0);
		frames.push_back(frame);
	}
}

}
