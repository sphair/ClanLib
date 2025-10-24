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
**    Magnus Norddahl
*/

/// \addtogroup clanCore_XML clanCore XML
/// \{

#pragma once

#include "../api_core.h"
#include "dom_node.h"

/// \brief DOM Processing Instruction class.
///
///    <p>The ProcessingInstruction interface represents a "processing instruction", used in
///    XML as a way to keep processor-specific information in the text of the document.</p> 
/// \xmlonly !group=Core/XML! !header=core.h! \endxmlonly
class CL_API_CORE CL_DomProcessingInstruction : public CL_DomNode
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a DOM Processing Instruction handle.
	CL_DomProcessingInstruction();

	/// \brief Constructs a DomProcessingInstruction
	///
	/// \param doc = Dom Document
	/// \param target = Dom String
	/// \param data = Dom String
	CL_DomProcessingInstruction(CL_DomDocument &doc, const CL_DomString &target, const CL_DomString &data);

	/// \brief Constructs a DomProcessingInstruction
	///
	/// \param impl = Shared Ptr
	CL_DomProcessingInstruction(const CL_SharedPtr<CL_DomNode_Generic> &impl);

	~CL_DomProcessingInstruction();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief The target of this processing instruction.
	/** <p>XML defines this as being the first token following the markup that begins
	    the processing instruction.</p>*/
	CL_DomString get_target() const;

	/// \brief The content of this processing instruction.
	/** <p>This is from the first non white space character after the target to the
	    character immediately preceding the ?&gt;.</p>*/
	CL_DomString get_data() const;

	/// \brief Set data
	///
	/// \param data = Dom String
	void set_data(const CL_DomString &data);

/// \}
/// \name Operations
/// \{

public:
/// \}
};

/// \}
