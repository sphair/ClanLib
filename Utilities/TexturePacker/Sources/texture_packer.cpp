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

#include "precomp.h"
#include "texture_packer.h"
#include <iostream>
#include <algorithm>

TexturePacker::TexturePacker()
{
}

TexturePacker::~TexturePacker()
{
}

void TexturePacker::load_resources(clan::Canvas &canvas, const std::string &filename)
{
	resources_doc = clan::XMLResourceDocument(filename);
	resources = clan::XMLResourceManager::create(resources_doc);

	// TODO: Delete items before clearing
	resource_items.clear();

	std::vector<std::string> resource_names = resources_doc.get_resource_names();
	std::vector<std::string>::iterator resource_it;
	for(resource_it = resource_names.begin(); resource_it != resource_names.end(); ++resource_it)
	{
		std::string resource_id = (*resource_it);
		clan::XMLResourceNode resource = resources_doc.get_resource(resource_id);

		resource_items.push_back(load_resource(canvas, resource_id, resource, resources));
	}
}

ResourceItem *TexturePacker::load_resource(clan::Canvas &canvas, std::string &resource_id, clan::XMLResourceNode &resource, clan::ResourceManager &resources)
{
	ResourceItem *item = 0;

	try
	{
		std::string type = resource.get_type();
		if(type == "sprite")
		{
			item = load_sprite(canvas, resource_id, resource, resources);
		}
		else if(type == "image")
		{
			item = load_image(canvas, resource_id, resource, resources);
		}
		else
		{
			throw clan::Exception(clan::string_format("Resourcetype %1 is not supported", type));
		}
	}
	catch(clan::Exception ex)
	{
		item = new NotSupportedResourceItem(resource, ex.message);
	}

	// Find resource path by traversing parents
	clan::DomElement &dom_element = resource.get_element();
	clan::DomNode parent = dom_element.get_parent_node();
	while (!parent.is_null())
	{
		clan::DomElement parent_element = parent.to_element();
		std::string parent_name = parent_element.get_attribute("name");
		if(parent_name.length() > 0)
			item->resource_path = clan::string_format("%1/%2", parent_name, item->resource_path);
		parent = parent.get_parent_node();
	}

	return item;
}

ResourceItem *TexturePacker::load_sprite(clan::Canvas &canvas, std::string &resource_id, clan::XMLResourceNode &resource, clan::ResourceManager &resources)
{
	clan::Sprite sprite = clan::Sprite::load(canvas, resource_id, resource.get_document());
	sprite.set_play_loop(true);
	sprite.set_alignment(clan::origin_top_left);
	sprite.set_angle(clan::Angle::from_degrees(360) - sprite.get_base_angle());

	SpriteResourceItem *item = new SpriteResourceItem(resource);
	item->sprite = sprite;

	return item;
}

ResourceItem *TexturePacker::load_image(clan::Canvas &canvas, std::string &resource_id, clan::XMLResourceNode &resource, clan::ResourceManager &resources)
{
	clan::Image image = clan::Image::load(canvas, resource_id, resource.get_document());
	image.set_alignment(clan::origin_top_left);

	ImageResourceItem *item = new ImageResourceItem(resource);
	item->image = image;

	return item;
}

