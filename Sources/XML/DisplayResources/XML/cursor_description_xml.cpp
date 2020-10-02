/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#include "XML/precomp.h"
#include "API/Core/IOData/file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/XML/Resources/xml_resource_node.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/XML/dom_node.h"
#include "API/XML/dom_element.h"
#include "API/Display/Window/cursor_description.h"
#include "API/Display/ImageProviders/provider_factory.h"

namespace clan
{
	CursorDescription CursorDescription::load(GraphicContext &gc, const std::string &resource_id, const XMLResourceDocument &resources, const ImageImportDescription &import_desc)
	{
		CursorDescription desc;

		XMLResourceNode resource = resources.get_resource(resource_id);
		if (resource.get_type() != "cursor" && resource.get_type() != "cursor_description" && resource.get_type() != "image")
			throw Exception(string_format("Resource '%1' is not of type 'cursor' or 'cursor_description' or 'image'", resource_id));

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
				if (cur_element.has_attribute("fileseq"))
				{
					int start_index = 0;
					if (cur_element.has_attribute("start_index"))
						start_index = StringHelp::text_to_int(cur_element.get_attribute("start_index"));

					int skip_index = 1;
					if (cur_element.has_attribute("skip_index"))
						skip_index = StringHelp::text_to_int(cur_element.get_attribute("skip_index"));

					int leading_zeroes = 0;
					if (cur_element.has_attribute("leading_zeroes"))
						leading_zeroes = StringHelp::text_to_int(cur_element.get_attribute("leading_zeroes"));

					std::string prefix = cur_element.get_attribute("fileseq");
					std::string suffix = "." + PathHelp::get_extension(prefix);
					prefix.erase(prefix.length() - suffix.length(), prefix.length()); //remove the extension

					FileSystem fs = resource.get_file_system();

					for (int i = start_index;; i += skip_index)
					{
						std::string file_name = prefix;

						std::string frame_text = StringHelp::int_to_text(i);
						for (int zeroes_to_add = (leading_zeroes + 1) - frame_text.length(); zeroes_to_add > 0; zeroes_to_add--)
							file_name += "0";

						file_name += frame_text + suffix;

						try
						{
							desc.add_frame(PathHelp::combine(resource.get_base_path(), file_name), fs, import_desc);
						}
						catch (const Exception&)
						{
							if (desc.get_frames().empty())
							{
								//must have been an error, pass it down
								throw;
							}
							//can't find anymore pics
							break;
						}
					}
				}
				else
				{
					std::string image_name = cur_element.get_attribute("file");
					FileSystem fs = resource.get_file_system();
					PixelBuffer pixelbuffer = ImageProviderFactory::load(PathHelp::combine(resource.get_base_path(), image_name), fs);
					pixelbuffer = import_desc.process(pixelbuffer);

					DomNode cur_child(cur_element.get_first_child());
					if (cur_child.is_null())
					{
						desc.add_frame(pixelbuffer);
					}
					else
					{
						do {
							DomElement cur_child_elemnt = cur_child.to_element();
							if (cur_child.get_node_name() == "grid")
							{
								int xpos = 0;
								int ypos = 0;
								int xarray = 1;
								int yarray = 1;
								int array_skipframes = 0;
								int xspacing = 0;
								int yspacing = 0;
								int width = 0;
								int height = 0;

								std::vector<std::string> image_size = StringHelp::split_text(cur_child_elemnt.get_attribute("size"), ",");
								if (image_size.size() > 0)
									width = StringHelp::text_to_int(image_size[0]);
								if (image_size.size() > 1)
									height = StringHelp::text_to_int(image_size[1]);

								if (cur_child_elemnt.has_attribute("pos"))
								{
									std::vector<std::string> image_pos = StringHelp::split_text(cur_child_elemnt.get_attribute("pos"), ",");
									if (image_pos.size() > 0)
										xpos = StringHelp::text_to_int(image_pos[0]);
									if (image_pos.size() > 1)
										ypos = StringHelp::text_to_int(image_pos[1]);
								}

								if (cur_child_elemnt.has_attribute("array"))
								{
									std::vector<std::string> image_array = StringHelp::split_text(cur_child_elemnt.get_attribute("array"), ",");
									if (image_array.size() == 2)
									{
										xarray = StringHelp::text_to_int(image_array[0]);
										yarray = StringHelp::text_to_int(image_array[1]);
									}
									else
									{
										throw Exception("Resource '" + resource.get_name() + "' has incorrect array attribute, must be \"X,Y\"!");
									}
								}

								if (cur_child_elemnt.has_attribute("array_skipframes"))
								{
									array_skipframes = StringHelp::text_to_int(cur_child_elemnt.get_attribute("array_skipframes"));
								}

								if (cur_child_elemnt.has_attribute("spacing"))
								{
									std::vector<std::string> image_spacing = StringHelp::split_text(cur_child_elemnt.get_attribute("spacing"), ",");
									xspacing = StringHelp::text_to_int(image_spacing[0]);
									yspacing = StringHelp::text_to_int(image_spacing[1]);
								}

								desc.add_gridclipped_frames(pixelbuffer,
									xpos, ypos,
									width, height,
									xarray, yarray,
									array_skipframes,
									xspacing, yspacing);
							}
							else if (cur_child.get_node_name() == "palette")
							{
								throw Exception("Resource '" + resource.get_name() + "' uses palette cutter - which is not supported anymore");
							}
							else if (cur_child.get_node_name() == "alpha")
							{
								int xpos = 0;
								int ypos = 0;
								float trans_limit = 0.05f;

								if (cur_child_elemnt.has_attribute("pos"))
								{
									std::vector<std::string> image_pos = StringHelp::split_text(cur_child_elemnt.get_attribute("pos"), ",");
									xpos = StringHelp::text_to_int(image_pos[0]);
									ypos = StringHelp::text_to_int(image_pos[1]);
								}

								if (cur_child_elemnt.has_attribute("trans_limit"))
								{
									trans_limit = StringHelp::text_to_float(cur_child_elemnt.get_attribute("trans_limit"));
								}

								if (cur_child_elemnt.has_attribute("free"))
								{
									desc.add_alphaclipped_frames_free(pixelbuffer,
										xpos, ypos,
										trans_limit);
								}
								else
								{
									desc.add_alphaclipped_frames(pixelbuffer,
										xpos, ypos,
										trans_limit);
								}
							}

							cur_child = cur_child.get_next_sibling();
						} while (!cur_child.is_null());
					}
				}
			}
			cur_node = cur_node.get_next_sibling();
		}

		if (desc.get_frames().empty())
			throw Exception("Cursor resource contained no frames!");
		return desc;
	}
}
