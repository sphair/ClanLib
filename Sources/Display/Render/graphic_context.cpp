/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#include "Display/precomp.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Render/texture.h"
#include "API/Display/Render/blend_mode.h"
#include "API/Display/Render/pen.h"
#include "API/Display/Render/polygon_rasterizer.h"
#include "API/Display/Font/font.h"
#include "API/Display/Render/buffer_control.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Image/pixel_format.h"
#include "API/Display/display.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Render/element_array_buffer.h"
#include "API/Display/Render/primitives_array.h"
#include "API/Display/TargetProviders/font_provider.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Math/angle.h"
#include "primitives_array_impl.h"
#include "graphic_context_impl.h"
#include "API/Display/Render/shared_gc_data.h" 

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext Construction:

CL_GraphicContext::CL_GraphicContext()
{
}

CL_GraphicContext::CL_GraphicContext(CL_GraphicContextProvider *provider)
: impl(new CL_GraphicContext_Impl(provider))
{
	reset_blend_mode();
	reset_pen();
	reset_buffer_control();
	reset_polygon_rasterizer();
	set_map_mode(cl_map_2d_upper_left);
}

CL_GraphicContext::~CL_GraphicContext()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext Attributes:

CL_Texture CL_GraphicContext::get_texture(int unit)
{
	//TODO: Used "unit" parameter
	return CL_Texture();
}

CL_BlendMode CL_GraphicContext::get_blend_mode()
{
	return CL_BlendMode();
}

CL_Pen CL_GraphicContext::get_pen()
{
	return CL_Pen();
}

CL_PolygonRasterizer CL_GraphicContext::get_polygon_rasterizer()
{
	return CL_PolygonRasterizer();
}

int CL_GraphicContext::get_width() const
{
	return impl->provider->get_width();
}

int CL_GraphicContext::get_height() const
{
	return impl->provider->get_height();
}

CL_Size CL_GraphicContext::get_size() const
{
	return CL_Size(impl->provider->get_width(), impl->provider->get_height());
}

CL_Rect CL_GraphicContext::get_cliprect() const
{
	if (!impl->cliprects.empty())
		return impl->cliprects.front();

	return CL_Rect(0,0,get_width(),get_height());
}

const CL_Mat4f &CL_GraphicContext::get_modelview() const
{
	return impl->modelviews[impl->modelview_index];
}

CL_Size CL_GraphicContext::get_max_texture_size() const
{
	return impl->provider->get_max_texture_size();
}

CL_GraphicContextProvider *CL_GraphicContext::get_provider()
{
	if (!impl.is_null())
		return impl->provider;
	else
		return 0;
}

const CL_GraphicContextProvider * const CL_GraphicContext::get_provider() const
{
	if (!impl.is_null())
		return impl->provider;
	else
		return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext Operations:

CL_GraphicContext CL_GraphicContext::create_worker_gc()
{
	return impl->provider->create_worker_gc();
}

CL_PixelBuffer CL_GraphicContext::get_pixeldata(const CL_Rect &rect) const
{
	return impl->provider->get_pixeldata(rect);
}

void CL_GraphicContext::set_texture(int unit_index, const CL_Texture &texture)
{
	impl->flush_batcher(*this);
	impl->provider->set_texture(unit_index, texture);
}

void CL_GraphicContext::reset_texture(int unit_index)
{
	impl->flush_batcher(*this);
	impl->provider->reset_texture(unit_index);
}

void CL_GraphicContext::set_program_object(CL_StandardProgram standard_program)
{
	impl->flush_batcher(*this);
	impl->provider->set_program_object(standard_program);
}

void CL_GraphicContext::set_program_object(const CL_ProgramObject &program)
{
	impl->flush_batcher(*this);
	impl->provider->set_program_object(program);
}

void CL_GraphicContext::reset_program_object()
{
	impl->flush_batcher(*this);
	impl->provider->reset_program_object();
}

void CL_GraphicContext::set_buffer_control(const CL_BufferControl &buffer_control)
{
	impl->flush_batcher(*this);
	impl->provider->set_buffer_control(buffer_control);
}

void CL_GraphicContext::reset_buffer_control()
{
	impl->flush_batcher(*this);
	static CL_BufferControl buffer_control;
	impl->provider->set_buffer_control(buffer_control);
}

void CL_GraphicContext::set_blend_mode(const CL_BlendMode &blend_mode)
{
	impl->flush_batcher(*this);
	impl->provider->set_blend_mode(blend_mode);
}

void CL_GraphicContext::reset_blend_mode()
{
	impl->flush_batcher(*this);
	static CL_BlendMode blend_mode;
	impl->provider->set_blend_mode(blend_mode);
}

void CL_GraphicContext::set_pen(const CL_Pen &pen)
{
	impl->flush_batcher(*this);
	impl->provider->set_pen(pen);
}

void CL_GraphicContext::reset_pen()
{
	impl->flush_batcher(*this);
	static CL_Pen pen;
	impl->provider->set_pen(pen);
}

void CL_GraphicContext::set_polygon_rasterizer(const CL_PolygonRasterizer &raster)
{
	impl->flush_batcher(*this);
	impl->provider->set_polygon_rasterizer(raster);
}

void CL_GraphicContext::reset_polygon_rasterizer()
{
	impl->flush_batcher(*this);
	static CL_PolygonRasterizer raster;
	impl->provider->set_polygon_rasterizer(raster);
}

void CL_GraphicContext::draw_primitives(CL_PrimitivesType type, int num_vertices, const CL_PrimitivesArray &prim_array)
{
	impl->flush_batcher(*this);
	if (impl->modelview_changed)
	{
		impl->provider->set_modelview(impl->modelviews[impl->modelview_index]);
		impl->modelview_changed = false;
	}
	impl->provider->draw_primitives(type, num_vertices, prim_array.impl);
}

void CL_GraphicContext::set_primitives_array(const CL_PrimitivesArray &prim_array)
{
	impl->flush_batcher(*this);
	impl->provider->set_primitives_array(prim_array.impl);
}

void CL_GraphicContext::draw_primitives_array(CL_PrimitivesType type, int num_vertices)
{
	impl->flush_batcher(*this);
	if (impl->modelview_changed)
	{
		impl->provider->set_modelview(impl->modelviews[impl->modelview_index]);
		impl->modelview_changed = false;
	}
	impl->provider->draw_primitives_array(type, 0, num_vertices);
}

void CL_GraphicContext::draw_primitives_array(CL_PrimitivesType type, int offset, int num_vertices)
{
	impl->flush_batcher(*this);
	if (impl->modelview_changed)
	{
		impl->provider->set_modelview(impl->modelviews[impl->modelview_index]);
		impl->modelview_changed = false;
	}
	impl->provider->draw_primitives_array(type, offset, num_vertices);
}

void CL_GraphicContext::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned int *indices)
{
	impl->flush_batcher(*this);
	if (impl->modelview_changed)
	{
		impl->provider->set_modelview(impl->modelviews[impl->modelview_index]);
		impl->modelview_changed = false;
	}
	impl->provider->draw_primitives_elements(type, count, indices);
}

void CL_GraphicContext::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned short *indices)
{
	impl->flush_batcher(*this);
	if (impl->modelview_changed)
	{
		impl->provider->set_modelview(impl->modelviews[impl->modelview_index]);
		impl->modelview_changed = false;
	}
	impl->provider->draw_primitives_elements(type, count, indices);
}

void CL_GraphicContext::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned char *indices)
{
	impl->flush_batcher(*this);
	if (impl->modelview_changed)
	{
		impl->provider->set_modelview(impl->modelviews[impl->modelview_index]);
		impl->modelview_changed = false;
	}
	impl->provider->draw_primitives_elements(type, count, indices);
}

