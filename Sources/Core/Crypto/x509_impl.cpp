/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#include "Core/precomp.h"
#include "x509_impl.h"
#include "asn1.h"
#include "API/Core/Crypto/secret.h"
#include "API/Core/System/databuffer.h"

namespace clan
{
	X509_Impl::X509_Impl()
	{
	}

	void X509_Impl::parse(const unsigned char *base_ptr, unsigned int length)
	{
		ASN1 asn1(base_ptr, length);
		if (!asn1.is_universal_sequence())
			throw_invalid();

		parse_tbs_certificate(asn1);
	}

	void X509_Impl::parse_tbs_certificate(ASN1 &asn1)
	{
		ASN1 tbs = asn1.get_next_universal_sequence();

		// version [0]  EXPLICIT Version DEFAULT v1
		// Version  ::=  INTEGER  {  v1(0), v2(1), v3(2)  }

		ASN1 item = tbs.get_next();

		if (item.is_context_specific())
		{
			uint32_t version_value = item.get_next_universal_integer_ubyte32();

			version = static_cast<X509_Version> (version_value);
			switch (version)
			{
			case x509_cert_v1:
			case x509_cert_v2:
			case x509_cert_v3:
				break;
			default:
				throw Exception("X509 - Unsupported version");

			}
			item = tbs.get_next();
		}
		else
		{
			version = x509_cert_v1;
		}

		// serialNumber (CertificateSerialNumber)
		item.get_universal_integer(serial_number);

		// signature (AlgorithmIdentifier)
		item = tbs.get_next_universal_sequence();
		item.get_object_identifier(signature);

		// issuer (Name)
		parse_name(tbs, issuer);

		// validity (Validity)
		parse_validity(tbs);

		// subject (Name)
		parse_name(tbs, subject);

		// subjectPublicKeyInfo (SubjectPublicKeyInfo)
		parse_subject_public_key(tbs);

		// *** NOW I HAVE THE PUBLIC KEY, AT THE MOMENT, THE REST OF THE CERT DECODING IS IGNORED! ***
		// *** ClanLib Developers need to decide if it is required ***
		// *** But .... of course, we need to validate the certificate ...
	}

	void X509_Impl::parse_subject_public_key(ASN1 &tbs)
	{
		//	SubjectPublicKeyInfo ::= SEQUENCE {
		//		algorithm AlgorithmIdentifier{{SupportedAlgorithms}},
		//		subjectPublicKey BIT STRING }

		ASN1 subject_public_key = tbs.get_next_universal_sequence();
		ASN1 algorithm = subject_public_key.get_next_universal_sequence();

		ASN1 tag_oid = algorithm.get_next_universal_oid();

		tag_oid.get_object_identifier(public_key_algorithm_identifier);

		subject_public_key.get_next_universal_bitstring(public_key);
	}

	void X509_Impl::parse_validity(ASN1 &tbs)
	{
		//	Validity ::= SEQUENCE {
		//		notBefore Time,
		//		notAfter Time }

		//	Time ::= CHOICE {
		//		utcTime UTCTime,
		//		generalizedTime GeneralizedTime }

		// – If the 2-digit value is 00 through 49 inclusive, the value shall have 2000 added to it.
		// – If the 2-digit value is 50 through 99 inclusive, the value shall have 1900 added to it.

		// UTCTime: YYMMDDHHMMSSZ
		// GeneralizedTime: YYYYMMDDHHMMSSZ
		ASN1 item = tbs.get_next_universal_sequence();

		valid_not_before = item.get_next_universal_time();
		valid_not_after = item.get_next_universal_time();
	}

