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
*/

#pragma once


#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/Font/font.h"
#include "API/Core/Math/mat4.h"
#include "Display/2D/render_batch2d.h"
#include "Display/2D/render_batch3d.h"
#include "Display/2D/render_batch_sprite.h"
#include "API/Display/Render/blend_mode.h"
#include "API/Display/Render/pen.h"
#include "API/Display/Render/polygon_rasterizer.h"
#include "API/Display/Render/buffer_control.h"
#include <list>

class CL_RenderBatcher;
class CL_PrimitivesArray_Impl;

class CL_GraphicContext_Impl
{
public:
	CL_GraphicContext_Impl(CL_GraphicContextProvider *provider);
	~CL_GraphicContext_Impl();

	void flush_batcher(CL_GraphicContext &gc);
	void update_batcher_modelview();
	void set_batcher(CL_GraphicContext &gc, CL_RenderBatcher *batcher);
	void set_internal_batcher(CL_MapMode mode);

	CL_SharedPtr<CL_PrimitivesArray_Impl> create_prim_array(CL_SharedPtr<CL_GraphicContext_Impl> this_gc);
	static void free_prim_array(CL_PrimitivesArray_Impl *prim_array);

	CL_GraphicContextProvider *provider;
	std::list<CL_Rect> cliprects;
	std::vector<CL_Mat4f> modelviews;
	std::vector<CL_PrimitivesArray_Impl *> free_prim_arrays;
	int max_attributes;
	bool modelview_changed;
	CL_RenderBatcher *active_batcher;
	CL_RenderBatcherSprite *current_internal_batcher;
	int modelview_index;

	std::vector<CL_Texture> selected_textures;
	CL_BlendMode selected_blend_mode;
	CL_Pen selected_pen;
	CL_BufferControl selected_buffer_control;
	CL_PolygonRasterizer selected_polygon_rasterizer;
	CL_FrameBuffer selected_read_frame_buffer;
	CL_FrameBuffer selected_write_frame_buffer;

private:
	CL_RenderBatch2D render_batcher_2d;
	CL_RenderBatch3D render_batcher_3d;
};
