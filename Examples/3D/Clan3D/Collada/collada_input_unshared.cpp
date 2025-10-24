/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "collada_input_unshared.h"
#include "collada_source.h"

class CL_Collada_Input_Unshared_Impl
{
public:
	CL_Collada_Input_Unshared_Impl() {}

	CL_String semantic;
	CL_Collada_Source source;
};

CL_Collada_Input_Unshared::CL_Collada_Input_Unshared()
{
}

CL_Collada_Input_Unshared::CL_Collada_Input_Unshared(CL_DomElement &input_element, std::vector<CL_Collada_Source> &sources) : impl(new CL_Collada_Input_Unshared_Impl())
{
	impl->semantic = input_element.get_attribute("semantic");
	CL_String source_text = input_element.get_attribute("source");

	source_text = source_text.substr(1);	// Remove the initial '#' symbol

	std::vector<CL_Collada_Source>::size_type size, cnt;
	size = sources.size();
	for (cnt=0; cnt< size; cnt++)
	{
		if (sources[cnt].get_id() == source_text)
		{
			impl->source = sources[cnt];
			break;
		}
	}
	if (impl->source.is_null())
		throw CL_Exception("Unable to find source");
}


CL_String &CL_Collada_Input_Unshared::get_semantic()
{
	return impl->semantic;
}

CL_Collada_Source &CL_Collada_Input_Unshared::get_source()
{
	return impl->source;
}

