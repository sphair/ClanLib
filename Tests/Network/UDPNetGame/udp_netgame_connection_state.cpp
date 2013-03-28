
#include "precomp.h"
#include "udp_netgame_connection_state.h"

using namespace clan;

UDPNetGameConnectionState::UDPNetGameConnectionState() : local_sequence(0), remote_sequence(0), round_trip_time(0.0f), next_ack(0), next_ack_bits(0)
{
}

DataBuffer UDPNetGameConnectionState::create_packet(DataBuffer payload, int &out_sequence)
{
	local_sequence++;

	sequence_send_time[local_sequence] = System::get_microseconds();

	if (sequence_send_time.find(local_sequence - 64) != sequence_send_time.end())
		sequence_send_time.erase(sequence_send_time.find(local_sequence - 64));

	DataBuffer packet(header_size + payload.get_size());
	unsigned int *data = packet.get_data<unsigned int>();

	data[0] = protocol_magic;
	data[1] = ((unsigned int)local_sequence << 16) | next_ack;
	data[2] = next_ack_bits;

	memcpy(data + 3, payload.get_data(), payload.get_size());

	out_sequence = local_sequence;
	return packet;
}

DataBuffer UDPNetGameConnectionState::receive_packet(DataBuffer packet, std::vector<int> &out_acked_sequences)
{
	out_acked_sequences.clear();

	if (packet.get_size() > header_size)
	{
		const unsigned int *data = packet.get_data<const unsigned int>();
		unsigned int protocol_id = data[0];
		unsigned short sequence = data[1] >> 16;
		unsigned short ack = data[1] & 0xffff;
		unsigned int ack_bits = data[2];

		if (protocol_id != protocol_magic)
			return DataBuffer();

		update_received_packets_ack(sequence);

		sent_packets_acknowledged(ack, ack_bits, out_acked_sequences);

		DataBuffer payload(data + header_size, packet.get_size() - header_size);
		return payload;
	}
	else
	{
		return DataBuffer();
	}
}

void UDPNetGameConnectionState::sent_packets_acknowledged(unsigned short ack, unsigned int ack_bits, std::vector<int> &out_acked_sequences)
{
	sent_packet_acknowledged(ack, out_acked_sequences);
	for (unsigned short i = 0; i < 32; i++)
	{
		unsigned int bit = 1 << i;
		if (ack_bits & bit)
		{
			sent_packet_acknowledged(ack - 1 - i, out_acked_sequences);
		}
	}
}

void UDPNetGameConnectionState::sent_packet_acknowledged(unsigned short sequence, std::vector<int> &out_acked_sequences)
{
	auto it = sequence_send_time.find(sequence);
	if (it != sequence_send_time.end())
	{
		unsigned long long send_time = it->second;
		update_round_trip_time((System::get_microseconds() - send_time) / 10e-6f);
		sequence_send_time.erase(it);
	}

	if (sequence_delta(sequence, remote_sequence) > 0)
	{
		remote_sequence = sequence;
	}

	out_acked_sequences.push_back(sequence);
}

void UDPNetGameConnectionState::update_round_trip_time(float packet_rtt)
{
	if (round_trip_time != 0.0f)
	{
		round_trip_time = mix(round_trip_time, packet_rtt, 0.10f);
	}
	else
	{
		round_trip_time = packet_rtt;
	}
}

void UDPNetGameConnectionState::update_received_packets_ack(unsigned short sequence)
{
	int delta = sequence_delta(sequence, next_ack);
	if (delta > 0)
	{
		next_ack = sequence;
		next_ack_bits = ((next_ack_bits << 1) | 1) << (delta - 1);
	}
	else if (delta < 0)
	{
		next_ack_bits |= 1 << (-delta - 1);
	}
}

int UDPNetGameConnectionState::sequence_delta(unsigned int s1, unsigned int s2)
{
	int delta = s1 - s2;
	if (delta >= 32767)
		delta -= 65536;
	else if (delta <= -32767)
		delta += 65536;
	return delta;
}
