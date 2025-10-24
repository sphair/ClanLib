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
#include "API/Core/XML/dom_processing_instruction.h"
#include "dom_node_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DomProcessingInstruction construction:

CL_DomProcessingInstruction::CL_DomProcessingInstruction()
{
}

CL_DomProcessingInstruction::CL_DomProcessingInstruction(CL_DomDocument &doc, const std::string &target, const std::string &data)
: CL_DomNode(doc, PROCESSING_INSTRUCTION_NODE)
{
	impl->node_value = data;
}

CL_DomProcessingInstruction::CL_DomProcessingInstruction(const CL_SharedPtr<CL_DomNode_Generic> &impl) : CL_DomNode(impl)
{
}

CL_DomProcessingInstruction::~CL_DomProcessingInstruction()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomProcessingInstruction attributes:

std::string CL_DomProcessingInstruction::get_target() const
{
	return std::string();
}

std::string CL_DomProcessingInstruction::get_data() const
{
	return std::string();
}

void CL_DomProcessingInstruction::set_data(const std::string &data)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomProcessingInstruction operations:

/////////////////////////////////////////////////////////////////////////////
// CL_DomProcessingInstruction implementation:
