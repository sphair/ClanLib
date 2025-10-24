/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
*/

#include "Display/precomp.h"
#include "sprite_impl.h"
#include "sprite_render_batch.h"
#include "API/Display/2D/sprite_description.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/XML/dom_element.h"
#include "Display/Render/graphic_context_impl.h"
#include <list>

/////////////////////////////////////////////////////////////////////////////
// CL_SpriteData implementation:

CL_SpriteData::CL_SpriteData(const CL_StringRef &resource_id, CL_ResourceManager *resources)
{
	//TODO: Use the resources :)
}

/////////////////////////////////////////////////////////////////////////////
// CL_Sprite_Impl construction:

CL_Sprite_Impl::CL_Sprite_Impl(CL_GraphicContext &gc) :
	angle(CL_Angle(0.0f, cl_radians)),
	angle_pitch(CL_Angle(0.0f, cl_radians)),
	angle_yaw(CL_Angle(0.0f, cl_radians)),
	base_angle(CL_Angle(0.0f, cl_radians)),
	scale_x(1.0f),
	scale_y(1.0f),
	color(1.0f, 1.0f, 1.0f, 1.0f),
	linear_filter(false),
	translation_hotspot(0,0),
	rotation_hotspot(0,0),
	translation_origin(origin_top_left),
	rotation_origin(origin_center),
	current_frame(0),
	delta_frame(1),
	update_time(0),
	last_time(0),
	id(0),
	finished(false),
	play_loop(true),
	play_backward(false),
	play_pingpong(false),
	show_on_finish(CL_Sprite::show_blank),
	prim_array(0),
	gc(gc),
	texture_group(gc, CL_Size(1,1))
{
	for (int i=0; i<6; i++)
		prim_color[i] = CL_Vec4f(color.r,color.g,color.b,color.a);

	prim_array = new CL_PrimitivesArray(gc);
	prim_array->set_attributes(0, prim_dest);
	prim_array->set_attributes(1, prim_color);
	prim_array->set_attributes(2, prim_src);
}

CL_Sprite_Impl::~CL_Sprite_Impl()
{
	delete prim_array;
}

