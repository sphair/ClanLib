/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "API/Core/Text/utf8_reader.h"

namespace clan
{

class UTF8_Reader_Impl
{
public:
	UTF8_Reader_Impl(const std::string::value_type *text, std::string::size_type length);

	unsigned int get_char();
	std::string::size_type get_char_length();
	void prev();
	void next();
	void move_to_leadbyte();

	std::string::size_type current_position;
	std::string::size_type length;
	const unsigned char *data;

	static const char trailing_bytes_for_utf8[256];
	static const unsigned char bitmask_leadbyte_for_utf8[6];
};


UTF8_Reader::UTF8_Reader(const std::string::value_type *text, std::string::size_type length) : impl(new UTF8_Reader_Impl(text, length))
{
}

UTF8_Reader_Impl::UTF8_Reader_Impl(const std::string::value_type *text, std::string::size_type length)
: current_position(0), length(length), data((unsigned char *)text)
{
}

bool UTF8_Reader::is_end()
{
	return impl->current_position >= impl->length;
}

unsigned int UTF8_Reader::get_char()
{
	return impl->get_char();
}

std::string::size_type UTF8_Reader::get_char_length()
{
	return impl->get_char_length();
}

void UTF8_Reader::prev()
{
	impl->prev();
}

void UTF8_Reader::next()
{
	impl->next();
}

void UTF8_Reader::move_to_leadbyte()
{
	impl->move_to_leadbyte();
}

std::string::size_type UTF8_Reader::get_position()
{
	return impl->current_position;
}

void UTF8_Reader::set_position(std::string::size_type position)
{
	impl->current_position = position;
}

std::string::size_type UTF8_Reader_Impl::get_char_length()
{
	if (current_position < length)
	{
		int trailing_bytes = trailing_bytes_for_utf8[data[current_position]];
		if (current_position+1+trailing_bytes > length)
			return 1;

		for (std::string::size_type i = 0; i < trailing_bytes; i++)
		{
			if ((data[current_position+1+i] & 0xC0) != 0x80)
				return 1;
		}

		return 1+trailing_bytes;
	}
	else
	{
		return 0;
	}
}

void UTF8_Reader_Impl::prev()
{
	if (current_position > length)
		current_position = length;

	if (current_position > 0)
	{
		current_position--;
		move_to_leadbyte();
	}
}

void UTF8_Reader_Impl::next()
{
	current_position += get_char_length();
}

void UTF8_Reader_Impl::move_to_leadbyte()
{
	if (current_position < length)
	{
		int lead_position = current_position;

		while (lead_position > 0 && (data[lead_position] & 0xC0) == 0x80)
			lead_position--;

		int trailing_bytes = trailing_bytes_for_utf8[data[lead_position]];
		if (lead_position + trailing_bytes >= current_position)
			current_position = lead_position;
	}
}

unsigned int UTF8_Reader_Impl::get_char()
{
	if (current_position >= length)
		return 0;

	int trailing_bytes = trailing_bytes_for_utf8[data[current_position]];
	if (trailing_bytes == 0 && (data[current_position] & 0x80) == 0x80)
		return '?';

	if (current_position+1+trailing_bytes > length)
	{
		return '?';
	}
	else
	{
		unsigned int ucs4 = (data[current_position] & bitmask_leadbyte_for_utf8[trailing_bytes]);
		for (std::string::size_type i = 0; i < trailing_bytes; i++)
		{
			if ((data[current_position+1+i] & 0xC0) == 0x80)
				ucs4 = (ucs4 << 6) + (data[current_position+1+i] & 0x3f);
			else
				return '?';
		}

		// To do: verify that the ucs4 value is in the range for the trailing_bytes specified in the lead byte.

		return ucs4;
	}
}

const char UTF8_Reader_Impl::trailing_bytes_for_utf8[256] =
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
};

const unsigned char UTF8_Reader_Impl::bitmask_leadbyte_for_utf8[6] =
{
	0x7f,
	0x1f,
	0x0f,
	0x07,
	0x03,
	0x01
};

}
