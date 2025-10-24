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

#include "Display/precomp.h"
#include "graphic_context_impl.h"
#include "primitives_array_impl.h"
#include "API/Display/Render/render_batcher.h"
#include "API/Display/Render/shared_gc_data.h"

CL_GraphicContext_Impl::CL_GraphicContext_Impl(CL_GraphicContextProvider *provider)
: provider(provider), max_attributes(0), modelview_changed(false), active_batcher(0), modelview_index(0), current_internal_batcher(&render_batcher_2d)
{
	selected_textures.resize(8);	// Create 8 unit indexes by default
	modelviews.push_back(CL_Mat4f::identity());
	max_attributes = provider->get_max_attributes();
	CL_SharedGCData::add_ref();
}

CL_GraphicContext_Impl::~CL_GraphicContext_Impl()
{
	CL_SharedGCData::release_ref();

	if (provider)
		provider->destroy();

	std::vector<CL_PrimitivesArray_Impl *>::size_type index, size;
	size = free_prim_arrays.size();
	for (index = 0; index < size; index++)
		delete free_prim_arrays[index];
}

CL_SharedPtr<CL_PrimitivesArray_Impl> CL_GraphicContext_Impl::create_prim_array(CL_SharedPtr<CL_GraphicContext_Impl> this_gc)
{
	CL_PrimitivesArray_Impl *prim_array_impl = 0;
	if (!free_prim_arrays.empty())
	{
		prim_array_impl = free_prim_arrays.back();
		free_prim_arrays.pop_back();
		prim_array_impl->clear();
		if (provider)
			provider->primitives_array_freed(prim_array_impl);
	}
	else
	{
		CL_WeakPtr<CL_GraphicContext_Impl> this_weakptr(this_gc);
		prim_array_impl = new CL_PrimitivesArray_Impl(max_attributes, this_weakptr );
	}
	return CL_SharedPtr<CL_PrimitivesArray_Impl>(prim_array_impl, &CL_GraphicContext_Impl::free_prim_array, 0);
}

void CL_GraphicContext_Impl::free_prim_array(CL_PrimitivesArray_Impl *prim_array)
{
	// If the graphic context has been destroyed, then simply delete the prim_array
	if (prim_array->gc.is_invalid_weak_link())
	{
		delete prim_array;
	}
	else
	{
		prim_array->gc->free_prim_arrays.push_back(prim_array);
	}
}

void CL_GraphicContext_Impl::flush_batcher(CL_GraphicContext &gc)
{
	if (active_batcher)
	{
		CL_RenderBatcher *batcher = active_batcher;
		active_batcher = 0;
		batcher->flush(gc);
	}
}

void CL_GraphicContext_Impl::update_batcher_modelview()
{
	if (active_batcher)
		active_batcher->modelview_changed(modelviews[modelview_index]);
}

void CL_GraphicContext_Impl::set_batcher(CL_GraphicContext &gc, CL_RenderBatcher *batcher)
{
	if (active_batcher != batcher)
	{
		flush_batcher(gc);
		active_batcher = batcher;
		if (active_batcher)
			active_batcher->modelview_changed(modelviews[modelview_index]);
	}
}

void CL_GraphicContext_Impl::set_internal_batcher(CL_MapMode mode)
{
	if (mode == cl_user_projection)
	{
		current_internal_batcher = &render_batcher_3d;
	}
	else
	{
		current_internal_batcher = &render_batcher_2d;
	}
}
