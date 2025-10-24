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

#ifdef WIN32
#pragma warning(disable : 4355)
#pragma warning(disable : 4503)
#endif

#include "netsession_generic.h"
#include "netcomputer_generic.h"
#include "API/Network/NetSession/netstream.h"
#include "API/Core/System/threadfunc_v0.h"
#include "API/Core/System/event_listener.h"
#include "API/Core/System/error.h"
#include "API/Core/System/clanstring.h"
#include "API/Core/System/log.h"
#include <cstdio>

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession_Generic construction:

CL_NetSession_Generic::CL_NetSession_Generic(const std::string &app_id)
:
	udp_sock(CL_Socket::udp), app_id(app_id),
	show_debug(false), accept_tcp_socket(CL_Socket::tcp), accept_udp_socket(CL_Socket::udp),
	listen_thread(new CL_ThreadFunc_Runnable_v0<CL_NetSession_Generic>(this, &CL_NetSession_Generic::accept_thread), true),
	send_udp_thread(new CL_ThreadFunc_Runnable_v0<CL_NetSession_Generic>(this, &CL_NetSession_Generic::udp_thread), true),
	in_keep_alive(false), ref(0)
{
	boot();
}

CL_NetSession_Generic::~CL_NetSession_Generic()
{
	shutdown();
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession_Generic attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession_Generic operations:

void CL_NetSession_Generic::add_ref()
{
	ref++;
}

int CL_NetSession_Generic::release_ref()
{
	ref--;
	if (ref == 0)
	{
		delete this;
		return 0;
	}
	return ref;
}

CL_NetComputer CL_NetSession_Generic::connect(const CL_IPAddress &address)
{
	// Connect to server:
	CL_Socket sock(CL_Socket::tcp);
	sock.connect(address);

	// Establish clanlib netcomputer protocol communication over it:
	CL_MutexSection mutex_section(&mutex);
	CL_NetComputer_Generic *comp = new CL_NetComputer_Generic(sock, this, false, address.get_port());
	comp->add_ref();
	computers.push_back(comp);

	all.push_back(CL_NetComputer(comp));
	return CL_NetComputer(comp);
}

CL_NetComputer CL_NetSession_Generic::connect_async(const std::string &hostname, const std::string &port)
{
	// Establish clanlib netcomputer protocol communication over it:
	CL_MutexSection mutex_section(&mutex);
	CL_NetComputer_Generic *comp = new CL_NetComputer_Generic(hostname, port, this);
	comp->add_ref();
	computers.push_back(comp);

	all.push_back(CL_NetComputer(comp));
	return CL_NetComputer(comp);
}

void CL_NetSession_Generic::start_listen(const std::string &port)
{
	accept_shutdown_trigger.reset();
	accept_tcp_socket = CL_Socket(CL_Socket::tcp);
	accept_tcp_socket.set_nonblocking();
	accept_tcp_socket.set_nodelay();
	accept_tcp_socket.bind(CL_IPAddress(port));
	accept_udp_socket = CL_Socket(CL_Socket::udp);
	accept_udp_socket.set_nodelay();
	accept_udp_socket.bind(CL_IPAddress(port));

	accept_tcp_socket.listen(5);
	listen_thread.start();
}

void CL_NetSession_Generic::stop_listen()
{
	accept_shutdown_trigger.set_flag();
	listen_thread.wait();
}

void CL_NetSession_Generic::setup_udp_map(CL_NetComputer_Generic *computer, const std::string &address, unsigned int secret)
{
	CL_MutexSection mutex_section(&mutex);

	// adds computer to udp connection pending list
	udp_secret_ip_map[SecretIPPair(secret, address)] = computer;
}

void CL_NetSession_Generic::remove_udp_map(CL_NetComputer_Generic *computer)
{
	CL_MutexSection mutex_section(&mutex);

	// removes computer from the list of udp ready connection if avialable
	udp_ip_port_map.erase(computer->udp_address);
}

void CL_NetSession_Generic::received_netpacket(const std::string &channel, const CL_NetPacket &packet, CL_NetComputer_Generic *comp)
{
	if (show_debug)
		CL_Log::log("debug", "Netpacket arrived in %1", channel);
	received_netpackets[channel].push(PacketCompPair(packet, CL_NetComputer(comp)));
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession_Generic implementation:

void CL_NetSession_Generic::boot()
{
	// Start udp thread to send/recive pending udp packets.
	send_udp_thread.start();
}

void CL_NetSession_Generic::shutdown()
{
	stop_listen();

	CL_MutexSection mutex_section(&mutex);

	// Disconnect all computers:
	all = CL_NetGroup();
	while (!computers.empty()) 
	{
		CL_NetComputer_Generic *computer = computers.front(); 
		computers.pop_front();
		// have to leave in order to enable the worker thread to return
		mutex_section.leave(); 

		computer->do_shutdown();
		computer->release_ref();

		mutex_section.enter();
	}
	computers = std::list<CL_NetComputer_Generic *>();

	// Empty list of received netpackets.
	received_netpackets = std::map< std::string, std::queue<PacketCompPair> >();

	// Stop udp thread.
	udp_shutdown_trigger.set_flag();
	send_udp_thread.wait();
}

void CL_NetSession_Generic::keep_alive()
{
	if (in_keep_alive)
		return;
	in_keep_alive = true;

	CL_MutexSection mutex_section(&mutex);

	// Signal received netpackets on each channel.
	std::map< std::string, std::queue<PacketCompPair> >::iterator it;
	for (it = received_netpackets.begin(); it != received_netpackets.end(); it++)
	{
		CL_Signal_v2<CL_NetPacket &, CL_NetComputer &> &chan_sig = map_netpacket_receive[it->first];
		std::queue<PacketCompPair> &packets = it->second;

		while (!packets.empty())
		{
			CL_NetPacket packet = packets.front().first;
			CL_NetComputer computer = packets.front().second;
			packets.pop();
			mutex_section.leave();
			chan_sig(packet, computer);
			mutex_section.enter();
		}
	}

	// Signal incoming connections:
	while (!new_connections.empty())
	{
		CL_NetComputer computer = new_connections.front();
		all.push_back(computer);
		new_connections.pop();
		mutex_section.leave();
		sig_computer_connected(computer);
		mutex_section.enter();
	}

	// Signal incoming reconnections:
	while (!reconnections.empty())
	{
		CL_NetComputer computer = reconnections.front();
		all.push_back(computer);
		reconnections.pop();
		mutex_section.leave();
		sig_computer_reconnected(computer);
		mutex_section.enter();
	}
	
	// Signal incoming netstreams:
	while (!new_streams.empty())
	{
		NewStreamPair &pair = new_streams.front();
		CL_Signal_v1<CL_NetStream &> &chan_sig = map_netstream_connect[pair.second];
		CL_NetStream stream(pair.first);
		new_streams.pop();
		mutex_section.leave();
		chan_sig(stream);
		mutex_section.enter();
	}

	// Signal incoming disconnections:
	while (!disconnections.empty())
	{
		CL_NetComputer computer = disconnections.front();
		all.remove(computer);
		disconnections.pop();
		mutex_section.leave();
		sig_computer_disconnected(computer);
		mutex_section.enter();
	}

	// extra list to handle mutexing correctly
	std::list<CL_NetComputer_Generic *> releasecomputers; 
	// Check if any computer handles need to be removed:
	std::list<CL_NetComputer_Generic *>::iterator it_computers = computers.begin();
	while (it_computers != computers.end())
	{
		CL_NetComputer_Generic *computer = *it_computers;
		if (computer->disconnected)
		{
			releasecomputers.push_back(computer);
			computers.erase(it_computers++);
		}
		else
		{
			++it_computers;
		}
	}

	mutex_section.leave();
	while (!releasecomputers.empty())
	{
		CL_NetComputer_Generic *computer = releasecomputers.front();
		releasecomputers.pop_front();
		computer->release_ref();
	}
	mutex_section.enter();

	in_keep_alive = false;
}

void CL_NetSession_Generic::accept_thread()
{
	char *buffer = 0;

	try
	{
		CL_EventListener listener;
		listener.add_trigger(accept_tcp_socket.get_read_trigger());
		listener.add_trigger(accept_udp_socket.get_read_trigger());
		listener.add_trigger(&udp_wakeup_server_trigger);
		listener.add_trigger(&accept_shutdown_trigger);

		int msg_size = accept_udp_socket.get_max_msg_size();
		buffer = new char[msg_size];

		while (true)
		{
			listener.wait();
			if (accept_shutdown_trigger.get_flag()) break; // stop_listen was called.

			if (accept_tcp_socket.get_read_trigger()->get_flag())
			{
				// Incoming TCP connection:

				CL_Socket sock = accept_tcp_socket.accept();
				sock.set_nonblocking();
				sock.set_nodelay();
				CL_MutexSection mutex_section(&mutex);

				CL_NetComputer_Generic *comp = 
					new CL_NetComputer_Generic(sock, this, true, std::string());
				comp->add_ref();
				computers.push_back(comp);
			}
			if (accept_udp_socket.get_read_trigger()->get_flag())
			{
				// Incoming UDP message from client:

				CL_IPAddress from;
				int bytes_available = accept_udp_socket.recv(buffer, msg_size, from);

				dispatch_udp_messege_server(from, bytes_available, buffer);
			}
			if (udp_wakeup_server_trigger.get_flag())
			{
				// Pending UDP packets to client:

				IPMassagePair udp_message;

				while (!send_udp_server_queue.empty())
				{
					udp_message = send_udp_server_queue.front();

					accept_udp_socket.send(udp_message.second.c_str(), udp_message.second.size(), udp_message.first);

					send_udp_server_queue.pop();
				}

				udp_wakeup_server_trigger.reset();
			}
		}

		accept_tcp_socket = CL_Socket(CL_Socket::tcp);
		accept_tcp_socket.set_nonblocking();
		accept_tcp_socket.set_nodelay();
	}
	catch (CL_Error err)
	{
		CL_Log::log("debug", "Error %1", err.message);
	}

	delete[] buffer;
}

void CL_NetSession_Generic::udp_thread()
{
	char *buffer = 0;

	try
	{
		CL_EventListener listener;

		// client listen
		listener.add_trigger(udp_sock.get_read_trigger());

		// server client udp send
		listener.add_trigger(&udp_wakeup_client_trigger);

		listener.add_trigger(&udp_shutdown_trigger);

		int msg_size = accept_udp_socket.get_max_msg_size();
		buffer = new char[msg_size];

		while (true)
		{
			listener.wait();
			if (udp_shutdown_trigger.get_flag()) break; // netsession is destructing it self

			if (udp_sock.get_read_trigger()->get_flag())
			{
				// Incoming UDP message from server:
				CL_IPAddress from;
				int bytes_available = udp_sock.recv(buffer, msg_size, from);

				dispatch_udp_messege_client(from, bytes_available, buffer);
			}

			// look for data to send
			if (udp_wakeup_client_trigger.get_flag())
			{
				IPMassagePair udp_message;

				while (!send_udp_client_queue.empty())
				{
					udp_message = send_udp_client_queue.front();

					udp_sock.send(udp_message.second.c_str(), udp_message.second.size(), udp_message.first);

					send_udp_client_queue.pop();
				}

				udp_wakeup_client_trigger.reset();
			}

		}

		accept_udp_socket = CL_Socket(CL_Socket::udp);
		accept_udp_socket.set_nodelay();
	}
	catch (CL_Error err)
	{
		CL_Log::log("debug", "Error %1", err.message);
	}

	delete[] buffer;
}

void CL_NetSession_Generic::dispatch_udp_messege_server(const CL_IPAddress &from, int bytes_available, char *buffer)
{
	if (bytes_available > 0)
	{
		// todo: search for ip address in the udp connection map and dispatch message
		CL_NetPacket packet(buffer, bytes_available);

		// get the computer of the host
		std::map<CL_IPAddress, CL_NetComputer_Generic *>::iterator iter_ip_port_map;
		iter_ip_port_map = udp_ip_port_map.find(from);

		// if computer is not listed, try peeping into data and see if it's a udp connection request
		if (iter_ip_port_map == udp_ip_port_map.end())
		{
			char sig = packet.input.read_char8();

			if (sig == CL_NetComputer_Generic::cmd_udp_connect)
			{
				// get secret(magic) key
				unsigned short secret = packet.input.read_ushort16();

				std::map<SecretIPPair, CL_NetComputer_Generic *>::iterator iter_secret_ip_map;

				iter_secret_ip_map = udp_secret_ip_map.find(SecretIPPair(secret, from.get_address()));

				// check if computer is listed in pending list
				if (iter_secret_ip_map != udp_secret_ip_map.end())
				{
					// FOUND!!!
					CL_NetComputer_Generic *comp = (*iter_secret_ip_map).second;

					// set udp adress to computer
					comp->udp_address = from;

					// return secret(magic) key to number generator
					secret_number_generator.return_number(secret);

					// now add computer to udp_ip_port_map list
					udp_ip_port_map[from] = comp;
					// and remove them from pending list
					udp_secret_ip_map.erase(iter_secret_ip_map);

					// Rebuild data
					packet = CL_NetPacket();
					packet.output.write_char8(CL_NetComputer_Generic::cmd_udp_connect);

					// send data to computer
					comp->received_udp_packet(packet);

				}
				else
				{
					// ambiguous remote computer trying connect with fake secret(magic) key???
				}

			}
			else
			{
				// ambiguous remote computer trying to do cheeky stuff???
			}
		}
		else
		{
			// Computer found
			// send data to computer
			(*iter_ip_port_map).second->received_udp_packet(packet);
		}
	}
}

void CL_NetSession_Generic::dispatch_udp_messege_client(const CL_IPAddress &from, int bytes_available, char *buffer)
{
	if (bytes_available > 0)
	{
		CL_NetPacket packet(buffer, bytes_available);

		// get the computer of the host
		std::map<CL_IPAddress, CL_NetComputer_Generic *>::iterator iter_ip_port_map;
		iter_ip_port_map = udp_ip_port_map.find(from);

		// if computer is listed send data
		if (iter_ip_port_map != udp_ip_port_map.end())
			{
			// Computer found
			// send data to computer
			(*iter_ip_port_map).second->received_udp_packet(packet);
	}
		else
	{
			// ambiguous remote computer trying to do cheeky stuff???
		}
	}
}
