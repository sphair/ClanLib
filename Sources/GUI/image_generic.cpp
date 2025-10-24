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

#include "precomp.h"
#include "image_generic.h"
#include "API/Display/surface.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/GUI/stylemanager.h"

CL_Image_Generic::CL_Image_Generic(CL_Image *self, CL_Surface *surface, bool delete_surface)
: surface(surface), delete_surface(delete_surface), mode(CL_Image::center), image(self)
{
	slot_set_options = image->sig_set_options().connect(
		this, &CL_Image_Generic::on_set_options);
}

CL_Image_Generic::~CL_Image_Generic()
{
	if(delete_surface && surface)
		delete surface;
}

void CL_Image_Generic::on_set_options(const CL_DomElement &options)
{
	if (options.has_attribute("surface"))
	{
		CL_ResourceManager *resources = image->get_style_manager()->get_resources();
		image->set_surface(new CL_Surface(options.get_attribute("surface"), resources), true);
	}
}
