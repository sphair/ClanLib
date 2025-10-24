#include "precomp.h"
#include "texture_packer.h"
#include <iostream>

TexturePacker::TexturePacker(CL_GraphicContext &gc) : gc(gc)
{
}

TexturePacker::~TexturePacker()
{
}

void TexturePacker::load_resources(const CL_String &filename)
{
	resources = CL_ResourceManager(filename);

	std::vector<CL_String> resource_names = resources.get_resource_names();
	std::vector<CL_String>::iterator resource_it;
	for(resource_it = resource_names.begin(); resource_it != resource_names.end(); ++resource_it)
	{
		CL_String resource_id = (*resource_it);
		CL_Resource resource = resources.get_resource(resource_id);

		resource_items.push_back(load_resource(resource_id, resource, resources));
	}
}

ResourceItem *TexturePacker::load_resource(CL_String &resource_id, CL_Resource &resource, CL_ResourceManager &resources)
{
	try
	{
		CL_String type = resource.get_type();
		if(type == "sprite")
		{
			return load_sprite(resource_id, resource, resources);
		}
		else if(type == "image")
		{
			return load_image(resource_id, resource, resources);
		}
		else
		{
			throw CL_Exception("Resourcetype is not supported");
		}
	}
	catch(CL_Exception ex)
	{
		NotSupportedResourceItem *item = new NotSupportedResourceItem(resource);
		item->error = ex.message;
		return item;
	}
}

ResourceItem *TexturePacker::load_sprite(CL_String &resource_id, CL_Resource &resource, CL_ResourceManager &resources)
{
	CL_SpriteDescription desc(resource_id, &resources, gc);

	CL_Sprite sprite(resource_id, &resources, gc);
	sprite.set_play_loop(true);
	sprite.set_alignment(origin_top_left);

	SpriteResourceItem *item = new SpriteResourceItem(resource);
	item->sprite_description = desc;
	item->sprite = sprite;

	return item;
}

ResourceItem *TexturePacker::load_image(CL_String &resource_id, CL_Resource &resource, CL_ResourceManager &resources)
{
	CL_SpriteDescription desc(resource_id, &resources, gc);

	CL_Image image(gc, resource_id, &resources);
	image.set_alignment(origin_top_left);

	ImageResourceItem *item = new ImageResourceItem(resource);
	item->sprite_description = desc;
	item->image = image;

	return item;
}

CL_TextureGroup *TexturePacker::pack(const CL_Size &texture_size, int border_size)
{
	CL_TextureGroup *group = new CL_TextureGroup(gc, texture_size);

	std::vector<ResourceItem *> &items = get_resource_items();
	std::vector<ResourceItem *>::size_type item_index, item_size;
	item_size = items.size();
	for(item_index = 0; item_index < item_size; ++item_index)
	{
		SpriteResourceItem *sprite_item = dynamic_cast<SpriteResourceItem *>(items[item_index]);
		if(sprite_item)
		{
			sprite_item->packed_sub_textures.clear();

			const std::vector<CL_SpriteDescriptionFrame> &frames = sprite_item->sprite_description.get_frames();
			std::vector<CL_SpriteDescriptionFrame>::size_type index, size;
			size = frames.size();
			for(index = 0; index < size; ++index)
			{
				CL_Rect frame_rect = frames[index].rect;

				CL_Subtexture sub_texture = group->add(gc, CL_Size(frame_rect.get_width() + border_size*2, frame_rect.get_height() + border_size*2));
				sprite_item->packed_sub_textures.push_back(sub_texture);

				CL_Texture texture = frames[index].texture;
				CL_PixelBuffer &pb = texture.get_pixeldata();
				CL_PixelBuffer new_pb;
				add_border(pb, new_pb, border_size);
				sub_texture.get_texture().set_subimage(sub_texture.get_geometry().get_top_left(), new_pb);
			}
		}

		ImageResourceItem *image_item = dynamic_cast<ImageResourceItem *>(items[item_index]);
		if(image_item)
		{
			image_item->packed_sub_textures.clear();

			const std::vector<CL_SpriteDescriptionFrame> &frames = image_item->sprite_description.get_frames();
			std::vector<CL_SpriteDescriptionFrame>::size_type index, size;
			size = frames.size();
			for(index = 0; index < size; ++index)
			{
				CL_Rect frame_rect = frames[index].rect;

				CL_Subtexture sub_texture = group->add(gc, CL_Size(frame_rect.get_width() + border_size*2, frame_rect.get_height() + border_size*2));
				image_item->packed_sub_textures.push_back(sub_texture);

				CL_Texture texture = frames[index].texture;
				CL_PixelBuffer &pb = texture.get_pixeldata();
				CL_PixelBuffer new_pb;
				add_border(pb, new_pb, border_size);
				sub_texture.get_texture().set_subimage(sub_texture.get_geometry().get_top_left(), new_pb);
			}
		}

		if(!func_pack_progress.is_null())
			func_pack_progress.invoke((int)item_index + 1, (int)item_size);
	}

	return group;
}

