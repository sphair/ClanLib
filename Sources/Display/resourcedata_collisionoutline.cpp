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
#include "API/Core/Resources/resource_manager.h"
#include "API/Display/Providers/provider_factory.h"
#include "API/Core/System/clanstring.h"
#include "API/Display/pixel_buffer.h"
#include "API/Display/pixel_format.h"
#include "Display/Collision/collision_outline_generic.h"
#include "API/Display/Collision/outline_provider_file.h"
#include "Display/resourcedata_collisionoutline.h"
#include "API/Display/Collision/outline_provider_bitmap.h"

//////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_CollisionOutline construction:

CL_ResourceData_CollisionOutline::CL_ResourceData_CollisionOutline(CL_Resource &resource)
: CL_ResourceData(resource)
{
	resource.attach_data("collisionoutline", this);
}

CL_ResourceData_CollisionOutline::~CL_ResourceData_CollisionOutline()
{
}
	
//////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Outline attributes:


//////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Outline implementation:

void CL_ResourceData_CollisionOutline::on_load()
{
	CL_Resource resource = get_resource();
	std::string filename = resource.get_element().get_attribute("file");
	int alpha_limit = atoi( (resource.get_element().get_attribute("alpha_value","128")).c_str());
	std::string accuracy_str = resource.get_element().get_attribute("accuracy","medium");
	CL_OutlineAccuracy accuracy;

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

	CL_CollisionOutline_Generic *outline;

	if( CL_String::right(filename, 3) == "out" )
	{
		outline = new CL_CollisionOutline_Generic(
			new CL_OutlineProviderFile(filename,resource.get_manager().get_resource_provider()), accuracy_raw);
	}
	else
	{
		CL_PixelBuffer pbuf = CL_ProviderFactory::load(filename,"",resource.get_manager().get_resource_provider());
		outline = new CL_CollisionOutline_Generic(
			new CL_OutlineProviderBitmap(pbuf, alpha_limit), accuracy );
	}

	CL_CollisionOutline *collision_outline_tmp = 
		new CL_CollisionOutline(outline->contours,outline->width,outline->height);
	collision_outline = *collision_outline_tmp;

	delete collision_outline_tmp;

	collision_outline.calculate_radius();
	
	delete outline;
}
	
void CL_ResourceData_CollisionOutline::on_unload()
{
	collision_outline = CL_CollisionOutline();
}
