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

//! clanCore="XML"
//! header=core.h

#ifndef header_dom_processing_instruction
#define header_dom_processing_instruction

#ifdef CL_API_DLL
#ifdef CL_CORE_EXPORT
#define CL_API_CORE __declspec(dllexport)
#else
#define CL_API_CORE __declspec(dllimport)
#endif
#else
#define CL_API_CORE
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "dom_node.h"

//: DOM Processing Instruction class.
//- !group=Core/XML!
//- !header=core.h!
//- <p>The ProcessingInstruction interface represents a "processing instruction", used in
//- XML as a way to keep processor-specific information in the text of the document.</p>
class CL_API_CORE CL_DomProcessingInstruction : public CL_DomNode
{
//! Construction:
public:
	//: Constructs a DOM Processing Instruction handle.
	CL_DomProcessingInstruction();
	
	CL_DomProcessingInstruction(CL_DomDocument &doc, const std::string &target, const std::string &data);

	CL_DomProcessingInstruction(const CL_SharedPtr<CL_DomNode_Generic> &impl);

	~CL_DomProcessingInstruction();
	
//! Attributes:
public:
	//: The target of this processing instruction.
	//- <p>XML defines this as being the first token following the markup that begins
	//- the processing instruction.</p>
	std::string get_target() const;

	//: The content of this processing instruction.
	//- <p>This is from the first non white space character after the target to the
	//- character immediately preceding the ?&gt;.</p>
	std::string get_data() const;
	
	void set_data(const std::string &data);

//! Operations:
public:
};

#endif
