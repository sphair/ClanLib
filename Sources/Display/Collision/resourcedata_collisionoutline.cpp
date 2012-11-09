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
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

#include "Display/precomp.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/Resources/resource.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/ImageProviders/provider_factory.h"
#include "API/Display/Image/pixel_buffer.h"
#include "outline_provider_file.h"
#include "outline_provider_bitmap.h"
#include "collision_outline_generic.h"
#include "resourcedata_collisionoutline.h"

namespace clan
{

//////////////////////////////////////////////////////////////////////////////
// ResourceData_CollisionOutline construction:

ResourceData_CollisionOutline::ResourceData_CollisionOutline(Resource &resource)
{
	std::string filename = resource.get_element().get_attribute("file");
	int alpha_limit = StringHelp::text_to_int( (resource.get_element().get_attribute("alpha_value", "128")));
	std::string accuracy_str = resource.get_element().get_attribute("accuracy", "medium");
	OutlineAccuracy accuracy;

	if(accuracy_str == "high")
		accuracy = accuracy_high;
	else if(accuracy_str == "medium")
		accuracy = accuracy_medium;
	else if(accuracy_str == "low")
		accuracy = accuracy_low;
	else if(accuracy_str == "poor")
		accuracy = accuracy_poor;
    else
		accuracy = accuracy_raw;

	CollisionOutline_Impl *outline;

	if (filename.length() >= 3 && filename.substr(filename.length()-3, 3) == "out" )
	{
		OutlineProviderFile outline_provider(filename, resource.get_manager().get_directory(resource));
		outline = new CollisionOutline_Impl(outline_provider.get_contours(), outline_provider.get_size(), accuracy_raw);
	}
	else
	{
		PixelBuffer pbuf = ImageProviderFactory::load(filename, resource.get_manager().get_directory(resource), "");
		OutlineProviderBitmap outline_provider(pbuf, alpha_limit);
		outline = new CollisionOutline_Impl(outline_provider.get_contours(), outline_provider.get_size(), accuracy );
	}

	CollisionOutline *collision_outline_tmp = 
		new CollisionOutline(outline->contours, outline->width, outline->height);
	collision_outline = *collision_outline_tmp;

	delete collision_outline_tmp;

	collision_outline.calculate_radius();
	
	delete outline;
}

ResourceData_CollisionOutline::~ResourceData_CollisionOutline()
{
}
	
//////////////////////////////////////////////////////////////////////////////
// ResourceData_Outline attributes:


//////////////////////////////////////////////////////////////////////////////
// ResourceData_Outline implementation:

}
