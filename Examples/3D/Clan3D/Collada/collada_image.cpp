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
**    Mark Page
*/

#include "precomp.h"
#include "collada_image.h"

class CL_Collada_Image_Impl
{
public:
	CL_Collada_Image_Impl() {}

	void load_image(CL_DomElement &image_element);

	CL_String id;
	CL_String filename;
};

CL_Collada_Image::CL_Collada_Image()
{
}

CL_Collada_Image::CL_Collada_Image(CL_DomElement &image_element) : impl(new CL_Collada_Image_Impl())
{
	impl->load_image(image_element);
}

void CL_Collada_Image_Impl::load_image(CL_DomElement &image_element)
{
	id = image_element.get_attribute("id");

	CL_DomElement init_from_element = image_element.named_item("init_from").to_element();
	if (init_from_element.is_null())
		throw CL_Exception("Only init_from images are supported");

	filename = init_from_element.get_text();
}

bool CL_Collada_Image::is_null()
{
	return impl.is_null();
}

CL_String &CL_Collada_Image::get_filename()
{
	return impl->filename;
}

CL_String &CL_Collada_Image::get_id()
{
	return impl->id;
}

