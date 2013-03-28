
#pragma once

class UDPNetGameConnectionState
{
public:
	UDPNetGameConnectionState();

	clan::DataBuffer create_packet(clan::DataBuffer payload, int &out_sequence);
	clan::DataBuffer receive_packet(clan::DataBuffer packet, std::vector<int> &out_acked_sequences);

	float get_round_trip_time() const { return round_trip_time; }

private:
	void sent_packets_acknowledged(unsigned short ack, unsigned int ack_bits, std::vector<int> &out_acked_sequences);
	void sent_packet_acknowledged(unsigned short sequence, std::vector<int> &out_acked_sequences);

	void update_round_trip_time(float packet_rtt);

	void update_received_packets_ack(unsigned short sequence);

	static int sequence_delta(unsigned int s1, unsigned int s2);

	unsigned short local_sequence;
	unsigned short remote_sequence;

	unsigned short next_ack;
	unsigned int next_ack_bits;

	float round_trip_time;
	std::map<unsigned short, unsigned long long> sequence_send_time;

	static const unsigned int protocol_magic = (unsigned int)'c' | ((unsigned int)'l' << 8) | ((unsigned int)'a' << 16) | ((unsigned int)'n' << 24);
	static const unsigned int header_size = 3 * 4;
};