bool ImageWidthSortPredicate(ResourceItem *d1, ResourceItem *d2)
{
	int max_width_d1 = 0;
	int max_width_d2 = 0;

	SpriteResourceItem *sprite_item1 = dynamic_cast<SpriteResourceItem *>(d1);
	if(sprite_item1)
	{
		unsigned int frame_size = sprite_item1->sprite.get_frame_count();

		for(unsigned int index = 0; index < frame_size; ++index)
		{
			clan::Size frame_size = sprite_item1->sprite.get_size();
			if(frame_size.width > max_width_d1)
				max_width_d1 = frame_size.width;
		}
	}
	ImageResourceItem *image_item1 = dynamic_cast<ImageResourceItem *>(d1);
	if(image_item1)
	{
		clan::Size frame_size = image_item1->image.get_size();
		if(frame_size.width > max_width_d1)
			max_width_d1 = frame_size.width;
	}
	SpriteResourceItem *sprite_item2 = dynamic_cast<SpriteResourceItem *>(d2);
	if(sprite_item2)
	{
		unsigned int frame_size = sprite_item2->sprite.get_frame_count();
		for(unsigned int index = 0; index < frame_size; ++index)
		{
			clan::Size frame_size = sprite_item2->sprite.get_frame_size(index);
			if(frame_size.width > max_width_d2)
				max_width_d2 = frame_size.width;
		}
	}
	ImageResourceItem *image_item2 = dynamic_cast<ImageResourceItem *>(d2);
	if(image_item2)
	{
		clan::Rect frame_rect = image_item2->image.get_size();
		if(frame_rect.get_width() > max_width_d2)
			max_width_d2 = frame_rect.get_width();
	}

	return max_width_d1 > max_width_d2;
}

clan::TextureGroup *TexturePacker::pack(clan::Canvas &canvas, const clan::Size &texture_size, int border_size, bool sort_on_width)
{
	clan::TextureGroup *group = new clan::TextureGroup(texture_size);

	std::vector<ResourceItem *> items = get_resource_items();

	if(sort_on_width)
		std::sort(items.begin(), items.end(), ImageWidthSortPredicate);

	std::vector<ResourceItem *>::size_type item_index, item_size;
	item_size = items.size();
	for(item_index = 0; item_index < item_size; ++item_index)
	{
		SpriteResourceItem *sprite_item = dynamic_cast<SpriteResourceItem *>(items[item_index]);
		if(sprite_item)
		{
			sprite_item->packed_sub_textures.clear();

			unsigned int size = sprite_item->sprite.get_frame_count();
			for(unsigned int index = 0; index < size; ++index)
			{
				clan::Size frame_size = sprite_item->sprite.get_frame_size(index);

				clan::Subtexture sub_texture = group->add(canvas, clan::Size(frame_size.width + border_size*2, frame_size.height + border_size*2));
				sprite_item->packed_sub_textures.push_back(sub_texture);

				clan::Subtexture subtexture = sprite_item->sprite.get_frame_texture(index);
				clan::PixelBuffer pb = subtexture.get_texture().get_pixeldata(canvas, clan::tf_rgba8);
				pb = pb.copy(subtexture.get_geometry());
				last_border_size = border_size;
				if (last_border_size < 0) last_border_size= 0;
				clan::PixelBuffer new_pb = clan::PixelBufferHelp::add_border(pb, border_size, pb.get_size());
				sub_texture.get_texture().set_subimage(canvas, sub_texture.get_geometry().get_top_left(), new_pb, new_pb.get_size());
			}
		}

		ImageResourceItem *image_item = dynamic_cast<ImageResourceItem *>(items[item_index]);
		if(image_item)
		{
			image_item->packed_sub_textures.clear();

			clan::Size frame_size = image_item->image.get_size();

			clan::Subtexture sub_texture = group->add(canvas, clan::Size(frame_size.width + border_size*2, frame_size.height + border_size*2));
			image_item->packed_sub_textures.push_back(sub_texture);

			clan::Subtexture subtexture = image_item->image.get_texture();
			clan::PixelBuffer pb = subtexture.get_texture().get_pixeldata(canvas, clan::tf_rgba8);
			pb = pb.copy(subtexture.get_geometry());
			last_border_size = border_size;
			if (last_border_size < 0) last_border_size = 0;
			clan::PixelBuffer new_pb = clan::PixelBufferHelp::add_border(pb, border_size, pb.get_size());
			sub_texture.get_texture().set_subimage(canvas, sub_texture.get_geometry().get_top_left(), new_pb, new_pb.get_size());
		}

		if(!func_pack_progress.is_null())
			func_pack_progress.invoke((int)item_index + 1, (int)item_size);
	}

	return group;
}

