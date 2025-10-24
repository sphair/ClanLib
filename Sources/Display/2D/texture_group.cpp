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
**    Kenneth Gangstoe
*/

#include "Display/precomp.h"
#include "API/Display/2D/texture_group.h"
#include "API/Display/2D/subtexture.h"
#include "texture_group_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_TextureGroup Construction:

CL_TextureGroup::CL_TextureGroup()
{
}

CL_TextureGroup::CL_TextureGroup(CL_GraphicContext &context, const CL_Size &texture_sizes)
: impl(new CL_TextureGroup_Impl(context, texture_sizes))
{
	set_texture_allocation_policy(create_new_texture);
}

CL_TextureGroup::~CL_TextureGroup()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_TextureGroup Attributes:

void CL_TextureGroup::throw_if_null() const
{
	if (impl.is_null())
		throw CL_Exception("CL_TextureGroup is null");
}

int CL_TextureGroup::get_subtexture_count() const
{
	return impl->get_subtexture_count();
}

int CL_TextureGroup::get_subtexture_count(unsigned int texture_index) const
{
	return impl->get_subtexture_count(texture_index);
}

int CL_TextureGroup::get_texture_count() const
{
	return impl->root_nodes.size();
}

CL_TextureGroup::TextureAllocationPolicy CL_TextureGroup::get_texture_allocation_policy() const
{
	return impl->texture_allocation_policy;
}

CL_Size CL_TextureGroup::get_texture_sizes() const
{
	return impl->initial_texture_size;
}

std::vector<CL_Texture> CL_TextureGroup::get_textures() const
{
	return impl->get_textures();
}

/////////////////////////////////////////////////////////////////////////////
// CL_TextureGroup Operations:

CL_Subtexture CL_TextureGroup::add(CL_GraphicContext &context, const CL_Size &size)
{
	return impl->add_new_node(context, size);
}

void CL_TextureGroup::remove(CL_Subtexture &subtexture)
{
	impl->remove(subtexture);
}

void CL_TextureGroup::set_texture_allocation_policy(TextureAllocationPolicy policy)
{
	impl->texture_allocation_policy = policy;
}

void CL_TextureGroup::insert_texture(CL_Texture &texture, const CL_Rect &texture_rect)
{
	impl->insert_texture(texture, texture_rect);
}

/////////////////////////////////////////////////////////////////////////////
// CL_TextureGroup Implementation:
