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

/// \addtogroup clanNetwork_Socket clanNetwork Socket
/// \{


#pragma once


#include "../api_network.h"
#include "../../Core/System/sharedptr.h"

class CL_DataBuffer;
class CL_DNSPacket;
class CL_DNSResourceRecord_Impl;

/// \brief DNS resource record.
///
/// \xmlonly !group=Network/Socket! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_DNSResourceRecord
{
/// \name Construction
/// \{

public:
	CL_DNSResourceRecord();

	CL_DNSResourceRecord(const CL_DNSResourceRecord &other);

	~CL_DNSResourceRecord();


/// \}
/// \name Attributes
/// \{

public:
	CL_String get_name() const;

	CL_String get_type() const;

	CL_String get_class() const;

	int get_ttl() const;

	const CL_DNSPacket &get_packet() const;

	int get_record_offset() const;

	int get_rdata_offset() const;

	int get_rdata_length() const;

	CL_String get_cname_cname() const;

	int get_mx_preference() const;

	CL_String get_mx_exchange() const;

	CL_String get_ns_nsdname() const;

	CL_String get_ptr_ptrdname() const;

	CL_String get_soa_mname() const;

	CL_String get_soa_rname() const;

	unsigned int get_soa_serial() const;

	int get_soa_refresh() const;

	int get_soa_retry() const;

	int get_soa_expire() const;

	unsigned int get_soa_minimum() const;

	unsigned int get_a_address() const;

	CL_String get_a_address_str() const;

	unsigned int get_wks_address() const;

	CL_String get_wks_address_str() const;

	unsigned char get_wks_protocol() const;

	CL_DataBuffer get_wks_bit_map() const;


/// \}
/// \name Operation
/// \{

public:
	CL_DNSResourceRecord &operator =(const CL_DNSResourceRecord &other);

	void set_record(CL_DNSPacket packet, int record_offset);

	static int type_to_int(const CL_String &qtype);

	static CL_String type_from_int(int qtype);

	static CL_String type_description(const CL_String &qtype);

	static CL_String type_description(int qtype);

	static int class_to_int(const CL_String &qclass);

	static CL_String class_from_int(int qclass);

	static CL_String class_description(const CL_String &qclass);

	static CL_String class_description(int qclass);

	static int find_domain_name_end(const CL_DataBuffer &packet, int offset);

	static CL_String read_domain_name(const CL_DataBuffer &packet, int offset);


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_DNSResourceRecord_Impl> impl;
/// \}
};


/// \}
