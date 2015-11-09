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
#include "API/Display/2D/subtexture.h"
#include "API/Core/Math/point.h"
#include "API/Core/Math/rect.h"
#include "texture_group_impl.h"

namespace clan
{
	TextureGroup_Impl::TextureGroup_Impl(const Size &texture_sizes)
		: initial_texture_size(texture_sizes), active_root(nullptr), next_id(0)
	{
	}

	TextureGroup_Impl::~TextureGroup_Impl()
	{
		std::vector<RootNode *>::size_type index, size;
		size = root_nodes.size();
		for (index = 0; index < size; ++index)
		{
			root_nodes[index]->node.clear();
			delete root_nodes[index];
		}
	}

	int TextureGroup_Impl::get_subtexture_count() const
	{
		int count = 0;

		std::vector<RootNode *>::size_type index, size;
		size = root_nodes.size();
		for (index = 0; index < size; ++index)
			count += root_nodes[index]->node.get_subtexture_count();

		return count;
	}

	int TextureGroup_Impl::get_subtexture_count(unsigned int texture_index) const
	{
		int count = 0;

		if (texture_index < root_nodes.size())
			count = root_nodes[texture_index]->node.get_subtexture_count();

		return count;
	}

	std::vector<Texture2D> TextureGroup_Impl::get_textures() const
	{
		std::vector<Texture2D> textures;

		std::vector<RootNode *>::size_type index, size;
		size = root_nodes.size();
		for (index = 0; index < size; ++index)
		{
			textures.push_back(root_nodes[index]->texture);
		}

		return textures;
	}

	Subtexture TextureGroup_Impl::add_new_node(GraphicContext &context, const Size &texture_size)
	{
		// Try inserting in current active texture
		Node *node;
		if (!active_root)
		{
			// Create an initial root, if it does not exist
			node = nullptr;
			next_id = 1;
		}
		else
		{
			node = active_root->node.insert(texture_size, next_id);
		}

		if (node == nullptr) // Couldn't find a fit in current active texture
		{
			// Search previous textures if policy says so
			if (texture_allocation_policy == TextureGroup::search_previous_textures)
			{
				std::vector<RootNode *>::size_type index, size;
				size = root_nodes.size();
				for (index = 0; index < size; ++index)
				{
					node = root_nodes[index]->node.insert(texture_size, next_id);
					if (node)	// We found space in a previous texture
						break;
				}
			}

			if (node == nullptr) // Couldn't find a fit, so create a new texture
			{
				if (texture_size.width > initial_texture_size.width || texture_size.height > initial_texture_size.height)
				{
					// If the specified size is greater than the initial size,  then create a texture using the specified size
					node = add_new_root(context, texture_size)->node.insert(texture_size, next_id);
				}
				else
				{
					node = add_new_root(context, initial_texture_size)->node.insert(texture_size, next_id);
				}
			}

			if (node == nullptr)
				throw Exception("Unable to pack Texture into TextureGroup");
		}

		next_id++;

		return Subtexture(active_root->texture, node->image_rect);
	}

	TextureGroup_Impl::RootNode *TextureGroup_Impl::add_new_root(GraphicContext &context, const Size &texture_size)
	{
		Rect rect(Point(0, 0), texture_size);
		Node node(rect);

		active_root = new RootNode();
		active_root->texture = Texture2D(context, texture_size);
		active_root->node = node;

		root_nodes.push_back(active_root);

		return active_root;
	}

	void TextureGroup_Impl::insert_texture(Texture2D &texture, const Rect &texture_rect)
	{
		Node node(texture_rect);

		active_root = new RootNode();
		active_root->texture = texture;
		active_root->node = node;

		root_nodes.push_back(active_root);
	}

