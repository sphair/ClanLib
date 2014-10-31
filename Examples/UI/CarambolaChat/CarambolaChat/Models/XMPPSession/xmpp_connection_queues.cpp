
#include "precomp.h"
#include "xmpp_connection_queues.h"

void XMPPConnectionQueues::reset()
{
	CL_MutexSection mutex_lock(&mutex);
	send_event.reset();
	disconnected.set(0);
	send_queue.clear();
	receive_queue.clear();
}

void XMPPConnectionQueues::push_send(const XMPPStanza &stanza)
{
	if (!stanza.empty())
	{
		CL_MutexSection mutex_lock(&mutex);
		send_queue.push_back(stanza);
		mutex_lock.unlock();
		send_event.set();
	}
}

XMPPStanza XMPPConnectionQueues::pop_send()
{
	CL_MutexSection mutex_lock(&mutex);
	if (send_queue.empty())
	{
		send_event.reset();
		return XMPPStanza();
	}
	else
	{
		XMPPStanza stanza = send_queue.front();
		send_queue.erase(send_queue.begin());
		return stanza;
	}
}

void XMPPConnectionQueues::push_received(const XMPPStanza &stanza)
{
	if (!stanza.empty())
	{
		CL_MutexSection mutex_lock(&mutex);
		receive_queue.push_back(stanza);
		mutex_lock.unlock();
	}
}

XMPPStanza XMPPConnectionQueues::pop_received()
{
	CL_MutexSection mutex_lock(&mutex);
	if (receive_queue.empty())
	{
		return XMPPStanza();
	}
	else
	{
		XMPPStanza stanza = receive_queue.front();
		receive_queue.erase(receive_queue.begin());
		return stanza;
	}
}

void XMPPConnectionQueues::set_disconnected(const CL_String &reason)
{
	CL_MutexSection mutex_lock(&mutex);
	disconnected_reason = reason;
	mutex_lock.unlock();
	disconnected.set(1);
}

bool XMPPConnectionQueues::pop_disconnected(CL_String &out_reason)
{
	bool was_disconnected = (disconnected.get() != 0);
	if (was_disconnected)
	{
		disconnected.set(0);
		CL_MutexSection mutex_lock(&mutex);
		out_reason = disconnected_reason;
		return true;
	}
	else
	{
		return false;
	}
}
