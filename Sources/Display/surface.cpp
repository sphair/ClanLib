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
#include "API/Display/display.h"
#include "API/Display/display_window.h"
#include "API/Display/surface.h"
#include "API/Display/graphic_context.h"
#include "API/Display/Providers/provider_factory.h"
#include "API/Display/pixel_buffer.h"
#include "API/Core/System/error.h"
#include "API/Core/Resources/resource_manager.h"
#include "surface_generic.h"
#include "surface_target.h"
#include "display_target.h"
#include "resourcedata_surface.h"
#include "graphic_context_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Surface construction:

CL_Surface::CL_Surface()
: impl(new CL_Surface_Generic(0))
{
	impl->add_ref();
}

CL_Surface::CL_Surface(
	CL_PixelBuffer provider,
	int flag)
: impl(0)
{
	impl = new CL_Surface_Generic(
		CL_DisplayTarget::current()->create_surface(provider, flag));

	impl->add_ref();
}

CL_Surface::CL_Surface(
	const std::string &resource_id,
	CL_ResourceManager *manager)
: impl(0)
{
	resource = manager->get_resource(resource_id);

	if (resource.get_type() != "surface" )
		throw CL_Error("Resource '" + resource_id + "' is not of type 'surface'");

	resource.load();

	CL_ResourceData_Surface *data =
		(CL_ResourceData_Surface *) resource.get_data("surface");

	if (!data)
		throw CL_Error("Resource '" + resource_id + "' is not of type 'surface'");

	impl = data->get_surface().impl;
	impl->add_ref();
}

CL_Surface::CL_Surface(const std::string &filename, int flag)
: impl(0)
{
	operator =(CL_Surface(CL_ProviderFactory::load(filename), flag));
}

CL_Surface::CL_Surface(const CL_Surface &copy)
: impl(copy.impl)
{
	impl->add_ref();
	resource = copy.resource;
	resource.load();
}

