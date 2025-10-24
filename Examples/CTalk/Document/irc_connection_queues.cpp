
#include "precomp.h"
#include "irc_connection_queues.h"

void IRCConnectionQueues::reset()
{
	CL_MutexSection mutex_lock(&mutex);
	receive_event.reset();
	send_event.reset();
	disconnected.set(0);
	send_queue.clear();
	receive_queue.clear();
}

void IRCConnectionQueues::push_send(const IRCRawString &line)
{
	if (!line.empty())
	{
		CL_MutexSection mutex_lock(&mutex);
		send_queue.push_back(line);
		mutex_lock.unlock();
		send_event.set();
	}
}

IRCRawString IRCConnectionQueues::pop_send()
{
	CL_MutexSection mutex_lock(&mutex);
	if (send_queue.empty())
	{
		send_event.reset();
		return IRCRawString();
	}
	else
	{
		IRCRawString line = send_queue.front();
		send_queue.erase(send_queue.begin());
		return line;
	}
}

void IRCConnectionQueues::push_received(const IRCRawString &line)
{
	if (!line.empty())
	{
		CL_MutexSection mutex_lock(&mutex);
		receive_queue.push_back(line);
		mutex_lock.unlock();
		receive_event.set();
	}
}

IRCRawString IRCConnectionQueues::pop_received()
{
	CL_MutexSection mutex_lock(&mutex);
	if (receive_queue.empty())
	{
		receive_event.reset();
		return IRCRawString();
	}
	else
	{
		IRCRawString line = receive_queue.front();
		receive_queue.erase(receive_queue.begin());
		return line;
	}
}

void IRCConnectionQueues::set_disconnected(const CL_String &reason)
{
	CL_MutexSection mutex_lock(&mutex);
	disconnected_reason = reason;
	mutex_lock.unlock();
	disconnected.set(1);
	receive_event.set();
}

bool IRCConnectionQueues::pop_disconnected(CL_String &out_reason)
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
