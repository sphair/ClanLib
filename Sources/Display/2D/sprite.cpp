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
**    Kenneth Gangstoe
**    Harry Storbacka
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/Resources/resource.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/2D/draw.h"
#include "API/Display/2D/sprite.h"
#include "API/Display/2D/sprite_description.h"
#include "API/Display/Render/primitives_array.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/ImageProviders/provider_factory.h"
#include "API/Display/ImageProviders/targa_provider.h"
#include "API/Display/ImageProviders/png_provider.h"
#include "sprite_impl.h"
#include "render_batch2d.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Sprite Construction:

CL_Sprite::CL_Sprite()
{
}

CL_Sprite::CL_Sprite(CL_GraphicContext &gc, const CL_StringRef &fullname, const CL_ImageImportDescription &import_desc)
{
	CL_String path = CL_PathHelp::get_fullpath(fullname, CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs(path);
	CL_VirtualDirectory dir = vfs.get_root_directory();
	*this = CL_Sprite(gc, filename, dir, import_desc);
}

CL_Sprite::CL_Sprite(CL_GraphicContext &gc, const CL_StringRef &filename, CL_VirtualDirectory &dir, const CL_ImageImportDescription &import_desc)
: impl(new CL_Sprite_Impl())
{
	CL_SpriteDescription desc;
	desc.add_frame(filename, dir, import_desc );
	impl->create_textures(gc, desc);

	restart();
}

CL_Sprite::CL_Sprite(CL_GraphicContext &gc, CL_IODevice &file, const CL_String &image_type, const CL_ImageImportDescription &import_desc )
: impl(new CL_Sprite_Impl())
{
	CL_SpriteDescription desc;
	desc.add_frame(file, image_type, import_desc );
	impl->create_textures(gc, desc);
	restart();
}

CL_Sprite::CL_Sprite(CL_GraphicContext &gc, const CL_StringRef &resource_id, CL_ResourceManager *resources, const CL_ImageImportDescription &import_desc)
: impl(new CL_Sprite_Impl())
{
	CL_Resource resource = resources->get_resource(resource_id);
	CL_String type = resource.get_element().get_tag_name();
	
	if (type != "sprite")
		throw CL_Exception(cl_format("Resource '%1' is not of type 'sprite'", resource_id));

	impl->init(gc, resource_id, resources, import_desc );
}

CL_Sprite::CL_Sprite(CL_GraphicContext &gc, const CL_SpriteDescription &description)
: impl(new CL_Sprite_Impl())
{
	impl->create_textures(gc, description);
	restart();
}

CL_Sprite::~CL_Sprite()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Sprite Attributes:

void CL_Sprite::throw_if_null() const
{
	if (!impl)
		throw CL_Exception("CL_Sprite is null");
}

CL_Angle CL_Sprite::get_angle() const
{
	return impl->angle;
}

CL_Angle CL_Sprite::get_base_angle() const
{
	return impl->base_angle;
}

void CL_Sprite::get_scale(float &x, float &y) const
{
	x = impl->scale_x;
	y = impl->scale_y;
}

float CL_Sprite::get_alpha() const
{
	return impl->color.a;
}

CL_Colorf CL_Sprite::get_color() const
{
	return impl->color;
}

bool CL_Sprite::get_linear_filter() const
{
	return impl->linear_filter;
}

void CL_Sprite::get_alignment(CL_Origin &origin, int &x, int &y) const
{
	origin = impl->translation_origin;
	x = impl->translation_hotspot.x;
	y = impl->translation_hotspot.y;
}

void CL_Sprite::get_rotation_hotspot(CL_Origin &origin, int &x, int &y) const
{
	origin = impl->rotation_origin;
	x = impl->rotation_hotspot.x;
	y = impl->rotation_hotspot.y;
}

int CL_Sprite::get_current_frame() const
{
	return impl->current_frame;
}

int CL_Sprite::get_frame_count() const
{
	if (!impl) return 0;
	return impl->frames.size();
}

int CL_Sprite::get_frame_delay(int frameno) const
{
	const CL_Sprite_Impl::SpriteFrame *frame = impl->get_frame(frameno);
	if(frame)
		return frame->delay_ms;
	else
		return 0;
}

CL_Point CL_Sprite::get_frame_offset(int frameno) const
{
	const CL_Sprite_Impl::SpriteFrame *frame = impl->get_frame(frameno);
	if(frame)
		return frame->offset;
	else
		return CL_Point(0, 0);
}

CL_Size CL_Sprite::get_frame_size(int frameno) const
{
	const CL_Sprite_Impl::SpriteFrame *frame = impl->get_frame(frameno);
	if(frame)
		return CL_Size(frame->position.get_width(), frame->position.get_height());
	else
		return CL_Size(0, 0);
}

int CL_Sprite::get_width() const
{
	return impl->get_frame(impl->current_frame)->position.get_width();
}

int CL_Sprite::get_height() const
{
	return impl->get_frame(impl->current_frame)->position.get_height();
}

CL_Size CL_Sprite::get_size() const
{
	return impl->get_frame(impl->current_frame)->position.get_size();
}

int CL_Sprite::get_id() const
{
	return impl->id;
}

bool CL_Sprite::is_play_loop() const
{
	return impl->play_loop;
}

bool CL_Sprite::is_play_backward() const
{
	return impl->play_backward;
}

bool CL_Sprite::is_play_pingpong() const
{
	return impl->play_pingpong;
}

CL_Sprite::ShowOnFinish CL_Sprite::get_show_on_finish() const
{
	return impl->show_on_finish;
}

bool CL_Sprite::is_finished() const
{
	return impl->finished;
}

bool CL_Sprite::is_looping() const
{
	return impl->looping;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Sprite Operations:

CL_Sprite &CL_Sprite::operator =(const CL_Sprite &copy)
{
	impl = copy.impl;
//	resource_data_session = copy.resource_data_session;
	return *this;
}

void CL_Sprite::set_image_data(const CL_Sprite &image_source)
{
	impl->frames = image_source.impl->frames;

	impl->id = image_source.get_id();
	impl->play_loop = image_source.is_play_loop();
	impl->play_backward = image_source.is_play_backward();
	impl->play_pingpong = image_source.is_play_pingpong();
	impl->base_angle = image_source.get_base_angle();

	restart();
}

void CL_Sprite::clone(const CL_Sprite &source)
{
	if(!impl)
		impl = CL_SharedPtr<CL_Sprite_Impl>(new CL_Sprite_Impl());

	impl->angle = source.impl->angle;
	impl->angle_pitch = source.impl->angle_pitch;
	impl->angle_yaw = source.impl->angle_yaw;
	impl->base_angle = source.impl->base_angle;
	impl->scale_x = source.impl->scale_x;
	impl->scale_y = source.impl->scale_y;
	impl->color = source.impl->color;
	impl->linear_filter = source.impl->linear_filter;
	impl->translation_hotspot = source.impl->translation_hotspot;
	impl->rotation_hotspot = source.impl->rotation_hotspot;
	impl->translation_origin = source.impl->translation_origin;
	impl->rotation_origin = source.impl->rotation_origin;
	impl->id = source.impl->id;
	impl->play_loop = source.impl->play_loop;
	impl->play_backward = source.impl->play_backward;
	impl->play_pingpong = source.impl->play_pingpong;
	impl->show_on_finish = source.impl->show_on_finish;
	impl->texture_group = source.impl->texture_group;
	impl->frames = source.impl->frames;

	restart();
}

void CL_Sprite::draw(CL_GraphicContext &gc, float x, float y)
{
	if(impl->is_visible())
		impl->draw(gc, x, y);
}

void CL_Sprite::draw(CL_GraphicContext &gc, int x, int y)
{
	if(impl->is_visible())
		impl->draw(gc, (float) x, (float) y);
}

void CL_Sprite::draw(CL_GraphicContext &gc, const CL_Rectf &src, const CL_Rectf &dest)
{
	if(impl->is_visible())
		impl->draw(gc, src, dest);
}

void CL_Sprite::draw(CL_GraphicContext &gc, const CL_Rectf &dest)
{
	if(impl->is_visible())
		impl->draw(gc, dest);
}

int CL_Sprite::update(int time_elapsed)
{
	impl->looping = false;

	if(time_elapsed == -1)
		time_elapsed = impl->calc_time_elapsed();

	int total_frames = impl->frames.size();

	//we still want to know when a 1 frame 'anim' loops, based on the timer -mrfun
	//if(total_frames < 2 || impl->finished)
	//	return time_elapsed; 
	// but we need to stop when animation is finished -gpmfuchs
	if (impl->finished) return time_elapsed;

	CL_Sprite_Impl::SpriteFrame *frame = &impl->frames[impl->current_frame];

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
					return time_elapsed;
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

	return time_elapsed;
}

void CL_Sprite::set_angle(CL_Angle angle)
{
	float degrees = angle.to_degrees();

	if (degrees >= 0.0f)
		degrees = fmod(degrees, 360.0f);
	else
		degrees = fmod(degrees, 360.0f) + 360.0f;

	impl->angle = CL_Angle(degrees, cl_degrees);
}

void CL_Sprite::set_angle_pitch(CL_Angle angle)
{
	float degrees = angle.to_degrees();

	if (degrees >= 0.0f)
		degrees = fmod(degrees, 360.0f);
	else
		degrees = fmod(degrees, 360.0f) + 360.0f;

	impl->angle_pitch = CL_Angle(degrees, cl_degrees);
}

void CL_Sprite::set_angle_yaw(CL_Angle angle)
{
	float degrees = angle.to_degrees();

	if (degrees >= 0.0f)
		degrees = fmod(degrees, 360.0f);
	else
		degrees = fmod(degrees, 360.0f) + 360.0f;

	impl->angle_yaw = CL_Angle(degrees, cl_degrees);

	impl->angle_yaw = angle;
}

void CL_Sprite::rotate(CL_Angle angle)
{
	float degrees = angle.to_degrees();
	degrees+=impl->angle.to_degrees();

	if (degrees >= 0.0f)
		degrees = fmod(degrees, 360.0f);
	else
		degrees = fmod(degrees, 360.0f) + 360.0f;

	impl->angle = CL_Angle(degrees, cl_degrees);
}

void CL_Sprite::rotate_pitch(CL_Angle angle)
{
	float degrees = angle.to_degrees();
	degrees+=impl->angle_pitch.to_degrees();

	if (degrees >= 0.0f)
		degrees = fmod(degrees, 360.0f);
	else
		degrees = fmod(degrees, 360.0f) + 360.0f;

	impl->angle_pitch = CL_Angle(degrees, cl_degrees);
}

void CL_Sprite::rotate_yaw(CL_Angle angle)
{
	float degrees = angle.to_degrees();
	degrees+=impl->angle_yaw.to_degrees();

	if (degrees >= 0.0f)
		degrees = fmod(degrees, 360.0f);
	else
		degrees = fmod(degrees, 360.0f) + 360.0f;

	impl->angle_yaw = CL_Angle(degrees, cl_degrees);
}

void CL_Sprite::set_base_angle(CL_Angle angle)
{
	float degrees = angle.to_degrees();

	if (degrees >= 0.0f)
		degrees = fmod(degrees, 360.0f);
	else
		degrees = fmod(degrees, 360.0f) + 360.0f;

	impl->base_angle = CL_Angle(degrees, cl_degrees);
}

void CL_Sprite::set_scale(float x, float y)
{
	impl->scale_x = x;
	impl->scale_y = y;
}

void CL_Sprite::set_alpha(float alpha)
{
	impl->color.a = alpha;
}

void CL_Sprite::set_color(const CL_Colorf &color)
{
	impl->color = color;
}

void CL_Sprite::set_linear_filter(bool linear_filter)
{
	impl->linear_filter = linear_filter;

	std::vector<CL_Sprite_Impl::SpriteFrame>::size_type size = impl->frames.size();
	for (unsigned int cnt = 0; cnt < size; cnt++)
	{
		impl->frames[cnt].texture.set_mag_filter(linear_filter ? cl_filter_linear : cl_filter_nearest);
		impl->frames[cnt].texture.set_min_filter(linear_filter ? cl_filter_linear : cl_filter_nearest);
	}
}

void CL_Sprite::set_alignment(CL_Origin origin, int x, int y)
{
	impl->translation_origin = origin;
	impl->translation_hotspot.x = x;
	impl->translation_hotspot.y = y;
}

void CL_Sprite::set_rotation_hotspot(CL_Origin origin, int x, int y)
{
	impl->rotation_origin = origin;
	impl->rotation_hotspot.x = x;
	impl->rotation_hotspot.y = y;
}

void CL_Sprite::set_frame(unsigned int frame)
{
	if(frame < 0)
		impl->current_frame = 0;
	else if(frame >= impl->frames.size())
		impl->current_frame = impl->frames.size() - 1;
	else
		impl->current_frame = frame;
}

void CL_Sprite::set_delay(int delay_ms)
{
	std::vector<CL_Sprite_Impl::SpriteFrame>::size_type size = impl->frames.size();
	for (unsigned int cnt = 0; cnt < size; cnt++)
		impl->frames[cnt].delay_ms = delay_ms;
}

void CL_Sprite::set_frame_delay(int frameno, int delay_ms)
{
	CL_Sprite_Impl::SpriteFrame *frame = impl->get_frame(frameno);
	if(frame)
		frame->delay_ms = delay_ms;
}

void CL_Sprite::set_frame_offset(int frameno, CL_Point offset)
{
	CL_Sprite_Impl::SpriteFrame *frame = impl->get_frame(frameno);
	if(frame)
		frame->offset = offset;
}

void CL_Sprite::set_id(int id)
{
	impl->id = id;
}

void CL_Sprite::finish()
{
	impl->finished = true;
	if(impl->show_on_finish == CL_Sprite::show_first_frame)
		impl->current_frame = 0;
	else
		impl->current_frame = impl->frames.size() - 1;

	impl->sig_animation_finished.invoke();
}

void CL_Sprite::restart()
{
	impl->update_time_ms = 0;
	impl->last_time_ms = 0;
	impl->finished = false;
	impl->current_frame = impl->play_backward ? impl->frames.size() - 1 : 0;
	impl->delta_frame = impl->play_backward ? -1 : 1;
}

void CL_Sprite::set_play_loop(bool loop)
{
	impl->play_loop = loop;
}

void CL_Sprite::set_play_pingpong(bool pingpong)
{
	impl->play_pingpong = pingpong;
}

void CL_Sprite::set_play_backward(bool backward)
{
	impl->play_backward = backward;
}

void CL_Sprite::set_show_on_finish(CL_Sprite::ShowOnFinish show_on_finish)
{
	impl->show_on_finish = show_on_finish;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Sprite signals:

CL_Signal_v0 &CL_Sprite::sig_animation_finished()
{
	return impl->sig_animation_finished;
}
