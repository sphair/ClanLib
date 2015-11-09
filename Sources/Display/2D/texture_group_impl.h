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

#pragma once

#include <list>
#include "API/Display/Render/texture_2d.h"
#include "API/Display/2D/texture_group.h"

namespace clan
{
	class GraphicContext;

	/// \brief Texture group implementation interface.
	class TextureGroup_Impl
	{
	public:
		class Node
		{
		public:
			Node();
			Node(const Rect &rect);
			~Node();

			int get_subtexture_count() const;

			Node *insert(const Size &texture_size, int texture_id);
			Node *find_image_rect(const Rect &new_rect);

			void clear();

			Node *child[2];
			Rect node_rect;

			int id;
			Rect image_rect;
		};

		struct RootNode
		{
		public:
			Texture2D texture;
			Node node;
		};

		TextureGroup_Impl(const Size &texture_sizes);
		~TextureGroup_Impl();

		int get_subtexture_count() const;
		int get_subtexture_count(unsigned int texture_index) const;
		void insert_texture(Texture2D &texture, const Rect &texture_rect);
		void remove(Subtexture &subtexture);

		std::vector<Texture2D> get_textures() const;

		Subtexture add_new_node(GraphicContext &context, const Size &texture_size);

		std::vector<RootNode *> root_nodes;

		Size initial_texture_size;
		TextureGroup::TextureAllocationPolicy texture_allocation_policy;

	private:
		RootNode *add_new_root(GraphicContext &context, const Size &texture_size);

		RootNode *active_root;
		int next_id;
	};
}
