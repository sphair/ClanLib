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
#include "resourcedata_font.h"
#include "API/Display/sprite.h"
#include "API/Core/System/clanstring.h"
#include "API/Core/System/error.h"
#include "API/Core/Resources/resource_manager.h"

//////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Font construction:

CL_ResourceData_Font::CL_ResourceData_Font(CL_Resource &resource)
: CL_ResourceData(resource)
{
	resource.attach_data("font", this);
}
	
CL_ResourceData_Font::~CL_ResourceData_Font()
{
}
	
//////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Font implementation:

void CL_ResourceData_Font::on_load()
{
	CL_Resource resource = get_resource();
	CL_ResourceManager manager = resource.get_manager();
	
	CL_DomElement system_element = resource.get_element().named_item("system").to_element();
	CL_DomElement bitmap_element = resource.get_element().named_item("bitmap").to_element();
	
	// First check if it's a system font
	if (!system_element.is_null())
	{
		if (!system_element.has_attribute("height"))
			throw CL_Error("System font resource " + resource.get_name() + " has no 'height' attribute.");
		
		const std::string font_name = system_element.get_attribute("font_name");
		const int height = CL_String::to_int(system_element.get_attribute("height"));
		int width = 0;

		bool bold = false;
		bool italic = false;
		bool underline = false;
		bool strikeout = false;
		
		if (system_element.has_attribute("width"))
			width = CL_String::to_int(system_element.get_attribute("width"));
		
		if (system_element.has_attribute("bold"))
			bold = (CL_String::to_int(system_element.get_attribute("bold")) != 0);
		
		if (system_element.has_attribute("italic"))
			italic = (CL_String::to_int(system_element.get_attribute("italic")) != 0);
		
		if (system_element.has_attribute("underline"))
			underline = (CL_String::to_int(system_element.get_attribute("underline")) != 0);
		
		if (system_element.has_attribute("strikeout"))
			strikeout = (CL_String::to_int(system_element.get_attribute("strikeout")) != 0);
		
		if (system_element.has_attribute("letters"))
		{
			font = CL_Font(
				font_name,
				system_element.get_attribute("letters"),
				height,
				width,
				bold,
				italic,
				underline,
				strikeout);
		}
		else
		{
			font = CL_Font(
				font_name,
				height,
				width,
				bold,
				italic,
				underline,
				strikeout);
		}
	} 
	else if (!bitmap_element.is_null())
	{
		if (!bitmap_element.has_attribute("glyphs")) 
			throw CL_Error("Font resource " + resource.get_name() + " has no 'glyphs' attribute.");
		
		if (!bitmap_element.has_attribute("letters")) 
			throw CL_Error("Font resource " + resource.get_name() + " has no 'letters' attribute.");
		
		//Set most values to CL_Font defaults, then we can override them with options if they exist
		const CL_Sprite spr_glyphs(bitmap_element.get_attribute("glyphs"), &manager);
		const std::string letters = bitmap_element.get_attribute("letters");
		int spacelen = -1;
		bool monospace = false;
		
		if (bitmap_element.has_attribute("spacelen"))
			spacelen = CL_String::to_int(bitmap_element.get_attribute("spacelen"));
		
		if (bitmap_element.has_attribute("monospace"))
			monospace = (CL_String::to_int(bitmap_element.get_attribute("monospace")) != 0);
		
		font = CL_Font(
			spr_glyphs,
			letters,
			spacelen,
			monospace);
	}
	else
	{
		throw CL_Error(CL_String::format("Font resource %1 did not have a <system> or <bitmap> child element!", resource.get_name()));
	}
	
	if (resource.get_element().has_attribute("width_offset"))
		font.set_width_offset(CL_String::to_int(resource.get_element().get_attribute("width_offset")));
		
	if (resource.get_element().has_attribute("height_offset"))
		font.set_height_offset(CL_String::to_int(resource.get_element().get_attribute("height_offset")));
	
	if (resource.get_element().has_attribute("delims"))
		font.set_delims(resource.get_element().get_attribute("delims"));
}

void CL_ResourceData_Font::on_unload()
{
	font = CL_Font();
}
