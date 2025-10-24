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
*/

/// \addtogroup clanCore_XML clanCore XML
/// \{

#pragma once

#include "../api_core.h"
#include "../System/sharedptr.h"
#include "dom_string.h"

class CL_DomNode;
class CL_DomNamedNodeMap_Generic;

/// \brief DOM Named Node Map class.
///
///    <p>Objects implementing the NamedNodeMap interface are used to represent collections of nodes
///    that can be accessed by name. Note that NamedNodeMap does not inherit from NodeList;
///    NamedNodeMaps are not maintained in any particular order. Objects contained in an object
///    implementing NamedNodeMap may also be accessed by an ordinal index, but this is simply to
///    allow convenient enumeration of the contents of a NamedNodeMap, and does not imply that the
///    DOM specifies an order to these Nodes.</p> 
/// \xmlonly !group=Core/XML! !header=core.h! \endxmlonly
class CL_API_CORE CL_DomNamedNodeMap
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a DOM NamedNodeMap handle.
	CL_DomNamedNodeMap();

	/// \brief Constructs a DomNamedNodeMap
	///
	/// \param copy = Dom Named Node Map
	CL_DomNamedNodeMap(const CL_DomNamedNodeMap &copy);

	~CL_DomNamedNodeMap();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief The number of nodes in the map.
	int get_length() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Retrieves a node specified by name.
	CL_DomNode get_named_item(const CL_DomString &name) const;

	/// \brief Retrieves a node specified by namespace URI and local name.
	CL_DomNode get_named_item_ns(
		const CL_DomString &namespace_uri,
		const CL_DomString &local_name) const;

	/// \brief Adds a node using its node name attribute.
	/** <p>As the node name attribute is used to derive the name which the node must be stored
	    under, multiple nodes of certain types (those that have a "special" string value) cannot
	    be stored as the names would clash. This is seen as preferable to allowing nodes to be
	    aliased.</p>
	    \param node A node to store in a named node map. The node will later be accessible using the value of the node name attribute of the node. If a node with that name is already present in the map, it is replaced by the new one.
	    retval: If the new Node replaces an existing node with the same name the previously existing Node is returned, otherwise null is returned.*/
	CL_DomNode set_named_item(const CL_DomNode &node);

	/// \brief Adds a node using its namespace URI and local name.
	/** <p>If a node with that namespace URI and that local name is already present in this
	    map, it is replaced by the new one.</p>*/
	CL_DomNode set_named_item_ns(const CL_DomNode &node);

	/// \brief Removes a node specified by name.
	/** <p>If the removed node is an Attr with a default value it is immediately replaced.</p>
	    \param name The name of a node to remove.
	    retval: The node removed from the map or null if no node with such a name exists.*/
	CL_DomNode remove_named_item(const CL_DomString &name);

	/// \brief Removes a node specified by namespace URI and local name.
	/** <p>If the removed node is an Attr with a default value it is immediately replaced.</p>
	    \return The node removed from the map or null if no node with such a name exists.*/
	CL_DomNode remove_named_item_ns(
		const CL_DomString &namespace_uri,
		const CL_DomString &local_name);

	/// \brief Returns the indexth item in the map.
	/** <p>If index is greater than or equal to the number of nodes in the map, this returns null.</p>*/
	CL_DomNode item(unsigned long index) const;

/// \}
/// \name Implementation
/// \{

protected:

	/// \brief Constructs a DomNamedNodeMap
	///
	/// \param impl = Shared Ptr
	CL_DomNamedNodeMap(const CL_SharedPtr<CL_DomNamedNodeMap_Generic> &impl);

	CL_SharedPtr<CL_DomNamedNodeMap_Generic> impl;

	friend class CL_DomNode;
/// \}
};

/// \}