CL_Surface::~CL_Surface()
{
	if (impl) impl->release_ref();
	resource.unload();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Surface attributes:

int CL_Surface::get_width() const
{
	return impl->target->get_width();
}
	
int CL_Surface::get_height() const
{
	return impl->target->get_height();
}

float CL_Surface::get_angle() const
{
	return impl->angle;
}

void CL_Surface::get_scale(float &x, float &y) const
{
	x = impl->scale_x;
	y = impl->scale_y;
}

float CL_Surface::get_alpha() const
{
	return impl->alpha;
}

void CL_Surface::get_color(float &red, float &green, float &blue, float &alpha) const
{
	red = impl->red;
	green = impl->green;
	blue = impl->blue;
	alpha = impl->alpha;
}

void CL_Surface::get_blend_func(CL_BlendFunc &src, CL_BlendFunc &dest) const
{
	src  = impl->blend_src;
	dest = impl->blend_dest;
}

void CL_Surface::get_alignment(CL_Origin &origin, int &x, int &y) const
{
	x = (int)impl->translation_hotspot.x;
	y = (int)impl->translation_hotspot.y;
	origin = impl->translation_origin;
}

void CL_Surface::get_rotation_hotspot(CL_Origin &origin, int &x, int &y) const
{
	x = (int)impl->rotation_hotspot.x;
	y = (int)impl->rotation_hotspot.y;
	origin = impl->rotation_origin;
}

CL_PixelBuffer CL_Surface::get_pixeldata() const
{
	return impl->target->get_pixeldata();
}
	
/////////////////////////////////////////////////////////////////////////////
// CL_Surface operations:

CL_Surface &CL_Surface::operator =(const CL_Surface &copy)
{
	if (&copy == this)
		return *this;
	resource.unload();
	if (impl) impl->release_ref();
	impl = copy.impl;
	if (impl) impl->add_ref();
	resource = copy.resource;
	resource.load();
	return *this;
}

CL_Surface::operator bool() const
{
  return (impl->target != 0);
}

void CL_Surface::set_pixeldata(CL_PixelBuffer &pixeldata)
{
	set_pixeldata(CL_Point(0,0), CL_Rect(0,0,pixeldata.get_width(),pixeldata.get_height()), pixeldata);
}
	
void CL_Surface::set_pixeldata(int x, int y, CL_PixelBuffer &pixeldata)
{
	set_pixeldata(CL_Point(x,y), CL_Rect(0,0,pixeldata.get_width(),pixeldata.get_height()), pixeldata);
}
	
void CL_Surface::set_pixeldata(const CL_Point &pos, CL_PixelBuffer &pixeldata)
{
	set_pixeldata(pos, CL_Rect(0,0,pixeldata.get_width(),pixeldata.get_height()), pixeldata);
}
	
void CL_Surface::set_pixeldata(const CL_Point &pos, const CL_Rect &src_rect, CL_PixelBuffer &pixeldata)
{
	impl->target->set_pixeldata(pos, src_rect, pixeldata);
}

void CL_Surface::draw(
	const CL_Surface_DrawParams1& params1,
	CL_GraphicContext *gc)
{
	if (gc == 0) gc = CL_Display::get_current_window()->get_gc();
	impl->target->draw(
		impl,
		params1,
		gc);
}

void CL_Surface::draw(
	const CL_Surface_DrawParams2& params2,
	CL_GraphicContext *gc)
{
	if (gc == 0) gc = CL_Display::get_current_window()->get_gc();
	impl->target->draw(
		impl,
		params2,
		gc);
}

void CL_Surface::draw(
	float x,
	float y,
	CL_GraphicContext *gc)
{
	static CL_Surface_DrawParams2 params2;
	params2.srcX = 0;
	params2.srcY = 0;
	params2.srcWidth = impl->target->get_width();
	params2.srcHeight = impl->target->get_height();
	params2.destX = x;
	params2.destY = y;
	params2.destZ = 0.0;
	params2.red = impl->red;
	params2.green = impl->green;
	params2.blue = impl->blue;
	params2.alpha = impl->alpha;

	params2.blend_src = impl->blend_src;
	params2.blend_dest = impl->blend_dest;
	params2.blendfunc_src_alpha = impl->blendfunc_src_alpha;
	params2.blendfunc_dest_alpha = impl->blendfunc_dest_alpha;

	params2.scale_x = impl->scale_x;
	params2.scale_y = impl->scale_y;
	params2.translate_origin = impl->translation_origin;
	params2.translate_x = int(impl->translation_hotspot.x);
	params2.translate_y = int(impl->translation_hotspot.y);
	params2.rotate_angle = impl->angle;
	params2.rotate_pitch = impl->angle_pitch;
	params2.rotate_yaw = impl->angle_yaw;
	params2.rotate_origin = impl->rotation_origin;
	params2.rotate_x = int(impl->rotation_hotspot.x);
	params2.rotate_y = int(impl->rotation_hotspot.y);

	draw(params2, gc);
}

void CL_Surface::draw_subpixel(
					  float x,
					  float y,
					  CL_GraphicContext *gc)
{
	static CL_Surface_DrawParams2 params2;
	params2.srcX = 0;
	params2.srcY = 0;
	params2.srcWidth = impl->target->get_width();
	params2.srcHeight = impl->target->get_height();
	params2.destX = x;
	params2.destY = y;
	params2.destZ = 0.0;
	params2.red = impl->red;
	params2.green = impl->green;
	params2.blue = impl->blue;
	params2.alpha = impl->alpha;

	params2.blend_src = impl->blend_src;
	params2.blend_dest = impl->blend_dest;
	params2.blendfunc_src_alpha = impl->blendfunc_src_alpha;
	params2.blendfunc_dest_alpha = impl->blendfunc_dest_alpha;

	params2.scale_x = impl->scale_x;
	params2.scale_y = impl->scale_y;
	params2.translate_origin = impl->translation_origin;
	params2.translate_x = int(impl->translation_hotspot.x);
	params2.translate_y = int(impl->translation_hotspot.y);
	params2.rotate_angle = impl->angle;
	params2.rotate_pitch = impl->angle_pitch;
	params2.rotate_yaw = impl->angle_yaw;
	params2.rotate_origin = impl->rotation_origin;
	params2.rotate_x = int(impl->rotation_hotspot.x);
	params2.rotate_y = int(impl->rotation_hotspot.y);
	params2.sub_pixel_accuracy = true;

	draw(params2, gc);
}

void CL_Surface::draw(
	const CL_Rect &dest,
	CL_GraphicContext *gc)
{
	static CL_Surface_DrawParams2 params2;
	params2.srcX = 0;
	params2.srcY = 0;
	params2.srcWidth = impl->target->get_width();
	params2.srcHeight = impl->target->get_height();
	params2.destX = dest.left;
	params2.destY = dest.top;
	params2.destZ = 0.0;
	params2.red = impl->red;
	params2.green = impl->green;
	params2.blue = impl->blue;
	params2.alpha = impl->alpha;

	params2.blend_src = impl->blend_src;
	params2.blend_dest = impl->blend_dest;
	params2.blendfunc_src_alpha = impl->blendfunc_src_alpha;
	params2.blendfunc_dest_alpha = impl->blendfunc_dest_alpha;

	params2.scale_x = dest.get_width()/float(impl->target->get_width());
	params2.scale_y = dest.get_height()/float(impl->target->get_height());
	params2.translate_origin = impl->translation_origin;
	params2.translate_x = int(impl->translation_hotspot.x);
	params2.translate_y = int(impl->translation_hotspot.y);
	params2.rotate_angle = impl->angle;
	params2.rotate_pitch = impl->angle_pitch;
	params2.rotate_yaw = impl->angle_yaw;
	params2.rotate_origin = impl->rotation_origin;
	params2.rotate_x = int(impl->rotation_hotspot.x);
	params2.rotate_y = int(impl->rotation_hotspot.y);

	draw(params2, gc);
}

void CL_Surface::draw(
	const CL_Rect &src,
	const CL_Rect &dest,
	CL_GraphicContext *gc)
{
	static CL_Surface_DrawParams2 params2;
	params2.srcX = src.left;
	params2.srcY = src.top;
	params2.srcWidth = src.get_width();
	params2.srcHeight = src.get_height();
	params2.destX = dest.left;
	params2.destY = dest.top;
	params2.destZ = 0.0;
	params2.red = impl->red;
	params2.green = impl->green;
	params2.blue = impl->blue;
	params2.alpha = impl->alpha;

	params2.blend_src = impl->blend_src;
	params2.blend_dest = impl->blend_dest;
	params2.blendfunc_src_alpha = impl->blendfunc_src_alpha;
	params2.blendfunc_dest_alpha = impl->blendfunc_dest_alpha;

	params2.scale_x = dest.get_width()/float(src.get_width());
	params2.scale_y = dest.get_height()/float(src.get_height());
	params2.translate_origin = impl->translation_origin;
	params2.translate_x = int(impl->translation_hotspot.x);
	params2.translate_y = int(impl->translation_hotspot.y);
	params2.rotate_angle = impl->angle;
	params2.rotate_pitch = impl->angle_pitch;
	params2.rotate_yaw = impl->angle_yaw;
	params2.rotate_origin = impl->rotation_origin;
	params2.rotate_x = int(impl->rotation_hotspot.x);
	params2.rotate_y = int(impl->rotation_hotspot.y);
	
	draw(params2, gc);
}

void CL_Surface::draw_subpixel(
					  const CL_Rectf &src,
					  const CL_Rectf &dest,
					  CL_GraphicContext *gc)
{
	static CL_Surface_DrawParams2 params2;
	params2.srcX = int(src.left);
	params2.srcY = int(src.top);
	params2.srcWidth = int(src.get_width());
	params2.srcHeight = int(src.get_height());
	params2.destX = dest.left;
	params2.destY = dest.top;
	params2.destZ = 0.0;
	params2.red = impl->red;
	params2.green = impl->green;
	params2.blue = impl->blue;
	params2.alpha = impl->alpha;

	params2.blend_src = impl->blend_src;
	params2.blend_dest = impl->blend_dest;
	params2.blendfunc_src_alpha = impl->blendfunc_src_alpha;
	params2.blendfunc_dest_alpha = impl->blendfunc_dest_alpha;

	params2.scale_x = dest.get_width()/float(src.get_width());
	params2.scale_y = dest.get_height()/float(src.get_height());
	params2.translate_origin = impl->translation_origin;
	params2.translate_x = int(impl->translation_hotspot.x);
	params2.translate_y = int(impl->translation_hotspot.y);
	params2.rotate_angle = impl->angle;
	params2.rotate_pitch = impl->angle_pitch;
	params2.rotate_yaw = impl->angle_yaw;
	params2.rotate_origin = impl->rotation_origin;
	params2.rotate_x = int(impl->rotation_hotspot.x);
	params2.rotate_y = int(impl->rotation_hotspot.y);
	params2.sub_pixel_accuracy = true;

	draw(params2, gc);
}


void CL_Surface::setup_params(const CL_Rectf &src,
					  const CL_Rectf &dest,
					  CL_Surface_DrawParams1 &params1,
					  bool sub_pixel_accuracy)
{
	static CL_Surface_DrawParams2 params2;
	params2.srcX = int(src.left);
	params2.srcY = int(src.top);
	params2.srcWidth = int(src.get_width());
	params2.srcHeight = int(src.get_height());
	params2.destX = dest.left;
	params2.destY = dest.top;
	params2.destZ = 0.0;
	params2.red = impl->red;
	params2.green = impl->green;
	params2.blue = impl->blue;
	params2.alpha = impl->alpha;

	params2.blend_src = impl->blend_src;
	params2.blend_dest = impl->blend_dest;
	params2.blendfunc_src_alpha = impl->blendfunc_src_alpha;
	params2.blendfunc_dest_alpha = impl->blendfunc_dest_alpha;

	params2.scale_x = dest.get_width()/float(src.get_width());
	params2.scale_y = dest.get_height()/float(src.get_height());
	params2.translate_origin = impl->translation_origin;
	params2.translate_x = int(impl->translation_hotspot.x);
	params2.translate_y = int(impl->translation_hotspot.y);
	params2.rotate_angle = impl->angle;
	params2.rotate_pitch = impl->angle_pitch;
	params2.rotate_yaw = impl->angle_yaw;
	params2.rotate_origin = impl->rotation_origin;
	params2.rotate_x = int(impl->rotation_hotspot.x);
	params2.rotate_y = int(impl->rotation_hotspot.y);
	params2.sub_pixel_accuracy = sub_pixel_accuracy;

	static CL_Surface_TargetDrawParams1 t_params1;

	setup_target_params(params2, t_params1);
	setup_draw_params(params2, &t_params1, params1);
}

void CL_Surface::setup_target_params(
	const CL_Surface_DrawParams2 &params2,
	CL_Surface_TargetDrawParams1 &t_params1)
{
	impl->target->setup_target_params(params2, t_params1);
	
}

void CL_Surface::setup_draw_params(
	const CL_Surface_DrawParams2 & params2,
	const CL_Surface_TargetDrawParams1 *t_params1,
	CL_Surface_DrawParams1 &params1)
{

	impl->target->setup_draw_params(params2, t_params1, params1);
}

void CL_Surface::set_angle(float angle)
{
	impl = impl->copy_on_write();
	impl->angle = angle;

	while(impl->angle >= 360.0f)
		impl->angle -= 360.0f;
	while(impl->angle < 0.0f)
		impl->angle += 360.0f;
}

void CL_Surface::set_angle_pitch(float angle)
{
	impl = impl->copy_on_write();
	impl->angle_pitch = angle;

	while(impl->angle_pitch >= 360.0f)
		impl->angle_pitch -= 360.0f;
	while(impl->angle_pitch < 0.0f)
		impl->angle_pitch += 360.0f;
}

void CL_Surface::set_angle_yaw(float angle)
{
	impl = impl->copy_on_write();
	impl->angle_yaw = angle;

	while(impl->angle_yaw >= 360.0f)
		impl->angle_yaw -= 360.0f;
	while(impl->angle_yaw < 0.0f)
		impl->angle_yaw += 360.0f;
}

void CL_Surface::rotate(float angle)
{
	impl = impl->copy_on_write();
	impl->angle += angle;

	while(impl->angle >= 360.0f)
		impl->angle -= 360.0f;
	while(impl->angle < 0.0f)
		impl->angle += 360.0f;
}

void CL_Surface::rotate_pitch(float angle)
{
	impl = impl->copy_on_write();
	impl->angle_pitch += angle;

	while(impl->angle_pitch >= 360.0f)
		impl->angle_pitch -= 360.0f;
	while(impl->angle_pitch < 0.0f)
		impl->angle_pitch += 360.0f;
}

void CL_Surface::rotate_yaw(float angle)
{
	impl = impl->copy_on_write();
	impl->angle_yaw += angle;

	while(impl->angle_yaw >= 360.0f)
		impl->angle_yaw -= 360.0f;
	while(impl->angle_yaw < 0.0f)
		impl->angle_yaw += 360.0f;
}

void CL_Surface::set_scale(float x, float y)
{
	impl = impl->copy_on_write();
	impl->scale_x = x;
	impl->scale_y = y;
}

void CL_Surface::set_alpha(float alpha)
{
	impl = impl->copy_on_write();
	impl->alpha = alpha;
}

void CL_Surface::set_color(float r, float g, float b, float a)
{
	impl = impl->copy_on_write();
	impl->red   = r;
	impl->green = g;
	impl->blue  = b;
	impl->alpha = a;
}

void CL_Surface::set_blend_func_separate(CL_BlendFunc src, CL_BlendFunc dest,
													  CL_BlendFunc src_alpha, CL_BlendFunc dest_alpha)
{
	impl = impl->copy_on_write();
	impl->blend_src        = src;
	impl->blend_dest       = dest; 
	impl->blendfunc_src_alpha  = src_alpha;
	impl->blendfunc_dest_alpha = dest_alpha; 
}

void CL_Surface::set_blend_func(CL_BlendFunc src, CL_BlendFunc dest)
{
	impl = impl->copy_on_write();
	impl->blend_src        = src;
	impl->blend_dest       = dest;
	impl->blendfunc_src_alpha  = src;
	impl->blendfunc_dest_alpha = dest;	
}

void CL_Surface::set_alignment(CL_Origin origin, int x, int y)
{
	impl = impl->copy_on_write();
	impl->translation_hotspot.x = (float)x;
	impl->translation_hotspot.y = (float)y;
	impl->translation_origin = origin;
}

void CL_Surface::set_rotation_hotspot(CL_Origin origin, int x, int y)
{
	impl = impl->copy_on_write();
	impl->rotation_hotspot.x = (float)x;
	impl->rotation_hotspot.y = (float)y;
	impl->rotation_origin = origin;
}

int CL_Surface::get_handle()
{
	return impl->target->get_handle();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Surface implementation:
