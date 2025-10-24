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
#include "API/Display/sprite_description.h"
#include "API/Display/pixel_buffer.h"
#include "API/Display/pixel_format.h"
#include "API/Core/System/error.h"
#include "API/Core/Resources/resource_manager.h"
#include "sprite_description_generic.h"
#include "resourcedata_sprite_description.h"
#include <cstring>

/////////////////////////////////////////////////////////////////////////////
// CL_SpriteDescription construction:

CL_SpriteDescription::CL_SpriteDescription(const std::string &resource_id, CL_ResourceManager *resources)
: impl(0)
{
	CL_Resource &resource = resources->get_resource(resource_id);

	CL_ResourceData_SpriteDescription *data =
		(CL_ResourceData_SpriteDescription *) resource.get_data("sprite");

	if (!data)
		throw CL_Error("Resource '" + resource_id + "' is not of type 'sprite'");

	CL_SpriteDescription desc = data->load_description();
	impl = desc.impl;
	impl->add_ref();
}

CL_SpriteDescription::CL_SpriteDescription()
: impl(new CL_SpriteDescription_Generic)
{
	impl->add_ref();
}

CL_SpriteDescription::CL_SpriteDescription(const CL_SpriteDescription &copy)
: impl(copy.impl)
{
	impl->add_ref();
}

CL_SpriteDescription::~CL_SpriteDescription()
{
	if (impl->release_ref() == 0)
	{
		delete impl;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_SpriteDescription attributes:

const std::list<CL_SpriteDescription::FramePair> &CL_SpriteDescription::get_frames() const
{
	return impl->frames;
}

int CL_SpriteDescription::get_surface_flag() const
{
	return impl->flag;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SpriteDescription operations:

CL_SpriteDescription &CL_SpriteDescription::operator =(const CL_SpriteDescription &copy)
{
	if (impl->release_ref() == 0)
	{
		delete impl;
	}

	impl = copy.impl;
	impl->add_ref();

	return *this;
}

void CL_SpriteDescription::set_surface_flag(int flag)
{
	impl->flag = flag;
}

void CL_SpriteDescription::add_frame(CL_PixelBuffer provider)
{
	provider.lock();
	impl->frames.push_back(FramePair(provider, CL_Rect(0, 0, provider.get_width(), provider.get_height())));
	provider.unlock();
}

void CL_SpriteDescription::add_gridclipped_frames(
	CL_PixelBuffer provider,
	int xpos, int ypos,
	int width, int height,
	int xarray, int yarray,
	int array_skipframes,
	int xspace, int yspace)
{
	// TODO: Check if positions are within bounds of provider
	
	int ystart = ypos;
	for(int y = 0; y < yarray; y++)
	{
		int xstart = xpos;
		for(int x = 0; x < xarray; x++)
		{
			if (y == yarray -1 && x >= xarray - array_skipframes)
				break;

			impl->frames.push_back(FramePair(provider, CL_Rect(xstart, ystart, xstart + width, ystart + height)));
			xstart += width + xspace;
		}
		ystart += height + yspace;
	}
}

void CL_SpriteDescription::add_alphaclipped_frames(CL_PixelBuffer provider, int xpos, int ypos, float trans_limit)
{
	provider.lock();

	CL_PixelFormat format;
	format.set_red_mask(0xff000000);
	format.set_green_mask(0x00ff0000);
	format.set_blue_mask(0x0000ff00);
	format.set_alpha_mask(0x000000ff);
	format.set_depth(32);
	format.enable_colorkey(false);
	
	CL_PixelBuffer alpha_buffer(
		provider.get_width(),
		provider.get_height(),
		provider.get_width()*4,
		format);

	provider.convert(alpha_buffer);
	provider.unlock();

	int begin=0;
	bool prev_trans=true;

	int alpha_width = alpha_buffer.get_width();
	int alpha_height = alpha_buffer.get_height();
	bool found_opaque = false;
	bool found_trans = false;

	int *opaque_row = new int[alpha_width];
	memset(opaque_row, 0, alpha_width*sizeof(int));

	int cut_top = 0;
	int cut_bottom = alpha_height;
		
	alpha_buffer.lock();
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
	
	alpha_buffer.unlock();

	if (cut_top >= cut_bottom)
		throw CL_Error("Font image contained only alpha!");

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
				FramePair(provider, CL_Rect(begin, cut_top, x+1, cut_bottom)));

			prev_trans = true;
		}
	}
		
	if (!prev_trans)
	{
		impl->frames.push_back(
			FramePair(provider, CL_Rect(begin, cut_top, alpha_width, cut_bottom)));
	}

	delete[] opaque_row;
}

void CL_SpriteDescription::add_alphaclipped_frames_free(CL_PixelBuffer provider, int xpos, int ypos, float trans_limit)
{
	provider.lock();

	CL_PixelFormat format;
	format.set_red_mask(0xff000000);
	format.set_green_mask(0x00ff0000);
	format.set_blue_mask(0x0000ff00);
	format.set_alpha_mask(0x000000ff);
	format.set_depth(32);
	format.enable_colorkey(false);

	CL_PixelBuffer alpha_buffer(
		provider.get_width(),
		provider.get_height(),
		provider.get_width()*4,
		format);

	provider.convert(alpha_buffer);
	provider.unlock();

	int width = alpha_buffer.get_width();
	int height = alpha_buffer.get_height();

	int *explored = new int[width*height];
	memset(explored, 0, width*height*sizeof(int));

	alpha_buffer.lock();
	unsigned int *data = (unsigned int *) alpha_buffer.get_data();
	int x1, y1, x2, y2;
	bool more;

	for (int y=ypos; y < height; y++)
	{
		for (int x=xpos; x < width; x++)
		{
			if (explored[y*width+x] == 1) continue;
			explored[y*width+x] = 1;
			if ((data[y*width+x] & 255) <= trans_limit*255) continue;
			// initialize the bounding box to the current pixel
			x1 = x2 = x;
			y1 = y2 = y;
			more = true;
			while (more)
			{
				// assume that there are NO opaque pixels around the current 
				// bounding box
				more = false;
				// scan under the current bounding box and see if there any 
				// non-transparent pixels
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
					// no need to check above the bounding box, becuase we are scanning
					// from TOP to bottom, hence we will hit the first pixel from the 
					// top and there won't be anything above it.
// 					if (y1 - 1 >= 0)
// 					{
// 						explored[(y1-1)*w+i] = 1;
// 						if ((data[(y1-1)*w+i] & 255) > trans_limit*255)
// 						{
// 							more = true; 
// 							y1 = y1 - 1;
// 						}
// 					}
				}
				// now scan the left and right sides of the current bounding box
				for (int j = y1; j <= y2; j++)
				{
					// scan the right side
					if (x2 + 1 < width)
					{
						explored[j*width + x2+1] = 1;
						if ((data[j*width + x2+1] & 255) > trans_limit*255)
						{
							more = true; 
							x2 = x2 + 1;
						}
					}
					// scan the left side
					if (x1 - 1 >= 0)
					{
						explored[j*width + x1-1] = 1;
						if ((data[j*width + x1-1] & 255) > trans_limit*255)
						{
							more = true; 
							x1 = x1 - 1;
						}
					}
				} // end scanning left&right
			} // end while (more)
			// mark all pixels in the bounding box as being explored
			for (int i = x1; i <= x2; i++)
				for (int j = y1; j <= y2; j++)
					explored[j*width+i] = 1;
			impl->frames.push_back(
				FramePair(provider, CL_Rect(x1, y1, x2, y2)));
		} // end for (int x=xpos; x < width; x++)
	} // end for (int y=ypos; y < height; y++)
	alpha_buffer.unlock();

	delete[] explored;
}

void CL_SpriteDescription::add_paletteclipped_frames(CL_PixelBuffer provider, int xpos, int ypos)
{
	provider.lock();

	if (provider.get_format().get_depth() != 8)
	{
		provider.unlock();
		throw CL_Error("Paletteclipped frames only works on PAL8 (256 colours) images.");
	}
		
	unsigned char *surface_data = (unsigned char *) provider.get_data();

	// Find height
	int height;
	for (height = ypos; height < provider.get_height(); height++)
	{
		if (surface_data[height * provider.get_pitch() + xpos] == 255)
		{
			height -= ypos;
			break;
		}
	}
	
	int org_x = xpos;
	bool next_line = false;
	bool found;
	
	do
	{
		// Find width
		int width;
		int provider_width = provider.get_width();
		found = false;
		for (width = xpos; width < provider_width; width++)
		{
			if (surface_data[ypos * provider.get_pitch() + width] == 254)
			{
				found = true;
				width -= xpos;
				break;
			}
			else if (surface_data[ypos * provider.get_pitch() + width] == 253)
			{
				found = true;
				width -= xpos;
				next_line = true;
				break;
			}
		}
		
		// Cut frame if found
		if(found)
		{
			impl->frames.push_back(FramePair(provider, CL_Rect(xpos, ypos, xpos + width, ypos + height)));
			
			xpos += width + 1;

			if (next_line)
			{
				xpos = org_x;
				ypos += height + 1;
				next_line = false;
			}
		}
	} while(found);

	provider.unlock();
}

/////////////////////////////////////////////////////////////////////////////
// CL_SpriteDescription implementation:
