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

/// \addtogroup clanNetwork_Web clanNetwork Web
/// \{

#pragma once

#include "../api_network.h"

class CL_DomDocument;
class CL_DomElement;

/// \brief SOAP helper class.
///
/// \xmlonly !group=Network/Web! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_SoapHelp
{
/// \name Construction
/// \{

public:

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Namespace for SOAP 1.1
	/** <p>http://schemas.xmlsoap.org/soap/envelope/</p>*/
	static CL_StringRef soap11_ns;

	/// \brief Namespace for SOAP 1.2
	/** <p>http://www.w3.org/2003/05/soap-envelope</p>*/
	static CL_StringRef soap12_ns;

	/// \brief Namespace for XML Schema instance
	/** <p>http://www.w3.org/2001/XMLSchema-instance</p>*/
	static CL_StringRef xsi_ns;

	/// \brief Namespace for XML Schema
	/** <p>http://www.w3.org/2001/XMLSchema</p>*/
	static CL_StringRef xsd_ns;

	/// \brief Namespace for XML
	/** <p>http://www.w3.org/XML/1998/namespace</p>*/
	static CL_StringRef xml_ns;

/// \}
/// \name Operations
/// \{

public:
	static CL_DomDocument create_soap11_sender_fault(
		const CL_StringRef &reason,
		const CL_StringRef &lang = "en");

	static CL_DomDocument create_soap12_sender_fault(
		const CL_StringRef &reason,
		const CL_StringRef &lang = "en");

	static CL_DomDocument create_soap11_receiver_fault(
		const CL_StringRef &reason,
		const CL_StringRef &lang = "en");

	static CL_DomDocument create_soap12_receiver_fault(
		const CL_StringRef &reason,
		const CL_StringRef &lang = "en");

	static CL_DomDocument create_soap11_version_mismatch(
		const CL_StringRef &reason,
		const CL_StringRef &lang = "en",
		bool supports_soap11 = true,
		bool supports_soap12 = true);

	static CL_DomDocument create_soap12_version_mismatch(
		const CL_StringRef &reason,
		const CL_StringRef &lang = "en",
		bool supports_soap11 = true,
		bool supports_soap12 = true);

	static CL_DomDocument create_soap11_message(
		CL_DomElement &out_envelope,
		CL_DomElement &out_header,
		CL_DomElement &out_body);

	static CL_DomDocument create_soap11_message(
		CL_DomElement &out_envelope,
		CL_DomElement &out_body);

	static CL_DomDocument create_soap12_message(
		CL_DomElement &out_envelope,
		CL_DomElement &out_header,
		CL_DomElement &out_body);

	static CL_DomDocument create_soap12_message(
		CL_DomElement &out_envelope,
		CL_DomElement &out_body);

	static bool get_soap11_elements(
		CL_DomDocument &document,
		CL_DomElement &out_envelope,
		CL_DomElement &out_header,
		CL_DomElement &out_body,
		CL_DomElement &out_fault);

	static bool get_soap12_elements(
		CL_DomDocument &document,
		CL_DomElement &out_envelope,
		CL_DomElement &out_header,
		CL_DomElement &out_body,
		CL_DomElement &out_fault);

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

/// \}
