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
**    Magnus Norddahl
**    Ingo Ruhnke
*/

/// \addtogroup clanCore_XML clanCore XML
/// \{

#pragma once

#include "../api_core.h"
#include "../System/sharedptr.h"
#include "dom_string.h"
#include <vector>

class CL_DomNode;
class CL_DomNodeList_Generic;
class CL_DomDocument;

/// \brief DOM Node List class.
///
///    <p>The NodeList interface provides the abstraction of an ordered collection of nodes,
///    without defining or constraining how this collection is implemented.</p> 
/// \xmlonly !group=Core/XML! !header=core.h! \endxmlonly
class CL_API_CORE CL_DomNodeList
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a DOM NodeList handle.
	CL_DomNodeList();

	/// \brief Constructs a DomNodeList
	///
	/// \param node = Dom Node
	/// \param tag_name = Dom String
	CL_DomNodeList(CL_DomNode &node, const CL_DomString &tag_name);

	CL_DomNodeList(
		CL_DomNode &node,
		const CL_DomString &namespace_uri,
		const CL_DomString &name,
		bool local_name = false);

	~CL_DomNodeList();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief The number of nodes in the list.
	int get_length() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Returns the indexth item in the collection.
	/** <p>If index is greater than or equal to the number of nodes in the list, this returns an empty node.</p>*/
	CL_DomNode item(unsigned long index) const;

	/// \brief Adds a CL_DomNode to the list.
	void add_item(CL_DomNode &to_add);

/// \}
/// \name Implementation
/// \{

private:
	/** CL_SharedPtr<CL_DomNodeList_Generic> impl;*/
	std::vector<CL_DomNode> node_list;
/// \}
};

/// \}
