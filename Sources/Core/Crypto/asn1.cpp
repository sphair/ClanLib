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
#include "asn1.h"

namespace clan
{
	ASN1::ASN1(const unsigned char *base_ptr, unsigned int length)
	{
		read(base_ptr, length);
	}

	void ASN1::get_object_identifier(std::vector<uint32_t> &output)
	{
		output.clear();

		// 8.19 Encoding of an object identifier value (X.690-0207.pdf)

		// Calculate the output size
		const unsigned char *read_ptr = data_ptr;
		const unsigned char *end_ptr = data_ptr + length;
		unsigned int output_length = 1;	// First iteration contains 2 components
		while (read_ptr < end_ptr)
		{
			unsigned char temp;
			do
			{
				while (read_ptr >= end_ptr)
					break;
				temp = *(read_ptr++);
			} while (temp & 0x80);
			output_length++;
		}

		output.reserve(output_length);
		bool first_time = true;

		read_ptr = data_ptr;
		end_ptr = data_ptr + length;
		while (read_ptr < end_ptr)
		{
			uint32_t value = 0;
			unsigned char temp;

			do
			{
				while (read_ptr >= end_ptr)
					break;
				temp = *(read_ptr++);
				value = (value << 7) | (temp & 0x7f);
			} while (temp & 0x80);

			if (first_time)
			{
				first_time = false;
				// "The numerical value of the first subidentifier is derived from the values of the first two object identifier components in the object identifier value being encoded, using the formula:"
				// "(X*40) + Y"
				uint32_t out = value / 40;
				output.push_back(out);
				output.push_back(value - (out * 40));
			}
			else
			{
				output.push_back(value);
			}

		}

	}

	ASN1 ASN1::get_next()
	{
		ASN1 tag(data_ptr + offset, length - offset);
		offset = (tag.data_ptr - data_ptr) + tag.length;
		return tag;
	}

	ASN1 ASN1::get_next_universal_set()
	{
		ASN1 tag = get_next();
		if ((tag.type_class != ASN1::class_universal)
			|| (tag.tag != ASN1::tag_set))
			throw_invalid();
		return tag;
	}

	ASN1 ASN1::get_next_universal_sequence()
	{
		ASN1 tag = get_next();
		if ((tag.type_class != ASN1::class_universal)
			|| (tag.tag != ASN1::tag_sequence))
			throw_invalid();
		return tag;
	}

	ASN1 ASN1::get_next_universal_oid()
	{
		ASN1 tag = get_next();
		if ((tag.type_class != ASN1::class_universal)
			|| (tag.tag != ASN1::tag_oid))
			throw_invalid();
		return tag;
	}

	ASN1 ASN1::get_next_universal_printablestring()
	{
		ASN1 tag = get_next();
		if ((tag.type_class != ASN1::class_universal)
			|| (tag.tag != ASN1::tag_printablestring))
			throw_invalid();
		return tag;
	}

	bool ASN1::is_universal_sequence() const
	{
		if ((type_class != ASN1::class_universal)
			|| (tag != ASN1::tag_sequence))
			return false;
		return true;

	}

	bool ASN1::is_printablestring() const
	{
		if ((type_class != ASN1::class_universal)
			|| (tag != ASN1::tag_printablestring))
			return false;
		return true;

	}

	bool ASN1::is_context_specific() const
	{
		if (type_class == ASN1::class_context_specific)
			return true;
		return false;
	}

	uint32_t ASN1::get_next_universal_integer_ubyte32()
	{
		ASN1 tag = get_next();
		if ((tag.type_class != ASN1::class_universal)
			|| (tag.tag != ASN1::tag_integer))
			throw_invalid();

		uint32_t value = 0;
		for (unsigned int cnt = 0; cnt < tag.length; cnt++)
		{
			value = (value << 8) | tag.data_ptr[cnt];
		}
		return value;
	}

	void ASN1::get_next_universal_bitstring(std::vector<unsigned char> &output)
	{
		ASN1 tag = get_next();
		if ((tag.type_class != ASN1::class_universal)
			|| (tag.tag != ASN1::tag_bitstring))
			throw_invalid();

		output.clear();

		if (tag.length < 1)
			throw_invalid();

		unsigned int num_unused_bits = tag.data_ptr[0];
		if (num_unused_bits > 7)
			throw_invalid();
		int array_length = tag.length - 1;
		if (array_length)
		{
			output.resize(array_length);
			memcpy(&output[0], &tag.data_ptr[1], array_length);
		}

	}

