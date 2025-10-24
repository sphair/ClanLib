/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

class CL_NetGameConnection_Impl
{

public:
	CL_NetGameConnection_Impl();
	void start(CL_NetGameConnection *base, CL_NetGameConnectionSite *site, const CL_TCPConnection &connection);
	void start(CL_NetGameConnection *base, CL_NetGameConnectionSite *site, const CL_SocketName &socket_name);

	~CL_NetGameConnection_Impl();

	void set_data(const CL_StringRef &name, void *data);

	void *get_data(const CL_StringRef &name) const;

	void send_event(const CL_NetGameEvent &game_event);

	void disconnect();

private:

	void connection_main();

private:
	CL_NetGameConnection *base;

	CL_NetGameConnectionSite *site;
	CL_TCPConnection connection;
	CL_SocketName socket_name;
	bool is_connected;
	CL_Thread thread;
	CL_Event stop_event, queue_event;
	CL_Mutex mutex;
	struct Message
	{
		Message() : type(type_message), event(CL_String()) { }
		enum Type
		{
			type_message,
			type_disconnect
		};
		Type type;
		CL_NetGameEvent event;
	};
	std::vector<Message> send_queue;
	struct AttachedData
	{
		CL_String name;
		void *data;
	};
	std::vector<AttachedData> data;
};
