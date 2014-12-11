/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
**    Harry Storbacka
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Core/IOData/file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/Resources/xml_resource_node.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/2D/sprite.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/Render/primitives_array.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/ImageProviders/provider_factory.h"
#include "API/Display/ImageProviders/targa_provider.h"
#include "API/Display/ImageProviders/png_provider.h"
#include "API/Display/Collision/collision_outline.h"
#include "sprite_impl.h"
#include "render_batch_triangle.h"
#include "API/Display/Resources/display_cache.h"
#include "API/Display/2D/subtexture.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Sprite Construction:

Sprite::Sprite()
{
}

Sprite::Sprite(Canvas &canvas, const std::string &fullname, const ImageImportDescription &import_desc)
{
	std::string path = PathHelp::get_fullpath(fullname, PathHelp::path_type_file);
	std::string filename = PathHelp::get_filename(fullname, PathHelp::path_type_file);
	FileSystem vfs(path);
	*this = Sprite(canvas, filename, vfs, import_desc);
}

Sprite::Sprite(Canvas &canvas, const std::string &filename, const FileSystem &fs, const ImageImportDescription &import_desc)
: impl(std::make_shared<Sprite_Impl>())
{
	add_frame(canvas, filename, fs, import_desc );
	restart();
}

Sprite::Sprite(Canvas &canvas, IODevice &file, const std::string &image_type, const ImageImportDescription &import_desc )
: impl(std::make_shared<Sprite_Impl>())
{
	add_frame(canvas, file, image_type, import_desc );
	restart();
}

Sprite::Sprite(Canvas &canvas)
: impl(std::make_shared<Sprite_Impl>())
{
}

Sprite::~Sprite()
{
}

/////////////////////////////////////////////////////////////////////////////
// Sprite Resources:

Resource<Sprite> Sprite::resource(Canvas &canvas, const std::string &id, const ResourceManager &resources)
{
	return DisplayCache::get(resources).get_sprite(canvas, id);
}

