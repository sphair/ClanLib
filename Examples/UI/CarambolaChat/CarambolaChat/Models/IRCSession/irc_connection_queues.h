
#pragma once

#include "irc_raw_string.h"

class IRCConnectionQueues
{
public:

	void reset();
	void push_send(const IRCRawString &line);
	IRCRawString pop_send();
	void push_received(const IRCRawString &line);
	IRCRawString pop_received();
	bool is_receive_queue_empty();
	void set_disconnected(const std::string &reason);
	bool pop_disconnected(std::string &out_reason);

	//FIXME clan::Event send_event;

private:
	std::recursive_mutex mutex;
	std::vector<IRCRawString> send_queue;
	std::vector<IRCRawString> receive_queue;
	std::atomic_int disconnected;
	std::string disconnected_reason;
};
