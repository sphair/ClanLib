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

/// \addtogroup clanNetwork_Socket clanNetwork Socket
/// \{

#pragma once

#include "../api_network.h"
#include "../../Core/System/sharedptr.h"
#include "../../Core/Text/string_types.h"

class CL_DataBuffer;
class CL_DNSResourceRecord;
class CL_DNSPacket_Impl;

/// \brief DNS packet.
///
/// \xmlonly !group=Network/Socket! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_DNSPacket
{
/// \name Construction
/// \{

public:
	CL_DNSPacket();

	/// \brief Constructs a DNSPacket
	///
	/// \param data = Data Buffer
	CL_DNSPacket(const CL_DataBuffer &data);

	CL_DNSPacket(
		int query_id,
		int opcode,
		bool recursion_desired,
		const CL_String &question_name,
		int question_type,
		int question_class);

	~CL_DNSPacket();

/// \}
/// \name Attributes
/// \{

public:
	const CL_DataBuffer &get_data() const;

	unsigned short get_query_id() const;

	/// \brief Is Query
	///
	/// \return true = query
	bool is_query() const;

	/// \brief Is Response
	///
	/// \return true = response
	bool is_response() const;

	enum Opcode
	{
		opcode_query          = 0,
		opcode_inverse_query  = 1,
		opcode_status         = 2
	};

	/// \brief Get Opcode
	///
	/// \return opcode
	int get_opcode() const;

	/// \brief Is Authoriative answer
	///
	/// \return true = authoriative_answer
	bool is_authoriative_answer() const;

	/// \brief Is Truncated
	///
	/// \return true = truncated
	bool is_truncated() const;

	/// \brief Is Recursion desired
	///
	/// \return true = recursion_desired
	bool is_recursion_desired() const;

	/// \brief Is Recursion available
	///
	/// \return true = recursion_available
	bool is_recursion_available() const;

	enum ResponseCode
	{
		response_ok               = 0,
		response_format_error     = 1,
		response_server_failure   = 2,
		response_name_error       = 3,
		response_not_implemented  = 4,
		response_refused          = 5
	};

	/// \brief Get Response code
	///
	/// \return response_code
	int get_response_code() const;

	/// \brief Get Question count
	///
	/// \return question_count
	int get_question_count() const;

	/// \brief Get Answer count
	///
	/// \return answer_count
	int get_answer_count() const;

	/// \brief Get Nameserver count
	///
	/// \return nameserver_count
	int get_nameserver_count() const;

	/// \brief Get Additional count
	///
	/// \return additional_count
	int get_additional_count() const;

	/// \brief Get question name
	///
	/// \param index = value
	///
	/// \return String
	CL_String get_question_name(int index) const;

	/// \brief Get question type
	///
	/// \param index = value
	///
	/// \return int
	int get_question_type(int index) const;

	/// \brief Get question class
	///
	/// \param index = value
	///
	/// \return int
	int get_question_class(int index) const;

	/// \brief Get answer
	///
	/// \param index = value
	///
	/// \return DNSResource Record
	CL_DNSResourceRecord get_answer(int index) const;

	/// \brief Get nameserver
	///
	/// \param index = value
	///
	/// \return DNSResource Record
	CL_DNSResourceRecord get_nameserver(int index) const;

	/// \brief Get additional
	///
	/// \param index = value
	///
	/// \return DNSResource Record
	CL_DNSResourceRecord get_additional(int index) const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Set data
	///
	/// \param data = Data Buffer
	void set_data(const CL_DataBuffer &data);

	/// \brief Set query id
	///
	/// \param query_id = short
	void set_query_id(unsigned short query_id);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_DNSPacket_Impl> impl;
/// \}
};

/// \}
