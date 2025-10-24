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
**    Thomas Gottschalk Larsen
*/

/// \addtogroup clanCore_XML clanCore XML
/// \{

#pragma once

#include "../System/sharedptr.h"
#include <vector>

class CL_DomNode;
class CL_XPathObject_Impl;

/// \brief XPath result object.
///
/// \xmlonly !group=Core/XML! !header=core.h! \endxmlonly
class CL_XPathObject
{
/// \name Constuction
/// \{

public:
	CL_XPathObject();
	CL_XPathObject(bool value);
	CL_XPathObject(double value);
	CL_XPathObject(size_t value);
	CL_XPathObject(const CL_StringRef &value);
	CL_XPathObject(const std::vector<CL_DomNode> &value);

/// \}
/// \name Attributes
/// \{

public:
	enum Type
	{
		type_null,
		type_node_set,
		type_boolean,
		type_number,
		type_string
	};

	/// \brief Get Type
	///
	/// \return type
	Type get_type() const;

	/// \brief Is Null
	///
	/// \return true = null
	bool is_null() const;

	std::vector<CL_DomNode> get_node_set() const;

	/// \brief Get Boolean
	///
	/// \return boolean
	bool get_boolean() const;

	/// \brief Get Number
	///
	/// \return number
	double get_number() const;

	/// \brief Get String
	///
	/// \return string
	CL_StringRef get_string() const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Set null
	void set_null();

	/// \brief Set node set
	///
	/// \param vector = Dom Node
	void set_node_set(const std::vector<CL_DomNode> &node_set);

	/// \brief Set boolean
	///
	/// \param value = bool
	void set_boolean(bool value);

	/// \brief Set number
	///
	/// \param value = value
	void set_number(double value);

	/// \brief Set string
	///
	/// \param str = String Ref
	void set_string(const CL_StringRef &str);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_XPathObject_Impl> impl;
/// \}
};

/// \}
