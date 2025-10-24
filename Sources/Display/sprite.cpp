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
#include "API/Display/pixel_format.h"
#include "API/Display/pixel_buffer.h"
#include "API/Display/display.h"
#include "API/Display/display_window.h"
#include "API/Display/sprite.h"
#include "API/Display/sprite_description.h"
#include "API/Display/sprite_packer.h"
#include "API/Display/surface.h"
#include "API/Display/graphic_context.h"
#include "API/Core/System/error.h"
#include "API/Core/System/log.h"
#include "API/Core/Resources/resource_manager.h"
#include "sprite_generic.h"
#include "resourcedata_sprite.h"
#include "display_target.h"
#include "surface_generic.h"
#include "surface_target.h"

#include <math.h>

/////////////////////////////////////////////////////////////////////////////
// CL_Sprite construction:

CL_Sprite::CL_Sprite(const std::string &resource_id, CL_ResourceManager *manager)
: impl(0)
{
	resource = manager->get_resource(resource_id);
	resource.load();

	CL_ResourceData_Sprite *data =
		(CL_ResourceData_Sprite *) resource.get_data("sprite");

	if (!data)
		throw CL_Error("Resource '" + resource_id + "' is not of type 'sprite'");

	impl = new CL_Sprite_Generic;
	if (data->get_sprite().impl)
	{
		*impl = *data->get_sprite().impl;
		restart();
	}
}

