/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
**    Mark Page
*/

#pragma once

#include "x509.h"
#include "API/Core/System/datetime.h"

namespace clan
{
	class ASN1;

	typedef struct
	{
		std::string id_at_commonName;
		std::string id_at_surname;
		std::string id_at_countryName;
		std::string id_at_localityName;
		std::string id_at_stateOrProvinceName;
		std::string id_at_organizationName;
		std::string id_at_organizationalUnitName;
		std::string id_at_title;
		std::string id_at_name;
		std::string id_at_givenName;
		std::string id_at_initials;
		std::string id_at_generationQualifier;
		std::string id_at_dnQualifier;
		std::string id_at_emailAddress;
		std::string id_at_domainComponent;
	} X509_BasicInfo;

	class X509_Impl
	{
	public:
		X509_Impl();

		enum X509_Version
		{
			x509_cert_v1 = 0,
			x509_cert_v2 = 1,
			x509_cert_v3 = 2
		};

		void parse(const unsigned char *base_ptr, unsigned int length);
		void get_rsa_public_key(DataBuffer &output_exponent, DataBuffer &output_modulus) const;

	private:
		void set_string(const ASN1 &input, std::string &output);
		void parse_tbs_certificate(ASN1 &asn1);
		void parse_name(ASN1 &tbs, X509_BasicInfo &output);
		void parse_subject_public_key(ASN1 &tbs);
		void parse_validity(ASN1 &tbs);
		void throw_invalid() const;

		X509_Version version;
		std::vector<unsigned char> serial_number;
		std::vector<uint32_t> signature;

		std::vector<uint32_t> public_key_algorithm_identifier;
		std::vector<unsigned char> public_key;

		X509_BasicInfo subject;
		X509_BasicInfo issuer;
		DateTime valid_not_before;
		DateTime valid_not_after;
	};
}