void CL_GraphicContext::draw_primitives_elements(CL_PrimitivesType type, int count, CL_ElementArrayBuffer &elements_array, CL_VertexAttributeDataType indices_type, void *offset)
{
	impl->flush_batcher(*this);
	if (impl->modelview_changed)
	{
		impl->provider->set_modelview(impl->modelviews[impl->modelview_index]);
		impl->modelview_changed = false;
	}
	impl->provider->draw_primitives_elements(type, count, elements_array.get_provider(), indices_type, offset);
}

void CL_GraphicContext::reset_primitives_array()
{
	impl->flush_batcher(*this);
	impl->provider->reset_primitives_array();
}

void CL_GraphicContext::draw_pixels(float x, float y, const CL_PixelBufferRef &image, const CL_Colorf &color)
{
	impl->flush_batcher(*this);
	if (impl->modelview_changed)
	{
		impl->provider->set_modelview(impl->modelviews[impl->modelview_index]);
		impl->modelview_changed = false;
	}
	impl->provider->draw_pixels(x, y, 1.0, 1.0, image, color);
}

void CL_GraphicContext::draw_pixels(float x, float y, float zoom_x, float zoom_y, const CL_PixelBufferRef &image, const CL_Colorf &color)
{
	impl->flush_batcher(*this);
	if (impl->modelview_changed)
	{
		impl->provider->set_modelview(impl->modelviews[impl->modelview_index]);
		impl->modelview_changed = false;
	}
	impl->provider->draw_pixels(x, y, zoom_x, zoom_y, image, color);
}

void CL_GraphicContext::clear(const CL_Colorf &color)
{
	impl->flush_batcher(*this);
	impl->provider->clear( color );
}

void CL_GraphicContext::clear_stencil(int value)
{
	impl->flush_batcher(*this);
	impl->provider->clear_stencil(value);
}

void CL_GraphicContext::clear_depth(float value)
{
	impl->flush_batcher(*this);
	impl->provider->clear_depth(value);
}

void CL_GraphicContext::set_cliprect(const CL_Rect &rect)
{
	impl->flush_batcher(*this);
	if (!impl->cliprects.empty())
		impl->cliprects.front() = rect;
	else
		impl->cliprects.push_front(rect);

	impl->provider->set_clip_rect(rect);
}

void CL_GraphicContext::push_cliprect(const CL_Rect &rect)
{
	impl->flush_batcher(*this);
	if (!impl->cliprects.empty())
	{
		CL_Rect r = impl->cliprects.front();
		r.overlap(rect);
		impl->cliprects.push_front(r);
	}
	else
	{
		impl->cliprects.push_front(rect);
	}

	impl->provider->set_clip_rect(impl->cliprects.front());
}

void CL_GraphicContext::push_cliprect()
{
	impl->flush_batcher(*this);
	if (impl->cliprects.empty())
	{
		impl->cliprects.push_back(CL_Rect(0,0,get_width(),get_height()));
	}
	else
	{
		impl->cliprects.push_front(impl->cliprects.front());
		impl->provider->set_clip_rect(impl->cliprects.front());
	}
}

void CL_GraphicContext::pop_cliprect()
{
	impl->flush_batcher(*this);
	if (impl->cliprects.empty())
		throw CL_Exception(cl_text("CL_GraphicContext::pop_cliprect - popped too many times!"));

	impl->cliprects.pop_front();

	if (impl->cliprects.empty())
		reset_cliprect();
	else
		impl->provider->set_clip_rect(impl->cliprects.front());
}

void CL_GraphicContext::reset_cliprect()
{
	impl->flush_batcher(*this);
	impl->cliprects.clear();
	impl->provider->reset_clip_rect();
}

void CL_GraphicContext::set_map_mode(CL_MapMode mode)
{
	impl->flush_batcher(*this);

	if (impl->modelview_changed)
	{
		impl->provider->set_modelview(impl->modelviews[impl->modelview_index]);
		impl->modelview_changed = false;
	}

	impl->provider->set_map_mode(mode);
}

