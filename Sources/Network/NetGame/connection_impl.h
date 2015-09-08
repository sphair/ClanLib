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
**    Mark Page
*/

#pragma once

#include <mutex>
#include <thread>
#include "API/Network/Socket/tcp_connection.h"
#include "API/Network/Socket/socket_name.h"

namespace clan
{
	class NetGameConnection_Impl
	{
	public:
		NetGameConnection_Impl();
		~NetGameConnection_Impl();
		void start(NetGameConnection *base, NetGameConnectionSite *site, const TCPConnection &connection);
		void start(NetGameConnection *base, NetGameConnectionSite *site, const SocketName &socket_name);
		void set_data(const std::string &name, void *data);
		void *get_data(const std::string &name) const;
		void send_event(const NetGameEvent &game_event);
		void disconnect();
		SocketName get_remote_name() const;

	private:
		void connection_main();

		bool read_connection_data(DataBuffer &receive_buffer, int &bytes_received);
		bool write_connection_data(DataBuffer &send_buffer, int &bytes_sent, bool &send_graceful_close);

		bool read_data(const void *data, int size, int &out_bytes_consumed);
		bool write_data(DataBuffer &buffer);

		NetGameConnection *base;

		NetGameConnectionSite *site;

		NetworkConditionVariable worker_event;
		TCPConnection connection;
		SocketName socket_name;
		bool is_connected;
		std::thread thread;
		bool stop_flag = false;
		std::mutex mutex;
		struct Message
		{
			Message() : type(type_message), event(std::string()) { }
			enum Type
			{
				type_message,
				type_disconnect
			};
			Type type;
			NetGameEvent event;
		};
		std::vector<Message> send_queue;
		struct AttachedData
		{
			std::string name;
			void *data;
		};
		std::vector<AttachedData> data;
	};
}
