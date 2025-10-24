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

/// \brief CL_HTTPHeaderAccept
///
/// \xmlonly !group=Network/Web! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_HTTPHeaderAccept
{
/// \name Attributes
/// \{

public:
	CL_StringRef type;

	CL_StringRef subtype;

	CL_StringRef type_params;

	CL_StringRef quality;

	CL_StringRef accept_params;
/// \}
};

/// \brief CL_HTTPHeaderAcceptCharset
///
/// \xmlonly !group=Network/Web! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_HTTPHeaderAcceptCharset
{
/// \name Attributes
/// \{

public:
	CL_StringRef name;

	CL_StringRef quality;
/// \}
};

/// \brief CL_HTTPHeaderAcceptEncoding
///
/// \xmlonly !group=Network/Web! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_HTTPHeaderAcceptEncoding
{
/// \name Attributes
/// \{

public:
	CL_StringRef name;

	CL_StringRef quality;
/// \}
};

/// \brief CL_HTTPHeaderAcceptLanguage
///
/// \xmlonly !group=Network/Web! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_HTTPHeaderAcceptLanguage
{
/// \name Attributes
/// \{

public:
	CL_StringRef name;

	CL_StringRef variant;

	CL_StringRef quality;
/// \}
};

/// \brief CL_HTTPHeaderContentLanguage
///
/// \xmlonly !group=Network/Web! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_HTTPHeaderContentLanguage
{
/// \name Attributes
/// \{

public:
	CL_StringRef name;

	CL_StringRef variant;
/// \}
};

/// \brief CL_HTTPHeaderContentRange
///
/// \xmlonly !group=Network/Web! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_HTTPHeaderContentRange
{
/// \name Attributes
/// \{

public:
	CL_StringRef range_type;  // always 'bytes'

	CL_StringRef first_byte_pos;

	CL_StringRef last_byte_pos;

	CL_StringRef instance_length;
/// \}
};

/// \brief CL_HTTPHeaderContentType
///
/// \xmlonly !group=Network/Web! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_HTTPHeaderContentType
{
/// \name Attributes
/// \{

public:
	CL_StringRef type;

	CL_StringRef params;
/// \}
};

/// \brief CL_HTTPHeaderRange
///
/// \xmlonly !group=Network/Web! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_HTTPHeaderRange
{
/// \name Attributes
/// \{

public:
	CL_StringRef first_byte_pos;

	CL_StringRef last_byte_pos;
/// \}
};

/// \brief CL_HTTPHeaderTE
///
/// \xmlonly !group=Network/Web! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_HTTPHeaderTE // (TransferEncodingAccept)
{
/// \name Attributes
/// \{

public:
	CL_StringRef name;

	CL_StringRef quality;

	CL_StringRef accept_params;
/// \}
};

/// \brief CL_HTTPHeaderVia
///
/// \xmlonly !group=Network/Web! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_HTTPHeaderVia
{
/// \name Attributes
/// \{

public:
	CL_StringRef protocol_name;

	CL_StringRef protocol_version;

	CL_StringRef received_by;
/// \}
};

/// \brief CL_HTTPHeaderWarning
///
/// \xmlonly !group=Network/Web! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_HTTPHeaderWarning
{
/// \name Attributes
/// \{

public:
	CL_StringRef code;

	CL_StringRef agent;

	CL_StringRef text;

	CL_StringRef date;
/// \}
};

/// \brief CL_HTTPHeaderHelp
///
/// \xmlonly !group=Network/Web! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_HTTPHeaderHelp
{
/// \name Operations
/// \{

public:
	static CL_StringRef get_header_value(
		const CL_StringRef &name,
		const CL_StringRef &header_lines);

	static std::vector<CL_HTTPHeaderAccept> get_accept(const CL_StringRef &header_lines);

	static std::vector<CL_HTTPHeaderAcceptCharset> get_accept_charset(const CL_StringRef &header_lines);

	static std::vector<CL_HTTPHeaderAcceptEncoding> get_accept_encoding(const CL_StringRef &header_lines);

	static std::vector<CL_HTTPHeaderAcceptLanguage> get_accept_language(const CL_StringRef &header_lines);

	static std::vector<CL_StringRef> get_accept_range(const CL_StringRef &header_lines);

	/// \brief Get age
	///
	/// \param header_lines = String Ref
	///
	/// \return String Ref
	static CL_StringRef get_age(const CL_StringRef &header_lines);

	static std::vector<CL_StringRef> get_allow(const CL_StringRef &header_lines);

	/// \brief Get authorization
	///
	/// \param header_lines = String Ref
	///
	/// \return String Ref
	static CL_StringRef get_authorization(const CL_StringRef &header_lines);

	static std::vector<CL_StringRef> get_cache_control(const CL_StringRef &header_lines);

	static std::vector<CL_StringRef> get_connection(const CL_StringRef &header_lines);

	static std::vector<CL_StringRef> get_content_encoding(const CL_StringRef &header_lines);

	static std::vector<CL_HTTPHeaderContentLanguage> get_content_language(const CL_StringRef &header_lines);

	/// \brief Get content length
	///
	/// \param header_lines = String Ref
	///
	/// \return String Ref
	static CL_StringRef get_content_length(const CL_StringRef &header_lines);

	/// \brief Get content location
	///
	/// \param header_lines = String Ref
	///
	/// \return String Ref
	static CL_StringRef get_content_location(const CL_StringRef &header_lines);

	/// \brief Get content md5
	///
	/// \param header_lines = String Ref
	///
	/// \return String Ref
	static CL_StringRef get_content_md5(const CL_StringRef &header_lines);

	/// \brief Get content range
	///
	/// \param header_lines = String Ref
	///
	/// \return HTTPHeader Content Range
	static CL_HTTPHeaderContentRange get_content_range(const CL_StringRef &header_lines);

	/// \brief Get content type
	///
	/// \param header_lines = String Ref
	///
	/// \return HTTPHeader Content Type
	static CL_HTTPHeaderContentType get_content_type(const CL_StringRef &header_lines);

	/// \brief Get date
	///
	/// \param header_lines = String Ref
	///
	/// \return String Ref
	static CL_StringRef get_date(const CL_StringRef &header_lines);

	/// \brief Get etag
	///
	/// \param header_lines = String Ref
	///
	/// \return String Ref
	static CL_StringRef get_etag(const CL_StringRef &header_lines);

	static std::vector<CL_StringRef> get_expect(const CL_StringRef &header_lines);

	/// \brief Get expires
	///
	/// \param header_lines = String Ref
	///
	/// \return String Ref
	static CL_StringRef get_expires(const CL_StringRef &header_lines);

	/// \brief Get from
	///
	/// \param header_lines = String Ref
	///
	/// \return String Ref
	static CL_StringRef get_from(const CL_StringRef &header_lines);

	/// \brief Get host
	///
	/// \param header_lines = String Ref
	///
	/// \return String Ref
	static CL_StringRef get_host(const CL_StringRef &header_lines);

	static std::vector<CL_StringRef> get_if_match(const CL_StringRef &header_lines);

	/// \brief Get if modified since
	///
	/// \param header_lines = String Ref
	///
	/// \return String Ref
	static CL_StringRef get_if_modified_since(const CL_StringRef &header_lines);

	/// \brief Get if none match
	///
	/// \param header_lines = String Ref
	///
	/// \return String Ref
	static CL_StringRef get_if_none_match(const CL_StringRef &header_lines);

	/// \brief Get if range
	///
	/// \param header_lines = String Ref
	///
	/// \return String Ref
	static CL_StringRef get_if_range(const CL_StringRef &header_lines);

	/// \brief Get if unmodified since
	///
	/// \param header_lines = String Ref
	///
	/// \return String Ref
	static CL_StringRef get_if_unmodified_since(const CL_StringRef &header_lines);

	/// \brief Get last modified
	///
	/// \param header_lines = String Ref
	///
	/// \return String Ref
	static CL_StringRef get_last_modified(const CL_StringRef &header_lines);

	/// \brief Get location
	///
	/// \param header_lines = String Ref
	///
	/// \return String Ref
	static CL_StringRef get_location(const CL_StringRef &header_lines);

	/// \brief Get max forwards
	///
	/// \param header_lines = String Ref
	///
	/// \return String Ref
	static CL_StringRef get_max_forwards(const CL_StringRef &header_lines);

	static std::vector<CL_StringRef> get_pragma(const CL_StringRef &header_lines);

	/// \brief Get proxy authenticate
	///
	/// \param header_lines = String Ref
	///
	/// \return String Ref
	static CL_StringRef get_proxy_authenticate(const CL_StringRef &header_lines);

	/// \brief Get proxy authorization
	///
	/// \param header_lines = String Ref
	///
	/// \return String Ref
	static CL_StringRef get_proxy_authorization(const CL_StringRef &header_lines);

	static std::vector<CL_HTTPHeaderRange> get_range(const CL_StringRef &header_lines, CL_StringRef &out_range_type);

	/// \brief Get referer
	///
	/// \param header_lines = String Ref
	///
	/// \return String Ref
	static CL_StringRef get_referer(const CL_StringRef &header_lines);

	/// \brief Get retry after
	///
	/// \param header_lines = String Ref
	///
	/// \return String Ref
	static CL_StringRef get_retry_after(const CL_StringRef &header_lines);

	/// \brief Get server
	///
	/// \param header_lines = String Ref
	///
	/// \return String Ref
	static CL_StringRef get_server(const CL_StringRef &header_lines);

	static std::vector<CL_HTTPHeaderTE> get_te(const CL_StringRef &header_lines);

	static std::vector<CL_StringRef> get_trailer(const CL_StringRef &header_lines);

	static std::vector<CL_StringRef> get_transfer_encoding(const CL_StringRef &header_lines);

	/// \brief Get upgrade
	///
	/// \param header_lines = String Ref
	///
	/// \return String Ref
	static CL_StringRef get_upgrade(const CL_StringRef &header_lines);

	/// \brief Get user agent
	///
	/// \param header_lines = String Ref
	///
	/// \return String Ref
	static CL_StringRef get_user_agent(const CL_StringRef &header_lines);

	static std::vector<CL_StringRef> get_vary(const CL_StringRef &header_lines);

	static std::vector<CL_HTTPHeaderVia> get_via(const CL_StringRef &header_lines);

	/// \brief Get warning
	///
	/// \param header_lines = String Ref
	///
	/// \return HTTPHeader Warning
	static CL_HTTPHeaderWarning get_warning(const CL_StringRef &header_lines);

	/// \brief Get www authenticate
	///
	/// \param header_lines = String Ref
	///
	/// \return String Ref
	static CL_StringRef get_www_authenticate(const CL_StringRef &header_lines);
/// \}
};

/// \}
