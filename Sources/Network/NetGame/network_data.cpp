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
**    Magnus Norddahl
*/

#include "Network/precomp.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/memory_device.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Zip/zlib_compression.h"
#include "network_data.h"

namespace clan
{
	NetGameEvent NetGameNetworkData::receive_data(const void *data, int size, int &out_bytes_consumed)
	{
		if (size >= 2)
		{
			int payload_size = *static_cast<const unsigned short *>(data);
			if (payload_size > packet_limit)
				throw Exception("Incoming message too big");

			if (size >= 2 + payload_size)
			{
				out_bytes_consumed = 2 + payload_size;
				DataBuffer buffer(static_cast<const char*>(data)+2, payload_size);
				return decode_event(buffer);
			}
		}

		out_bytes_consumed = 0;
		return NetGameEvent(std::string());
	}

	DataBuffer NetGameNetworkData::send_data(const NetGameEvent &e)
	{
		DataBuffer buffer = encode_event(e);
		if (buffer.get_size() > packet_limit + 2)
			throw Exception("Outgoing message too big");
		return buffer;
	}

	NetGameEvent NetGameNetworkData::decode_event(const DataBuffer &data)
	{
		const unsigned char *d = data.get_data<unsigned char>();
		unsigned int length = data.get_size();
		if (length < 3)
			throw Exception("Invalid network data");

		unsigned int name_length = *reinterpret_cast<const unsigned short*>(d);
		if (length < 2 + name_length + 1)
			throw Exception("Invalid network data");
		std::string name = std::string(reinterpret_cast<const char*>(d + 2), name_length);

		NetGameEvent e(name);
		unsigned int pos = 2 + name_length;
		while (true)
		{
			if (pos >= length)
				throw Exception("Invalid network data");
			unsigned char type = d[pos++];
			if (type == 0)
				break;
			e.add_argument(decode_value(type, d, length, pos));
		}
		return e;
	}

	NetGameEventValue NetGameNetworkData::decode_value(unsigned char type, const unsigned char *d, unsigned int length, unsigned int &pos)
	{
		switch (type)
		{
		case 1: // null
			return NetGameEventValue(NetGameEventValue::Type::null);
		case 2: // uint
		{
			if (pos + 4 > length)
				throw Exception("Invalid network data");
			unsigned int v = *reinterpret_cast<const unsigned int*>(d + pos);
			pos += 4;
			return NetGameEventValue(v);
		}
		case 3: // int
		{
			if (pos + 4 > length)
				throw Exception("Invalid network data");
			int v = *reinterpret_cast<const int*>(d + pos);
			pos += 4;
			return NetGameEventValue(v);
		}
		case 4: // number
		{
			if (pos + 4 > length)
				throw Exception("Invalid network data");
			float v = *reinterpret_cast<const float*>(d + pos);
			pos += 4;
			return NetGameEventValue(v);
		}
		case 5: // false boolean
			return NetGameEventValue(false);
		case 6: // true boolean
			return NetGameEventValue(true);
		case 7: // string
		{
			if (pos + 2 > length)
				throw Exception("Invalid network data");
			unsigned short name_length = *reinterpret_cast<const unsigned short*>(d + pos);
			pos += 2;
			if (pos + name_length > length)
				throw Exception("Invalid network data");
			std::string value(reinterpret_cast<const char*>(d + pos), name_length);
			pos += name_length;
			return NetGameEventValue(value);
		}
		case 8: // complex
		{
			NetGameEventValue value(NetGameEventValue::Type::complex);
			while (true)
			{
				if (pos >= length)
					throw Exception("Invalid network data");
				unsigned char type = d[pos++];
				if (type == 0)
					break;
				value.add_member(decode_value(type, d, length, pos));
			}
			return value;
		}
		case 9: // uchar
		{
			if (pos + 1 > length)
				throw Exception("Invalid network data");
			unsigned char v = *reinterpret_cast<const unsigned char*>(d + pos);
			pos += 1;
			return NetGameEventValue(v);
		}
		case 10: // char
		{
			if (pos + 1 > length)
				throw Exception("Invalid network data");
			char v = *reinterpret_cast<const char*>(d + pos);
			pos += 1;
			return NetGameEventValue(v);
		}
		case 11: // binary
		{
			if (pos + 2 > length)
				throw Exception("Invalid network data");
			unsigned short binary_length = *reinterpret_cast<const unsigned short*>(d + pos);
			pos += 2;
			if (pos + binary_length > length)
				throw Exception("Invalid network data");
			DataBuffer value(reinterpret_cast<const char*>(d + pos), binary_length);
			pos += binary_length;
			return NetGameEventValue(value);
		}
		default:
			throw Exception("Invalid network data");
		}
	}

