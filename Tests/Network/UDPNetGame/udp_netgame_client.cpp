
#include "precomp.h"
#include "udp_netgame_client.h"

using namespace clan;

UDPNetGameClient::UDPNetGameClient()
{
}

void UDPNetGameClient::process()
{
	process_incoming();
	process_outgoing();
}

void UDPNetGameClient::process_incoming()
{
	DataBuffer packet(64*1024);
	while (true)
	{
		packet.set_size(64*1024);

		SocketName from;
		int received = socket.read(packet.get_data(), packet.get_size(), from);
		if (received == -1)
			break;

		if (from == server_name)
		{
			packet.set_size(received);

			std::vector<int> acked_sequences;
			DataBuffer payload = connection_state.receive_packet(packet, acked_sequences);
			if (!payload.is_null())
			{
				process_acked_sequences(acked_sequences);
				payload_received(payload);
			}
		}
	}
}

void UDPNetGameClient::process_outgoing()
{
}

void UDPNetGameClient::payload_received(clan::DataBuffer payload)
{
}

void UDPNetGameClient::send_payload(DataBuffer payload)
{
	int packet_sequence = 0;
	DataBuffer packet = connection_state.create_packet(payload, packet_sequence);
	socket.send(packet.get_data(), packet.get_size(), server_name);

	// To do: save packet_sequence
}

void UDPNetGameClient::process_acked_sequences(const std::vector<int> &acked_sequences)
{
	// To do: remove acked important messages
}
