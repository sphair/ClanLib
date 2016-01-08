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

#include "API/Core/System/cl_platform.h"
#include "API/Core/System/datetime.h"
#include <memory>

namespace clan
{
	/// \brief ASN1 class.
	class ASN1
	{
	public:
		ASN1(const unsigned char *base_ptr, unsigned int length);

		enum ASN1_Class
		{
			class_universal = 0,
			class_application = 1,
			class_context_specific = 2,
			class_private = 3
		};

		enum ASN1_Tag
		{
			tag_eoc = 0x00,
			tag_boolean = 0x01,
			tag_integer = 0x02,
			tag_bitstring = 0x03,
			tag_octetstring = 0x04,
			tag_null = 0x05,
			tag_oid = 0x06,
			tag_object_descriptor = 0x07,
			tag_external = 0x08,
			tag_real = 0x09,
			tag_enumerated = 0x0a,
			tag_utf8string = 0x0c,
			tag_relative_oid = 0x0d,
			tag_sequence = 0x10,
			tag_set = 0x11,
			tag_numericstring = 0x12,
			tag_printablestring = 0x13,
			tag_t61string = 0x14,
			tag_videotexstring = 0x15,
			tag_ia5string = 0x16,
			tag_utctime = 0x17,
			tag_generalizedtime = 0x18,
			tag_graphicstring = 0x19,
			tag_visiblestring = 0x1a,
			tag_generalstring = 0x1b,
			tag_universalstring = 0x1c,
			tag_bmpstring = 0x1d
		};

		void get_object_identifier(std::vector<uint32_t> &output);

		ASN1 get_next();
		ASN1 get_next_universal_set();
		ASN1 get_next_universal_sequence();
		ASN1 get_next_universal_oid();
		ASN1 get_next_universal_printablestring();

		uint32_t get_next_universal_integer_ubyte32();
		void get_next_universal_bitstring(std::vector<unsigned char> &output);

		void get_universal_integer(std::vector<unsigned char> &output) const;
		void get_next_universal_integer(std::vector<unsigned char> &output);

		bool is_universal_sequence() const;
		bool is_context_specific() const;
		bool is_printablestring() const;

		bool is_last() { return offset == length; }

		std::string get_string() const;

		DateTime get_next_universal_time();

	private:
		uint8_t identifier;
		ASN1_Class type_class;
		bool is_constructed;	// Else is primitive
		ASN1_Tag tag;
		unsigned int length;
		unsigned int offset;
		const unsigned char *data_ptr;

		void read(const unsigned char *base_ptr, unsigned int length);
		void throw_eof() const;
		void throw_invalid() const;
	};
}
