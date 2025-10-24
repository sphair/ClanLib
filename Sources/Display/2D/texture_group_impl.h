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
*/

#pragma once

#include <list>
#include "API/Display/Render/texture.h"
#include "API/Display/2D/texture_group.h"

class CL_GraphicContext;

/// \brief Texture group implementation interface.
class CL_TextureGroup_Impl
{
public:
	class Node
	{
	public:
		Node();
		Node(const CL_Rect &rect);
		~Node();

		int get_subtexture_count() const;

		Node *insert(const CL_Size &texture_size, int texture_id);
		Node *find_image_rect(const CL_Rect &new_rect);

		void clear();

		Node *child[2];
		CL_Rect node_rect;

	    int id;
	    CL_Rect image_rect;
	};

	struct RootNode
	{
	public:
		CL_Texture texture;
		Node node;
	};

public:
	CL_TextureGroup_Impl(const CL_Size &texture_sizes);
	~CL_TextureGroup_Impl();

	int get_subtexture_count() const;
	int get_subtexture_count(unsigned int texture_index) const;
	void insert_texture(CL_Texture &texture, const CL_Rect &texture_rect);
	void remove(CL_Subtexture &subtexture);

	std::vector<CL_Texture> get_textures() const;

	CL_Subtexture add_new_node(CL_GraphicContext &context, const CL_Size &texture_size);

	std::vector<RootNode *> root_nodes;

	CL_Size initial_texture_size;
	CL_TextureGroup::TextureAllocationPolicy texture_allocation_policy;

private:
	RootNode *add_new_root(CL_GraphicContext &context, const CL_Size &texture_size);

	RootNode *active_root;
	int next_id;
};
