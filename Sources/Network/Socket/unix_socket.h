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
*/

#pragma once

#ifndef SOCKET
typedef int SOCKET;
#endif

class CL_SocketName;

class CL_UnixSocket
{
public:
	CL_UnixSocket();
	CL_UnixSocket(int handle);
	~CL_UnixSocket();

	void set_handle(SOCKET handle, bool close_handle);

	void create_tcp();
	void create_udp();

	void disconnect_graceful(int timeout);
	void disconnect_abortive();

	void set_nodelay(bool enable);
	void set_keep_alive(bool enable, int timeout, int interval);

	void bind(const CL_SocketName &socketname, bool reuse_address);

	void listen(int backlog);
	int accept(CL_SocketName &out_socketname);

	void connect(const CL_SocketName &socketname);

	CL_SocketName get_local_name() const;
	CL_SocketName get_remote_name() const;

	int receive(void *data, int size);
	int peek(void *data, int size);
	int send(const void *data, int size);
	void close_send();

	int receive_from(void *data, int size, CL_SocketName &out_socketname);
	int peek_from(void *data, int size, CL_SocketName &out_socketname);
	int send_to(const void *data, int size, const CL_SocketName &socketname);

	int get_handle() const { return handle; }

private:
	void create_socket_handle(int type);
	void close_handle();
	void set_nonblocking();

	void throw_if_invalid(int result) const;
	void throw_if_failed(int result) const;
	void throw_if_socket_failed(int result) const;
	static CL_String error_to_string(int err);

	int handle;
	bool close_handle_flag;
};