void TexturePacker::save_resources(const CL_String &filename)
{
	// Map containing generated texture filenames for packed CL_Textures
	std::map<CL_Texture, CL_String> generated_texture_filenames;
	int generated_texture_index = 0;

	// Loop through all resource items
	std::vector<ResourceItem *> &items = get_resource_items();
	std::vector<ResourceItem *>::size_type item_index, item_size;
	item_size = items.size();
	for(item_index = 0; item_index < item_size; ++item_index)
	{
		SpriteResourceItem *sprite_item = dynamic_cast<SpriteResourceItem *>(items[item_index]);
		if (sprite_item)
			process_resource(sprite_item->resource, sprite_item->packed_sub_textures, generated_texture_filenames, generated_texture_index);

		ImageResourceItem *image_item = dynamic_cast<ImageResourceItem *>(items[item_index]);
		if (image_item)
			process_resource(image_item->resource, image_item->packed_sub_textures, generated_texture_filenames, generated_texture_index);
	}

	// Save the entire resource DOM
	resources.save(filename);
}

void TexturePacker::process_resource(CL_Resource &item_resource, std::vector<CL_Subtexture> &packed_sub_textures, std::map<CL_Texture, CL_String> &generated_texture_filenames, int &generated_texture_index)
{
	// Found a sprite resource, lets modify its content!
	CL_Resource resource = item_resource;

	// Iterate through all nodes, and remove all previous image tags
	CL_DomElement &element = resource.get_element();
	CL_DomNode cur = element.get_first_child();
	while (!cur.is_null())
	{
		CL_DomNode next = cur.get_next_sibling();
		CL_DomNode::NodeType nodeType = (CL_DomNode::NodeType)cur.get_node_type();

		// Only remove the <image> tag, as we want to keep the other sprite attributes
		if (cur.get_node_name() == "image")
			element.remove_child(cur);

		cur = next;
	}

	// Add new image tag to resource DOM
	std::vector<CL_Subtexture>::size_type index, size;
	size = packed_sub_textures.size();
	for(index = 0; index < size; ++index)
	{
		CL_Subtexture subtexture = packed_sub_textures[index];

		// Try to find out if we already have created a texture-on-disk for this subtexture
		CL_String texture_filename;
		CL_Texture texture = subtexture.get_texture();
		std::map<CL_Texture, CL_String>::iterator it;
		it = generated_texture_filenames.find(texture);
		if(it == generated_texture_filenames.end())
		{
			// Texture not found, generate a filename and dump texture to disk
			texture_filename = cl_format("texture%1.png", ++generated_texture_index);
			CL_PNGProvider::save(texture.get_pixeldata(), texture_filename);
			generated_texture_filenames[texture] = texture_filename;
		}
		else
		{
			// Previously dumped textures, lets reuse the filename
			texture_filename = (*it).second;
		}

		// Add <grid> DOM element
		CL_DomElement new_grid_element = element.get_owner_document().create_element("grid");
		new_grid_element.set_attribute("pos", cl_format("%1,%2", subtexture.get_geometry().left + last_border_size, subtexture.get_geometry().top + last_border_size));
		new_grid_element.set_attribute("size", cl_format("%1,%2", subtexture.get_geometry().get_width()- last_border_size*2, subtexture.get_geometry().get_height()- last_border_size*2));

		// Add <image> DOM element
		CL_DomElement new_image_element = element.get_owner_document().create_element("image");
		new_image_element.set_attribute("file", texture_filename);
		new_image_element.append_child(new_grid_element);

		// Add <image> element under <sprite> element
		element.append_child(new_image_element);
	}
}

void TexturePacker::add_border(CL_PixelBuffer &pb, CL_PixelBuffer &new_pb, int border_size)
{
	if (border_size <=0)
	{
		last_border_size = 0;
		new_pb = pb;
		return;
	}
	last_border_size = border_size;

	int new_width = pb.get_width() + border_size*2;
	int new_height = pb.get_height() + border_size*2;
	new_pb = CL_PixelBuffer(new_width, new_height, new_width *4, CL_PixelFormat::rgba8888, NULL);

	for (int ypos = 0; ypos < new_height; ypos++)
	{
		int real_ypos = ypos - border_size;
		if (real_ypos < 0)
			real_ypos = 0;

		if (real_ypos >= pb.get_height())
			real_ypos = pb.get_height()-1;

		int *src_data = (int *) pb.get_data();
		src_data += (pb.get_pitch() * real_ypos)/4;

		int *dest_data = (int *) new_pb.get_data();
		dest_data += (new_pb.get_pitch() * ypos)/4;

		for (int xpos = 0; xpos < new_width; xpos++)
		{
			int real_xpos = xpos - border_size;
			if (real_xpos < 0)
				real_xpos = 0;

			if (real_xpos >= pb.get_width())
				real_xpos = pb.get_width()-1;

			dest_data[xpos] = src_data[real_xpos];
		}
	}
}