Sprite Sprite::load(Canvas &canvas, const std::string &id, const XMLResourceDocument &doc)
{
	Sprite sprite(canvas);

 	XMLResourceNode resource = doc.get_resource(id);
	if (resource.get_type() != "sprite")
		throw Exception(string_format("Resource '%1' is not of type 'sprite'", id));

	// Load base angle
	float work_angle = StringHelp::text_to_float(resource.get_element().get_attribute("base_angle", "0"));
	sprite.set_base_angle(Angle(work_angle, angle_degrees));

	// Load id
	sprite.set_id(StringHelp::text_to_int(resource.get_element().get_attribute("id", "0")));

	// Load play options	
	DomNode cur_node = resource.get_element().get_first_child();
	while (!cur_node.is_null())
	{
		if (!cur_node.is_element())
		{
			cur_node = cur_node.get_next_sibling();
			continue;
		}

		DomElement cur_element = cur_node.to_element();

		std::string tag_name = cur_element.get_tag_name();

		if (tag_name == "image" || tag_name == "image-file")
		{
			if (cur_element.has_attribute("fileseq"))
			{
				int start_index = 0;
				if (cur_element.has_attribute("start_index"))
					start_index = StringHelp::text_to_int(cur_element.get_attribute("start_index"));

				int skip_index = 1;
				if (cur_element.has_attribute("skip_index"))
					skip_index = StringHelp::text_to_int(cur_element.get_attribute("skip_index"));

				int leading_zeroes = 0;
				if (cur_element.has_attribute("leading_zeroes"))
					leading_zeroes =  StringHelp::text_to_int(cur_element.get_attribute("leading_zeroes"));

				std::string prefix = cur_element.get_attribute("fileseq");
				std::string suffix = "." + PathHelp::get_extension(prefix);
				prefix.erase(prefix.length() - suffix.length(), prefix.length()); //remove the extension

				FileSystem fs = resource.get_file_system();

				bool found_initial = false;
				for (int i = start_index;; i = skip_index)
				{
					std::string file_name = prefix;

					std::string frame_text = StringHelp::int_to_text(i);
					for (int zeroes_to_add = (leading_zeroes + 1) - frame_text.length(); zeroes_to_add > 0; zeroes_to_add--)
						file_name = "0";

					file_name = frame_text + suffix;

					try
					{
						Texture2D texture = Texture2D(canvas, PathHelp::combine(resource.get_base_path(), file_name), fs);
						sprite.add_frame(texture);
						found_initial = true;
					}
					catch (const Exception&)
					{
						if (!found_initial)
						{
							//must have been an error, pass it down
							throw;
						}
						//can't find anymore pics
						break;
					}
				}
			}
			else
			{
				std::string image_name = cur_element.get_attribute("file");
				FileSystem fs = resource.get_file_system();
				Texture2D texture = Texture2D(canvas, PathHelp::combine(resource.get_base_path(), image_name), fs);

				DomNode cur_child(cur_element.get_first_child());
				if(cur_child.is_null()) 
				{
					sprite.add_frame(texture);
				}
				else 
				{
					do {
						DomElement cur_child_elemnt = cur_child.to_element();
						if(cur_child.get_node_name() == "grid")
						{
							int xpos = 0;
							int ypos = 0;
							int xarray = 1;
							int yarray = 1;
							int array_skipframes = 0;
							int xspacing = 0;
							int yspacing = 0;
							int width = 0;
							int height = 0;

							std::vector<std::string> image_size = StringHelp::split_text(cur_child_elemnt.get_attribute("size"), ",");
							if (image_size.size() > 0)
								width = StringHelp::text_to_int(image_size[0]);
							if (image_size.size() > 1)
								height = StringHelp::text_to_int(image_size[1]);

							if (cur_child_elemnt.has_attribute("pos"))
							{
								std::vector<std::string> image_pos = StringHelp::split_text(cur_child_elemnt.get_attribute("pos"), ",");
								if (image_pos.size() > 0)
									xpos = StringHelp::text_to_int(image_pos[0]);
								if (image_pos.size() > 1)
									ypos = StringHelp::text_to_int(image_pos[1]);
							}

							if (cur_child_elemnt.has_attribute("array"))
							{
								std::vector<std::string> image_array = StringHelp::split_text(cur_child_elemnt.get_attribute("array"), ",");
								if (image_array.size() == 2)
								{
									xarray = StringHelp::text_to_int(image_array[0]);
									yarray = StringHelp::text_to_int(image_array[1]);
								}
								else
								{
									throw Exception("Resource '" + resource.get_name() + "' has incorrect array attribute, must be \"X,Y\"!"); 
								}
							}

							if (cur_child_elemnt.has_attribute("array_skipframes"))
							{
								array_skipframes = StringHelp::text_to_int(cur_child_elemnt.get_attribute("array_skipframes"));
							}

							if (cur_child_elemnt.has_attribute("spacing"))
							{
								std::vector<std::string> image_spacing = StringHelp::split_text(cur_child_elemnt.get_attribute("spacing"), ",");
								xspacing = StringHelp::text_to_int(image_spacing[0]);
								yspacing = StringHelp::text_to_int(image_spacing[1]);
							}

							sprite.add_gridclipped_frames(canvas, 
								texture,
								xpos, ypos,
								width, height,
								xarray, yarray,
								array_skipframes,
								xspacing, yspacing);
						}
						else if( cur_child.get_node_name() == "palette")
						{
							throw Exception("Resource '" + resource.get_name() + "' uses palette cutter - which is not supported anymore"); 
						}
						else if( cur_child.get_node_name() == "alpha")
						{
							int xpos = 0;
							int ypos = 0;
							float trans_limit = 0.05f;

							if (cur_child_elemnt.has_attribute("pos"))
							{
								std::vector<std::string> image_pos = StringHelp::split_text(cur_child_elemnt.get_attribute("pos"), ",");
								xpos = StringHelp::text_to_int(image_pos[0]);
								ypos = StringHelp::text_to_int(image_pos[1]);
							}

							if (cur_child_elemnt.has_attribute("trans_limit"))
							{
								trans_limit = StringHelp::text_to_float(cur_child_elemnt.get_attribute("trans_limit"));
							}

							if (cur_child_elemnt.has_attribute("free"))
							{
								sprite.add_alphaclipped_frames_free(canvas, 
									texture,
									xpos, ypos,
									trans_limit);
							}
							else
							{
								sprite.add_alphaclipped_frames(canvas, 
									texture,
									xpos, ypos,
									trans_limit);
							}
						}

						cur_child = cur_child.get_next_sibling();
					} while(!cur_child.is_null());
				}
			}
		}
		cur_node = cur_node.get_next_sibling();
	}

	cur_node = resource.get_element().get_first_child();
	while (!cur_node.is_null())
	{
		if (!cur_node.is_element())
		{
			cur_node = cur_node.get_next_sibling();
			continue;
		}

		DomElement cur_element = cur_node.to_element();

		std::string tag_name = cur_element.get_tag_name();

		// <color red="float" green="float" blue="float" alpha="float" />
		if (tag_name == "color")
		{
			Colorf color;
			color.r = (float)StringHelp::text_to_float(cur_element.get_attribute("red", "1.0"));
			color.g = (float)StringHelp::text_to_float(cur_element.get_attribute("green", "1.0"));
			color.b = (float)StringHelp::text_to_float(cur_element.get_attribute("blue", "1.0"));
			color.a = (float)StringHelp::text_to_float(cur_element.get_attribute("alpha", "1.0"));
			sprite.set_color(color);
		}
		// <animation speed="integer" loop="[yes,no]" pingpong="[yes,no]" direction="[backward,forward]" on_finish="[blank,last_frame,first_frame]"/>
		else if (tag_name == "animation")
		{
			int delay_ms = StringHelp::text_to_int(cur_element.get_attribute("speed", "60"));

			int frame_count = sprite.get_frame_count();
			for(int i=0; i<frame_count; ++i)
				sprite.set_frame_delay(i, delay_ms);

			sprite.set_play_loop((cur_element.get_attribute("loop", "yes")) == "yes");
			sprite.set_play_pingpong((cur_element.get_attribute("pingpong", "no")) == "yes");
			sprite.set_play_backward((cur_element.get_attribute("direction", "forward")) == "backward");

			std::string on_finish = cur_element.get_attribute("on_finish", "blank");
			if (on_finish == "first_frame")
				sprite.set_show_on_finish(Sprite::show_first_frame);
			else if(on_finish == "last_frame")
				sprite.set_show_on_finish(Sprite::show_last_frame);
			else
				sprite.set_show_on_finish(Sprite::show_blank);
		}
		// <scale x="float" y="float />
		else if (tag_name == "scale")
		{
			float scale_x = StringHelp::text_to_float(cur_element.get_attribute("x", "1.0"));
			float scale_y = StringHelp::text_to_float(cur_element.get_attribute("y", "1.0"));
			sprite.set_scale(scale_x, scale_y);
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

			sprite.set_alignment(origin, xoffset, yoffset);
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

			sprite.set_rotation_hotspot(origin, xoffset, yoffset);
		}
		// <frame nr="integer" speed="integer" x="integer" y="integer" />
		else if (tag_name == "frame")
		{
			int nr = StringHelp::text_to_int(cur_element.get_attribute("nr", "0"));

			int yoffset = StringHelp::text_to_int(cur_element.get_attribute("y", "0"));
			int xoffset = StringHelp::text_to_int(cur_element.get_attribute("x", "0"));

			if (nr < 0 || nr >= sprite.get_frame_count())
			{
				throw Exception("Invalid sprite frame index specified");
			}

			if (cur_element.has_attribute("speed")) 
			{
				sprite.set_frame_delay(nr, StringHelp::text_to_int(cur_element.get_attribute("speed", "60")));
			}

			sprite.set_frame_offset(nr, Point(xoffset, yoffset));
		}

		cur_node = cur_node.get_next_sibling();
	}

	sprite.restart();

	return sprite;
}

/////////////////////////////////////////////////////////////////////////////
// Sprite Attributes:

void Sprite::throw_if_null() const
{
	if (!impl)
		throw Exception("Sprite is null");
}

Angle Sprite::get_angle() const
{
	return impl->angle;
}

Angle Sprite::get_base_angle() const
{
	return impl->base_angle;
}

void Sprite::get_scale(float &x, float &y) const
{
	x = impl->scale.x;
	y = impl->scale.y;
}

float Sprite::get_alpha() const
{
	return impl->color.a;
}

Colorf Sprite::get_color() const
{
	return impl->color;
}

bool Sprite::get_linear_filter() const
{
	return impl->linear_filter;
}

void Sprite::get_alignment(Origin &origin, int &x, int &y) const
{
	origin = impl->translation_origin;
	x = impl->translation_hotspot.x;
	y = impl->translation_hotspot.y;
}

void Sprite::get_rotation_hotspot(Origin &origin, int &x, int &y) const
{
	origin = impl->rotation_origin;
	x = impl->rotation_hotspot.x;
	y = impl->rotation_hotspot.y;
}

int Sprite::get_current_frame() const
{
	return impl->current_frame;
}

int Sprite::get_frame_count() const
{
	if (!impl) return 0;
	return impl->frames.size();
}

int Sprite::get_frame_delay(int frameno) const
{
	const Sprite_Impl::SpriteFrame *frame = impl->get_frame(frameno);
	if(frame)
		return frame->delay_ms;
	else
		return 0;
}

Point Sprite::get_frame_offset(int frameno) const
{
	const Sprite_Impl::SpriteFrame *frame = impl->get_frame(frameno);
	if(frame)
		return frame->offset;
	else
		return Point(0, 0);
}

Size Sprite::get_frame_size(int frameno) const
{
	const Sprite_Impl::SpriteFrame *frame = impl->get_frame(frameno);
	if(frame)
		return Size(frame->position.get_width(), frame->position.get_height());
	else
		return Size(0, 0);
}

int Sprite::get_width() const
{
	return impl->get_frame(impl->current_frame)->position.get_width();
}

int Sprite::get_height() const
{
	return impl->get_frame(impl->current_frame)->position.get_height();
}

Size Sprite::get_size() const
{
	return impl->get_frame(impl->current_frame)->position.get_size();
}

Subtexture Sprite::get_frame_texture(int frameno) const
{
	const Sprite_Impl::SpriteFrame *frame = impl->get_frame(frameno);
	if(frame)
		return Subtexture(frame->texture, frame->position);
	throw Exception("Invalid frame number");
}


int Sprite::get_id() const
{
	return impl->id;
}

bool Sprite::is_play_loop() const
{
	return impl->play_loop;
}

bool Sprite::is_play_backward() const
{
	return impl->play_backward;
}

bool Sprite::is_play_pingpong() const
{
	return impl->play_pingpong;
}

Sprite::ShowOnFinish Sprite::get_show_on_finish() const
{
	return impl->show_on_finish;
}

bool Sprite::is_finished() const
{
	return impl->finished;
}

bool Sprite::is_looping() const
{
	return impl->looping;
}

/////////////////////////////////////////////////////////////////////////////
// Sprite Operations:

Sprite &Sprite::operator =(const Sprite &copy)
{
	impl = copy.impl;
//	resource_data_session = copy.resource_data_session;
	return *this;
}

void Sprite::set_image_data(const Sprite &image_source)
{
	impl->frames = image_source.impl->frames;

	impl->id = image_source.get_id();
	impl->play_loop = image_source.is_play_loop();
	impl->play_backward = image_source.is_play_backward();
	impl->play_pingpong = image_source.is_play_pingpong();
	impl->base_angle = image_source.get_base_angle();
	impl->show_on_finish = image_source.get_show_on_finish();

	restart();
}

Sprite Sprite::clone() const
{
	Sprite copy;
	copy.impl = std::shared_ptr<Sprite_Impl>(new Sprite_Impl());

	copy.impl->angle = impl->angle;
	copy.impl->angle_pitch = impl->angle_pitch;
	copy.impl->angle_yaw = impl->angle_yaw;
	copy.impl->base_angle = impl->base_angle;
	copy.impl->scale = impl->scale;
	copy.impl->color = impl->color;
	copy.impl->linear_filter = impl->linear_filter;
	copy.impl->translation_hotspot = impl->translation_hotspot;
	copy.impl->rotation_hotspot = impl->rotation_hotspot;
	copy.impl->translation_origin = impl->translation_origin;
	copy.impl->rotation_origin = impl->rotation_origin;
	copy.impl->id = impl->id;
	copy.impl->play_loop = impl->play_loop;
	copy.impl->play_backward = impl->play_backward;
	copy.impl->play_pingpong = impl->play_pingpong;
	copy.impl->show_on_finish = impl->show_on_finish;
	copy.impl->frames = impl->frames;

	copy.restart();
	return copy;
}

void Sprite::draw(Canvas &canvas, float x, float y)
{
	if(impl->is_visible())
		impl->draw(canvas, x, y);
}

void Sprite::draw(Canvas &canvas, int x, int y)
{
	if(impl->is_visible())
		impl->draw(canvas, (float) x, (float) y);
}

void Sprite::draw(Canvas &canvas, const Rectf &src, const Rectf &dest)
{
	if(impl->is_visible())
		impl->draw(canvas, src, dest);
}

void Sprite::draw(Canvas &canvas, const Rectf &dest)
{
	if(impl->is_visible())
		impl->draw(canvas, dest);
}

void Sprite::update(int time_elapsed)
{
	impl->looping = false;

	int total_frames = impl->frames.size();

	if (impl->finished)
		return;

	Sprite_Impl::SpriteFrame *frame = &impl->frames[impl->current_frame];

	impl->update_time_ms += time_elapsed;

	while(impl->update_time_ms > frame->delay_ms)
	{
		impl->update_time_ms -= frame->delay_ms;
		impl->current_frame += impl->delta_frame;

		// Beginning or end of loop ?
		if(impl->current_frame >= total_frames || impl->current_frame < 0)
		{
			if(impl->play_loop == false)
			{
				int delta_frame = impl->play_backward ? -1 : 1;
				if(delta_frame != impl->delta_frame || impl->play_pingpong == false)
				{
					finish();
					return;
				}
			}
				
			if(impl->play_pingpong == true)
			{
				impl->delta_frame = -impl->delta_frame;	// Change direction
				if(impl->delta_frame > 0)
				{
					impl->current_frame = 1;
					//during a ping pong, we only count when we re-start in the forward direction as a loop
					impl->looping = true;
				}
				else
					impl->current_frame = total_frames - 2;
			}
			else // Restart
			{
				impl->current_frame = impl->play_backward ? total_frames - 1 : 0;
				impl->looping = true;
			}
		}
	}
}

void Sprite::set_angle(Angle angle)
{
	float degrees = angle.to_degrees();

	if (degrees >= 0.0f)
		degrees = fmod(degrees, 360.0f);
	else
		degrees = fmod(degrees, 360.0f) + 360.0f;

	impl->angle = Angle(degrees, angle_degrees);
}

void Sprite::set_angle_pitch(Angle angle)
{
	float degrees = angle.to_degrees();

	if (degrees >= 0.0f)
		degrees = fmod(degrees, 360.0f);
	else
		degrees = fmod(degrees, 360.0f) + 360.0f;

	impl->angle_pitch = Angle(degrees, angle_degrees);
}

