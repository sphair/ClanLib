/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

void TexturePacker::load_resources(Canvas &canvas, const std::string &filename)
{
	resources = ResourceManager(filename);

	// TODO: Delete items before clearing
	resource_items.clear();

	std::vector<std::string> resource_names = resources.get_resource_names();
	std::vector<std::string>::iterator resource_it;
	for(resource_it = resource_names.begin(); resource_it != resource_names.end(); ++resource_it)
	{
		std::string resource_id = (*resource_it);
		Resource resource = resources.get_resource(resource_id);

		resource_items.push_back(load_resource(canvas, resource_id, resource, resources));
	}
}

ResourceItem *TexturePacker::load_resource(Canvas &canvas, std::string &resource_id, Resource &resource, ResourceManager &resources)
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
			throw Exception(string_format("Resourcetype %1 is not supported", type));
		}
	}
	catch(Exception ex)
	{
		item = new NotSupportedResourceItem(resource, ex.message);
	}

	// Find resource path by traversing parents
	DomElement &dom_element = resource.get_element();
	DomNode parent = dom_element.get_parent_node();
	while (!parent.is_null())
	{
		DomElement parent_element = parent.to_element();
		std::string parent_name = parent_element.get_attribute("name");
		if(parent_name.length() > 0)
			item->resource_path = string_format("%1/%2", parent_name, item->resource_path);
		parent = parent.get_parent_node();
	}

	return item;
}

ResourceItem *TexturePacker::load_sprite(Canvas &canvas, std::string &resource_id, Resource &resource, ResourceManager &resources)
{
	SpriteDescription desc(canvas, resource_id, &resources);

	Sprite sprite(canvas, resource_id, &resources);
	sprite.set_play_loop(true);
	sprite.set_alignment(origin_top_left);
	sprite.set_angle(Angle::from_degrees(360) - sprite.get_base_angle());

	SpriteResourceItem *item = new SpriteResourceItem(resource);
	item->sprite_description = desc;
	item->sprite = sprite;

	return item;
}

ResourceItem *TexturePacker::load_image(Canvas &canvas, std::string &resource_id, Resource &resource, ResourceManager &resources)
{
	SpriteDescription desc(canvas, resource_id, &resources);

	Image image(canvas, resource_id, &resources);
	image.set_alignment(origin_top_left);

	ImageResourceItem *item = new ImageResourceItem(resource);
	item->sprite_description = desc;
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
		const std::vector<SpriteDescriptionFrame> &frames = sprite_item1->sprite_description.get_frames();
		std::vector<SpriteDescriptionFrame>::size_type index;
		for(index = 0; index < frames.size(); ++index)
		{
			Rect frame_rect = frames[index].rect;
			if(frame_rect.get_width() > max_width_d1)
				max_width_d1 = frame_rect.get_width();
		}
	}
	ImageResourceItem *image_item1 = dynamic_cast<ImageResourceItem *>(d1);
	if(image_item1)
	{
		const std::vector<SpriteDescriptionFrame> &frames = image_item1->sprite_description.get_frames();
		std::vector<SpriteDescriptionFrame>::size_type index;
		for(index = 0; index < frames.size(); ++index)
		{
			Rect frame_rect = frames[index].rect;
			if(frame_rect.get_width() > max_width_d1)
				max_width_d1 = frame_rect.get_width();
		}
	}
	SpriteResourceItem *sprite_item2 = dynamic_cast<SpriteResourceItem *>(d2);
	if(sprite_item2)
	{
		const std::vector<SpriteDescriptionFrame> &frames = sprite_item2->sprite_description.get_frames();
		std::vector<SpriteDescriptionFrame>::size_type index;
		for(index = 0; index < frames.size(); ++index)
		{
			Rect frame_rect = frames[index].rect;
			if(frame_rect.get_width() > max_width_d2)
				max_width_d2 = frame_rect.get_width();
		}
	}
	ImageResourceItem *image_item2 = dynamic_cast<ImageResourceItem *>(d2);
	if(image_item2)
	{
		const std::vector<SpriteDescriptionFrame> &frames = image_item2->sprite_description.get_frames();
		std::vector<SpriteDescriptionFrame>::size_type index;
		for(index = 0; index < frames.size(); ++index)
		{
			Rect frame_rect = frames[index].rect;
			if(frame_rect.get_width() > max_width_d2)
				max_width_d2 = frame_rect.get_width();
		}
	}

	return max_width_d1 > max_width_d2;
}

