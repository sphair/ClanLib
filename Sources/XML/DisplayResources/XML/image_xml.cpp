/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

#include "XML/precomp.h"
#include "API/Core/IOData/file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Display/2D/image.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/2D/subtexture.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Render/texture_2d.h"
#include "API/XML/dom_element.h"
#include "API/Core/Text/string_help.h"
#include "API/XML/Resources/xml_resource_document.h"

namespace clan
{
	Image Image::load(Canvas &canvas, const std::string &id, const XMLResourceDocument &doc)
	{
		Image image;

		XMLResourceNode resource = doc.get_resource(id);

		DomNode cur_node = resource.get_element().get_first_child();
		while (!cur_node.is_null())
		{
			if (!cur_node.is_element())
			{
				cur_node = cur_node.get_next_sibling();
				continue;
			}

			DomElement cur_element = cur_node.to_element();
			std::string tag_name = cur_element.get_tag_name();
			if (tag_name == "image" || tag_name == "image-file")
			{
				std::string image_name = cur_element.get_attribute("file");
				Texture2D texture = Texture2D(canvas, PathHelp::combine(resource.get_base_path(), image_name), resource.get_file_system());

				DomNode cur_child(cur_element.get_first_child());
				if (cur_child.is_null())
				{
					image = Image(texture, texture.get_size());
				}
				else
				{
					do {
						DomElement cur_child_elemnt = cur_child.to_element();
						if (cur_child.get_node_name() == "grid")
						{
							Point position;
							Size texture_size = texture.get_size();
							Size size = texture_size;

							std::vector<std::string> image_size = StringHelp::split_text(cur_child_elemnt.get_attribute("size"), ",");
							if (image_size.size() > 0)
								size.width = StringHelp::text_to_int(image_size[0]);
							if (image_size.size() > 1)
								size.height = StringHelp::text_to_int(image_size[1]);

							if (cur_child_elemnt.has_attribute("pos"))
							{
								std::vector<std::string> image_pos = StringHelp::split_text(cur_child_elemnt.get_attribute("pos"), ",");
								if (image_pos.size() > 0)
									position.x = StringHelp::text_to_int(image_pos[0]);
								if (image_pos.size() > 1)
									position.y = StringHelp::text_to_int(image_pos[1]);
							}
							if ((size.width + position.x) > texture_size.width)
								size.width = (texture_size.width - position.x);
							if ((size.height + position.y) > texture_size.height)
								size.height = (texture_size.height - position.y);

							image = Image(texture, Rect(position, size));
						}

						cur_child = cur_child.get_next_sibling();
					} while (!cur_child.is_null());
				}

				break;
			}
			cur_node = cur_node.get_next_sibling();
		}
		if (image.is_null())
			throw Exception("Image resource contained no frames!");

		cur_node = resource.get_element().get_first_child();
		while (!cur_node.is_null())
		{
			if (!cur_node.is_element())
			{
				cur_node = cur_node.get_next_sibling();
				continue;
			}

			DomElement cur_element = cur_node.to_element();

			std::string tag_name = cur_element.get_tag_name();

			// <color red="float" green="float" blue="float" alpha="float" />
			if (tag_name == "color")
			{
				Colorf color;
				color.r = (float)StringHelp::text_to_float(cur_element.get_attribute("red", "1.0"));
				color.g = (float)StringHelp::text_to_float(cur_element.get_attribute("green", "1.0"));
				color.b = (float)StringHelp::text_to_float(cur_element.get_attribute("blue", "1.0"));
				color.a = (float)StringHelp::text_to_float(cur_element.get_attribute("alpha", "1.0"));
				image.set_color(color);
			}
			// <scale x="float" y="float />
			else if (tag_name == "scale")
			{
				float scale_x = StringHelp::text_to_float(cur_element.get_attribute("x", "1.0"));
				float scale_y = StringHelp::text_to_float(cur_element.get_attribute("y", "1.0"));
				image.set_scale(scale_x, scale_y);
			}
			// <translation origin="string" x="integer" y="integer" />
			else if (tag_name == "translation")
			{
				std::string hotspot = cur_element.get_attribute("origin", "top_left");
				Origin origin;

				if (hotspot == "center")
					origin = Origin::center;
				else if (hotspot == "top_center")
					origin = Origin::top_center;
				else if (hotspot == "top_right")
					origin = Origin::top_right;
				else if (hotspot == "center_left")
					origin = Origin::center_left;
				else if (hotspot == "center_right")
					origin = Origin::center_right;
				else if (hotspot == "bottom_left")
					origin = Origin::bottom_left;
				else if (hotspot == "bottom_center")
					origin = Origin::bottom_center;
				else if (hotspot == "bottom_right")
					origin = Origin::bottom_right;
				else
					origin = Origin::top_left;

				int xoffset = StringHelp::text_to_int(cur_element.get_attribute("x", "0"));
				int yoffset = StringHelp::text_to_int(cur_element.get_attribute("y", "0"));

				// TODO Find out what is going on here...
				xoffset /= image.get_texture().get_texture().get_pixel_ratio();
				yoffset /= image.get_texture().get_texture().get_pixel_ratio();

				image.set_alignment(origin, xoffset, yoffset);
			}

			cur_node = cur_node.get_next_sibling();
		}

		return image;
	}
}
