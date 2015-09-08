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
*/

#include "Display/precomp.h"
#include "API/Display/2D/texture_group.h"
#include "API/Display/2D/subtexture.h"
#include "texture_group_impl.h"

namespace clan
{
	TextureGroup::TextureGroup()
	{
	}

	TextureGroup::TextureGroup(const Size &texture_sizes)
		: impl(std::make_shared<TextureGroup_Impl>(texture_sizes))
	{
		set_texture_allocation_policy(create_new_texture);
	}

	TextureGroup::~TextureGroup()
	{
	}

	void TextureGroup::throw_if_null() const
	{
		if (!impl)
			throw Exception("TextureGroup is null");
	}

	int TextureGroup::get_subtexture_count() const
	{
		return impl->get_subtexture_count();
	}

	int TextureGroup::get_subtexture_count(unsigned int texture_index) const
	{
		return impl->get_subtexture_count(texture_index);
	}

	int TextureGroup::get_texture_count() const
	{
		return impl->root_nodes.size();
	}

	TextureGroup::TextureAllocationPolicy TextureGroup::get_texture_allocation_policy() const
	{
		return impl->texture_allocation_policy;
	}

	Size TextureGroup::get_texture_sizes() const
	{
		return impl->initial_texture_size;
	}

	std::vector<Texture2D> TextureGroup::get_textures() const
	{
		return impl->get_textures();
	}

	Subtexture TextureGroup::add(GraphicContext &context, const Size &size)
	{
		return impl->add_new_node(context, size);
	}

	void TextureGroup::remove(Subtexture &subtexture)
	{
		impl->remove(subtexture);
	}

	void TextureGroup::set_texture_allocation_policy(TextureAllocationPolicy policy)
	{
		impl->texture_allocation_policy = policy;
	}

	void TextureGroup::insert_texture(Texture2D &texture, const Rect &texture_rect)
	{
		impl->insert_texture(texture, texture_rect);
	}
}