void Sprite::set_angle_yaw(Angle angle)
{
	float degrees = angle.to_degrees();

	if (degrees >= 0.0f)
		degrees = fmod(degrees, 360.0f);
	else
		degrees = fmod(degrees, 360.0f) + 360.0f;

	impl->angle_yaw = Angle(degrees, angle_degrees);

	impl->angle_yaw = angle;
}

void Sprite::rotate(Angle angle)
{
	float degrees = angle.to_degrees();
	degrees+=impl->angle.to_degrees();

	if (degrees >= 0.0f)
		degrees = fmod(degrees, 360.0f);
	else
		degrees = fmod(degrees, 360.0f) + 360.0f;

	impl->angle = Angle(degrees, angle_degrees);
}

void Sprite::rotate_pitch(Angle angle)
{
	float degrees = angle.to_degrees();
	degrees += impl->angle_pitch.to_degrees();

	if (degrees >= 0.0f)
		degrees = fmod(degrees, 360.0f);
	else
		degrees = fmod(degrees, 360.0f) + 360.0f;

	impl->angle_pitch = Angle(degrees, angle_degrees);
}

void Sprite::rotate_yaw(Angle angle)
{
	float degrees = angle.to_degrees();
	degrees += impl->angle_yaw.to_degrees();

	if (degrees >= 0.0f)
		degrees = fmod(degrees, 360.0f);
	else
		degrees = fmod(degrees, 360.0f) + 360.0f;

	impl->angle_yaw = Angle(degrees, angle_degrees);
}

void Sprite::set_base_angle(Angle angle)
{
	float degrees = angle.to_degrees();

	if (degrees >= 0.0f)
		degrees = fmod(degrees, 360.0f);
	else
		degrees = fmod(degrees, 360.0f) + 360.0f;

	impl->base_angle = Angle(degrees, angle_degrees);
}

void Sprite::set_scale(float x, float y)
{
	impl->scale.x = x;
	impl->scale.y = y;
}

void Sprite::set_alpha(float alpha)
{
	impl->color.a = alpha;
}

void Sprite::set_color(const Colorf &color)
{
	impl->color = color;
}

void Sprite::set_linear_filter(bool linear_filter)
{
	impl->linear_filter = linear_filter;

	std::vector<Sprite_Impl::SpriteFrame>::size_type size = impl->frames.size();
	for (unsigned int cnt = 0; cnt < size; cnt++)
	{
		impl->frames[cnt].texture.set_mag_filter(linear_filter ? filter_linear : filter_nearest);
		impl->frames[cnt].texture.set_min_filter(linear_filter ? filter_linear : filter_nearest);
	}
}

void Sprite::set_alignment(Origin origin, int x, int y)
{
	impl->translation_origin = origin;
	impl->translation_hotspot.x = x;
	impl->translation_hotspot.y = y;
}

void Sprite::set_rotation_hotspot(Origin origin, int x, int y)
{
	impl->rotation_origin = origin;
	impl->rotation_hotspot.x = x;
	impl->rotation_hotspot.y = y;
}

void Sprite::set_frame(unsigned int frame)
{
	if(frame >= impl->frames.size())
		impl->current_frame = impl->frames.size() - 1;
	else
		impl->current_frame = frame;
}

void Sprite::set_delay(int delay_ms)
{
	std::vector<Sprite_Impl::SpriteFrame>::size_type size = impl->frames.size();
	for (unsigned int cnt = 0; cnt < size; cnt++)
		impl->frames[cnt].delay_ms = delay_ms;
}

void Sprite::set_frame_delay(int frameno, int delay_ms)
{
	Sprite_Impl::SpriteFrame *frame = impl->get_frame(frameno);
	if(frame)
		frame->delay_ms = delay_ms;
}