void CL_Sprite::createFromDescription(const CL_SpriteDescription &spritedescription, bool pack_texture, unsigned int min_width, unsigned int min_height)
{
	impl = new CL_Sprite_Generic;
	
	if (pack_texture && CL_DisplayTarget::current()->enable_packer())
	{
		// Fetch max texture size
		CL_DisplayWindow *window = CL_Display::get_current_window();
		CL_Size max_surface_size = window->get_gc()->get_max_surface_size();

		// Pack frames into textures
		CL_SpritePacker packer(spritedescription);
		if(packer.pack(max_surface_size.width, max_surface_size.height, min_width, min_height) == false)
			throw CL_Error("Couldn't pack all frames into textures");

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
//		CL_Log::log("debug", "New sprite:");
		for(unsigned int i = 0; i < texture_sizes.size(); i++)
		{
			int width = texture_sizes[i].width;
			int height = texture_sizes[i].height;
			
			CL_PixelBuffer buffer(width, height, 4 * width, format);
			buffer.lock();

//			CL_Log::log("debug", "- Packing texture %1x%2", width, height);

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
                                if(x < dest_rect.right-1) {
									if((line[x+1] & 0xff000000) != 0)
									{
										r+= (line[x+1] & 0x00ff0000) >> 16;
										g+= (line[x+1] & 0x0000ff00) >> 8;
										b+= (line[x+1] & 0x000000ff);
										n++;
									}
								}
								if(x > 0) {
									if((line[x-1] & 0xff000000) != 0)
									{
										r+= (line[x-1] & 0x00ff0000) >> 16;
										g+= (line[x-1] & 0x0000ff00) >> 8;
										b+= (line[x-1] & 0x000000ff);
										n++;
									}
								}
								if(y < dest_rect.bottom-1) {
									line = (unsigned int *) (data_bytes+(y+1)*dest_pitch);
									if((line[x] & 0xff000000) != 0)
									{
										r+= (line[x] & 0x00ff0000) >> 16;
										g+= (line[x] & 0x0000ff00) >> 8;
										b+= (line[x] & 0x000000ff);
										n++;
									}
								}
								if(y > 0) {
									line = (unsigned int *) (data_bytes+(y-1)*dest_pitch);
									if((line[x] & 0xff000000) != 0)
									{
										r+= (line[x] & 0x00ff0000) >> 16;
										g+= (line[x] & 0x0000ff00) >> 8;
										b+= (line[x] & 0x000000ff);
										n++;
									}
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
	
	restart();
}

CL_Sprite::CL_Sprite(const CL_SpriteDescription &spritedescription, bool pack_texture, unsigned int min_width, unsigned int min_height)
{
	createFromDescription(spritedescription, pack_texture, min_width, min_height);
}

CL_Sprite::CL_Sprite(const CL_SpriteDescription &spritedescription, bool pack_texture)
{
	createFromDescription(spritedescription, pack_texture, 16, 16);
}


CL_Sprite::CL_Sprite(const CL_Sprite &sprite)
: impl(0)
{
	if (sprite.impl)
	{
		impl = new CL_Sprite_Generic;
		*impl = *sprite.impl;
	
		resource = sprite.resource;
		resource.load();
	}
}

CL_Sprite::CL_Sprite()
: impl(0)
{
}

CL_Sprite::~CL_Sprite()
{
	delete impl;
	resource.unload();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Sprite attributes:

float CL_Sprite::get_angle() const
{
	return impl->angle;
}

float CL_Sprite::get_angle_pitch() const
{
	return impl->angle_pitch;
}

float CL_Sprite::get_angle_yaw() const
{
	return impl->angle_yaw;
}

float CL_Sprite::get_base_angle() const
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
	return impl->alpha;
}

void CL_Sprite::get_color(float &red, float &green, float &blue, float &alpha) const
{
	red = impl->red;
	green = impl->green;
	blue = impl->blue;
	alpha = impl->alpha;
}

void CL_Sprite::get_blend_func(CL_BlendFunc &src, CL_BlendFunc &dest) const
{
	src  = impl->blend_src;
	dest = impl->blend_dest;
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

float CL_Sprite::get_frame_delay(int frameno) const
{
	CL_Sprite_Generic::SpriteFrame *frame = impl->get_frame(frameno);
	if(frame)
		return frame->delay;
	else
		return 0;
}

CL_Point CL_Sprite::get_frame_offset(int frameno) const
{
	CL_Sprite_Generic::SpriteFrame *frame = impl->get_frame(frameno);
	if(frame)
		return frame->offset;
	else
		return CL_Point(0, 0);
}

CL_Size CL_Sprite::get_frame_size(int frameno) const
{
	CL_Sprite_Generic::SpriteFrame *frame = impl->get_frame(frameno);
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

CL_Surface CL_Sprite::get_frame_surface(int frameno) const
{
	CL_Sprite_Generic::SpriteFrame *frame = impl->get_frame(frameno);
	if (frame)
		return frame->surface;
	else
		return CL_Surface();
}

CL_Surface CL_Sprite::get_frame_surface(int frameno, CL_Rect &surface_rect) const
{
	CL_Sprite_Generic::SpriteFrame *frame = impl->get_frame(frameno);
	if (frame)
	{
		surface_rect = frame->position;
		return frame->surface;
	}
	else
		return CL_Surface();
}

CL_PixelBuffer CL_Sprite::get_frame_pixeldata(int frameno) const
{
	CL_Sprite_Generic::SpriteFrame *frame = impl->get_frame(frameno);
	if (frame)
	{
		CL_PixelBuffer pb = frame->surface.get_pixeldata();
		int w = get_width();
		int h = get_height();
		// Make sure dimensions are the same as this sprite.
		// pb will always be larger than, or equal to the sprite's size
		
		if (w != pb.get_width() || h != pb.get_height())
		{
			int p = pb.get_pitch();
			int bytes_per_pixel = (pb.get_format().get_depth() + 7)/8;
			CL_PixelBuffer new_pb(w, h, p, pb.get_format());
			unsigned char *src_bytes = (unsigned char*) pb.get_data();
			unsigned char *dest_bytes = (unsigned char*) new_pb.get_data();

			src_bytes += frame->position.top * p; //move down to where it really starts if applicable

			for (int y = 0; y < h; y++)
				for (int x = 0; x < w * bytes_per_pixel; x++)
					dest_bytes[y * p + x] = src_bytes[y*p + x + (frame->position.left * bytes_per_pixel)];
			return new_pb;
		}
		else
			return pb;
	}
	else
		return CL_PixelBuffer();
}

int CL_Sprite::get_id() const
{
	return impl->id;
}

bool CL_Sprite::is_null() const
{
	return (impl == 0);
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
// CL_Sprite operations:

CL_Sprite &CL_Sprite::operator =(const CL_Sprite &copy)
{
	resource.unload();
	if (impl) delete impl;
	impl = 0;
	if (copy.impl)
	{
		impl = new CL_Sprite_Generic;
		*impl = *copy.impl;
	}
	resource = copy.resource;
	resource.load();
	return *this;
}

CL_Sprite::operator bool() const
{
  return (impl != 0);
}

void CL_Sprite::set_image_data(const CL_Sprite &image_source)
{
	if(!impl)
		impl = new CL_Sprite_Generic;

	impl->frames = image_source.impl->frames;

	impl->id = image_source.get_id();
	impl->play_loop = image_source.is_play_loop();
	impl->play_backward = image_source.is_play_backward();
	impl->play_pingpong = image_source.is_play_pingpong();
	impl->base_angle = image_source.get_base_angle();

	restart();
}
	


bool CL_Sprite::setup_draw_params(float x, float y, CL_Surface_DrawParams1 & params1, bool sub_pixel_accuracy)
{
	if (impl->finished && impl->show_on_finish == show_blank)
	{
		return false; //nothing to draw, signal invalid data
	}

	CL_Sprite_Generic::SpriteFrame &frame = impl->frames[impl->current_frame];

	static CL_Surface_DrawParams2 params2;
	
	params2.destX = x;
	params2.destY = y;
	params2.srcX = frame.position.left;
	params2.srcY = frame.position.top;
	params2.srcWidth = frame.position.get_width();
	params2.srcHeight = frame.position.get_height();
	params2.destZ = 0.0;
	params2.red = impl->red;
	params2.green = impl->green;
	params2.blue = impl->blue;
	params2.alpha = impl->alpha;
	params2.blend_src = impl->blend_src;
	params2.blend_dest = impl->blend_dest;
	params2.blendfunc_src_alpha  = impl->blendfunc_src_alpha;
	params2.blendfunc_dest_alpha = impl->blendfunc_dest_alpha;
	params2.scale_x = impl->scale_x;
	params2.scale_y = impl->scale_y;
	params2.translate_origin = impl->translation_origin;
	params2.translate_x = impl->translation_hotspot.x + frame.offset.x;
	params2.translate_y = impl->translation_hotspot.y + frame.offset.y;
	params2.rotate_angle = impl->angle - impl->base_angle;
	params2.rotate_pitch = impl->angle_pitch;
	params2.rotate_yaw = impl->angle_yaw;
	params2.rotate_origin = impl->rotation_origin;
	params2.rotate_x = impl->rotation_hotspot.x + frame.offset.x;
	params2.rotate_y = impl->rotation_hotspot.y + frame.offset.y;
	params2.sub_pixel_accuracy = sub_pixel_accuracy;

	static CL_Surface_TargetDrawParams1 t_params1;

	frame.surface.setup_target_params(params2, t_params1);
	frame.surface.setup_draw_params(params2, &t_params1, params1);
	
	return true;
}


void CL_Sprite::draw(const CL_Surface_DrawParams1 & params1, CL_GraphicContext *gc /* = 0 */)
{
	if(impl->finished == false || impl->show_on_finish != show_blank)
	{
		CL_Sprite_Generic::SpriteFrame &frame = impl->frames[impl->current_frame];
		if (gc == 0) gc = CL_Display::get_current_window()->get_gc();

		frame.surface.draw(params1, gc);
	}
}

void CL_Sprite::draw(
	float x,
	float y,
	CL_GraphicContext *gc)
{
	if(impl->finished == false || impl->show_on_finish != show_blank)
	{
	
		CL_Sprite_Generic::SpriteFrame &frame = impl->frames[impl->current_frame];

		if (gc == 0) gc = CL_Display::get_current_window()->get_gc();

		static CL_Surface_DrawParams2 params2;
		
		params2.destX = x;
		params2.destY = y;
		params2.srcX = frame.position.left;
		params2.srcY = frame.position.top;
		params2.srcWidth = frame.position.get_width();
		params2.srcHeight = frame.position.get_height();
		params2.destZ = 0.0;
		params2.red = impl->red;
		params2.green = impl->green;
		params2.blue = impl->blue;
		params2.alpha = impl->alpha;
		params2.blend_src = impl->blend_src;
		params2.blend_dest = impl->blend_dest;
		params2.blendfunc_src_alpha  = impl->blendfunc_src_alpha;
		params2.blendfunc_dest_alpha = impl->blendfunc_dest_alpha;
		params2.scale_x = impl->scale_x;
		params2.scale_y = impl->scale_y;
		params2.translate_origin = impl->translation_origin;
		params2.translate_x = impl->translation_hotspot.x + frame.offset.x;
		params2.translate_y = impl->translation_hotspot.y + frame.offset.y;
		params2.rotate_angle = impl->angle - impl->base_angle;
		params2.rotate_pitch = impl->angle_pitch;
		params2.rotate_yaw = impl->angle_yaw;
		params2.rotate_origin = impl->rotation_origin;
		params2.rotate_x = impl->rotation_hotspot.x + frame.offset.x;
		params2.rotate_y = impl->rotation_hotspot.y + frame.offset.y;

		frame.surface.draw(params2,
                                   gc);
	}
}


void CL_Sprite::draw_subpixel(
					 float x,
					 float y,
					 CL_GraphicContext *gc)
{
	if(impl->finished == false || impl->show_on_finish != show_blank)
	{
	
		CL_Sprite_Generic::SpriteFrame &frame = impl->frames[impl->current_frame];

		if (gc == 0) gc = CL_Display::get_current_window()->get_gc();

		static CL_Surface_DrawParams2 params2;
	
		params2.destX = x;
		params2.destY = y;
		params2.srcX = frame.position.left;
		params2.srcY = frame.position.top;
		params2.srcWidth = frame.position.get_width();
		params2.srcHeight = frame.position.get_height();
		params2.destZ = 0.0;
		params2.red = impl->red;
		params2.green = impl->green;
		params2.blue = impl->blue;
		params2.alpha = impl->alpha;
		params2.blend_src = impl->blend_src;
		params2.blend_dest = impl->blend_dest;
		params2.blendfunc_src_alpha  = impl->blendfunc_src_alpha;
		params2.blendfunc_dest_alpha = impl->blendfunc_dest_alpha;
		params2.scale_x = impl->scale_x;
		params2.scale_y = impl->scale_y;
		params2.translate_origin = impl->translation_origin;
		params2.translate_x = impl->translation_hotspot.x + frame.offset.x;
		params2.translate_y = impl->translation_hotspot.y + frame.offset.y;
		params2.rotate_angle = impl->angle - impl->base_angle;
		params2.rotate_pitch = impl->angle_pitch;
		params2.rotate_yaw = impl->angle_yaw;
		params2.rotate_origin = impl->rotation_origin;
		params2.rotate_x = impl->rotation_hotspot.x + frame.offset.x;
		params2.rotate_y = impl->rotation_hotspot.y + frame.offset.y;
		params2.sub_pixel_accuracy = true;

		frame.surface.draw(params2,
			gc);
	}
}

void CL_Sprite::draw(
	const CL_Rect &dest,
	CL_GraphicContext *gc)
{
	if(impl->finished == false || impl->show_on_finish != show_blank)
	{
		CL_Sprite_Generic::SpriteFrame &frame = impl->frames[impl->current_frame];
		
		if (gc == 0) gc = CL_Display::get_current_window()->get_gc();

		static CL_Surface_DrawParams2 params2;
		params2.srcX = frame.position.left;
		params2.srcY = frame.position.top;
		params2.srcWidth = frame.position.get_width();
		params2.srcHeight = frame.position.get_height();
		params2.destX = dest.left;
		params2.destY = dest.top;
		params2.destZ = 0.0;
		params2.red = impl->red;
		params2.green = impl->green;
		params2.blue = impl->blue;
		params2.alpha = impl->alpha;
		params2.blend_src = impl->blend_src;
		params2.blend_dest = impl->blend_dest;
		params2.blendfunc_src_alpha  = impl->blendfunc_src_alpha;
		params2.blendfunc_dest_alpha = impl->blendfunc_dest_alpha;
		params2.scale_x = dest.get_width()/float(frame.position.get_width());
		params2.scale_y = dest.get_height()/float(frame.position.get_height());
		params2.translate_origin = impl->translation_origin;
		params2.translate_x = impl->translation_hotspot.x + frame.offset.x;
		params2.translate_y = impl->translation_hotspot.y + frame.offset.y;
		params2.rotate_angle = impl->angle - impl->base_angle;
		params2.rotate_pitch = impl->angle_pitch;
		params2.rotate_yaw = impl->angle_yaw;
		params2.rotate_origin = impl->rotation_origin;
		params2.rotate_x = impl->rotation_hotspot.x + frame.offset.x;
		params2.rotate_y = impl->rotation_hotspot.y + frame.offset.y;

		frame.surface.draw(
			params2,
			gc);
	}
}

void CL_Sprite::draw(
					 const CL_Rectf &dest,
					 CL_GraphicContext *gc)
{
	if(impl->finished == false || impl->show_on_finish != show_blank)
	{
		CL_Sprite_Generic::SpriteFrame &frame = impl->frames[impl->current_frame];

		if (gc == 0) gc = CL_Display::get_current_window()->get_gc();

		static CL_Surface_DrawParams2 params2;
		params2.srcX = frame.position.left;
		params2.srcY = frame.position.top;
		params2.srcWidth = frame.position.get_width();
		params2.srcHeight = frame.position.get_height();
		params2.destX = dest.left;
		params2.destY = dest.top;
		params2.destZ = 0.0;
		params2.red = impl->red;
		params2.green = impl->green;
		params2.blue = impl->blue;
		params2.alpha = impl->alpha;
		params2.blend_src = impl->blend_src;
		params2.blend_dest = impl->blend_dest;
		params2.blendfunc_src_alpha  = impl->blendfunc_src_alpha;
		params2.blendfunc_dest_alpha = impl->blendfunc_dest_alpha;
		params2.scale_x = dest.get_width()/float(frame.position.get_width());
		params2.scale_y = dest.get_height()/float(frame.position.get_height());
		params2.translate_origin = impl->translation_origin;
		params2.translate_x = impl->translation_hotspot.x + frame.offset.x;
		params2.translate_y = impl->translation_hotspot.y + frame.offset.y;
		params2.rotate_angle = impl->angle - impl->base_angle;
		params2.rotate_pitch = impl->angle_pitch;
		params2.rotate_yaw = impl->angle_yaw;
		params2.rotate_origin = impl->rotation_origin;
		params2.rotate_x = impl->rotation_hotspot.x + frame.offset.x;
		params2.rotate_y = impl->rotation_hotspot.y + frame.offset.y;
		params2.sub_pixel_accuracy = true;

		frame.surface.draw(
			params2,
			gc);
	}
}

float CL_Sprite::update(float time_elapsed)
{
	impl->looping = false;
	
	if(time_elapsed == 0)
		time_elapsed = impl->calc_time_elapsed();

	int total_frames = impl->frames.size();
	
	//we still want to know when a 1 frame 'anim' loops, based on the timer -mrfun
	//if(total_frames < 2 || impl->finished)
	//	return time_elapsed; 
	// but we need to stop when animation is finished -gpmfuchs
	if (impl->finished) return time_elapsed;

	CL_Sprite_Generic::SpriteFrame *frame = &impl->frames[impl->current_frame];

	impl->update_time += time_elapsed;
	while(impl->update_time > frame->delay)
	{
		impl->update_time -= frame->delay;
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

void CL_Sprite::set_angle(float angle)
{
	impl->angle = angle;

	if (impl->angle >= 0)
		impl->angle = (float)fmod(impl->angle, 360.0f);
	else
		impl->angle = (float)fmod (impl->angle, 360.0f) + 360.0f;
}

void CL_Sprite::set_angle_pitch(float angle)
{
	impl->angle_pitch = angle;

	if (impl->angle_pitch >= 0)
		impl->angle_pitch = (float)fmod(impl->angle_pitch, 360.0f);
	else
		impl->angle_pitch = (float)fmod (impl->angle_pitch, 360.0f) + 360.0f;
}

void CL_Sprite::set_angle_yaw(float angle)
{
	impl->angle_yaw = angle;

	if (impl->angle_yaw >= 0)
		impl->angle_yaw = (float)fmod(impl->angle_yaw, 360.0f);
	else
		impl->angle_yaw = (float)fmod (impl->angle_yaw, 360.0f) + 360.0f;
}

void CL_Sprite::rotate(float angle)
{
	impl->angle += angle;

	if (impl->angle >= 0)
		impl->angle = (float)fmod(impl->angle, 360.0f);
	else
		impl->angle = (float)fmod(impl->angle, 360.0f) + 360.0f;
}

void CL_Sprite::rotate_pitch(float angle)
{
	impl->angle_pitch += angle;

	if (impl->angle_pitch >= 0)
		impl->angle_pitch = (float)fmod(impl->angle_pitch, 360.0f);
	else
		impl->angle_pitch = (float)fmod(impl->angle_pitch, 360.0f) + 360.0f;
}

void CL_Sprite::rotate_yaw(float angle)
{
	impl->angle_yaw += angle;

	if (impl->angle_yaw >= 0)
		impl->angle_yaw = (float)fmod(impl->angle_yaw, 360.0f);
	else
		impl->angle_yaw = (float)fmod(impl->angle_yaw, 360.0f) + 360.0f;
}

void CL_Sprite::set_base_angle(float angle)
{
	impl->base_angle = angle;

	while (impl->base_angle < 0.0f)
		impl->base_angle += 360.0f;
	while (impl->base_angle > 360.0f)
		impl->base_angle -= 360.0f;
}

void CL_Sprite::set_scale(float x, float y)
{
	impl->scale_x = x;
	impl->scale_y = y;
}

void CL_Sprite::set_alpha(float alpha)
{
	impl->alpha = alpha;
}

void CL_Sprite::set_color(float r, float g, float b, float a)
{
	impl->red = r;
	impl->green = g;
	impl->blue = b;
	impl->alpha = a;
}

void CL_Sprite::set_blend_func(CL_BlendFunc src, CL_BlendFunc dest)
{
	impl->blend_src        = src;
	impl->blend_dest       = dest;
	impl->blendfunc_src_alpha  = src;
	impl->blendfunc_dest_alpha = dest;
}

void CL_Sprite::set_blend_func_separate(CL_BlendFunc src, CL_BlendFunc dest, 
                                        CL_BlendFunc src_alpha, CL_BlendFunc dest_alpha)
{
	impl->blend_src        = src;
	impl->blend_dest       = dest;
	impl->blendfunc_src_alpha  = src_alpha;
	impl->blendfunc_dest_alpha = dest_alpha;
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

void CL_Sprite::set_frame_delay(int frameno, float delay)
{
	CL_Sprite_Generic::SpriteFrame *frame = impl->get_frame(frameno);
	if(frame)
		frame->delay = delay;
}

void CL_Sprite::set_frame_offset(int frameno, CL_Point offset)
{
	CL_Sprite_Generic::SpriteFrame *frame = impl->get_frame(frameno);
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

	impl->sig_animation_finished();
}

void CL_Sprite::restart()
{
	impl->update_time = 0;
	impl->last_time = 0;
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

void CL_Sprite::add_frame(CL_Surface surface, const CL_Rect& rect, float delay, CL_Point offset)
{
	if (!impl)
		impl = new CL_Sprite_Generic;
	
	CL_Sprite_Generic::SpriteFrame frame;
			
	frame.surface  = surface;
        if (rect == CL_Rect(0,0,0,0))
          frame.position = CL_Rect(CL_Point(0, 0), CL_Size(surface.get_width(), surface.get_height()));
        else
          frame.position = rect;
	frame.delay    = delay;
	frame.offset   = offset;
			
	impl->frames.push_back(frame);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Sprite signals:

CL_Signal_v0 &CL_Sprite::sig_animation_finished()
{
	return impl->sig_animation_finished;
}
