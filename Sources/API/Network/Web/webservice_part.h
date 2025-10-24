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

/// \addtogroup clanNetwork_Web clanNetwork Web
/// \{


#pragma once


#include "../api_network.h"
#include "../../Core/System/sharedptr.h"

class CL_DomElement;
class CL_DataBuffer;
class CL_WebservicePart_Impl;

/// \brief Web service message part.
///
/// \xmlonly !group=Network/Web! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_WebservicePart
{
/// \name Construction
/// \{

public:
	CL_WebservicePart(const CL_DomElement &part_element);

	~CL_WebservicePart();


/// \}
/// \name Attributes
/// \{

public:
	CL_String get_name() const;

	CL_String get_target_namespace() const;

	CL_DomElement get_element() const;

	CL_String get_value_string() const;

	int get_value_int() const;

	double get_value_double() const;

	CL_DataBuffer get_value_base64() const;


/// \}
/// \name Operations
/// \{

public:
	void set_value_string(const CL_StringRef &value);

	void set_value_int(int value);

	void set_value_double(double value);

	void set_value_base64(const CL_DataBuffer &value);


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_WebservicePart_Impl> impl;
/// \}
};


/// \}