void CL_Sprite_Impl::init(const CL_StringRef &resource_id, CL_ResourceManager *resources, CL_SharedPtr<CL_SpriteData> sprite_data)
{
	data = sprite_data;

//	bool pack_texture = resource.get_element().get_attribute(cl_text("pack_texture"), cl_text("yes")) == cl_text("yes");
	CL_Resource resource = resources->get_resource(resource_id);

	// Create sprite from sprite description
	CL_SpriteDescription desc(gc, resource_id, resources);
	create_textures(desc);

	// Load base angle
	float work_angle = CL_StringHelp::text_to_float(resource.get_element().get_attribute(cl_text("base_angle"), cl_text("0")));
	base_angle= CL_Angle(work_angle, cl_degrees);

	// Load id
	id = CL_StringHelp::text_to_int(resource.get_element().get_attribute(cl_text("id"), cl_text("0")));

	// Load play options	
	CL_DomNode cur_node = resource.get_element().get_first_child();
	while (!cur_node.is_null())
	{
		if (!cur_node.is_element())
			continue;

		CL_DomElement cur_element = cur_node.to_element();

		CL_String tag_name = cur_element.get_tag_name();

		// <color red="float" green="float" blue="float" alpha="float" />
		if (tag_name == cl_text("color"))
		{
			color.r = (float)CL_StringHelp::text_to_float(cur_element.get_attribute(cl_text("red"), cl_text("1.0")));
			color.g = (float)CL_StringHelp::text_to_float(cur_element.get_attribute(cl_text("green"), cl_text("1.0")));
			color.b = (float)CL_StringHelp::text_to_float(cur_element.get_attribute(cl_text("blue"), cl_text("1.0")));
			color.a = (float)CL_StringHelp::text_to_float(cur_element.get_attribute(cl_text("alpha"), cl_text("1.0")));
		}
		// <animation speed="integer" loop="[yes,no]" pingpong="[yes,no]" direction="[backward,forward]" on_finish="[blank,last_frame,first_frame]"/>
		else if (tag_name == cl_text("animation"))
		{
			float delay = CL_StringHelp::text_to_float(cur_element.get_attribute(cl_text("speed"), cl_text("60"))) / 1000.0f;
			int frame_count = frames.size();
			for(int i=0; i<frame_count; ++i)
				get_frame(i)->delay = delay;

			play_loop = ((cur_element.get_attribute(cl_text("loop"), cl_text("yes"))) == cl_text("yes"));
			play_pingpong = ((cur_element.get_attribute(cl_text("pingpong"), cl_text("no"))) == cl_text("yes"));
			play_backward = ((cur_element.get_attribute(cl_text("direction"), cl_text("forward"))) == cl_text("backward"));

			CL_String on_finish = cur_element.get_attribute(cl_text("on_finish"), cl_text("blank"));
			if (on_finish == cl_text("first_frame"))
				show_on_finish = CL_Sprite::show_first_frame;
			else if(on_finish == cl_text("last_frame"))
				show_on_finish = CL_Sprite::show_last_frame;
			else
				show_on_finish = CL_Sprite::show_blank;
		}
		// <scale x="float" y="float />
		else if (tag_name == cl_text("scale"))
		{
			scale_x = CL_StringHelp::text_to_float(cur_element.get_attribute(cl_text("x"), cl_text("1.0")));
			scale_y = CL_StringHelp::text_to_float(cur_element.get_attribute(cl_text("y"), cl_text("1.0")));
		}
		// <translation origin="string" x="integer" y="integer" />
		else if (tag_name == cl_text("translation"))
		{
			CL_String hotspot = cur_element.get_attribute(cl_text("origin"), cl_text("top_left"));
			CL_Origin origin;

			if(hotspot == cl_text("center"))
				origin = origin_center;
			else if(hotspot == cl_text("top_center"))
				origin = origin_top_center;
			else if(hotspot == cl_text("top_right"))
				origin = origin_top_right;
			else if(hotspot == cl_text("center_left"))
				origin = origin_center_left;
			else if(hotspot == cl_text("center_right"))
				origin = origin_center_right;
			else if(hotspot == cl_text("bottom_left"))
				origin = origin_bottom_left;
			else if(hotspot == cl_text("bottom_center"))
				origin = origin_bottom_center;
			else if(hotspot == cl_text("bottom_right"))
				origin = origin_bottom_right;
			else
				origin = origin_top_left;

			int xoffset = CL_StringHelp::text_to_int(cur_element.get_attribute(cl_text("x"), cl_text("0")));
			int yoffset = CL_StringHelp::text_to_int(cur_element.get_attribute(cl_text("y"), cl_text("0")));

			translation_origin = origin;
			translation_hotspot.x = xoffset;
			translation_hotspot.y = yoffset;
		}
		// <rotation origin="string" x="integer" y="integer" />
		else if (tag_name == cl_text("rotation"))
		{
			CL_String hotspot = cur_element.get_attribute(cl_text("origin"), cl_text("center"));
			CL_Origin origin;

			if(hotspot == cl_text("top_left"))
				origin = origin_top_left;
			else if(hotspot == cl_text("top_center"))
				origin = origin_top_center;
			else if(hotspot == cl_text("top_right"))
				origin = origin_top_right;
			else if(hotspot == cl_text("center_left"))
				origin = origin_center_left;
			else if(hotspot == cl_text("center_right"))
				origin = origin_center_right;
			else if(hotspot == cl_text("bottom_left"))
				origin = origin_bottom_left;
			else if(hotspot == cl_text("bottom_center"))
				origin = origin_bottom_center;
			else if(hotspot == cl_text("bottom_right"))
				origin = origin_bottom_right;
			else
				origin = origin_center;

			int xoffset = CL_StringHelp::text_to_int(cur_element.get_attribute(cl_text("x"), cl_text("0")));
			int yoffset = CL_StringHelp::text_to_int(cur_element.get_attribute(cl_text("y"), cl_text("0")));

			rotation_origin = origin;
			rotation_hotspot.x = xoffset;
			rotation_hotspot.y = yoffset;
		}
		// <frame nr="integer" speed="integer" x="integer" y="integer" />
		else if (tag_name == cl_text("frame"))
		{
			int nr = CL_StringHelp::text_to_int(cur_element.get_attribute(cl_text("nr"), cl_text("0")));
			float delay = CL_StringHelp::text_to_float(cur_element.get_attribute(cl_text("speed"), cl_text("60"))) / 1000.0f;
			int yoffset = CL_StringHelp::text_to_int(cur_element.get_attribute(cl_text("y"), cl_text("0")));
			int xoffset = CL_StringHelp::text_to_int(cur_element.get_attribute(cl_text("x"), cl_text("0")));

			SpriteFrame *sptr = get_frame(nr);
			if (sptr == NULL)
			{
				throw CL_Exception(cl_text("Invalid sprite frame index specified"));
			}

			sptr->delay = delay;
			sptr->offset = CL_Point(xoffset, yoffset);
		}

		cur_node = cur_node.get_next_sibling();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_Sprite_Impl attributes:

const CL_Sprite_Impl::SpriteFrame *CL_Sprite_Impl::get_frame(unsigned int index) const
{
	if(index >= 0 && index < frames.size())
		return &frames[index];
	else
		return NULL;
}

CL_Sprite_Impl::SpriteFrame *CL_Sprite_Impl::get_frame(unsigned int index) 
{
	if(index >= 0 && index < frames.size())
		return &frames[index];
	else
		return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Sprite_Impl operations:

CL_Sprite_Impl &CL_Sprite_Impl::operator =(const CL_Sprite_Impl &copy)
{
	angle = copy.angle;
	angle_pitch = copy.angle_pitch;
	angle_yaw = copy.angle_yaw;
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
	update_time = copy.update_time;
	last_time = copy.last_time;
	id = copy.id;
	finished = copy.finished;
	play_loop = copy.play_loop;
	play_backward = copy.play_backward;
	play_pingpong = copy.play_pingpong;
	show_on_finish = copy.show_on_finish;
	gc = copy.gc;

	frames = copy.frames;

	return *this;
}

void CL_Sprite_Impl::draw(CL_GraphicContext &gc, float x, float y)
{
	SpriteFrame &frame = frames[current_frame];
	CL_Sizef tex_size((float)frame.texture.get_width(0), (float)frame.texture.get_height(0));

	CL_Surface_DrawParams2 params2;
	params2.srcX = frame.position.left;
	params2.srcY = frame.position.top;
	params2.srcWidth = frame.position.get_width();
	params2.srcHeight = frame.position.get_height();
	params2.destX = x;
	params2.destY = y;
	params2.destZ = 1.0;
	params2.color = color;
	params2.scale_x = scale_x;
	params2.scale_y = scale_y;
	params2.translate_origin = translation_origin;
	params2.translate_x = translation_hotspot.x + frame.offset.x;
	params2.translate_y = translation_hotspot.y + frame.offset.y;
	params2.rotate_angle = angle - base_angle;
	params2.rotate_pitch = angle_pitch;
	params2.rotate_yaw = angle_yaw;
	params2.rotate_origin = rotation_origin;
	params2.rotate_x = rotation_hotspot.x + frame.offset.x;
	params2.rotate_y = rotation_hotspot.y + frame.offset.y;
	params2.sub_pixel_accuracy = true;
	draw(gc, params2, tex_size);
}

void CL_Sprite_Impl::draw(CL_GraphicContext &gc, const CL_Rectf &src, const CL_Rectf &dest)
{
	SpriteFrame &frame = frames[current_frame];
	CL_Sizef tex_size((float)frame.texture.get_width(0), (float)frame.texture.get_height(0));

	CL_Surface_DrawParams1 params1;
	params1.srcX[0] = (int)src.left;
	params1.srcX[1] = (int)src.right;
	params1.srcX[2] = (int)src.left;
	params1.srcX[3] = (int)src.right;
	params1.srcY[0] = (int)src.top;
	params1.srcY[1] = (int)src.top;
	params1.srcY[2] = (int)src.bottom;
	params1.srcY[3] = (int)src.bottom;
	params1.destX[0] = dest.left;
	params1.destX[1] = dest.right;
	params1.destX[2] = dest.left;
	params1.destX[3] = dest.right;
	params1.destY[0] = dest.top;
	params1.destY[1] = dest.top;
	params1.destY[2] = dest.bottom;
	params1.destY[3] = dest.bottom;
	for (int i = 0; i < 4; i++)
	{
		params1.color[i] = CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f);
	}
	params1.sub_pixel_accuracy = true;
	params1.destZ = 1.0;
	draw(gc, params1, tex_size);
}

void CL_Sprite_Impl::draw(CL_GraphicContext &gc, const CL_Rectf &dest)
{
	SpriteFrame &frame = frames[current_frame];
	CL_Sizef tex_size((float)frame.texture.get_width(0), (float)frame.texture.get_height(0));

	CL_Surface_DrawParams2 params2;
	params2.srcX = frame.position.left;
	params2.srcY = frame.position.top;
	params2.srcWidth = frame.position.get_width();
	params2.srcHeight = frame.position.get_height();
	params2.destX = dest.left;
	params2.destY = dest.top;
	params2.destZ = 1.0;
	params2.color = color;
	params2.scale_x = dest.get_width()/float(frame.position.get_width());
	params2.scale_y = dest.get_height()/float(frame.position.get_height());
	params2.translate_origin = translation_origin;
	params2.translate_x = translation_hotspot.x + frame.offset.x;
	params2.translate_y = translation_hotspot.y + frame.offset.y;
	params2.rotate_angle = angle - base_angle;
	params2.rotate_pitch = angle_pitch;
	params2.rotate_yaw = angle_yaw;
	params2.rotate_origin = rotation_origin;
	params2.rotate_x = rotation_hotspot.x + frame.offset.x;
	params2.rotate_y = rotation_hotspot.y + frame.offset.y;
	params2.sub_pixel_accuracy = true;
	draw(gc, params2, tex_size);
}

void CL_Sprite_Impl::draw(CL_GraphicContext &gc, const CL_Surface_DrawParams1 &params1, const CL_Sizef &tex_size)
{
	CL_SpriteRenderBatch *batcher = &gc.impl->sprite_batcher;
	batcher->draw_sprite(gc, &params1, frames[current_frame].texture);
}

void CL_Sprite_Impl::draw(CL_GraphicContext &gc, const CL_Surface_DrawParams2 &params2, const CL_Sizef &tex_size)
{
	CL_Surface_TargetDrawParams1 t_params1;
	CL_Surface_DrawParams1 params1;
	draw_calcs_step1(params2, t_params1);
	draw_calcs_step2(params2, &t_params1, params1);
	draw(gc, params1, tex_size);
}

void CL_Sprite_Impl::draw_calcs_step1(
	const CL_Surface_DrawParams2 &params2,
	CL_Surface_TargetDrawParams1 &t_params1)
{
	// Find size of surface:
	float size_width  = (float) params2.srcWidth;
	float size_height = (float) params2.srcHeight;

	// Calculate translation hotspot
	CL_Pointf translation_hotspot = calc_hotspot(
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


void CL_Sprite_Impl::draw_calcs_step2(
	const CL_Surface_DrawParams2 & params2,
	const CL_Surface_TargetDrawParams1 *t_params1,
	CL_Surface_DrawParams1 &params1)
{
	// Calculate unit vectors for rotated surface:
	// (cached for speed reasons)
	static float vect_rotate_x[2] = { 1.0f, 0.0f };
	static float vect_rotate_y[2] = { 0.0f, 1.0f };
	static CL_Angle last_angle(0, cl_radians);

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
			vect_rotate_y[0] = cos(CL_PI/2+angle_rad);
			vect_rotate_y[1] = sin(CL_PI/2+angle_rad);
		}
	}

	// Calculate final source rectangle points for render:

	params1.srcX[0] = params2.srcX;
	params1.srcX[1] = params2.srcX+params2.srcWidth;
	params1.srcX[2] = params2.srcX;
	params1.srcX[3] = params2.srcX+params2.srcWidth;

	params1.srcY[0] = params2.srcY;
	params1.srcY[1] = params2.srcY;
	params1.srcY[2] = params2.srcY+params2.srcHeight;
	params1.srcY[3] = params2.srcY+params2.srcHeight;

	// Calculate final destination rectangle points for surface rectangle:

	if (params2.rotate_angle.to_radians() == 0.0f)
	{
		params1.destX[0] = t_params1->pixDestX;
		params1.destX[1] = t_params1->pixDestX+t_params1->destWidth;
		params1.destX[2] = t_params1->pixDestX;
		params1.destX[3] = t_params1->pixDestX+t_params1->destWidth;

		params1.destY[0] = t_params1->pixDestY;
		params1.destY[1] = t_params1->pixDestY;
		params1.destY[2] = t_params1->pixDestY+t_params1->destHeight;
		params1.destY[3] = t_params1->pixDestY+t_params1->destHeight;
	}
	else
	{
		// roll
		params1.destX[0] = calc_rotate_x(t_params1->pixDestX, t_params1->pixDestY, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[0], vect_rotate_y[0]);
		params1.destX[1] = calc_rotate_x(t_params1->pixDestX+t_params1->destWidth, t_params1->pixDestY, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[0], vect_rotate_y[0]);
		params1.destX[2] = calc_rotate_x(t_params1->pixDestX, t_params1->pixDestY+t_params1->destHeight, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[0], vect_rotate_y[0]);
		params1.destX[3] = calc_rotate_x(t_params1->pixDestX+t_params1->destWidth, t_params1->pixDestY+t_params1->destHeight, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[0], vect_rotate_y[0]);

		params1.destY[0] = calc_rotate_y(t_params1->pixDestX, t_params1->pixDestY, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[1], vect_rotate_y[1]);
		params1.destY[1] = calc_rotate_y(t_params1->pixDestX+t_params1->destWidth, t_params1->pixDestY, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[1], vect_rotate_y[1]);
		params1.destY[2] = calc_rotate_y(t_params1->pixDestX, t_params1->pixDestY+t_params1->destHeight, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[1], vect_rotate_y[1]);
		params1.destY[3] = calc_rotate_y(t_params1->pixDestX+t_params1->destWidth, t_params1->pixDestY+t_params1->destHeight, t_params1->rotation_hotspot.x, t_params1->rotation_hotspot.y, vect_rotate_x[1], vect_rotate_y[1]);
	}

	// pitch
	if (params2.rotate_pitch.to_radians() != 0.0f)
	{
		float pitch_rad = sin(CL_PI/2 + params2.rotate_pitch.to_radians());
		params1.destY[0] = (params1.destY[0] - t_params1->rotation_hotspot.y) * pitch_rad + t_params1->rotation_hotspot.y;
		params1.destY[1] = (params1.destY[1] - t_params1->rotation_hotspot.y) * pitch_rad + t_params1->rotation_hotspot.y;
		params1.destY[2] = (params1.destY[2] - t_params1->rotation_hotspot.y) * pitch_rad + t_params1->rotation_hotspot.y;
		params1.destY[3] = (params1.destY[3] - t_params1->rotation_hotspot.y) * pitch_rad + t_params1->rotation_hotspot.y;
	}
	// yaw
	if (params2.rotate_yaw.to_radians() != 0.0f)
	{
		float yaw_rad = cos(params2.rotate_yaw.to_radians());
		params1.destX[0] = (params1.destX[0] - t_params1->rotation_hotspot.x) * yaw_rad + t_params1->rotation_hotspot.x;
		params1.destX[1] = (params1.destX[1] - t_params1->rotation_hotspot.x) * yaw_rad + t_params1->rotation_hotspot.x;
		params1.destX[2] = (params1.destX[2] - t_params1->rotation_hotspot.x) * yaw_rad + t_params1->rotation_hotspot.x;
		params1.destX[3] = (params1.destX[3] - t_params1->rotation_hotspot.x) * yaw_rad + t_params1->rotation_hotspot.x;
	}

	params1.color[0] = params2.color;
	params1.color[1] = params2.color;
	params1.color[2] = params2.color;
	params1.color[3] = params2.color;

	params1.destZ = params2.destZ;
	params1.sub_pixel_accuracy = params2.sub_pixel_accuracy;
}

float CL_Sprite_Impl::calc_time_elapsed()
{
	// Calculate amount of time since last frame
	float new_time = (float)CL_System::get_time();
	if(last_time == 0)
		last_time = new_time;

	float delta_time = (new_time - last_time) / 1000.0f;
	last_time = new_time;

	return delta_time;
}

void CL_Sprite_Impl::create_textures(const CL_SpriteDescription &description)
{
	// Fetch frames
	const std::vector<CL_SpriteDescriptionFrame> &description_frames = description.get_frames();
	std::vector<CL_SpriteDescriptionFrame>::const_iterator it_frames;

	// Calculate estimated texture group size
	if (texture_group.get_texture_sizes().width <=1)
	{
		// *** This algorithm may not work! ***
		int max_width = 1;
		int max_height = 1;
		int num_objects = 0;
		int min_width = 9999999;
		int min_height = min_width;
		int total_width = 1;

		const int allowed_pixel_gap = 4;

		for (it_frames = description_frames.begin(); it_frames != description_frames.end(); ++it_frames)
		{
			CL_SpriteDescriptionFrame description_frame = (*it_frames);
			int width = description_frame.rect.get_width();
			int height = description_frame.rect.get_width();

			if (max_width < width) max_width = width;
			if (max_height < height) max_height = height;

			if (min_width > width) min_width = width;
			if (min_height > height) min_height = height;

			total_width += width + allowed_pixel_gap;

			num_objects++;
		}

		int group_width = total_width;
		int group_height = max_height;

		// Check all will fit into a single line
		const int max_group_size = 512;
		if (group_width > max_group_size)
		{
			group_width = max_group_size;
			group_height *= (total_width + max_group_size - 1) / max_group_size;
		}

		// Ensure power of 2 - width
		if (group_width > 256)
			group_width = 512;
		else if (group_width > 128)
			group_width = 256;
		else if (group_width > 64)
			group_width = 128;
		else if (group_width > 32)
			group_width = 64;
		else group_width = 32;

		// Ensure power of 2 - height
		if (group_height > 256)
			group_height = 512;
		else if (group_height > 128)
			group_height = 256;
		else if (group_height > 64)
			group_height = 128;
		else if (group_height > 32)
			group_height = 64;
		else group_height = 32;

		// Only create group if an object will definately fit into it
		if ( (group_width >= min_width) && (group_height >= min_height) && (num_objects > 1) )
		{
			texture_group = CL_TextureGroup(gc, CL_Size(group_width, group_height));
		}
	}

	for (it_frames = description_frames.begin(); it_frames != description_frames.end(); ++it_frames)
	{
		CL_SpriteDescriptionFrame description_frame = (*it_frames);

		if(description_frame.type == CL_SpriteDescriptionFrame::type_pixelbuffer)
		{
			CL_PixelBuffer image = description_frame.pixelbuffer;
			if (description_frame.rect.get_width() <= texture_group.get_texture_sizes().width &&
				description_frame.rect.get_height() <= texture_group.get_texture_sizes().height)
			{
				CL_Subtexture subtexture = texture_group.add(gc, description_frame.rect.get_size());
				subtexture.get_texture().set_subimage(subtexture.get_geometry().get_top_left(), image.get_subimage(description_frame.rect));
				subtexture.get_texture().set_mag_filter(linear_filter ? cl_filter_linear : cl_filter_nearest);
				subtexture.get_texture().set_min_filter(linear_filter ? cl_filter_linear : cl_filter_nearest);

				SpriteFrame frame;
				frame.position = subtexture.get_geometry();
				frame.texture = subtexture.get_texture();
				frame.delay = 0.05f;
				frame.offset = CL_Point(0, 0);
				frames.push_back(frame);
			}
			else
			{
				int width = description_frame.rect.get_width();
				int height = description_frame.rect.get_height();

				// Note, forced power of 2 textures have clamping issues
				int texture_width = width;
				int texture_height = height;

				CL_Texture texture(gc, texture_width, texture_height);
				texture.set_subimage(CL_Point(0,0), image.get_subimage(description_frame.rect));
				texture.set_mag_filter(linear_filter ? cl_filter_linear : cl_filter_nearest);
				texture.set_min_filter(linear_filter ? cl_filter_linear : cl_filter_nearest);

				SpriteFrame frame;
				frame.position = CL_Rect(0, 0, width, height);
				frame.texture = texture;
				frame.delay = 0.05f;
				frame.offset = CL_Point(0, 0);
				frames.push_back(frame);
			}
		}
		else if(description_frame.type == CL_SpriteDescriptionFrame::type_texture)
		{
			SpriteFrame frame;
			frame.position = description_frame.rect;
			frame.texture = description_frame.texture;
			frame.delay = 0.05f;
			frame.offset = CL_Point(0, 0);
			frames.push_back(frame);
		}
	}
		/*
	if (pack_texture && CL_DisplayTarget::current()->enable_packer())
	{
		// Fetch max texture size
		CL_DisplayWindow *window = CL_Display::get_current_window();
		CL_Size max_surface_size = window->get_gc()->get_max_surface_size();

		// Pack frames into textures
		CL_SpritePacker packer(spritedescription);
		if(packer.pack(max_surface_size.width, max_surface_size.height) == false)
			throw CL_Exception("Couldn't pack all frames into textures");

		// Fetch texture, description-frame and packed-frame lists
		const std::list<CL_SpriteDescription::FramePair> &frames = spritedescription.get_frames();
		std::list<CL_SpriteDescription::FramePair>::const_iterator it_frames;

		// Lock all pixelbuffers to avoid multiple loadings of same pb's:
		for (it_frames = frames.begin(); it_frames != frames.end(); ++it_frames)
		{
			CL_PixelBuffer buffer = (*it_frames).first;
			buffer.lock();
		}
		
		const std::list<CL_SpritePacker::TexturePair> &packed_frames = packer.get_frames();
		std::list<CL_SpritePacker::TexturePair>::const_iterator it_packed_frames;
		
		const std::vector<CL_Size> &texture_sizes = packer.get_texture_sizes();

		std::vector<CL_Surface> surfaces;
			
		// Set up a pixelbuffer format
		CL_PixelFormat format;
		format.set_depth(32);
		format.enable_colorkey(false);
		format.set_alpha_mask(0xFF000000);
		format.set_blue_mask (0x00FF0000);
		format.set_green_mask(0x0000FF00);
		format.set_red_mask  (0x000000FF);

		// Create all the textures needed for packing
//		cl_log_event("debug", "New sprite:");
		for(unsigned int i = 0; i < texture_sizes.size(); i++)
		{
			int width = texture_sizes[i].width;
			int height = texture_sizes[i].height;
			
			CL_PixelBuffer buffer(width, height, 4 * width, format);
			buffer.lock();

//			cl_log_event("debug", "- Packing texture %1x%2", width, height);

			// Copy frames into this texture
			for(it_frames = frames.begin(), it_packed_frames = packed_frames.begin();
				it_frames != frames.end();
				++it_frames, ++it_packed_frames)
			{
				if((*it_packed_frames).first == static_cast<int>(i))	// Frame uses this buffer
				{
					CL_PixelBuffer src = (*it_frames).first;
					CL_PixelBuffer dest = buffer;

					CL_Rect src_rect  = (*it_frames).second;
					CL_Rect dest_rect = (*it_packed_frames).second;

					// Remove border around image:
					dest_rect.left++;
					dest_rect.right--;
					dest_rect.top++;
					dest_rect.bottom--;
					
					src.convert(dest.get_data(), format, dest.get_pitch(), dest_rect, src_rect);

					int x, y;
					int dest_pitch = dest.get_pitch();
					unsigned char *data_bytes = (unsigned char*) dest.get_data();

					// Make transparent pixels grey:
					for (y=dest_rect.top; y<dest_rect.bottom; y++)
					{
						unsigned int *line = (unsigned int *) (data_bytes+y*dest_pitch);
						for (x=dest_rect.left; x<dest_rect.right; x++)
						{
							// Test for transparent pixels surrounded by non transparent ones
							if ((line[x] & 0xff000000) == 0)
							{
								int r=0,g=0,b=0,n=0;
								if((line[x+1] & 0xff000000) != 0)
								{
									r+= (line[x+1] & 0x00ff0000) >> 16;
									g+= (line[x+1] & 0x0000ff00) >> 8;
									b+= (line[x+1] & 0x000000ff);
									n++;
								}
								if((line[x-1] & 0xff000000) != 0)
								{
									r+= (line[x-1] & 0x00ff0000) >> 16;
									g+= (line[x-1] & 0x0000ff00) >> 8;
									b+= (line[x-1] & 0x000000ff);
									n++;
								}
								line = (unsigned int *) (data_bytes+(y+1)*dest_pitch);
								if((line[x] & 0xff000000) != 0)
								{
									r+= (line[x] & 0x00ff0000) >> 16;
									g+= (line[x] & 0x0000ff00) >> 8;
									b+= (line[x] & 0x000000ff);
									n++;
								}
								line = (unsigned int *) (data_bytes+(y-1)*dest_pitch);
								if((line[x] & 0xff000000) != 0)
								{
									r+= (line[x] & 0x00ff0000) >> 16;
									g+= (line[x] & 0x0000ff00) >> 8;
									b+= (line[x] & 0x000000ff);
									n++;
								}

								line = (unsigned int *) (data_bytes+y*dest_pitch);
								if(n>0)
								{
									// Set colour to the avarage of the found pixels
									line[x] = ( ((r/n)<<16) | ((g/n)<<8) | (b/n) );
								}
								else line[x] = 0x007f7f7f;
							}
						}
					}

					// Copy pixels in border area:
					unsigned int *top_line = (unsigned int *) (data_bytes+(dest_rect.top-1)*dest_pitch);
					unsigned int *next_line = (unsigned int *) (data_bytes+dest_rect.top*dest_pitch);
					unsigned int *bottom_line = (unsigned int *) (data_bytes+(dest_rect.bottom)*dest_pitch);
					unsigned int *prev_line = (unsigned int *) (data_bytes+(dest_rect.bottom-1)*dest_pitch);
					for (x=dest_rect.left; x<dest_rect.right; x++)
					{
						top_line[x] = next_line[x];
						bottom_line[x] = prev_line[x];
					}
					for (y=dest_rect.top; y<dest_rect.bottom; y++)
					{
						unsigned int *line = (unsigned int *) (data_bytes+y*dest_pitch);
						line[dest_rect.left-1] = line[dest_rect.left];
						line[dest_rect.right] = line[dest_rect.right-1];
					}
					#define dest_pixel(x,y) (*((unsigned int *) (data_bytes+(y)*dest_pitch)+x))
					dest_pixel(dest_rect.left-1, dest_rect.top-1) = dest_pixel(dest_rect.left, dest_rect.top);
					dest_pixel(dest_rect.right, dest_rect.top-1) = dest_pixel(dest_rect.right-1, dest_rect.top);
					dest_pixel(dest_rect.left-1, dest_rect.bottom) = dest_pixel(dest_rect.left, dest_rect.bottom-1);
					dest_pixel(dest_rect.right, dest_rect.bottom) = dest_pixel(dest_rect.right-1, dest_rect.bottom-1);
				}
			}

			// Create CL_Surface
			surfaces.push_back(CL_Surface(buffer, spritedescription.get_surface_flag()));

			// Clean up
			buffer.unlock();
		}

		// Create SpriteFrames
		for(it_frames = frames.begin(), it_packed_frames = packed_frames.begin();
			it_frames != frames.end();
			++it_frames, ++it_packed_frames)
		{
			CL_Sprite_Generic::SpriteFrame frame;
			
			frame.surface = surfaces[(*it_packed_frames).first];
			frame.position = (*it_packed_frames).second;
			frame.delay = 0.06f;
			frame.offset = CL_Point(0, 0);
			
			// Remove border around image:
			frame.position.left++;
			frame.position.right--;
			frame.position.top++;
			frame.position.bottom--;
			
			impl->frames.push_back(frame);
		}

		// Unlock all pixelbuffers:
		for (it_frames = frames.begin(); it_frames != frames.end(); ++it_frames)
		{
			CL_PixelBuffer buffer = (*it_frames).first;
			buffer.unlock();
		}
	}
	else
	{
		// Create SpriteFrames
		const std::list<CL_SpriteDescription::FramePair> &frames = spritedescription.get_frames();
		std::list<CL_SpriteDescription::FramePair>::const_iterator it_frames;
		std::map<CL_PixelBuffer, CL_Surface> surfaces;

		for(it_frames = frames.begin();
			it_frames != frames.end();
			++it_frames)
		{	
			CL_Sprite_Generic::SpriteFrame frame;

			std::map<CL_PixelBuffer, CL_Surface>::iterator i = surfaces.find((*it_frames).first);
			if (i == surfaces.end())
			{
				frame.surface  = CL_Surface((*it_frames).first);
				surfaces[(*it_frames).first] = frame.surface;
			}
			else
			{
				frame.surface = i->second;
			}

			frame.position = (*it_frames).second;
			frame.delay = 0.06f;
			frame.offset = CL_Point(0, 0);

			impl->frames.push_back(frame);
		}
	}
*/
}

