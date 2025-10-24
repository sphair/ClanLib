/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

	Type get_type() const;

	bool is_null() const;

	std::vector<CL_DomNode> get_node_set() const;

	bool get_boolean() const;

	double get_number() const;

	CL_StringRef get_string() const;


/// \}
/// \name Operations
/// \{

public:
	void set_null();

	void set_node_set(const std::vector<CL_DomNode> &node_set);

	void set_boolean(bool value);

	void set_number(double value);

	void set_string(const CL_StringRef &str);


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_XPathObject_Impl> impl;
/// \}
};


/// \}
