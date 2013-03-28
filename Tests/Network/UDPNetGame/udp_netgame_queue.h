
#pragma once

#include "udp_netgame_resend_method.h"

class UDPNetGameQueue
{
public:
	UDPNetGameQueue();

	clan::DataBuffer create_payload(int payload_sequence);
	void payload_acked(int payload_sequence);

private:
	struct QueuedEvent
	{
		QueuedEvent(clan::NetGameEvent net_event, UDPNetGameResendMethod resend_method) : net_event(net_event), resend_method(resend_method) { }

		clan::NetGameEvent net_event;
		UDPNetGameResendMethod resend_method;
	};

	struct InflightPayload
	{
		InflightPayload() : send_time(0) { }

		unsigned long long send_time;
		std::vector<int> events_ids;
	};

	int next_event_id;

	std::map<int, QueuedEvent> queued_events;
	std::map<int, InflightPayload> payloads;
};
