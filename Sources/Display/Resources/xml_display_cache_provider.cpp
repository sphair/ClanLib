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
**    Magnus Norddahl
*/

#include "Display/precomp.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/2D/sprite.h"
#include "API/Display/2D/image.h"
#include "API/Display/Render/texture_2d.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_sprite.h"
#include "API/Display/Font/font_description.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Render/texture.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/IOData/path_help.h"
#include "xml_display_cache_provider.h"

namespace clan
{

XMLDisplayCacheProvider::XMLDisplayCacheProvider(const XMLResourceDocument &doc)
	: doc(doc)
{
}

XMLDisplayCacheProvider::~XMLDisplayCacheProvider()
{
}

Resource<Sprite> XMLDisplayCacheProvider::get_sprite(GraphicContext &gc, const std::string &id)
{
	std::map<std::string, Resource<Sprite> >::iterator it = sprites.find(id);
	if (it != sprites.end())
		return it->second;

	Resource<Sprite> sprite = load_sprite(gc, id);
	sprites[id] = sprite;
	return sprite;
}

Resource<Image> XMLDisplayCacheProvider::get_image(GraphicContext &gc, const std::string &id)
{
	std::map<std::string, Resource<Image> >::iterator it = images.find(id);
	if (it != images.end())
		return it->second;

	Resource<Image> image = load_image(gc, id);
	images[id] = image;
	return image;
}

Resource<Texture> XMLDisplayCacheProvider::get_texture(GraphicContext &gc, const std::string &id)
{
	std::map<std::string, Resource<Texture> >::iterator it = textures.find(id);
	if (it != textures.end())
		return it->second;

	Resource<Texture> texture = load_texture(gc, id);
	textures[id] = texture;
	return texture;
}

Resource<Font> XMLDisplayCacheProvider::get_font(Canvas &canvas, const FontDescription &desc)
{
	std::string id = desc.get_unique_id();

	std::map<std::string, Resource<Font> >::iterator it = fonts.find(id);
	if (it != fonts.end())
		return it->second;

	Resource<Font> font = load_font(canvas, desc);
	fonts[id] = font;
	return font;
}

Resource<Sprite> XMLDisplayCacheProvider::load_sprite(GraphicContext &gc, const std::string &id)
{
	Sprite sprite(gc);

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
			continue;

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
					leading_zeroes =  StringHelp::text_to_int(cur_element.get_attribute("leading_zeroes"));

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
						Texture2D texture = Texture2D(gc, PathHelp::combine(resource.get_base_path(), file_name), fs);
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
				Texture2D texture = Texture2D(gc, PathHelp::combine(resource.get_base_path(), image_name), fs);

				DomNode cur_child(cur_element.get_first_child());
				if(cur_child.is_null()) 
				{
					sprite.add_frame(texture);
				}
				else 
				{
					do {
						DomElement cur_child_elemnt = cur_child.to_element();
						if(cur_child.get_node_name() == "grid")
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

							sprite.add_gridclipped_frames(gc, 
								texture,
								xpos, ypos,
								width, height,
								xarray, yarray,
								array_skipframes,
								xspacing, yspacing);
						}
						else if( cur_child.get_node_name() == "palette")
						{
							throw Exception("Resource '" + resource.get_name() + "' uses palette cutter - which is not supported anymore"); 
						}
						else if( cur_child.get_node_name() == "alpha")
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
								sprite.add_alphaclipped_frames_free(gc, 
									texture,
									xpos, ypos,
									trans_limit);
							}
							else
							{
								sprite.add_alphaclipped_frames(gc, 
									texture,
									xpos, ypos,
									trans_limit);
							}
						}

