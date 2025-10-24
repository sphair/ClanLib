/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "Display/display_precomp.h"
#include "resourcedata_sprite_description.h"
#include "API/Display/sprite_description.h"
#include "API/Display/pixel_buffer.h"
#include "API/Core/System/clanstring.h"
#include "API/Core/System/error.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Display/Providers/provider_factory.h"

//////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_SpriteDescription construction:

CL_ResourceData_SpriteDescription::CL_ResourceData_SpriteDescription(CL_Resource &resource, const std::string &type)
: CL_ResourceData(resource)
{
	resource.attach_data(type, this);
}
	
CL_ResourceData_SpriteDescription::~CL_ResourceData_SpriteDescription()
{
}
	
//////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_SpriteDescription attributes:

//////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_SpriteDescription operations:

CL_SpriteDescription CL_ResourceData_SpriteDescription::load_description()
{
	CL_Resource resource = get_resource();
	CL_SpriteDescription desc;

	std::string str_keep_pixelbuffer;
	std::string str_force_16bit;

	std::string load_description = resource.get_element().get_attribute("description", "");
	if(load_description != "")
	{
		CL_Resource global_resource = resource.get_manager().get_resource(load_description);
		add_images(desc, global_resource);

		if (global_resource.get_element().has_attribute("keep_pixelbuffer"))
			str_keep_pixelbuffer = global_resource.get_element().get_attribute("keep_pixelbuffer", "false");
		if (global_resource.get_element().has_attribute("force_16bit"))
			str_force_16bit = global_resource.get_element().get_attribute("force_16bit", "false");
	}

	if (resource.get_element().has_attribute("keep_pixelbuffer"))
		str_keep_pixelbuffer = resource.get_element().get_attribute("keep_pixelbuffer", "false");
	if (resource.get_element().has_attribute("force_16bit"))
		str_force_16bit = resource.get_element().get_attribute("force_16bit", "false");

	bool keep_pixelbuffer = CL_String::to_bool(str_keep_pixelbuffer);
	bool force_16bit = CL_String::to_bool(str_force_16bit);

	desc.set_surface_flag(keep_pixelbuffer * CL_Surface::flag_keep_pixelbuffer | force_16bit * CL_Surface::flag_force_16bit);

	add_images(desc, resource);

	if (desc.get_frames().empty()) throw CL_Error("Sprite resource contained no frames!");

	return desc;
}

//////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_SpriteDescription implementation:

void CL_ResourceData_SpriteDescription::add_images(CL_SpriteDescription &desc, CL_Resource &resource)
{
	CL_DomNode cur_node;

	for (
		cur_node = resource.get_element().get_first_child();
		!cur_node.is_null();
		cur_node = cur_node.get_next_sibling())
	{
		if (!cur_node.is_element()) continue;

		CL_DomElement cur_element = cur_node.to_element();
		
		
		if (cur_element.get_tag_name() == "image")

		{
			bool colorkey_active = false;

			CL_Color transparent_color;

			if (cur_element.has_attribute("transparent_color"))
			{
				std::vector<std::string> color_strings = CL_String::tokenize(cur_element.get_attribute("transparent_color"), ",");
				if (color_strings.size() < 3)
				{
					throw CL_Error("the transparent_color tag should specify an RGB color like this: 255,255,255");
				}
					unsigned char r = atoi(color_strings[0].c_str());
					unsigned char g = atoi(color_strings[1].c_str());
					unsigned char b = atoi(color_strings[2].c_str());
					colorkey_active = true;
					transparent_color = CL_Color(r,g,b);
			}

			
			//check for the special tag that designates a sequence
			if (cur_element.has_attribute("fileseq"))
			{
				int leading_zeroes = 0;
				
				if (cur_element.has_attribute("leading_zeroes"))
				{
					leading_zeroes = atoi(cur_element.get_attribute("leading_zeroes").c_str());
				}
				
				std::string prefix = cur_element.get_attribute("fileseq");
				std::string suffix = "." + CL_String::get_extension(prefix);
				prefix.erase(prefix.length() - suffix.length(), prefix.length()); //remove the extension
				std::string file_name;
				//loop through and load all images we can find

				for (int i=0;;i++)
				{
					CL_PixelBuffer image;
					file_name = prefix;
					std::string frame_text = CL_String::from_int(i);
					for (int zeroes_to_add = (leading_zeroes+1) - frame_text.length(); zeroes_to_add > 0; zeroes_to_add--)
					{
						file_name += "0";
					}

					file_name += frame_text + suffix;

					try
					{
						image = load_image(file_name);
					}
					catch (CL_Error e)
					{
						if (!i) continue; //it might start at frame 1 instead of 0, let's give it another chance
						
						if (desc.get_frames().empty())
						{
							//must have been an error, pass it down
							throw e;
						}
						//can't find anymore pics I guess
						return;
					}
					if (colorkey_active) image.set_colorkey(true, transparent_color.color);
					desc.add_frame(image);
				}
			}

			std::string image_name = cur_element.get_attribute("file");
			
			CL_PixelBuffer image = load_image(image_name);

			if (colorkey_active) image.set_colorkey(true, transparent_color.color);

			CL_DomNode cur_child ( cur_element.get_first_child() );
			if( cur_child.is_null() ) 
			{
				desc.add_frame(image);
			}
			else do
			{
				CL_DomElement cur_child_elemnt = cur_child.to_element();
				if( cur_child.get_node_name() == "grid" )
				{
					int xpos = 0;
					int ypos = 0;
					int xarray = 1;
					int yarray = 1;
					int array_skipframes = 0;
					int xspacing = 0;
					int yspacing = 0;

					std::vector<std::string> image_size = CL_String::tokenize(cur_child_elemnt.get_attribute("size"), ",");
					int width = 0;
					if (image_size.size() > 0)
						width = atoi(image_size[0].c_str());
					int height = 0;
					if (image_size.size() > 1)
						height = atoi(image_size[1].c_str());

					if (cur_child_elemnt.has_attribute("pos"))
					{
						std::vector<std::string> image_pos = CL_String::tokenize(cur_child_elemnt.get_attribute("pos"), ",");
						xpos = atoi(image_pos[0].c_str());
						ypos = atoi(image_pos[1].c_str());
					}

					if (cur_child_elemnt.has_attribute("array"))
					{
						std::vector<std::string> image_array = CL_String::tokenize(cur_child_elemnt.get_attribute("array"), ",");
											if (image_array.size() == 2)
											  {
												xarray = atoi(image_array[0].c_str());
												yarray = atoi(image_array[1].c_str());
											  }
											else
											  {
												throw CL_Error("Resource '" + resource.get_name() + "' has incorrect array attribute, must be \"X,Y\"!"); 
											  }
					}

					if (cur_child_elemnt.has_attribute("array_skipframes"))
					{
						array_skipframes = atoi(cur_child_elemnt.get_attribute("array_skipframes").c_str());
					}

					if (cur_child_elemnt.has_attribute("spacing"))
					{
						std::vector<std::string> image_spacing = CL_String::tokenize(cur_child_elemnt.get_attribute("spacing"), ",");
						xspacing = atoi(image_spacing[0].c_str());
						yspacing = atoi(image_spacing[1].c_str());
					}

					desc.add_gridclipped_frames(
						image,
						xpos, ypos,
						width, height,
						xarray, yarray,
						array_skipframes,
						xspacing, yspacing);
				}
				else if( cur_child.get_node_name() == "palette" )
				{
					int xpos = 0;
					int ypos = 0;

					if (cur_child_elemnt.has_attribute("pos"))
					{
						std::vector<std::string> image_pos = CL_String::tokenize(cur_child_elemnt.get_attribute("pos"), ",");
						xpos = atoi(image_pos[0].c_str());
						ypos = atoi(image_pos[1].c_str());
					}

					desc.add_paletteclipped_frames(
						image,
						xpos, ypos);
				}
				else if( cur_child.get_node_name() == "alpha" )
				{
					int xpos = 0;
					int ypos = 0;
					float trans_limit = 0.05f;

					if (cur_child_elemnt.has_attribute("pos"))
					{
						std::vector<std::string> image_pos = CL_String::tokenize(cur_child_elemnt.get_attribute("pos"), ",");
						xpos = atoi(image_pos[0].c_str());
						ypos = atoi(image_pos[1].c_str());
					}

					if (cur_child_elemnt.has_attribute("trans_limit"))
					{
						trans_limit = (float)atof(cur_child_elemnt.get_attribute("trans_limit").c_str());
					}

					if (cur_child_elemnt.has_attribute("free"))
					{
						desc.add_alphaclipped_frames_free(
							image,
							xpos, ypos,
							trans_limit);
					}
					else
					{
						desc.add_alphaclipped_frames(
							image,
							xpos, ypos,
							trans_limit);
					}
				}

				cur_child = cur_child.get_next_sibling();
				
			} while(!cur_child.is_null());
		}
	}
}

CL_PixelBuffer CL_ResourceData_SpriteDescription::load_image(const std::string &filename)
{
	return CL_ProviderFactory::load(filename, "", get_resource().get_manager().get_resource_provider());
}
