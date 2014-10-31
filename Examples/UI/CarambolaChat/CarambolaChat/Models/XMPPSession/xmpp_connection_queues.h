
#pragma once

#include "xmpp_stanza.h"

class XMPPConnectionQueues
{
public:
	void reset();
	void push_send(const XMPPStanza &stanza);
	XMPPStanza pop_send();
	void push_received(const XMPPStanza &stanza);
	XMPPStanza pop_received();
	void set_disconnected(const CL_String &reason);
	bool pop_disconnected(CL_String &out_reason);

	CL_Event send_event;

private:
	CL_Mutex mutex;
	std::vector<XMPPStanza> send_queue;
	std::vector<XMPPStanza> receive_queue;
	CL_InterlockedVariable disconnected;
	CL_String disconnected_reason;
};
