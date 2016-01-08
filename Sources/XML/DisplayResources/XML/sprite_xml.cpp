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
**    Harry Storbacka
**    Mark Page
*/

#include "XML/precomp.h"
#include "API/Core/IOData/file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/XML/Resources/xml_resource_node.h"
#include "API/XML/dom_element.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/2D/sprite.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/Render/texture_2d.h"

namespace clan
{
	Sprite Sprite::load(Canvas &canvas, const std::string &id, const XMLResourceDocument &doc)
	{
		Sprite sprite(canvas);

		XMLResourceNode resource = doc.get_resource(id);
		if (resource.get_type() != "sprite")
			throw Exception(string_format("Resource '%1' is not of type 'sprite'", id));

		// Load base angle
		float work_angle = StringHelp::text_to_float(resource.get_element().get_attribute("base_angle", "0"));
		sprite.set_base_angle(Angle(work_angle, angle_degrees));

		// Load id
		sprite.set_id(StringHelp::text_to_int(resource.get_element().get_attribute("id", "0")));

		// Load play options	
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

					bool found_initial = false;
					for (int i = start_index;; i = skip_index)
					{
						std::string file_name = prefix;

						std::string frame_text = StringHelp::int_to_text(i);
						for (int zeroes_to_add = (leading_zeroes + 1) - frame_text.length(); zeroes_to_add > 0; zeroes_to_add--)
							file_name = "0";

						file_name = frame_text + suffix;

						try
						{
							Texture2D texture = Texture2D(canvas, PathHelp::combine(resource.get_base_path(), file_name), fs);
							sprite.add_frame(texture);
							found_initial = true;
						}
						catch (const Exception&)
						{
							if (!found_initial)
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
					Texture2D texture = Texture2D(canvas, PathHelp::combine(resource.get_base_path(), image_name), fs);

					DomNode cur_child(cur_element.get_first_child());
					if (cur_child.is_null())
					{
						sprite.add_frame(texture);
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

								sprite.add_gridclipped_frames(canvas,
									texture,
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
									sprite.add_alphaclipped_frames_free(canvas,
										texture,
										xpos, ypos,
										trans_limit);
								}
								else
								{
									sprite.add_alphaclipped_frames(canvas,
										texture,
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
				sprite.set_color(color);
			}
			// <animation speed="integer" loop="[yes,no]" pingpong="[yes,no]" direction="[backward,forward]" on_finish="[blank,last_frame,first_frame]"/>
			else if (tag_name == "animation")
			{
				int delay_ms = StringHelp::text_to_int(cur_element.get_attribute("speed", "60"));

				int frame_count = sprite.get_frame_count();
				for (int i = 0; i < frame_count; ++i)
					sprite.set_frame_delay(i, delay_ms);

				sprite.set_play_loop((cur_element.get_attribute("loop", "yes")) == "yes");
				sprite.set_play_pingpong((cur_element.get_attribute("pingpong", "no")) == "yes");
				sprite.set_play_backward((cur_element.get_attribute("direction", "forward")) == "backward");

				std::string on_finish = cur_element.get_attribute("on_finish", "blank");
				if (on_finish == "first_frame")
					sprite.set_show_on_finish(Sprite::show_first_frame);
				else if (on_finish == "last_frame")
					sprite.set_show_on_finish(Sprite::show_last_frame);
				else
					sprite.set_show_on_finish(Sprite::show_blank);
			}
			// <scale x="float" y="float />
			else if (tag_name == "scale")
			{
				float scale_x = StringHelp::text_to_float(cur_element.get_attribute("x", "1.0"));
				float scale_y = StringHelp::text_to_float(cur_element.get_attribute("y", "1.0"));
				sprite.set_scale(scale_x, scale_y);
			}
			// <translation origin="string" x="integer" y="integer" />
			else if (tag_name == "translation")
			{
				std::string hotspot = cur_element.get_attribute("origin", "top_left");
				Origin origin;

				if (hotspot == "center")
					origin = origin_center;
				else if (hotspot == "top_center")
					origin = origin_top_center;
				else if (hotspot == "top_right")
					origin = origin_top_right;
				else if (hotspot == "center_left")
					origin = origin_center_left;
				else if (hotspot == "center_right")
					origin = origin_center_right;
				else if (hotspot == "bottom_left")
					origin = origin_bottom_left;
				else if (hotspot == "bottom_center")
					origin = origin_bottom_center;
				else if (hotspot == "bottom_right")
					origin = origin_bottom_right;
				else
					origin = origin_top_left;

				int xoffset = StringHelp::text_to_int(cur_element.get_attribute("x", "0"));
				int yoffset = StringHelp::text_to_int(cur_element.get_attribute("y", "0"));

				sprite.set_alignment(origin, xoffset, yoffset);
			}
			// <rotation origin="string" x="integer" y="integer" />
			else if (tag_name == "rotation")
			{
				std::string hotspot = cur_element.get_attribute("origin", "center");
				Origin origin;

				if (hotspot == "top_left")
					origin = origin_top_left;
				else if (hotspot == "top_center")
					origin = origin_top_center;
				else if (hotspot == "top_right")
					origin = origin_top_right;
				else if (hotspot == "center_left")
					origin = origin_center_left;
				else if (hotspot == "center_right")
					origin = origin_center_right;
				else if (hotspot == "bottom_left")
					origin = origin_bottom_left;
				else if (hotspot == "bottom_center")
					origin = origin_bottom_center;
				else if (hotspot == "bottom_right")
					origin = origin_bottom_right;
				else
					origin = origin_center;

				int xoffset = StringHelp::text_to_int(cur_element.get_attribute("x", "0"));
				int yoffset = StringHelp::text_to_int(cur_element.get_attribute("y", "0"));

				sprite.set_rotation_hotspot(origin, xoffset, yoffset);
			}
			// <frame nr="integer" speed="integer" x="integer" y="integer" />
			else if (tag_name == "frame")
			{
				int nr = StringHelp::text_to_int(cur_element.get_attribute("nr", "0"));

				int yoffset = StringHelp::text_to_int(cur_element.get_attribute("y", "0"));
				int xoffset = StringHelp::text_to_int(cur_element.get_attribute("x", "0"));

				if (nr < 0 || nr >= sprite.get_frame_count())
				{
					throw Exception("Invalid sprite frame index specified");
				}

				if (cur_element.has_attribute("speed"))
				{
					sprite.set_frame_delay(nr, StringHelp::text_to_int(cur_element.get_attribute("speed", "60")));
				}

				sprite.set_frame_offset(nr, Point(xoffset, yoffset));
			}

			cur_node = cur_node.get_next_sibling();
		}

		sprite.restart();

		return sprite;
	}
}
