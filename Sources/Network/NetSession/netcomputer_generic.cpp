/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "netcomputer_generic.h"
#include "netstream_generic.h"
#include "netsession_generic.h"
#include "netpacket_generic.h"
#include "API/Core/System/threadfunc_v0.h"
#include "API/Core/System/event_trigger.h"
#include "API/Core/System/event_listener.h"
#include "API/Core/System/error.h"
#include "API/Core/IOData/outputsource_memory.h"
#include <cstdio>
#include <cstring>

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer_Generic construction:

CL_NetComputer_Generic::CL_NetComputer_Generic(
	const CL_Socket &sock, CL_NetSession_Generic *netsession,
	bool server, const std::string &server_port)
:
	tcp_sock(sock), udp_available(false), disconnected(false),
	netsession(netsession), data_left(0), netstream(0), local_atom_counter(0),
	send_pos(0), ref(0), async_boot(false), async_port(""), channel_id_counter(0), shutdown(false),
	server(server), server_port(server_port)
{
	tcp_sock.set_nodelay();
	address = tcp_sock.get_dest_address();
	netsession->new_connections.push(CL_NetComputer(this));

	thread = CL_Thread(new CL_ThreadFunc_Runnable_v0<CL_NetComputer_Generic>(this, &CL_NetComputer_Generic::worker_thread), true);
	thread.start();
}

CL_NetComputer_Generic::CL_NetComputer_Generic(const std::string &hostname, const std::string &port, CL_NetSession_Generic *netsession)
:
	tcp_sock(CL_Socket::tcp), udp_available(false), disconnected(false),
	netsession(netsession), data_left(0), netstream(0), local_atom_counter(0), send_pos(0),
	ref(0), async_boot(true), async_hostname(hostname), async_port(port),
	channel_id_counter(0), shutdown(false), server(false)
{
	tcp_sock.set_nodelay();
	netsession->new_connections.push(CL_NetComputer(this));

	thread = CL_Thread(new CL_ThreadFunc_Runnable_v0<CL_NetComputer_Generic>(this, &CL_NetComputer_Generic::worker_thread), true);
	thread.start();
}