TextureGroup *TexturePacker::pack(Canvas &canvas, const Size &texture_size, int border_size, bool sort_on_width)
{
	TextureGroup *group = new TextureGroup(texture_size);

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

			const std::vector<SpriteDescriptionFrame> &frames = sprite_item->sprite_description.get_frames();
			std::vector<SpriteDescriptionFrame>::size_type index, size;
			size = frames.size();
			for(index = 0; index < size; ++index)
			{
				Rect frame_rect = frames[index].rect;

				Subtexture sub_texture = group->add(canvas, Size(frame_rect.get_width() + border_size*2, frame_rect.get_height() + border_size*2));
				sprite_item->packed_sub_textures.push_back(sub_texture);

				Texture2D texture = frames[index].texture;
				const PixelBuffer &pb = texture.get_pixeldata(canvas);
				last_border_size = border_size;
				if (last_border_size < 0) last_border_size= 0;
				PixelBuffer new_pb = PixelBufferHelp::add_border(pb, border_size, pb.get_size());
				sub_texture.get_texture().set_subimage(canvas, sub_texture.get_geometry().get_top_left(), new_pb, new_pb.get_size());
			}
		}

		ImageResourceItem *image_item = dynamic_cast<ImageResourceItem *>(items[item_index]);
		if(image_item)
		{
			image_item->packed_sub_textures.clear();

			const std::vector<SpriteDescriptionFrame> &frames = image_item->sprite_description.get_frames();
			std::vector<SpriteDescriptionFrame>::size_type index, size;
			size = frames.size();
			for(index = 0; index < size; ++index)
			{
				Rect frame_rect = frames[index].rect;

				Subtexture sub_texture = group->add(canvas, Size(frame_rect.get_width() + border_size*2, frame_rect.get_height() + border_size*2));
				image_item->packed_sub_textures.push_back(sub_texture);

				Texture2D texture = frames[index].texture;
				const PixelBuffer &pb = texture.get_pixeldata(canvas);
				last_border_size = border_size;
				if (last_border_size < 0) last_border_size = 0;
				PixelBuffer new_pb = PixelBufferHelp::add_border(pb, border_size, pb.get_size());
				sub_texture.get_texture().set_subimage(canvas, sub_texture.get_geometry().get_top_left(), new_pb, new_pb.get_size());
			}
		}

		if(!func_pack_progress.is_null())
			func_pack_progress.invoke((int)item_index + 1, (int)item_size);
	}

	return group;
}

void TexturePacker::save_resources(Canvas &canvas, const std::string &filename)
{
	// Map containing generated texture filenames for packed Textures
	std::map<Texture, std::string> generated_texture_filenames;
	int generated_texture_index = 0;

	std::string images_pathname = PathHelp::get_fullpath(filename);

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
	resources.save(filename);
}

void TexturePacker::process_resource(Canvas &canvas, Resource &item_resource, std::vector<Subtexture> &packed_sub_textures, std::map<Texture, std::string> &generated_texture_filenames, int &generated_texture_index, const std::string &image_pathname )
{
	// Found a sprite resource, lets modify its content!
	Resource resource = item_resource;

	// Iterate through all nodes, and remove all previous image tags
	DomElement &element = resource.get_element();
	DomNode cur = element.get_first_child();
	while (!cur.is_null())
	{
		DomNode next = cur.get_next_sibling();
		DomNode::NodeType nodeType = (DomNode::NodeType)cur.get_node_type();

		// Only remove the <image> tag, as we want to keep the other sprite attributes
		if (cur.get_node_name() == "image")
			element.remove_child(cur);

		cur = next;
	}

	// Add new image tag to resource DOM
	std::vector<Subtexture>::size_type index, size;
	size = packed_sub_textures.size();
	for(index = 0; index < size; ++index)
	{
		Subtexture subtexture = packed_sub_textures[index];

		// Try to find out if we already have created a texture-on-disk for this subtexture
		std::string texture_filename;
		Texture2D texture = subtexture.get_texture();
		std::map<Texture, std::string>::iterator it;
		it = generated_texture_filenames.find(texture);
		if(it == generated_texture_filenames.end())
		{
			// Texture not found, generate a filename and dump texture to disk
			texture_filename = string_format("texture%1.png", ++generated_texture_index);
			PNGProvider::save(texture.get_pixeldata(canvas), image_pathname + texture_filename);
			generated_texture_filenames[texture] = texture_filename;
		}
		else
		{
			// Previously dumped textures, lets reuse the filename
			texture_filename = (*it).second;
		}

		// Add <grid> DOM element
		DomElement new_grid_element = element.get_owner_document().create_element("grid");
		new_grid_element.set_attribute("pos", string_format("%1,%2", subtexture.get_geometry().left + last_border_size, subtexture.get_geometry().top + last_border_size));
		new_grid_element.set_attribute("size", string_format("%1,%2", subtexture.get_geometry().get_width()- last_border_size*2, subtexture.get_geometry().get_height()- last_border_size*2));

		// Add <image> DOM element
		DomElement new_image_element = element.get_owner_document().create_element("image");
		new_image_element.set_attribute("file", texture_filename);
		new_image_element.append_child(new_grid_element);

		// Add <image> element under <sprite> element
		element.append_child(new_image_element);
	}
}

