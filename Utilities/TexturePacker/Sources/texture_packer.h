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

#pragma once

class ResourceItem
{
public:
	ResourceItem(CL_Resource resource) : resource(resource) {};
	virtual ~ResourceItem() {};

	CL_Resource resource;
};

class NotSupportedResourceItem : public ResourceItem
{
public:
	NotSupportedResourceItem(CL_Resource resource) : ResourceItem(resource) {};

	CL_String error;
};

class SpriteResourceItem : public ResourceItem
{
public:
	SpriteResourceItem(CL_Resource resource) : ResourceItem(resource) {};

	CL_SpriteDescription sprite_description;
	CL_Sprite sprite;

	std::vector<CL_Subtexture> packed_sub_textures;
};

class ImageResourceItem : public ResourceItem
{
public:
	ImageResourceItem(CL_Resource resource) : ResourceItem(resource) {};

	CL_SpriteDescription sprite_description;
	CL_Image image;

	std::vector<CL_Subtexture> packed_sub_textures;
};

class TexturePacker
{
public:
	TexturePacker();
	~TexturePacker();

	void load_resources(CL_GraphicContext &gc, const CL_String &filename);
	void save_resources(const CL_String &filename);

	std::vector<ResourceItem *> &get_resource_items() { return resource_items; }

	CL_TextureGroup *pack(CL_GraphicContext &gc, const CL_Size &texture_size, int border_size);
	CL_Callback_v2<int, int> func_pack_progress;

private:
	ResourceItem *load_resource(CL_GraphicContext &gc, CL_String &resource_id, CL_Resource &resource, CL_ResourceManager &resources);
	ResourceItem *load_image(CL_GraphicContext &gc, CL_String &resource_id, CL_Resource &resource, CL_ResourceManager &resources);
	ResourceItem *load_sprite(CL_GraphicContext &gc, CL_String &resource_id, CL_Resource &resource, CL_ResourceManager &resources);
	void process_resource(CL_Resource &item_resource, std::vector<CL_Subtexture> &packed_sub_textures, std::map<CL_Texture, CL_String> &generated_texture_filenames, int &generated_texture_index, const CL_String &image_pathname);

	CL_ResourceManager resources;

	std::vector<ResourceItem *> resource_items;
	int last_border_size;
};
