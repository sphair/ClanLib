/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#pragma once

#include "API/Network/NetGame/event.h"
#include "API/Network/Socket/tcp_connection.h"
#include <map>

namespace clan
{
	class DataBuffer;

	class NetGameNetworkData
	{
	public:
		static NetGameEvent receive_data(const void *data, int size, int &out_bytes_consumed);
		static DataBuffer send_data(const NetGameEvent &e);

	private:
		static NetGameEvent decode_event(const DataBuffer &data);
		static DataBuffer encode_event(const NetGameEvent &e);

		static unsigned int get_encoded_length(const NetGameEventValue &value);
		static unsigned int encode_value(unsigned char *d, const NetGameEventValue &value);

		static NetGameEventValue decode_value(unsigned char type, const unsigned char *d, unsigned int length, unsigned int &pos);

		enum { packet_limit = 32000 };
	};
}
