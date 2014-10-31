
#include "precomp.h"
#include "irc_connection_queues.h"

void IRCConnectionQueues::reset()
{
	clan::MutexSection mutex_lock(&mutex);
	send_event.reset();
	disconnected.set(0);
	send_queue.clear();
	receive_queue.clear();
}

void IRCConnectionQueues::push_send(const IRCRawString &line)
{
	if (!line.empty())
	{
		clan::MutexSection mutex_lock(&mutex);
		send_queue.push_back(line);
		send_event.set();
		mutex_lock.unlock();
	}
}

IRCRawString IRCConnectionQueues::pop_send()
{
	clan::MutexSection mutex_lock(&mutex);
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
		clan::MutexSection mutex_lock(&mutex);
		receive_queue.push_back(line);
		mutex_lock.unlock();
	}
}

IRCRawString IRCConnectionQueues::pop_received()
{
	clan::MutexSection mutex_lock(&mutex);
	if (receive_queue.empty())
	{
		return IRCRawString();
	}
	else
	{
		IRCRawString line = receive_queue.front();
		receive_queue.erase(receive_queue.begin());
		return line;
	}
}

bool IRCConnectionQueues::is_receive_queue_empty()
{
	clan::MutexSection mutex_lock(&mutex);
	return receive_queue.empty();
}

void IRCConnectionQueues::set_disconnected(const std::string &reason)
{
	clan::MutexSection mutex_lock(&mutex);
	disconnected_reason = reason;
	disconnected.set(1);
	mutex_lock.unlock();
}

bool IRCConnectionQueues::pop_disconnected(std::string &out_reason)
{
	bool was_disconnected = (disconnected.get() != 0);
	if (was_disconnected)
	{
		clan::MutexSection mutex_lock(&mutex);
		disconnected.set(0);
		out_reason = disconnected_reason;
		return true;
	}
	else
	{
		return false;
	}
}
