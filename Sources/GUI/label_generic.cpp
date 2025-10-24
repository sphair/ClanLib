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
#include "label_generic.h"
#include "API/Core/XML/dom_element.h"


/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_Label_Generic::CL_Label_Generic(CL_Label *self, const std::string &_text)
: alignment(origin_top_left), label(self)
{
	text = _text;
	font = NULL;
	slot_set_options = label->sig_set_options().connect(
		this, &CL_Label_Generic::on_set_options);
}

void CL_Label_Generic::on_set_options(const CL_DomElement &options)
{
	if (options.has_attribute("text"))
		text = options.get_attribute("text");
}