CL_NetComputer_Generic::~CL_NetComputer_Generic()
{
	do_shutdown();
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer_Generic attributes:

int CL_NetComputer_Generic::get_ref() const
{
	return ref;
}

CL_Thread const & CL_NetComputer_Generic::get_thread_handle() const
{
	return thread;
}


/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer_Generic operations:

void CL_NetComputer_Generic::add_ref()
{
	ref++;
}

int CL_NetComputer_Generic::release_ref()
{
	ref--;
	if (ref == 0)
	{
		delete this;
		return 0;
	}
	return ref;
}

void CL_NetComputer_Generic::send_udp_info()
{
	unsigned short secret;

	try
	{
		secret = netsession->secret_number_generator.get_number();
	}
	catch (CL_Error error)
	{
		// out of secret keys. all numbers have been used up.
		// The likely hood of this happening is so small that it can be ignored.
		// But catch it anyways.

		return;
	}

	netsession->setup_udp_map(this, address.get_address(), secret);

	CL_OutputSource_Memory cmd;
	cmd.write_uchar8(cmd_udp_info);
	cmd.write_ushort16(secret);

	bool tcp_queue_empty = send_tcp_queue.empty();
	send_tcp_queue.push(cmd.get_data());
	if (tcp_queue_empty) wakeup_trigger.set_flag();
}

void CL_NetComputer_Generic::send_packet(const std::string &packet_channel, CL_NetPacket_Generic *packet)
{
	CL_MutexSection mutex_section(&mutex);
	CL_OutputSource_Memory cmd;

	std::map<std::string, int>::iterator it = local_atoms.find(packet_channel);
	if (it == local_atoms.end())
	{
		local_atoms[packet_channel] = local_atom_counter++;
		it = local_atoms.find(packet_channel);

		cmd.write_uchar8(cmd_create_atom);
		cmd.write_ushort16(it->second);
		cmd.write_string(it->first);
	}

	cmd.write_uchar8(cmd_netpacket_msg);
	cmd.write_ushort16(it->second);
	cmd.write_ushort16(packet->size);
	cmd.write(packet->data, packet->size);

	bool tcp_queue_empty = send_tcp_queue.empty();
	send_tcp_queue.push(cmd.get_data());
	if (tcp_queue_empty) wakeup_trigger.set_flag();
}

//: Send packet to computer using udp.
void CL_NetComputer_Generic::send_packet_udp(const std::string &packet_channel, CL_NetPacket_Generic *packet)
{
	CL_MutexSection mutex_section(&mutex);
	CL_OutputSource_Memory cmd;

	std::map<std::string, int>::iterator it = local_atoms.find(packet_channel);
	if (it == local_atoms.end())
	{
		CL_OutputSource_Memory cmd_atom;

		local_atoms[packet_channel] = local_atom_counter++;
		it = local_atoms.find(packet_channel);

		cmd_atom.write_uchar8(cmd_create_atom);
		cmd_atom.write_ushort16(it->second);
		cmd_atom.write_string(it->first);

		// use tcp to send create atom signal
		bool tcp_queue_empty = send_tcp_queue.empty();
		send_tcp_queue.push(cmd_atom.get_data());
		if (tcp_queue_empty) wakeup_trigger.set_flag();
	}

	cmd.write_uchar8(cmd_netpacket_msg);
	cmd.write_ushort16(it->second);
	cmd.write(packet->data, packet->size);

	// discard data if packet size is too big
	if (cmd.size() > netsession->udp_sock.get_max_msg_size())
		return;

	if (udp_available)
	{
		CL_MutexSection mutex_section(&netsession->mutex);

		if (server)
		{
			bool udp_queue_empty = netsession->send_udp_server_queue.empty();
			netsession->send_udp_server_queue.push(CL_NetSession_Generic::IPMassagePair(udp_address, cmd.get_data()));
			if (udp_queue_empty) netsession->udp_wakeup_server_trigger.set_flag();
		}
		else
		{
			bool udp_queue_empty = netsession->send_udp_client_queue.empty();
			netsession->send_udp_client_queue.push(CL_NetSession_Generic::IPMassagePair(udp_address, cmd.get_data()));
			if (udp_queue_empty) netsession->udp_wakeup_client_trigger.set_flag();
		}
	}
	else
	{
		// send through tcp???
		bool tcp_queue_empty = send_tcp_queue.empty();
		send_tcp_queue.push(cmd.get_data());
		if (tcp_queue_empty) wakeup_trigger.set_flag();
	}
}

void CL_NetComputer_Generic::send_stream_message(unsigned int channel_id, const std::string &data)
{
	CL_OutputSource_Memory cmd;
	cmd.write_uchar8(cmd_netstream_msg);
	cmd.write_ushort16(channel_id);
	cmd.write_ushort16(data.length());
	cmd.write(data.data(), data.length());

	CL_MutexSection mutex_section(&mutex);

	bool tcp_queue_empty = send_tcp_queue.empty();
	send_tcp_queue.push(cmd.get_data());
	if (tcp_queue_empty) wakeup_trigger.set_flag();
}

int CL_NetComputer_Generic::send_stream_connect(const std::string &stream_channel, CL_NetStream_Generic *stream)
{
	CL_MutexSection mutex_section(&mutex);
	CL_OutputSource_Memory cmd;

	std::map<std::string, int>::iterator it = local_atoms.find(stream_channel);
	if (it == local_atoms.end())
	{
		local_atoms[stream_channel] = local_atom_counter++;
		it = local_atoms.find(stream_channel);

		cmd.write_uchar8(cmd_create_atom);
		cmd.write_ushort16(it->second);
		cmd.write_string(it->first);
	}

	int channel_id = channel_id_counter++;
	streams[channel_id] = stream;
	
	cmd.write_uchar8(cmd_netstream_connect);
	cmd.write_ushort16(it->second); // channel atom
	cmd.write_ushort16(channel_id);

	bool tcp_queue_empty = send_tcp_queue.empty();
	send_tcp_queue.push(cmd.get_data());
	if (tcp_queue_empty) wakeup_trigger.set_flag();
	
	return channel_id;
}

void CL_NetComputer_Generic::send_stream_close(int channel_id)
{
	CL_MutexSection mutex_section(&mutex);
	CL_OutputSource_Memory cmd;

	cmd.write_uchar8(cmd_netstream_closed);
	cmd.write_ushort16(channel_id);
	
	bool tcp_queue_empty = send_tcp_queue.empty();
	send_tcp_queue.push(cmd.get_data());
	if (tcp_queue_empty) wakeup_trigger.set_flag();
	
	std::map<int, CL_NetStream_Generic *>::iterator it = streams.find(channel_id);
	if (it != streams.end())
	{
		it->second->closed = true;
		streams.erase(it);
	}
}

void CL_NetComputer_Generic::disconnect(std::string reason)
{
	disconnect_reason = reason;
	disconnected = true;
	wakeup_trigger.set_flag();
}

void CL_NetComputer_Generic::do_shutdown()
{
	if (shutdown) return;

	netsession->remove_udp_map(this);

	shutdown = true;
	wakeup_trigger.set_flag();
	thread.wait();
}

void CL_NetComputer_Generic::received_udp_packet(CL_NetPacket &packet)
{
	try
	{
		unsigned char command = packet.input.read_uchar8();
		switch (command)
		{
		case cmd_udp_connect:
			{
				// Ok got udp connection,
				// inform we are go for udp:

				CL_OutputSource_Memory cmd;
				cmd.write_uchar8(cmd_udp_connect);

				CL_MutexSection mutex_section(&mutex);
				bool tcp_queue_empty = send_tcp_queue.empty();
				send_tcp_queue.push(cmd.get_data());
				if (tcp_queue_empty) wakeup_trigger.set_flag();

				udp_available = true;
			}
			break;

		case cmd_netpacket_msg:
			{
				std::map<int, std::string>::iterator iter = remote_atoms.find(packet.input.read_ushort16());

				// if it's an unknown channel, ignore message.
				if (iter == remote_atoms.end()) break;

				std::string channel_name = (*iter).second;

				int pos = packet.input.tell();
				CL_NetPacket message(packet.get_data()+pos, packet.get_size()-pos);
				CL_MutexSection mutex_section(&netsession->mutex);
				netsession->received_netpacket(channel_name, message, this);
			}
			break;
		}
	}
	catch (CL_Error error)
	{
		// bogus packet. Should this cause clannet to disconnect the netcomputer?
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer_Generic implementation:

void CL_NetComputer_Generic::worker_thread()
{
	try
	{
		if (async_boot) // async connect. Do the connect now.
		{
			async_boot = false;
			address = CL_IPAddress(async_hostname, async_port);
			tcp_sock.bind(CL_IPAddress());
			tcp_sock.connect(address);
			server_port = async_port;
		}

		if (server)
		{
			CL_MutexSection mutex_section(&mutex);
			send_udp_info();
		}

		while (!disconnected)
		{
			CL_MutexSection mutex_section(&mutex, false);

			// Wait for activity:
			CL_EventListener listener;
			mutex_section.enter();
			listener.add_trigger(&wakeup_trigger);
			listener.add_trigger(tcp_sock.get_read_trigger());
			if (!send_tcp_queue.empty()) listener.add_trigger(tcp_sock.get_write_trigger());
			mutex_section.leave();

			listener.wait();
			if (shutdown) return;

			// Process activity:
			wakeup_trigger.reset();
			update_socket();
		}
	}
	catch (CL_Error err)
	{
		CL_MutexSection mutex_section(&mutex, false);

		disconnect_reason = err.message;
		disconnected = true;
	}

	// Remove computer from processing queue if disconnected.
	if (disconnected)
	{
		CL_MutexSection mutex_section(&netsession->mutex);
		netsession->computers.remove(this);
		netsession->disconnections.push(CL_NetComputer(this));
	}

	// Close socket connection:
	CL_MutexSection mutex_section(&mutex);
	tcp_sock = CL_Socket();
}

#define cl_min(a, b) (a < b ? a : b)

void CL_NetComputer_Generic::update_socket()
{
	if (disconnected) return;

	if (tcp_sock.get_read_trigger()->get_flag()) // Data available on socket:
	{
		char buffer[16*1024];

		if (data_left > 0) // currently routing message data.
		{
			int received = 0;
			
			received = tcp_sock.recv(buffer, cl_min(data_left, 16*1024));
			if (received == 0) throw CL_Error("Connection closed by foreign host");
			
			if (netstream) // Send data to netstream:
			{
				CL_MutexSection mutex_section(&netstream->mutex);
				netstream->receive_queue.push(std::string(buffer, received));
				netstream->read_trigger.set_flag();
			}
			else // Send data to netpacket
			{
				int pos = netpacket.get_size();
				netpacket.resize(pos + received);
				memcpy(netpacket.get_data()+pos, buffer, received);
			}

			data_left -= received;

			if (data_left == 0) // finished reading message data
			{
				if (!netstream)
				{
					CL_MutexSection mutex_section(&netsession->mutex);
					netsession->received_netpacket(remote_atoms[dest_netpacket_atom], netpacket, this);
					netpacket = CL_NetPacket();
				}

				netstream = 0;
			}
		}
		else // hmm, new message.
		{
			unsigned char command = tcp_sock.input.read_uchar8();
			switch (command)
			{
			case cmd_create_atom:
				{
					int atom_id = tcp_sock.input.read_ushort16();
					std::string atom_str = tcp_sock.input.read_string();

					remote_atoms[atom_id] = atom_str;
				}
				break;

			case cmd_udp_info:
				if (!server)
				{
					udp_connect_secret = tcp_sock.input.read_ushort16();
					udp_address = CL_IPAddress(address.get_address(), server_port);

					CL_NetPacket packet;
					packet.output.write_uchar8(cmd_udp_connect);
					packet.output.write_ushort16(udp_connect_secret);
					netsession->udp_sock.send(packet.get_data(), packet.get_size(), udp_address);
					// todo: keep sending this packet until a cmd_udp_connect is received

					// now add computer to udp_ip_port_map list
					netsession->udp_ip_port_map[udp_address] = this;
				}
				break;

			case cmd_udp_connect:
				{
					udp_available = true;
				}
				break;

			case cmd_netstream_connect:
				{
					std::map< std::string, CL_Signal_v1<CL_NetStream &> >::iterator it_connect;

					std::string channel_name = remote_atoms[tcp_sock.input.read_ushort16()];
					int channel_id = tcp_sock.input.read_ushort16();
					CL_MutexSection mutex_section(&netsession->mutex);
					it_connect = netsession->map_netstream_connect.find(channel_name);
					if (it_connect == netsession->map_netstream_connect.end())
					{
						// Tried to connect to non-existant netstream.
						mutex_section.leave();
						send_stream_close(channel_id);
					}
					CL_NetStream_Generic *stream = new CL_NetStream_Generic(channel_id, this, netsession);
					streams[channel_id] = stream;
					CL_NetSession_Generic::NewStreamPair pair(stream, channel_name);
					netsession->new_streams.push(pair);
				}
				break;

			case cmd_netstream_msg:
				{
					std::map< int, CL_NetStream_Generic *>::iterator it_channel;

					CL_MutexSection mutex_section(&mutex);
					unsigned int channel_id = tcp_sock.input.read_ushort16();
					data_left = tcp_sock.input.read_ushort16();
					it_channel = streams.find(channel_id);
					if (it_channel == streams.end())
					{
						// Tried to send data to non-existant netstream connection.
						netstream = 0;
						send_stream_close(channel_id);
					}
					else
					{
						netstream = it_channel->second;
					}
				}
				break;

			case cmd_netstream_closed:
				{
					std::map<int, CL_NetStream_Generic *>::iterator it_channel;

					CL_MutexSection mutex_section(&mutex);
					unsigned int channel_id = tcp_sock.input.read_ushort16();
					it_channel = streams.find(channel_id);
					if (it_channel != streams.end())
					{
						CL_MutexSection mutex_section(&it_channel->second->mutex);
						it_channel->second->closed = true;
						it_channel->second->read_trigger.set_flag();
						streams.erase(it_channel);
					}
				}
				break;

			case cmd_netpacket_msg:
				{
					dest_netpacket_atom = tcp_sock.input.read_ushort16();
					data_left = tcp_sock.input.read_ushort16();
				}
				break;
			}
		}
	}

	CL_MutexSection mutex_section(&mutex);
//		if (tcp_sock.get_write_trigger()->get_flag()) // Can write data to socket
	while (!send_tcp_queue.empty())
	{
		std::string &msg = send_tcp_queue.front();

		int sent = tcp_sock.send(msg.substr(send_pos));
		send_pos += sent;
		if (send_pos != static_cast<int>(msg.size())) break;

		send_tcp_queue.pop();
		send_pos = 0;
	}
}
