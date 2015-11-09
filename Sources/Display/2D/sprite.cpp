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
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/2D/sprite.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/Render/primitives_array.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/ImageProviders/provider_factory.h"
#include "API/Display/ImageProviders/targa_provider.h"
#include "API/Display/ImageProviders/png_provider.h"
#include "sprite_impl.h"
#include "render_batch_triangle.h"
#include "API/Display/Resources/display_cache.h"
#include "API/Display/2D/subtexture.h"

namespace clan
{
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
		add_frame(canvas, filename, fs, import_desc);
		restart();
	}

	Sprite::Sprite(Canvas &canvas, IODevice &file, const std::string &image_type, const ImageImportDescription &import_desc)
		: impl(std::make_shared<Sprite_Impl>())
	{
		add_frame(canvas, file, image_type, import_desc);
		restart();
	}

	Sprite::Sprite(Canvas &canvas)
		: impl(std::make_shared<Sprite_Impl>())
	{
	}

	Sprite::~Sprite()
	{
	}

	Resource<Sprite> Sprite::resource(Canvas &canvas, const std::string &id, const ResourceManager &resources)
	{
		return DisplayCache::get(resources).get_sprite(canvas, id);
	}

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
		if (frame)
			return frame->delay_ms;
		else
			return 0;
	}

	Point Sprite::get_frame_offset(int frameno) const
	{
		const Sprite_Impl::SpriteFrame *frame = impl->get_frame(frameno);
		if (frame)
			return frame->offset;
		else
			return Point(0, 0);
	}

	Size Sprite::get_frame_size(int frameno) const
	{
		const Sprite_Impl::SpriteFrame *frame = impl->get_frame(frameno);
		if (frame)
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
		if (frame)
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
		if (impl->is_visible())
			impl->draw(canvas, x, y);
	}

	void Sprite::draw(Canvas &canvas, int x, int y)
	{
		if (impl->is_visible())
			impl->draw(canvas, (float)x, (float)y);
	}

	void Sprite::draw(Canvas &canvas, const Rectf &src, const Rectf &dest)
	{
		if (impl->is_visible())
			impl->draw(canvas, src, dest);
	}

	void Sprite::draw(Canvas &canvas, const Rectf &dest)
	{
		if (impl->is_visible())
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

		while (impl->update_time_ms > frame->delay_ms)
		{
			impl->update_time_ms -= frame->delay_ms;
			impl->current_frame += impl->delta_frame;

			// Beginning or end of loop ?
			if (impl->current_frame >= total_frames || impl->current_frame < 0)
			{
				if (impl->play_loop == false)
				{
					int delta_frame = impl->play_backward ? -1 : 1;
					if (delta_frame != impl->delta_frame || impl->play_pingpong == false)
					{
						finish();
						return;
					}
				}

				if (impl->play_pingpong == true)
				{
					impl->delta_frame = -impl->delta_frame;	// Change direction
					if (impl->delta_frame > 0)
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
		degrees += impl->angle.to_degrees();

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
		if (frame >= impl->frames.size())
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
		if (frame)
			frame->delay_ms = delay_ms;
	}

	void Sprite::set_frame_offset(int frameno, Point offset)
	{
		Sprite_Impl::SpriteFrame *frame = impl->get_frame(frameno);
		if (frame)
			frame->offset = offset;
	}

	void Sprite::set_id(int id)
	{
		impl->id = id;
	}

	void Sprite::finish()
	{
		impl->finished = true;
		if (impl->show_on_finish == Sprite::show_first_frame)
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
		for (int i = 0; i < num_frames; ++i)
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

	void Sprite::add_alphaclipped_frames_free(Canvas &canvas, const Texture2D &texture, int xpos, int ypos, float trans_limit)
	{
		impl->add_alphaclipped_frames_free(canvas, texture, xpos, ypos, trans_limit);
	}

	Signal<void()> &Sprite::sig_animation_finished()
	{
		return impl->sig_animation_finished;
	}
}