void TexturePacker::save_resources(clan::Canvas &canvas, const std::string &filename)
{
	// Map containing generated texture filenames for packed clan::Textures
	std::map<clan::Texture2D, std::string> generated_texture_filenames;
	int generated_texture_index = 0;

	std::string images_pathname = clan::PathHelp::get_fullpath(filename);

	// Loop through all resource items
	std::vector<ResourceItem *> &items = get_resource_items();
	std::vector<ResourceItem *>::size_type item_index, item_size;
	item_size = items.size();
	for(item_index = 0; item_index < item_size; ++item_index)
	{
		SpriteResourceItem *sprite_item = dynamic_cast<SpriteResourceItem *>(items[item_index]);
		if (sprite_item)
			process_resource(canvas, sprite_item->resource, sprite_item->packed_sub_textures, generated_texture_filenames, generated_texture_index, images_pathname);

		ImageResourceItem *image_item = dynamic_cast<ImageResourceItem *>(items[item_index]);
		if (image_item)
			process_resource(canvas, image_item->resource, image_item->packed_sub_textures, generated_texture_filenames, generated_texture_index, images_pathname);
	}

	// Save the entire resource DOM
	resources_doc.save(filename);
}

void TexturePacker::process_resource(clan::Canvas &canvas, clan::XMLResourceNode &item_resource, std::vector<clan::Subtexture> &packed_sub_textures, std::map<clan::Texture2D, std::string> &generated_texture_filenames, int &generated_texture_index, const std::string &image_pathname )
{
	// Found a sprite resource, lets modify its content!
	clan::XMLResourceDocument resource = item_resource.get_document();

	// Iterate through all nodes, and remove all previous image tags
	clan::DomElement &element = item_resource.get_element();
	clan::DomNode cur = element.get_first_child();
	while (!cur.is_null())
	{
		clan::DomNode next = cur.get_next_sibling();
		clan::DomNode::NodeType nodeType = (clan::DomNode::NodeType)cur.get_node_type();

		// Only remove the <image> tag, as we want to keep the other sprite attributes
		if (cur.get_node_name() == "image")
			element.remove_child(cur);

		cur = next;
	}

	// Add new image tag to resource DOM
	std::vector<clan::Subtexture>::size_type index, size;
	size = packed_sub_textures.size();
	for(index = 0; index < size; ++index)
	{
		clan::Subtexture subtexture = packed_sub_textures[index];

		// Try to find out if we already have created a texture-on-disk for this subtexture
		std::string texture_filename;
		clan::Texture2D texture = subtexture.get_texture();
		std::map<clan::Texture2D, std::string>::iterator it;
		it = generated_texture_filenames.find(texture);
		if(it == generated_texture_filenames.end())
		{
			// Texture not found, generate a filename and dump texture to disk
			texture_filename = clan::string_format("texture%1.png", ++generated_texture_index);
			clan::PNGProvider::save(texture.get_pixeldata(canvas), image_pathname + texture_filename);
			generated_texture_filenames[texture] = texture_filename;
		}
		else
		{
			// Previously dumped textures, lets reuse the filename
			texture_filename = (*it).second;
		}

		// Add <grid> DOM element
		clan::DomElement new_grid_element = element.get_owner_document().create_element("grid");
		new_grid_element.set_attribute("pos", clan::string_format("%1,%2", subtexture.get_geometry().left + last_border_size, subtexture.get_geometry().top + last_border_size));
		new_grid_element.set_attribute("size", clan::string_format("%1,%2", subtexture.get_geometry().get_width()- last_border_size*2, subtexture.get_geometry().get_height()- last_border_size*2));

		// Add <image> DOM element
		clan::DomElement new_image_element = element.get_owner_document().create_element("image");
		new_image_element.set_attribute("file", texture_filename);
		new_image_element.append_child(new_grid_element);

		// Add <image> element under <sprite> element
		element.append_child(new_image_element);
	}
}