	DateTime ASN1::get_next_universal_time()
	{
		ASN1 tag = get_next();
		if (tag.type_class != ASN1::class_universal)
			throw_invalid();

		int century = 0;
		int year;
		int month;
		int day;
		int hour;
		int minute;
		int seconds;

		const unsigned char *dptr = tag.data_ptr;

#define EXTRACT_THE_DATE ((dptr[0]-'0') * 10 + (dptr[1]-'0')); dptr+=2;

		if (tag.tag == tag_utctime)
		{
			if (tag.length != 13)
				throw_invalid();
			if (tag.data_ptr[12] != 'Z')
				throw_invalid();

		}
		else if (tag.tag == tag_generalizedtime)
		{
			if (tag.length != 15)
				throw_invalid();
			if (tag.data_ptr[14] != 'Z')
				throw_invalid();

			century = EXTRACT_THE_DATE
		}
		else
		{
			throw Exception("ASN1 - Invalid Time");
		}

		year = EXTRACT_THE_DATE
			month = EXTRACT_THE_DATE
			day = EXTRACT_THE_DATE
			hour = EXTRACT_THE_DATE
			minute = EXTRACT_THE_DATE
			seconds = EXTRACT_THE_DATE

			if (tag.tag == tag_utctime)
			{
				if (year < 50)
				{
					century = 20;
				}
				else
				{
					century = 19;
				}
			}

		return DateTime(century * 100 + year, month, day, hour, minute, seconds);

	}

	void ASN1::get_universal_integer(std::vector<unsigned char> &output) const
	{
		if ((type_class != ASN1::class_universal)
			|| (tag != ASN1::tag_integer))
			throw_invalid();

		output.clear();
		if (length > 0)
		{
			output.resize(length);
			memcpy(&output[0], data_ptr, length);
		}
	}

	void ASN1::get_next_universal_integer(std::vector<unsigned char> &output)
	{
		ASN1 tag = get_next();
		if ((tag.type_class != ASN1::class_universal)
			|| (tag.tag != ASN1::tag_integer))
			throw_invalid();

		output.clear();
		if (tag.length > 0)
		{
			output.resize(tag.length);
			memcpy(&output[0], tag.data_ptr, tag.length);
		}
	}

	std::string ASN1::get_string() const
	{
		if (type_class != ASN1::class_universal)
			throw_invalid();

		if (!((tag == ASN1::tag_printablestring) || (tag == ASN1::tag_t61string)))
			throw_invalid();

		const unsigned char *read_ptr = data_ptr;

		// Validate input text does not contain NUL's
		for (int cnt = 0; cnt < length; cnt++)
		{
			if ((*(read_ptr++)) == 0)
				throw Exception("ASN1 - Input text contains nul's");
		}

		return std::string((char *)data_ptr, length);
	}

	void ASN1::read(const unsigned char *base_ptr, unsigned int x_length)
	{
		const unsigned char *read_ptr = base_ptr;
		const unsigned char *end_ptr = base_ptr + x_length;

		if (read_ptr >= end_ptr)
			throw_eof();

		identifier = *(read_ptr++);

		type_class = static_cast<ASN1::ASN1_Class> (identifier >> 6);
		is_constructed = (identifier & (1 << 5)) != 0;

		int read_tag = identifier & 0x1f;

		if (read_tag == 0x1f)	// Long form
		{
			if (read_ptr >= end_ptr)
				throw_eof();

			uint8_t value = *(read_ptr++);
			read_tag = (read_tag << 7) | (value & 0x4f);
		}

		tag = static_cast<ASN1::ASN1_Tag> (read_tag);

		if (read_ptr >= end_ptr)
			throw_eof();

		uint8_t value = *(read_ptr++);
		if (value & 0x80)
		{
			// Note - The length value 0x80, used only in constructed form types, is defined as "indefinite length".
			if (value == 0xff)
				throw Exception("ASN1 - Unsupported length extension");

			value = value & 0x7f;
			if (value > 4)
				throw Exception("ASN1 - We only support 32bit lengths");
			length = 0;
			for (; value > 0; value--)
			{
				if (read_ptr >= end_ptr)
					throw_eof();

				length = (length << 8) | *(read_ptr++);
			}
		}
		else
		{
			length = value;
		}

		if ((read_ptr + length) > end_ptr)
			throw_eof();

		data_ptr = read_ptr;

		offset = 0;

	}

	void ASN1::throw_eof() const
	{
		throw Exception("ASN1 unexpected eof");
	}

	void ASN1::throw_invalid() const
	{
		throw Exception("ASN1 - Invalid");
	}
}
