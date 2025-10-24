/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
#include "API/Display/2D/sprite_description.h"
#include "API/Display/Image/pixel_format.h"
#include "API/Display/ImageProviders/provider_factory.h"
#include "sprite_description_impl.h"
#include "API/Display/Render/shared_gc_data.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SpriteDescription construction:

CL_SpriteDescription::CL_SpriteDescription(CL_GraphicContext &gc, const CL_StringRef &resource_id, CL_ResourceManager *resources, const CL_ImageImportDescription &import_desc)
: impl(new CL_SpriteDescription_Impl)
{
	CL_Resource resource = resources->get_resource(resource_id);
	if (resource.get_type() != "sprite" && resource.get_type() != "sprite_description" && resource.get_type() != "image")
		throw CL_Exception(cl_format("Resource '%1' is not of type 'sprite' or 'sprite_description' or 'image'", resource_id));

	CL_DomNode cur_node = resource.get_element().get_first_child();

	while(!cur_node.is_null())
	{
		if (!cur_node.is_element()) 
			continue;

		CL_DomElement cur_element = cur_node.to_element();
		CL_String tag_name = cur_element.get_tag_name();
		if (tag_name == "image" || tag_name == "image-file")
		{
			if (cur_element.has_attribute("fileseq"))
			{
				int start_index = 0;
				if (cur_element.has_attribute("start_index"))
					start_index = CL_StringHelp::text_to_int(cur_element.get_attribute("start_index"));

				int skip_index = 1;
				if (cur_element.has_attribute("skip_index"))
					skip_index = CL_StringHelp::text_to_int(cur_element.get_attribute("skip_index"));

				int leading_zeroes = 0;
				if (cur_element.has_attribute("leading_zeroes"))
					leading_zeroes =  CL_StringHelp::text_to_int(cur_element.get_attribute("leading_zeroes"));

				CL_String prefix = cur_element.get_attribute("fileseq");
				CL_String suffix = "." + CL_PathHelp::get_extension(prefix);
				prefix.erase(prefix.length() - suffix.length(), prefix.length()); //remove the extension

				CL_VirtualDirectory virtual_directory = resources->get_directory(resource);

				for (int i = start_index;; i += skip_index)
				{
					CL_String file_name = prefix;

					CL_String frame_text = CL_StringHelp::int_to_text(i);
					for (int zeroes_to_add = (leading_zeroes+1) - frame_text.length(); zeroes_to_add > 0; zeroes_to_add--)
						file_name += "0";

					file_name += frame_text + suffix;

					try
					{
						CL_Texture texture = CL_SharedGCData::load_texture(gc, file_name, virtual_directory, import_desc );
						add_frame(texture);
					}
					catch (const CL_Exception&)
					{
						if (impl->frames.empty())
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
				CL_String image_name = cur_element.get_attribute("file");
				CL_Texture texture;
				CL_VirtualDirectory virtual_directory = resources->get_directory(resource);
				texture = CL_SharedGCData::load_texture(gc, image_name, virtual_directory, import_desc );

				CL_DomNode cur_child(cur_element.get_first_child());
				if(cur_child.is_null()) 
				{
					add_frame(texture);
				}
				else 
				{
					do {
						CL_DomElement cur_child_elemnt = cur_child.to_element();
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

							std::vector<CL_String> image_size = CL_StringHelp::split_text(cur_child_elemnt.get_attribute("size"), ",");
							if (image_size.size() > 0)
								width = CL_StringHelp::text_to_int(image_size[0]);
							if (image_size.size() > 1)
								height = CL_StringHelp::text_to_int(image_size[1]);

							if (cur_child_elemnt.has_attribute("pos"))
							{
								std::vector<CL_String> image_pos = CL_StringHelp::split_text(cur_child_elemnt.get_attribute("pos"), ",");
								if (image_pos.size() > 0)
									xpos = CL_StringHelp::text_to_int(image_pos[0]);
								if (image_pos.size() > 1)
									ypos = CL_StringHelp::text_to_int(image_pos[1]);
							}

							if (cur_child_elemnt.has_attribute("array"))
							{
								std::vector<CL_String> image_array = CL_StringHelp::split_text(cur_child_elemnt.get_attribute("array"), ",");
								if (image_array.size() == 2)
								{
									xarray = CL_StringHelp::text_to_int(image_array[0]);
									yarray = CL_StringHelp::text_to_int(image_array[1]);
								}
								else
								{
									throw CL_Exception("Resource '" + resource.get_name() + "' has incorrect array attribute, must be \"X,Y\"!"); 
								}
							}

							if (cur_child_elemnt.has_attribute("array_skipframes"))
							{
								array_skipframes = CL_StringHelp::text_to_int(cur_child_elemnt.get_attribute("array_skipframes"));
							}

							if (cur_child_elemnt.has_attribute("spacing"))
							{
								std::vector<CL_String> image_spacing = CL_StringHelp::split_text(cur_child_elemnt.get_attribute("spacing"), ",");
								xspacing = CL_StringHelp::text_to_int(image_spacing[0]);
								yspacing = CL_StringHelp::text_to_int(image_spacing[1]);
							}

							add_gridclipped_frames(
								texture,
								xpos, ypos,
								width, height,
								xarray, yarray,
								array_skipframes,
								xspacing, yspacing);
						}
						else if( cur_child.get_node_name() == "palette")
						{
							throw CL_Exception("Resource '" + resource.get_name() + "' uses palette cutter - which is not supported anymore"); 
						}
						else if( cur_child.get_node_name() == "alpha")
						{
							int xpos = 0;
							int ypos = 0;
							float trans_limit = 0.05f;

							if (cur_child_elemnt.has_attribute("pos"))
							{
								std::vector<CL_String> image_pos = CL_StringHelp::split_text(cur_child_elemnt.get_attribute("pos"), ",");
								xpos = CL_StringHelp::text_to_int(image_pos[0]);
								ypos = CL_StringHelp::text_to_int(image_pos[1]);
							}

							if (cur_child_elemnt.has_attribute("trans_limit"))
							{
								trans_limit = CL_StringHelp::text_to_float(cur_child_elemnt.get_attribute("trans_limit"));
							}

							if (cur_child_elemnt.has_attribute("free"))
							{
								add_alphaclipped_frames_free(
									texture,
									xpos, ypos,
									trans_limit);
							}
							else
							{
								add_alphaclipped_frames(
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

	if (get_frames().empty()) 
		throw CL_Exception("Sprite resource contained no frames!");
}

CL_SpriteDescription::CL_SpriteDescription()
: impl(new CL_SpriteDescription_Impl)
{
}

CL_SpriteDescription::CL_SpriteDescription(const CL_SpriteDescription &copy)
: impl(copy.impl) 
{
}

CL_SpriteDescription::~CL_SpriteDescription()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SpriteDescription attributes:

const std::vector<CL_SpriteDescriptionFrame> &CL_SpriteDescription::get_frames() const
{
	return impl->frames;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SpriteDescription operations:

CL_SpriteDescription &CL_SpriteDescription::operator =(const CL_SpriteDescription &copy)
{
	impl = copy.impl;
	return *this;
}

void CL_SpriteDescription::add_frame(const CL_PixelBuffer &pixelbuffer)
{
	impl->frames.push_back(CL_SpriteDescriptionFrame(pixelbuffer, CL_Rect(0, 0, pixelbuffer.get_width(), pixelbuffer.get_height())));
}

void CL_SpriteDescription::add_frame(const CL_Texture &texture)
{
	impl->frames.push_back(CL_SpriteDescriptionFrame(texture, CL_Rect(0, 0, texture.get_width(), texture.get_height())));
}

void CL_SpriteDescription::add_frame(const CL_StringRef &filename, CL_VirtualDirectory &dir, const CL_ImageImportDescription &import_desc)
{
	CL_PixelBuffer image = CL_ImageProviderFactory::load(filename, dir, "");
	image = import_desc.process(image);
	add_frame(image);
}

void CL_SpriteDescription::add_frame(const CL_StringRef &fullname, const CL_ImageImportDescription &import_desc)
{
	CL_String path = CL_PathHelp::get_fullpath(fullname, CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs(path);
	CL_VirtualDirectory dir = vfs.get_root_directory();
	add_frame(filename, dir, import_desc );
}

void CL_SpriteDescription::add_frame(CL_IODevice &file, const CL_String &image_type, const CL_ImageImportDescription &import_desc)
{
	CL_PixelBuffer image = CL_ImageProviderFactory::load(file, image_type);
	image = import_desc.process(image);
	add_frame(image);
}

void CL_SpriteDescription::add_frames(const CL_Texture &texture, CL_Rect *frames, int num_frames)
{
	for(int i=0; i<num_frames; ++i)
		impl->frames.push_back(CL_SpriteDescriptionFrame(texture, frames[i]));
}

void CL_SpriteDescription::add_gridclipped_frames(
	const CL_PixelBuffer &pixelbuffer,
	int xpos, int ypos,
	int width, int height,
	int xarray, int yarray,
	int array_skipframes,
	int xspace, int yspace)
{
	int ystart = ypos;
	for(int y = 0; y < yarray; y++)
	{
		int xstart = xpos;
		for(int x = 0; x < xarray; x++)
		{
			if (y == yarray -1 && x >= xarray - array_skipframes)
				break;

			if(xstart + width > pixelbuffer.get_width() || ystart + height > pixelbuffer.get_height())
				throw CL_Exception("add_gridclipped_frames: Outside pixelbuffer bounds");

			impl->frames.push_back(CL_SpriteDescriptionFrame(pixelbuffer, CL_Rect(xstart, ystart, xstart + width, ystart + height)));
			xstart += width + xspace;
		}
		ystart += height + yspace;
	}
}

void CL_SpriteDescription::add_gridclipped_frames(
	const CL_Texture &texture, 
	int xpos, int ypos, 
	int width, int height, 
	int xarray, int yarray, 
	int array_skipframes, 
	int xspace, int yspace)
{
	int ystart = ypos;
	for(int y = 0; y < yarray; y++)
	{
		int xstart = xpos;
		for(int x = 0; x < xarray; x++)
		{
			if (y == yarray -1 && x >= xarray - array_skipframes)
				break;

			if(xstart + width > texture.get_width() || ystart + height > texture.get_height())
				throw CL_Exception("add_gridclipped_frames: Outside texture bounds");

			impl->frames.push_back(CL_SpriteDescriptionFrame(texture, CL_Rect(xstart, ystart, xstart + width, ystart + height)));
			xstart += width + xspace;
		}
		ystart += height + yspace;
	}
}

void CL_SpriteDescription::add_alphaclipped_frames(
	const CL_Texture &texture, 
	int xpos, int ypos, 
	double trans_limit)
{
	CL_PixelBuffer pixelbuffer = texture.get_pixeldata(cl_rgba8);

	add_alphaclipped_frames(pixelbuffer, xpos, ypos, trans_limit);
}

void CL_SpriteDescription::add_alphaclipped_frames(
	const CL_PixelBuffer &pixelbuffer, 
	int xpos, int ypos, 
	double trans_limit)
{
	CL_PixelBuffer alpha_buffer = pixelbuffer.to_format(cl_rgba8);

	int begin = 0;
	bool prev_trans = true;

	int alpha_width = alpha_buffer.get_width();
	int alpha_height = alpha_buffer.get_height();
	bool found_opaque = false;
	bool found_trans = false;

	std::vector<int> opaque_row_vector;
	opaque_row_vector.resize(alpha_width);

	int *opaque_row = &(opaque_row_vector[0]);
	memset(opaque_row, 0, alpha_width*sizeof(int));

	int cut_top = 0;
	int cut_bottom = alpha_height;
		
	char *data = (char *) alpha_buffer.get_data();
		
	for (int y=0; y < alpha_height; y++)
	{
		bool opaque_line = false;
		unsigned int *line = (unsigned int *) (data + alpha_buffer.get_pitch()*y);
		for (int x=0; x < alpha_width; x++)
		{
			if ((line[x] & 255) > trans_limit*255)
			{
				opaque_row[x] = 1;
				opaque_line = true;
				found_opaque = true;
			}
		}
			
		if (opaque_line == false) // cut something of top or bottom
		{
			if (found_opaque)
			{
				cut_bottom--;
				found_trans = true;
			}
			else
				cut_top ++;
		}
		else if (found_trans)
		{
			found_trans = false;
			cut_bottom = alpha_height;
		}
	}
	
	if (cut_top >= cut_bottom)
		throw CL_Exception("add_alphaclipped_frames: Image contained only alpha!");

	for(int x=0; x < alpha_width; x++)
	{
		if(opaque_row[x] && prev_trans)
		{
			begin = x;
			prev_trans = false;
		}
		else if (!opaque_row[x] && !prev_trans)
		{
			impl->frames.push_back(
				CL_SpriteDescriptionFrame(pixelbuffer, CL_Rect(begin, cut_top, x+1, cut_bottom)));

			prev_trans = true;
		}
	}
		
	if (!prev_trans)
	{
		impl->frames.push_back(
			CL_SpriteDescriptionFrame(pixelbuffer, CL_Rect(begin, cut_top, alpha_width, cut_bottom)));
	}
}

void CL_SpriteDescription::add_alphaclipped_frames_free(
	const CL_Texture &texture, 
	int xpos, int ypos, 
	double trans_limit)
{
	CL_PixelBuffer pixelbuffer = texture.get_pixeldata(cl_rgba8);

	add_alphaclipped_frames_free(pixelbuffer, xpos, ypos, trans_limit);
}

void CL_SpriteDescription::add_alphaclipped_frames_free(
	const CL_PixelBuffer &pixelbuffer, 
	int xpos, int ypos, 
	double trans_limit)
{
	CL_PixelBuffer alpha_buffer = pixelbuffer.to_format(cl_rgba8);

	int width = alpha_buffer.get_width();
	int height = alpha_buffer.get_height();

	std::vector<int> explored_vector;
	explored_vector.resize(width * height);
	int *explored = &(explored_vector[0]);
	memset(explored, 0, width * height * sizeof(int));

	unsigned int *data = (unsigned int *) alpha_buffer.get_data();
	int x1, y1, x2, y2;
	bool more;

	for (int y=ypos; y < height; y++)
	{
		for (int x=xpos; x < width; x++)
		{
			if (explored[y*width+x] == 1) continue;
			explored[y*width+x] = 1;
			if ((data[y*width+x] & 255) <= trans_limit*255) 
				continue;

			// Initialize the bounding box to the current pixel
			x1 = x2 = x;
			y1 = y2 = y;
			more = true;
			while (more)
			{
				// Assume that there are NO opaque pixels around the current bounding box
				more = false;

				// Scan under the current bounding box and see if there any non-transparent pixels
				for (int i = x1; i <= x2; i++)
				{
					if (y2 + 1 < height)
					{
						explored[(y2+1)*width+i] = 1;
						if ((data[(y2+1)*width+i] & 255) > trans_limit*255)
						{
							more = true; 
							y2 = y2 + 1;
						}
					}
				}

				// Now scan the left and right sides of the current bounding box
				for (int j = y1; j <= y2; j++)
				{
					// Scan the right side
					if (x2 + 1 < width)
					{
						explored[j*width + x2+1] = 1;
						if ((data[j*width + x2+1] & 255) > trans_limit*255)
						{
							more = true; 
							x2 = x2 + 1;
						}
					}
					// Scan the left side
					if (x1 - 1 >= 0)
					{
						explored[j*width + x1-1] = 1;
						if ((data[j*width + x1-1] & 255) > trans_limit*255)
						{
							more = true; 
							x1 = x1 - 1;
						}
					}
				}
			} 

			// Mark all pixels in the bounding box as being explored
			for (int i = x1; i <= x2; i++)
			{
				for (int j = y1; j <= y2; j++)
				{
					explored[j*width+i] = 1;
				}
			}

			impl->frames.push_back(CL_SpriteDescriptionFrame(pixelbuffer, CL_Rect(x1, y1, x2, y2)));
		}
	}
}

void CL_SpriteDescription::set_frame_delay(int frame, double delay)
{
	impl->frames[frame].delay = delay;
}
