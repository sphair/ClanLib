
#pragma once

#include "udp_netgame_connection_state.h"
#include "udp_netgame_resend_method.h"

class UDPNetGameClient
{
public:
	UDPNetGameClient();

	std::function<clan::NetGameEvent()> &func_event_received() { return cb_event_received; }

	void process();

private:
	void process_incoming();
	void process_outgoing();

	void process_acked_sequences(const std::vector<int> &acked_sequences);
	void payload_received(clan::DataBuffer buffer);
	void send_payload(clan::DataBuffer buffer);

	clan::SocketName server_name;
	clan::UDPSocket socket;

	UDPNetGameConnectionState connection_state;

	std::function<clan::NetGameEvent()> cb_event_received;
};
