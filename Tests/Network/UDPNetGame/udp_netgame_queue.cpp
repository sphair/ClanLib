
#include "precomp.h"
#include "udp_netgame_queue.h"

using namespace clan;

UDPNetGameQueue::UDPNetGameQueue()
: next_event_id(0)
{
}

DataBuffer UDPNetGameQueue::create_payload(int payload_sequence)
{
	return DataBuffer();
}

void UDPNetGameQueue::payload_acked(int payload_sequence)
{
}
