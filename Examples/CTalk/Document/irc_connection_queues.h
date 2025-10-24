
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
	void set_disconnected(const CL_String &reason);
	bool pop_disconnected(CL_String &out_reason);

	CL_Event send_event, receive_event;

private:
	CL_Mutex mutex;
	std::vector<IRCRawString> send_queue;
	std::vector<IRCRawString> receive_queue;
	CL_InterlockedVariable disconnected;
	CL_String disconnected_reason;
};