void Sprite::set_frame_offset(int frameno, Point offset)
{
	Sprite_Impl::SpriteFrame *frame = impl->get_frame(frameno);
	if(frame)
		frame->offset = offset;
}

void Sprite::set_id(int id)
{
	impl->id = id;
}

void Sprite::finish()
{
	impl->finished = true;
	if(impl->show_on_finish == Sprite::show_first_frame)
		impl->current_frame = 0;
	else
		impl->current_frame = impl->frames.size() - 1;

	impl->sig_animation_finished();
}

void Sprite::restart()
{
	impl->update_time_ms = 0;
	impl->finished = false;
	impl->current_frame = impl->play_backward ? impl->frames.size() - 1 : 0;
	impl->delta_frame = impl->play_backward ? -1 : 1;
}

void Sprite::set_play_loop(bool loop)
{
	impl->play_loop = loop;
}

void Sprite::set_play_pingpong(bool pingpong)
{
	impl->play_pingpong = pingpong;
}

void Sprite::set_play_backward(bool backward)
{
	impl->play_backward = backward;
	impl->delta_frame = impl->play_backward ? -1 : 1;
}

void Sprite::set_show_on_finish(Sprite::ShowOnFinish show_on_finish)
{
	impl->show_on_finish = show_on_finish;
}

void Sprite::add_frame(const Texture2D &texture)
{
	impl->add_frame(texture);
}

void Sprite::add_frame(Canvas &canvas, const std::string &filename, const FileSystem &fs, const ImageImportDescription &import_desc)
{
	impl->add_frame(Texture2D(canvas, filename, fs, import_desc));
}

void Sprite::add_frame(Canvas &canvas, const std::string &fullname, const ImageImportDescription &import_desc)
{
	impl->add_frame(Texture2D(canvas, fullname, import_desc));
}

void Sprite::add_frame(Canvas &canvas, IODevice &file, const std::string &image_type, const ImageImportDescription &import_desc)
{
	impl->add_frame(Texture2D(canvas, file, image_type, import_desc));

}

void Sprite::add_frames(const Texture2D &texture, Rect *frames, int num_frames)
{
	for(int i=0; i<num_frames; ++i)
		impl->add_frame(texture, frames[i]);
}

void Sprite::add_frame(const Texture2D &texture, const Rect &rect)
{
	impl->add_frame(texture, rect);
}

void Sprite::add_gridclipped_frames(Canvas &canvas, 
	const Texture2D &texture, 
	int xpos, int ypos, 
	int width, int height, 
	int xarray, int yarray, 
	int array_skipframes, 
	int xspace, int yspace)
{
	impl->add_gridclipped_frames(canvas, texture, xpos, ypos, width, height, xarray, yarray, array_skipframes, xspace, yspace);
}

void Sprite::add_alphaclipped_frames(Canvas &canvas, 
	const Texture2D &texture, 
	int xpos, int ypos, 
	float trans_limit)
{
	impl->add_alphaclipped_frames(canvas, texture, xpos, ypos, trans_limit);
}

void Sprite::add_alphaclipped_frames_free(Canvas &canvas, const Texture2D &texture,int xpos, int ypos,	float trans_limit)
{
	impl->add_alphaclipped_frames_free(canvas, texture, xpos, ypos, trans_limit);
}
std::vector<CollisionOutline> Sprite::create_collision_outlines(Canvas &canvas, int alpha_limit, OutlineAccuracy accuracy) const
{
	return impl->create_collision_outlines(canvas, alpha_limit, accuracy);
}

CollisionOutline Sprite::create_collision_outline(Canvas &canvas, int alpha_limit, OutlineAccuracy accuracy) const
{
	return impl->create_collision_outline(canvas, alpha_limit, accuracy);

}

/////////////////////////////////////////////////////////////////////////////
// Sprite signals:

Signal<void()> &Sprite::sig_animation_finished()
{
	return impl->sig_animation_finished;
}

}
