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

#include "Core/precomp.h"
#include "API/Core/XML/dom_cdata_section.h"
#include "dom_node_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DomCDATASection construction:

CL_DomCDATASection::CL_DomCDATASection()
{
}

CL_DomCDATASection::CL_DomCDATASection(CL_DomDocument &doc, const std::string &data)
: CL_DomText(doc, CDATA_SECTION_NODE)
{
	impl->node_value = data;
}

CL_DomCDATASection::CL_DomCDATASection(const CL_SharedPtr<CL_DomNode_Generic> &impl) : CL_DomText(impl)
{
}

CL_DomCDATASection::~CL_DomCDATASection()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomCDATASection attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_DomCDATASection operations:

/////////////////////////////////////////////////////////////////////////////
// CL_DomCDATASection implementation:
