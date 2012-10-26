/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
#include <memory>

namespace clan
{

class DataBuffer;
class DNSPacket;
class DNSResourceRecord_Impl;

/// \brief DNS resource record.
///
/// \xmlonly !group=Network/Socket! !header=network.h! \endxmlonly
class CL_API_NETWORK DNSResourceRecord
{
/// \name Construction
/// \{

public:
	DNSResourceRecord();

	~DNSResourceRecord();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Get Name
	///
	/// \return name
	std::string get_name() const;

	/// \brief Get Type
	///
	/// \return type
	std::string get_type() const;

	/// \brief Get Class
	///
	/// \return class
	std::string get_class() const;

	/// \brief Get Ttl
	///
	/// \return ttl
	int get_ttl() const;

	const DNSPacket &get_packet() const;

	/// \brief Get Record offset
	///
	/// \return record_offset
	int get_record_offset() const;

	/// \brief Get Rdata offset
	///
	/// \return rdata_offset
	int get_rdata_offset() const;

	/// \brief Get Rdata length
	///
	/// \return rdata_length
	int get_rdata_length() const;

	/// \brief Get Cname cname
	///
	/// \return cname_cname
	std::string get_cname_cname() const;

	/// \brief Get Mx preference
	///
	/// \return mx_preference
	int get_mx_preference() const;

	/// \brief Get Mx exchange
	///
	/// \return mx_exchange
	std::string get_mx_exchange() const;

	/// \brief Get Ns nsdname
	///
	/// \return ns_nsdname
	std::string get_ns_nsdname() const;

	/// \brief Get Ptr ptrdname
	///
	/// \return ptr_ptrdname
	std::string get_ptr_ptrdname() const;

	/// \brief Get Soa mname
	///
	/// \return soa_mname
	std::string get_soa_mname() const;

	/// \brief Get Soa rname
	///
	/// \return soa_rname
	std::string get_soa_rname() const;

	unsigned int get_soa_serial() const;

	/// \brief Get Soa refresh
	///
	/// \return soa_refresh
	int get_soa_refresh() const;

	/// \brief Get Soa retry
	///
	/// \return soa_retry
	int get_soa_retry() const;

	/// \brief Get Soa expire
	///
	/// \return soa_expire
	int get_soa_expire() const;

	unsigned int get_soa_minimum() const;

	unsigned int get_a_address() const;

	/// \brief Get A address str
	///
	/// \return a_address_str
	std::string get_a_address_str() const;

	unsigned int get_wks_address() const;

	/// \brief Get Wks address str
	///
	/// \return wks_address_str
	std::string get_wks_address_str() const;

	unsigned char get_wks_protocol() const;

	/// \brief Get Wks bit map
	///
	/// \return wks_bit_map
	DataBuffer get_wks_bit_map() const;

/// \}
/// \name Operation
/// \{

public:

	/// \brief Set record
	///
	/// \param packet = DNSPacket
	/// \param record_offset = value
	void set_record(DNSPacket packet, int record_offset);

	/// \brief Type to int
	///
	/// \param qtype = String
	///
	/// \return int
	static int type_to_int(const std::string &qtype);

	/// \brief Type from int
	///
	/// \param qtype = value
	///
	/// \return String
	static std::string type_from_int(int qtype);

	/// \brief Type description
	///
	/// \param qtype = String
	///
	/// \return String
	static std::string type_description(const std::string &qtype);

	/// \brief Type description
	///
	/// \param qtype = value
	///
	/// \return String
	static std::string type_description(int qtype);

	/// \brief Class to int
	///
	/// \param qclass = String
	///
	/// \return int
	static int class_to_int(const std::string &qclass);

	/// \brief Class from int
	///
	/// \param qclass = value
	///
	/// \return String
	static std::string class_from_int(int qclass);

	/// \brief Class description
	///
	/// \param qclass = String
	///
	/// \return String
	static std::string class_description(const std::string &qclass);

	/// \brief Class description
	///
	/// \param qclass = value
	///
	/// \return String
	static std::string class_description(int qclass);

	/// \brief Find domain name end
	///
	/// \param packet = Data Buffer
	/// \param offset = value
	///
	/// \return int
	static int find_domain_name_end(const DataBuffer &packet, int offset);

	/// \brief Read domain name
	///
	/// \param packet = Data Buffer
	/// \param offset = value
	///
	/// \return String
	static std::string read_domain_name(const DataBuffer &packet, int offset);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<DNSResourceRecord_Impl> impl;
/// \}
};

}

/// \}
