/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

class ResourceItem
{
public:
	ResourceItem(Resource resource) : resource(resource) {};
	virtual ~ResourceItem() {};

	Resource resource;

	std::string resource_path;
};

class NotSupportedResourceItem : public ResourceItem
{
public:
	NotSupportedResourceItem(Resource resource, const std::string &error) : ResourceItem(resource), error(error) {};

	std::string error;
};

class SpriteResourceItem : public ResourceItem
{
public:
	SpriteResourceItem(Resource resource) : ResourceItem(resource) {};

	SpriteDescription sprite_description;
	Sprite sprite;

	std::vector<Subtexture> packed_sub_textures;
};

class ImageResourceItem : public ResourceItem
{
public:
	ImageResourceItem(Resource resource) : ResourceItem(resource) {};

	SpriteDescription sprite_description;
	Image image;

	std::vector<Subtexture> packed_sub_textures;
};

class TexturePacker
{
public:
	TexturePacker();
	~TexturePacker();

	void load_resources(Canvas &canvas, const std::string &filename);
	void save_resources(Canvas &canvas, const std::string &filename);

	std::vector<ResourceItem *> &get_resource_items() { return resource_items; }

	TextureGroup *pack(Canvas &canvas, const Size &texture_size, int border_size, bool sort_on_width);
	Callback_v2<int, int> func_pack_progress;

private:
	ResourceItem *load_resource(Canvas &canvas, std::string &resource_id, Resource &resource, DisplayCache &resources);
	ResourceItem *load_image(Canvas &canvas, std::string &resource_id, Resource &resource, DisplayCache &resources);
	ResourceItem *load_sprite(Canvas &canvas, std::string &resource_id, Resource &resource, DisplayCache &resources);
	void process_resource(Canvas &canvas, Resource &item_resource, std::vector<Subtexture> &packed_sub_textures, std::map<Texture, std::string> &generated_texture_filenames, int &generated_texture_index, const std::string &image_pathname);

	DisplayCache resources;

	std::vector<ResourceItem *> resource_items;
	int last_border_size;
};