						cur_child = cur_child.get_next_sibling();
					} while(!cur_child.is_null());
				}
			}
		}
		cur_node = cur_node.get_next_sibling();
	}

	cur_node = resource.get_element().get_first_child();
	while (!cur_node.is_null())
	{
		if (!cur_node.is_element())
			continue;

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
			for(int i=0; i<frame_count; ++i)
				sprite.set_frame_delay(i, delay_ms);

			sprite.set_play_loop((cur_element.get_attribute("loop", "yes")) == "yes");
			sprite.set_play_pingpong((cur_element.get_attribute("pingpong", "no")) == "yes");
			sprite.set_play_backward((cur_element.get_attribute("direction", "forward")) == "backward");

			std::string on_finish = cur_element.get_attribute("on_finish", "blank");
			if (on_finish == "first_frame")
				sprite.set_show_on_finish(Sprite::show_first_frame);
			else if(on_finish == "last_frame")
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

			if(hotspot == "center")
				origin = origin_center;
			else if(hotspot == "top_center")
				origin = origin_top_center;
			else if(hotspot == "top_right")
				origin = origin_top_right;
			else if(hotspot == "center_left")
				origin = origin_center_left;
			else if(hotspot == "center_right")
				origin = origin_center_right;
			else if(hotspot == "bottom_left")
				origin = origin_bottom_left;
			else if(hotspot == "bottom_center")
				origin = origin_bottom_center;
			else if(hotspot == "bottom_right")
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

			if(hotspot == "top_left")
				origin = origin_top_left;
			else if(hotspot == "top_center")
				origin = origin_top_center;
			else if(hotspot == "top_right")
				origin = origin_top_right;
			else if(hotspot == "center_left")
				origin = origin_center_left;
			else if(hotspot == "center_right")
				origin = origin_center_right;
			else if(hotspot == "bottom_left")
				origin = origin_bottom_left;
			else if(hotspot == "bottom_center")
				origin = origin_bottom_center;
			else if(hotspot == "bottom_right")
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

	return Resource<Sprite>(sprite);
}

Resource<Image> XMLDisplayCacheProvider::load_image(GraphicContext &gc, const std::string &id)
{
	Image image;

	XMLResourceNode resource = doc.get_resource(id);

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
			Texture2D texture = Texture2D(gc, PathHelp::combine(resource.get_base_path(), image_name), resource.get_file_system());

			DomNode cur_child(cur_element.get_first_child());
			if(cur_child.is_null()) 
			{
				image = Image(gc, texture, texture.get_size());
			}
			else 
			{
				do {
					DomElement cur_child_elemnt = cur_child.to_element();
					if(cur_child.get_node_name() == "grid")
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

						image = Image(gc, texture, Rect(position, size));
					}

					cur_child = cur_child.get_next_sibling();
				} while(!cur_child.is_null());
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
			continue;

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

			if(hotspot == "center")
				origin = origin_center;
			else if(hotspot == "top_center")
				origin = origin_top_center;
			else if(hotspot == "top_right")
				origin = origin_top_right;
			else if(hotspot == "center_left")
				origin = origin_center_left;
			else if(hotspot == "center_right")
				origin = origin_center_right;
			else if(hotspot == "bottom_left")
				origin = origin_bottom_left;
			else if(hotspot == "bottom_center")
				origin = origin_bottom_center;
			else if(hotspot == "bottom_right")
				origin = origin_bottom_right;
			else
				origin = origin_top_left;

			int xoffset = StringHelp::text_to_int(cur_element.get_attribute("x", "0"));
			int yoffset = StringHelp::text_to_int(cur_element.get_attribute("y", "0"));

			image.set_alignment(origin, xoffset, yoffset);
		}

		cur_node = cur_node.get_next_sibling();
	}

	return Resource<Image>(image);
}

Resource<Texture> XMLDisplayCacheProvider::load_texture(GraphicContext &gc, const std::string &id)
{
	XMLResourceNode resource = doc.get_resource(id);

	std::string type = resource.get_type();

	if (type != "texture")
		throw Exception(string_format("Resource '%1' is not of type 'texture'", id));

	ImageImportDescription import_desc; // The infamous ImageImportDescription strikes again!

	std::string filename = resource.get_element().get_attribute("file");
	FileSystem fs = resource.get_file_system();

	Texture2D texture(gc, PathHelp::combine(resource.get_base_path(), filename), fs, import_desc);

	return Resource<Texture>(texture);
}

Resource<Font> XMLDisplayCacheProvider::load_font(Canvas &canvas, const FontDescription &desc)
{
	// To do: Fix this retarded way of detecting sprite fonts..
	bool is_sprite_font = false;
	try
	{
		doc.get_resource(desc.get_typeface_name());
		is_sprite_font = true;
	}
	catch (Exception &)
	{
		is_sprite_font = false;
	}

	if (is_sprite_font)
	{
		return load_sprite_font(canvas, desc.get_typeface_name());
	}
	else
	{
		return Resource<Font>(Font(canvas, desc));
	}
}

Resource<Font> XMLDisplayCacheProvider::load_sprite_font(Canvas &canvas, const std::string &id)
{
	XMLResourceNode resource = doc.get_resource(id);
	std::string type = resource.get_element().get_tag_name();
	
	if (type != "font")
		throw Exception(string_format("Resource '%1' is not of type 'font'", id));

	DomElement bitmap_element = resource.get_element().named_item("bitmap").to_element();

	FontMetrics font_metrics;

	if (!bitmap_element.is_null())
	{
		if (!bitmap_element.has_attribute("glyphs")) 
			throw Exception(string_format("Font resource %1 has no 'glyphs' attribute.", resource.get_name()));
		
		if (!bitmap_element.has_attribute("letters")) 
			throw Exception(string_format("Font resource %1 has no 'letters' attribute.", resource.get_name()));

		GraphicContext gc = canvas;
		Resource<Sprite> spr_glyphs = get_sprite(gc, bitmap_element.get_attribute("glyphs"));

		const std::string &letters = bitmap_element.get_attribute("letters");

		int spacelen = StringHelp::text_to_int(bitmap_element.get_attribute("spacelen", "-1"));
		bool monospace = StringHelp::text_to_bool(bitmap_element.get_attribute("monospace", "false"));

		// Modify the default font metrics, if specified

		if (bitmap_element.has_attribute("height")) 
			font_metrics.set_height(StringHelp::text_to_float(bitmap_element.get_attribute("height", "0")));

		if (bitmap_element.has_attribute("ascent")) 
			font_metrics.set_ascent(StringHelp::text_to_float(bitmap_element.get_attribute("ascent", "0")));

		if (bitmap_element.has_attribute("descent")) 
			font_metrics.set_descent(StringHelp::text_to_float(bitmap_element.get_attribute("descent", "0")));

		if (bitmap_element.has_attribute("internal_leading")) 
			font_metrics.set_internal_leading(StringHelp::text_to_float(bitmap_element.get_attribute("internal_leading", "0")));

		if (bitmap_element.has_attribute("external_leading")) 
			font_metrics.set_external_leading(StringHelp::text_to_float(bitmap_element.get_attribute("external_leading", "0")));

		if (bitmap_element.has_attribute("average_character_width")) 
			font_metrics.set_average_character_width(StringHelp::text_to_float(bitmap_element.get_attribute("average_character_width", "0")));

		if (bitmap_element.has_attribute("max_character_width")) 
			font_metrics.set_max_character_width(StringHelp::text_to_float(bitmap_element.get_attribute("max_character_width", "0")));

		if (bitmap_element.has_attribute("weight")) 
			font_metrics.set_weight(StringHelp::text_to_float(bitmap_element.get_attribute("weight", "0")));

		if (bitmap_element.has_attribute("overhang")) 
			font_metrics.set_overhang(StringHelp::text_to_float(bitmap_element.get_attribute("overhang", "0")));

		if (bitmap_element.has_attribute("digitized_aspect_x")) 
			font_metrics.set_digitized_aspect_x(StringHelp::text_to_float(bitmap_element.get_attribute("digitized_aspect_x", "0")));

		if (bitmap_element.has_attribute("digitized_aspect_y")) 
			font_metrics.set_digitized_aspect_y(StringHelp::text_to_float(bitmap_element.get_attribute("digitized_aspect_y", "0")));

		if (bitmap_element.has_attribute("italic")) 
			font_metrics.set_italic(StringHelp::text_to_bool(bitmap_element.get_attribute("italic", "0")));

		if (bitmap_element.has_attribute("underlined")) 
			font_metrics.set_underlined(StringHelp::text_to_bool(bitmap_element.get_attribute("underlined", "0")));

		if (bitmap_element.has_attribute("struck_out")) 
			font_metrics.set_struck_out(StringHelp::text_to_bool(bitmap_element.get_attribute("struck_out", "0")));

		if (bitmap_element.has_attribute("fixed_pitch")) 
			font_metrics.set_fixed_pitch(StringHelp::text_to_bool(bitmap_element.get_attribute("fixed_pitch", "0")));

		return Resource<Font>(Font_Sprite(spr_glyphs.get(), letters, spacelen, monospace, font_metrics));
	}
	else
	{
		throw Exception(string_format("Font resource %1 did not have a <bitmap> child element!", resource.get_name()));
	}
}

}