	void TextureGroup_Impl::remove(Subtexture &subtexture)
	{
		// Find the texture
		Node *node = nullptr;
		Texture2D texture = subtexture.get_texture();
		Rect rect = subtexture.get_geometry();

		std::vector<RootNode *>::size_type index, size;
		size = root_nodes.size();
		for (index = 0; index < size; ++index)
		{
			// Find a texture match
			if (root_nodes[index]->texture == texture)
			{
				node = root_nodes[index]->node.find_image_rect(rect);
				break;
			}
		}
		if (node)
		{
			node->clear();
			if (root_nodes[index]->node.get_subtexture_count() <= 0)
			{
				root_nodes[index]->node.clear();
				delete root_nodes[index];
				root_nodes.erase(root_nodes.begin() + index);
			}
			if (root_nodes.empty())
			{
				active_root = nullptr;
			}
			else
			{
				active_root = root_nodes.back();
			}
		}
		else
		{
			throw Exception("Cannot find the Subtexture in the TextureGroup");
		}
	}

	/////////////////////////////////////////////////////////////////////////////

	TextureGroup_Impl::Node::Node()
	{
		child[0] = nullptr;
		child[1] = nullptr;

		id = 0;
	}

	TextureGroup_Impl::Node::Node(const Rect &new_rect)
	{
		node_rect = new_rect;

		child[0] = nullptr;
		child[1] = nullptr;

		id = 0;
	}

	TextureGroup_Impl::Node::~Node()
	{
		clear();
	}

	int TextureGroup_Impl::Node::get_subtexture_count() const
	{
		int count = 0;

		if (child[0])
			count += child[0]->get_subtexture_count();
		if (child[1])
			count += child[1]->get_subtexture_count();

		if (id)
			count++;

		return count;
	}

	void TextureGroup_Impl::Node::clear()
	{
		if (child[0])
		{
			delete child[0];
			child[0] = nullptr;
		}
		if (child[1])
		{
			delete child[1];
			child[1] = nullptr;
		}
		id = 0;
	}

	TextureGroup_Impl::Node *TextureGroup_Impl::Node::insert(const Size &texture_size, int texture_id)
	{
		// If we're not a leaf
		if (child[0] && child[1])
		{
			// Try inserting into first child
			Node *new_node = child[0]->insert(texture_size, texture_id);
			if (new_node != nullptr)
				return new_node;

			// No room, insert into second
			return child[1]->insert(texture_size, texture_id);
		}
		else
		{
			// If there's already a texture here, return
			if (id)
				return nullptr;

			// If we're too small, return
			if (texture_size.width > node_rect.get_width() || texture_size.height > node_rect.get_height())
				return nullptr;

			// If we're just right, accept
			if (texture_size.width == node_rect.get_width() && texture_size.height == node_rect.get_height())
			{
				id = texture_id;
				image_rect = node_rect;
				return this;
			}

			// Otherwise, decide which way to split
			int dw = node_rect.get_width() - texture_size.width;
			int dh = node_rect.get_height() - texture_size.height;

			if (dw > dh)
			{
				child[0] = new Node(Rect(node_rect.left, node_rect.top, node_rect.left + texture_size.width, node_rect.bottom));
				child[1] = new Node(Rect(node_rect.left + texture_size.width, node_rect.top, node_rect.right, node_rect.bottom));
			}
			else
			{
				child[0] = new Node(Rect(node_rect.left, node_rect.top, node_rect.right, node_rect.top + texture_size.height));
				child[1] = new Node(Rect(node_rect.left, node_rect.top + texture_size.height, node_rect.right, node_rect.bottom));
			}

			// Insert into first child we created
			return child[0]->insert(texture_size, texture_id);
		}
	}

	TextureGroup_Impl::Node *TextureGroup_Impl::Node::find_image_rect(const Rect &new_rect)
	{
		// Check leaf
		if (child[0])
		{
			Node *node = child[0]->find_image_rect(new_rect);
			if (node)
				return node;
		}

		if (child[1])
		{
			Node *node = child[1]->find_image_rect(new_rect);
			if (node)
				return node;
		}

		if (id == 0)
			return nullptr;

		if (image_rect == new_rect)
			return this;

		return nullptr;
	}
}
