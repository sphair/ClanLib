
#pragma once

#include "udp_netgame_connection_state.h"
#include "udp_netgame_resend_method.h"

class UDPNetGameClient
{
public:
	UDPNetGameClient();

	clan::Callback_v1<clan::NetGameEvent> &func_event_received() { return cb_event_received; }

	void send_event(clan::NetGameEvent net_event, UDPNetGameResendMethod resend_method);

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

	clan::Callback_v1<clan::NetGameEvent> cb_event_received;
};
