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
	TexturePacker(CL_GraphicContext &gc);
	~TexturePacker();

	void load_resources(const CL_String &filename);
	void save_resources(const CL_String &filename);

	std::vector<ResourceItem *> &get_resource_items() { return resource_items; }

	CL_TextureGroup *pack(const CL_Size &texture_size, int border_size);
	CL_Callback_v2<int, int> func_pack_progress;

private:
	ResourceItem *load_resource(CL_String &resource_id, CL_Resource &resource, CL_ResourceManager &resources);
	ResourceItem *load_image(CL_String &resource_id, CL_Resource &resource, CL_ResourceManager &resources);
	ResourceItem *load_sprite(CL_String &resource_id, CL_Resource &resource, CL_ResourceManager &resources);
	void process_resource(CL_Resource &item_resource, std::vector<CL_Subtexture> &packed_sub_textures, std::map<CL_Texture, CL_String> &generated_texture_filenames, int &generated_texture_index);
	void add_border(const CL_PixelBuffer &pb, CL_PixelBuffer &new_pb, int border_size);

	CL_GraphicContext gc;

	CL_ResourceManager resources;

	std::vector<ResourceItem *> resource_items;
	int last_border_size;
};