	void X509_Impl::parse_name(ASN1 &tbs, X509_BasicInfo &output)
	{
		// Reset the strings first
		output.id_at_commonName = std::string();
		output.id_at_surname = std::string();
		output.id_at_countryName = std::string();
		output.id_at_localityName = std::string();
		output.id_at_stateOrProvinceName = std::string();
		output.id_at_organizationName = std::string();
		output.id_at_organizationalUnitName = std::string();
		output.id_at_title = std::string();
		output.id_at_name = std::string();
		output.id_at_givenName = std::string();
		output.id_at_initials = std::string();
		output.id_at_generationQualifier = std::string();
		output.id_at_dnQualifier = std::string();
		output.id_at_emailAddress = std::string();
		output.id_at_domainComponent = std::string();

		ASN1 item = tbs.get_next_universal_sequence();

		ASN1 tag_set = item.get_next_universal_set();

		while (true)
		{
			ASN1 tag_sequence = tag_set.get_next_universal_sequence();

			ASN1 tag_oid = tag_sequence.get_next_universal_oid();

			std::vector<uint32_t> oid;
			tag_oid.get_object_identifier(oid);

			ASN1 tag_string = tag_sequence.get_next();

			//id-at   OBJECT IDENTIFIER       ::=     {joint-iso-ccitt(2) ds(5) 4}
			if (oid.size() == 4)
			{
				if (oid[0] == 2 && oid[1] == 5 && oid[2] == 4)
				{
					switch (oid[3])
					{
					case 3: set_string(tag_string, output.id_at_commonName); break;
					case 4: set_string(tag_string, output.id_at_surname); break;
					case 6: set_string(tag_string, output.id_at_countryName); break;
					case 7: set_string(tag_string, output.id_at_localityName); break;
					case 8: set_string(tag_string, output.id_at_stateOrProvinceName); break;
					case 10: set_string(tag_string, output.id_at_organizationName); break;
					case 11: set_string(tag_string, output.id_at_organizationalUnitName); break;
					case 12: set_string(tag_string, output.id_at_title); break;
					case 41: set_string(tag_string, output.id_at_name); break;
					case 42: set_string(tag_string, output.id_at_givenName); break;
					case 43: set_string(tag_string, output.id_at_initials); break;
					case 44: set_string(tag_string, output.id_at_generationQualifier); break;
					case 46: set_string(tag_string, output.id_at_dnQualifier); break;
					}
				}
			}
			else if (oid.size() == 7)
			{
				if (oid[0] == 1 &&	//{iso(1) member-body(2) us(840) rsadsi(113549) pkcs(1) pkcs-9(9) emailAddress(1)}
					oid[1] == 2 &&
					oid[2] == 840 &&
					oid[3] == 113549 &&
					oid[4] == 1 &&
					oid[5] == 9 &&
					oid[6] == 1)
				{
					set_string(tag_string, output.id_at_emailAddress); break;

				}
				else if (oid[0] == 0 &&	// itu-t(0) data(9) pss(2342) ucl(19200300) pilot(100) pilotAttributeType(1) domainComponent(25)}
					oid[1] == 9 &&
					oid[2] == 2342 &&
					oid[3] == 19200300 &&
					oid[4] == 100 &&
					oid[5] == 1 &&
					oid[6] == 25)
				{
					set_string(tag_string, output.id_at_domainComponent); break;
				}
			}

			if (item.is_last())
				break;

			tag_set = item.get_next_universal_set();	// Update for next loop iteration
		}
	}

	void X509_Impl::set_string(const ASN1 &input, std::string &output)
	{
		output = input.get_string();
	}

	void X509_Impl::throw_invalid() const
	{
		throw Exception("X509 - Invalid certificate");
	}

	void X509_Impl::get_rsa_public_key(DataBuffer &output_exponent, DataBuffer &output_modulus) const
	{
		if (public_key.empty())
			throw Exception("Public key not set");

		ASN1 asn1(&public_key[0], public_key.size());
		if (!asn1.is_universal_sequence())
			throw_invalid();

		std::vector<unsigned char> output;
		asn1.get_next_universal_integer(output);
		if (output.empty())
			throw Exception("Invalid modulus");
		output_modulus = DataBuffer(output.size());
		memcpy(output_modulus.get_data(), &output[0], output.size());

		asn1.get_next_universal_integer(output);
		if (output.empty())
			throw Exception("Invalid exponent");
		output_exponent = DataBuffer(output.size());
		memcpy(output_exponent.get_data(), &output[0], output.size());
	}
}
