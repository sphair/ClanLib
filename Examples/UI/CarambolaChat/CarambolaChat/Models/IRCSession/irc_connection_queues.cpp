
#include "precomp.h"
#include "irc_connection_queues.h"

void IRCConnectionQueues::reset()
{
	disconnected = false;
	send_queue.clear();
	receive_queue.clear();
}

void IRCConnectionQueues::push_send(const IRCRawString &line)
{
	if (!line.empty())
	{
		send_queue.push_back(line);
	}
}

IRCRawString IRCConnectionQueues::pop_send()
{
	if (send_queue.empty())
	{
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
		receive_queue.push_back(line);
	}
}

IRCRawString IRCConnectionQueues::pop_received()
{
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
	return receive_queue.empty();
}

void IRCConnectionQueues::set_disconnected(const std::string &reason)
{
	disconnected_reason = reason;
	disconnected = true;
}

bool IRCConnectionQueues::pop_disconnected(std::string &out_reason)
{
	if (disconnected)
	{
		disconnected = false;
		out_reason = disconnected_reason;
		return true;
	}
	else
	{
		return false;
	}
}
