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
**    Magnus Norddahl
*/

#include "Display/precomp.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/Resources/resource.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/XML/dom_node.h"
#include "API/Core/XML/dom_element.h"
#include "API/Display/2D/image_description.h"
#include "API/Display/ImageProviders/provider_factory.h"
#include "image_description_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// ImageDescription construction:

ImageDescription::ImageDescription(GraphicContext &gc, const std::string &resource_id, ResourceManager *resources, const ImageImportDescription &import_desc)
: impl(new ImageDescription_Impl)
{
	Resource resource = resources->get_resource(resource_id);
	if (resource.get_type() != "image" && resource.get_type() != "image_description" && resource.get_type() != "image")
		throw Exception(string_format("Resource '%1' is not of type 'image' or 'image_description' or 'image'", resource_id));

	DomNode cur_node = resource.get_element().get_first_child();

	while(!cur_node.is_null())
	{
		if (!cur_node.is_element()) 
			continue;

		DomElement cur_element = cur_node.to_element();
		std::string tag_name = cur_element.get_tag_name();
		if (tag_name == "image" || tag_name == "image-file")
		{
			std::string image_name = cur_element.get_attribute("file");
			VirtualDirectory virtual_directory = resources->get_directory(resource);
			Texture2D texture = Texture2D(gc, image_name, virtual_directory, import_desc );

			add_frame(texture);
			break;
		}
		cur_node = cur_node.get_next_sibling();
	}

	if (impl->frame.texture.is_null()) 
		throw Exception("Image resource contained no frames!");
}

ImageDescription::ImageDescription()
: impl(new ImageDescription_Impl)
{
}

ImageDescription::ImageDescription(const ImageDescription &copy)
: impl(copy.impl) 
{
}

ImageDescription::~ImageDescription()
{
}

/////////////////////////////////////////////////////////////////////////////
// ImageDescription attributes:

const ImageDescriptionFrame &ImageDescription::get_frame() const
{
	return impl->frame;
}

/////////////////////////////////////////////////////////////////////////////
// ImageDescription operations:

ImageDescription &ImageDescription::operator =(const ImageDescription &copy)
{
	impl = copy.impl;
	return *this;
}

void ImageDescription::add_frame(const Texture2D &texture)
{
	impl->frame = ImageDescriptionFrame(texture, texture.get_size());
}

void ImageDescription::add_frame(GraphicContext &gc, const std::string &filename, VirtualDirectory &dir, const ImageImportDescription &import_desc)
{
	add_frame(Texture2D(gc, filename, dir, import_desc));
}

void ImageDescription::add_frame(GraphicContext &gc, const std::string &fullname, const ImageImportDescription &import_desc)
{
	add_frame(Texture2D(gc, fullname, import_desc));
}

void ImageDescription::add_frame(GraphicContext &gc, IODevice &file, const std::string &image_type, const ImageImportDescription &import_desc)
{
	add_frame(Texture2D(gc, file, image_type, import_desc));
}

void ImageDescription::add_frame(const Texture2D &texture, const Rect &frame)
{
	impl->frame = ImageDescriptionFrame(texture, frame);
}

}