	DataBuffer NetGameNetworkData::encode_event(const NetGameEvent &e)
	{
		unsigned int length = 3 + e.get_name().length();
		for (unsigned int i = 0; i < e.get_argument_count(); i++)
			length += get_encoded_length(e.get_argument(i));

		DataBuffer data(length + 2);

		*data.get_data<unsigned short>() = length;

		unsigned char *d = data.get_data<unsigned char>() + 2;

		// Write name (2 + name length)
		unsigned int name_length = e.get_name().length();
		*reinterpret_cast<unsigned short*>(d) = name_length;
		d += 2;
		memcpy(d, e.get_name().data(), name_length);
		d += name_length;

		for (unsigned int i = 0; i < e.get_argument_count(); i++)
			d += encode_value(d, e.get_argument(i));

		// Write end marker
		*d = 0;

		return data;
	}

	unsigned int NetGameNetworkData::encode_value(unsigned char *d, const NetGameEventValue &value)
	{
		switch (value.get_type())
		{
		case NetGameEventValue::Type::null:
			*d = 1;
			return 1;
		case NetGameEventValue::Type::uinteger:
			*d = 2;
			*reinterpret_cast<unsigned int*>(d + 1) = value.get_uinteger();
			return 5;
		case NetGameEventValue::Type::integer:
			*d = 3;
			*reinterpret_cast<int*>(d + 1) = value.get_integer();
			return 5;
		case NetGameEventValue::Type::number:
			*d = 4;
			*reinterpret_cast<float*>(d + 1) = value.get_number();
			return 5;
		case NetGameEventValue::Type::boolean:
			*d = value.get_boolean() ? 6 : 5;
			return 1;
		case NetGameEventValue::Type::string:
		{
			std::string s = value.get_string();
			*d = 7;
			*reinterpret_cast<unsigned short*>(d + 1) = s.length();
			memcpy(d + 3, s.data(), s.length());
			return 3 + s.length();
		}
		case NetGameEventValue::Type::complex:
		{
			d[0] = 8;
			unsigned l = 1;
			for (unsigned int i = 0; i < value.get_member_count(); i++)
				l += encode_value(d + l, value.get_member(i));
			d[l] = 0;
			l++;
			return l;
		}
		case NetGameEventValue::Type::ucharacter:
			*d = 9;
			*reinterpret_cast<unsigned char*>(d + 1) = value.get_ucharacter();
			return 2;
		case NetGameEventValue::Type::character:
			*d = 10;
			*reinterpret_cast<char*>(d + 1) = value.get_character();
			return 2;
		case NetGameEventValue::Type::binary:
		{
			DataBuffer s = value.get_binary();
			*d = 11;
			*reinterpret_cast<unsigned short*>(d + 1) = s.get_size();
			memcpy(d + 3, s.get_data(), s.get_size());
			return 3 + s.get_size();
		}
		default:
			throw Exception("Unknown game event value type");
		}
	}

	unsigned int NetGameNetworkData::get_encoded_length(const NetGameEventValue &value)
	{
		switch (value.get_type())
		{
		case NetGameEventValue::Type::null:
		case NetGameEventValue::Type::boolean:
			return 1;
		case NetGameEventValue::Type::character:
		case NetGameEventValue::Type::ucharacter:
			return 2;
		case NetGameEventValue::Type::uinteger:
		case NetGameEventValue::Type::integer:
		case NetGameEventValue::Type::number:
			return 5;
		case NetGameEventValue::Type::string:
			return 1 + 2 + value.get_string().length();
		case NetGameEventValue::Type::binary:
			return 1 + 2 + value.get_binary().get_size();
		case NetGameEventValue::Type::complex:
		{
			unsigned l = 2;
			for (unsigned int i = 0; i < value.get_member_count(); i++)
				l += get_encoded_length(value.get_member(i));
			return l;
		}
		default:
			throw Exception("Unknown game event value type");
		}
	}
}