void CL_GraphicContext::set_projection(const CL_Mat4f &matrix)
{
	impl->flush_batcher(*this);
	impl->provider->set_projection(matrix);
}

void CL_GraphicContext::set_modelview(const CL_Mat4f &matrix)
{
	impl->modelviews[impl->modelview_index] = matrix;
	impl->modelview_changed = true;
	impl->update_batcher_modelview();
}

void CL_GraphicContext::mult_modelview(const CL_Mat4f &matrix)
{
	impl->modelviews[impl->modelview_index].multiply(matrix);
	impl->modelview_changed = true;
	impl->update_batcher_modelview();
}

void CL_GraphicContext::set_frame_buffer(const CL_FrameBuffer &frame_buffer)
{
	impl->flush_batcher(*this);
	impl->provider->set_frame_buffer(frame_buffer);
}

void CL_GraphicContext::reset_frame_buffer()
{
	impl->flush_batcher(*this);
	impl->provider->reset_frame_buffer();
}

void CL_GraphicContext::push_modelview()
{
	impl->modelviews.push_back(impl->modelviews[impl->modelview_index]);
	impl->modelview_index++;
}

void CL_GraphicContext::set_translate(float x, float y, float z)
{
	CL_Mat4f matrix = CL_Mat4f::translate(x, y, z);
	set_modelview(matrix);
}

void CL_GraphicContext::mult_translate(float x, float y, float z)
{
	CL_Mat4f matrix = CL_Mat4f::translate(x, y, z);
	mult_modelview(matrix);
}

void CL_GraphicContext::push_translate(float x, float y, float z)
{
	push_modelview();
	mult_translate(x, y, z);
}

void CL_GraphicContext::set_rotate(const CL_Angle &angle, float x, float y, float z, bool normalize)
{
	set_modelview(CL_Mat4f::rotate(angle, x, y, z, normalize));
}

void CL_GraphicContext::mult_rotate(const CL_Angle &angle, float x, float y, float z, bool normalize)
{
	mult_modelview(CL_Mat4f::rotate(angle, x, y, z, normalize));
}

void CL_GraphicContext::push_rotate(const CL_Angle &angle, float x, float y, float z)
{
	push_modelview();
	mult_rotate(angle, x, y, z);
}

void CL_GraphicContext::set_scale(float x, float y, float z)
{
	CL_Mat4f matrix = CL_Mat4f::scale(x, y, z);
	set_modelview(matrix);
}

void CL_GraphicContext::mult_scale(float x, float y, float z)
{
	CL_Mat4f matrix = CL_Mat4f::scale(x, y, z);
	mult_modelview(matrix);
}

void CL_GraphicContext::push_scale(float x, float y, float z)
{
	push_modelview();
	mult_scale(x, y, z);
}

void CL_GraphicContext::pop_modelview()
{
/*	impl->modelviews.pop_front();
	if (impl->modelviews.empty())
		impl->modelviews.push_front(CL_Mat4f::identity());
*/
	impl->modelviews.pop_back();
	impl->modelview_index--;

	if (impl->modelviews.empty())
	{
		impl->modelviews.push_back(CL_Mat4f::identity());
		impl->modelview_index = 0;
	}

	impl->modelview_changed = true;
	impl->update_batcher_modelview();
}

void CL_GraphicContext::set_viewport(const CL_Rectf &viewport)
{
	impl->flush_batcher(*this);
	impl->provider->set_viewport(viewport);
}

void CL_GraphicContext::flush_batcher()
{
	impl->flush_batcher(*this);
}

void CL_GraphicContext::set_batcher(CL_RenderBatcher *batcher)
{
	impl->set_batcher(*this, batcher);
}

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext Events:

CL_Signal_v0 & CL_GraphicContext::sig_gc_destruction_imminent()
{
	return CL_SharedGCData::func_gc_destruction_imminent();
}

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext Implementation:
