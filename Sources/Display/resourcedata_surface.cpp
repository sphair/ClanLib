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
#include "resourcedata_surface.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Display/Providers/provider_factory.h"
#include "API/Core/System/clanstring.h"
#include "API/Display/pixel_buffer.h"
#include "API/Display/pixel_format.h"

//////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Surface construction:

CL_ResourceData_Surface::CL_ResourceData_Surface(CL_Resource &resource)
: CL_ResourceData(resource)
{
	resource.attach_data("surface", this);
}

CL_ResourceData_Surface::~CL_ResourceData_Surface()
{
}
	
//////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Surface attributes:


//////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Surface implementation:

void CL_ResourceData_Surface::on_load()
{
	CL_Resource resource = get_resource();
	std::string name = resource.get_element().get_attribute("file");
	std::string image_format = resource.get_element().get_attribute("format");
	std::string keep_pixelbuffer = resource.get_element().get_attribute("keep_pixelbuffer", "false");
	std::string force_16bit = resource.get_element().get_attribute("force_16bit", "false");

	int flag = CL_Surface::flag_none;
	if(CL_String::to_bool(keep_pixelbuffer) == true)
		flag |= CL_Surface::flag_keep_pixelbuffer;
	if(CL_String::to_bool(force_16bit) == true)
		flag |= CL_Surface::flag_force_16bit;

	CL_PixelBuffer image = CL_ProviderFactory::load(name, image_format, resource.get_manager().get_resource_provider());

	// now check if we just need a portion of the surface
	int xpos = -1, ypos = -1, width = -1, height = -1;
	if (resource.get_element().has_attribute("pos"))
	{
		std::vector<std::string> image_pos = CL_String::tokenize(resource.get_element().get_attribute("pos"), ",");
		
		xpos = atoi(image_pos[0].c_str());
		ypos = atoi(image_pos[1].c_str());
		
		// if only the position is given, we need the complete image from that point
		width = image.get_width() - xpos;
		height = image.get_height() - ypos;
	}
	if (resource.get_element().has_attribute("size"))
	{
		std::vector<std::string> image_pos = CL_String::tokenize(resource.get_element().get_attribute("pos"), ",");
		
		xpos = atoi(image_pos[0].c_str());
		ypos = atoi(image_pos[1].c_str());

		// if no position is given, we only need the upperleft corner
		if( xpos == -1 || ypos == -1 )
		{
			xpos = 0;
			ypos = 0;
		}
	}

	// if we only need a portion, convert the image
	if (xpos != -1 && ypos != -1 && width != -1 && height != -1)
	{
		CL_PixelBuffer chopped_image( width, height, image.get_format().get_depth()*width, image.get_format(), NULL );
		chopped_image.lock();
		image.convert( 
			chopped_image.get_data(), 
			chopped_image.get_format(), 
			image.get_format().get_depth()*width, 
			CL_Rect(0,0,width,height), 
			CL_Rect(xpos,ypos,xpos+width,ypos+height) );
		chopped_image.unlock();
		image = chopped_image;
	}

	surface = CL_Surface(image, flag);
}
	
void CL_ResourceData_Surface::on_unload()
{
	surface = CL_Surface();
}
