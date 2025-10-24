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
#include <vector>

class CL_DomDocument;
class CL_DomElement;
class CL_DataBuffer;
class CL_WebservicePart;
class CL_WebserviceMessage_Impl;

/// \brief Web service message.
///
/// \xmlonly !group=Network/Web! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_WebserviceMessage
{
/// \name Construction
/// \{

public:
	CL_WebserviceMessage(const CL_StringRef &name, const CL_StringRef &target_namespace);

	CL_WebserviceMessage(const CL_DomDocument &doc);

	~CL_WebserviceMessage();


/// \}
/// \name Attributes
/// \{

public:
	CL_DomDocument get_document() const;

	CL_DomElement get_message_element() const;

	CL_DomElement get_fault_element() const;

	CL_String get_name() const;

	CL_String get_target_namespace() const;

	CL_WebservicePart get_part(const CL_StringRef &name);

	CL_WebservicePart get_part_ns(const CL_StringRef &namespace_uri, const CL_StringRef &name);

	CL_String get_string(const CL_StringRef &name);

	CL_String get_string_ns(const CL_StringRef &namespace_uri, const CL_StringRef &name);

	int get_int(const CL_StringRef &name);

	int get_int_ns(const CL_StringRef &namespace_uri, const CL_StringRef &name);

	double get_double(const CL_StringRef &name);

	double get_double_ns(const CL_StringRef &namespace_uri, const CL_StringRef &name);

	CL_DataBuffer get_base64(const CL_StringRef &name);

	CL_DataBuffer get_base64_ns(const CL_StringRef &namespace_uri, const CL_StringRef &name);


/// \}
/// \name Operations
/// \{

public:
	/// \brief Creates a qualified name
	/** <p>This function adds the target namespace prefix (if any) to the name specified.</p>*/
	CL_String create_qname(const CL_StringRef &name);

	CL_WebservicePart append_part(const CL_StringRef &name);

	CL_WebservicePart append_part_ns(const CL_StringRef &namespace_uri, const CL_StringRef &name);

	CL_WebservicePart append_string(const CL_StringRef &name, const CL_StringRef &value);

	CL_WebservicePart append_string_ns(const CL_StringRef &namespace_uri, const CL_StringRef &name, const CL_StringRef &value);

	CL_WebservicePart append_int(const CL_StringRef &name, int value);

	CL_WebservicePart append_int_ns(const CL_StringRef &namespace_uri, const CL_StringRef &name, int value);

	CL_WebservicePart append_double(const CL_StringRef &name, double value);

	CL_WebservicePart append_double_ns(const CL_StringRef &namespace_uri, const CL_StringRef &name, double value);

	CL_WebservicePart append_base64(const CL_StringRef &name, const CL_DataBuffer &value);

	CL_WebservicePart append_base64_ns(const CL_StringRef &namespace_uri, const CL_StringRef &name, const CL_DataBuffer &value);


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_WebserviceMessage_Impl> impl;
/// \}
};


/// \}
