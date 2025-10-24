
#include "precomp.h"
#include "ircconnection.h"

/////////////////////////////////////////////////////////////////////////////
// IRCConnection construction:

IRCConnection::IRCConnection(const CL_String &server, const CL_String &port)
: CL_IRCConnection(server, port)
#if defined(netsession_implemented)
, netsession("CTalk"), ctalk_port("6001")
#endif
{
	slots.connect(sig_ping(), this, &IRCConnection::on_ping);
	slots.connect(sig_nick(), this, &IRCConnection::on_nick);
	slots.connect(sig_privmsg_ctcp(), this, &IRCConnection::on_privmsg_ctcp);
	slots.connect(sig_join(), this, &IRCConnection::on_join);
	slots.connect(sig_numeric_reply(), this, &IRCConnection::on_numeric_reply);
#if defined(netsession_implemented)
	slots.connect(netsession.sig_computer_connected(), this, &IRCConnection::on_computer_connected);
	slots.connect(netsession.sig_computer_reconnected(), this, &IRCConnection::on_computer_reconnected);
	slots.connect(netsession.sig_computer_disconnected(), this, &IRCConnection::on_computer_disconnected);
	slots.connect(netsession.sig_netpacket_receive("nickpacket"), this, &IRCConnection::on_nickpacket_receive);
	slots.connect(netsession.sig_netpacket_receive("typepacket"), this, &IRCConnection::on_typepacket_receive);
	netsession.start_listen(ctalk_port);
#endif
}

IRCConnection::~IRCConnection()
{
}

/////////////////////////////////////////////////////////////////////////////
// IRCConnection attributes:


/////////////////////////////////////////////////////////////////////////////
// IRCConnection operations:

#if defined(netsession_implemented)
void IRCConnection::send_type_packet(const CL_String &receiver, const CL_String &text)
{
	CL_NetPacket packet;
	packet.output.write_string(receiver);
	packet.output.write_string(text);
	netsession.get_all().send("typepacket", packet);
}
#endif

/////////////////////////////////////////////////////////////////////////////
// IRCConnection implementation:

void IRCConnection::on_ping(const CL_String &daemon1, const CL_String &daemon2)
{
	send_pong(daemon1, daemon2);
}

void IRCConnection::on_nick(const CL_String &old_nick, const CL_String &new_nick)
{
	if (CL_IRCConnection::extract_nick(old_nick) == nick)
		nick = CL_IRCConnection::extract_nick(new_nick);
}

void IRCConnection::on_numeric_reply(const CL_String &prefix, int command, const std::vector<CL_String> &params)
{
	if (command == RPL_WELCOME)
	{
		std::vector<CL_String> params;
		params.push_back(nick);
		send_command("", "USERHOST", params);
	}
	else if (command == RPL_USERHOST && our_hostname.empty() && params.size() >= 2)
	{
		CL_String::size_type pos = params[1].find("=+");
		if (pos == CL_String::npos) pos = params[1].find("=-");
		if (pos != CL_String::npos)
		{
			our_hostname = params[1].substr(pos+2);
			pos = our_hostname.find("@");
			if (pos != CL_String::npos)
			{
				our_hostname = CL_StringHelp::trim(our_hostname.substr(pos+1));
			}
		}
	}
}

void IRCConnection::on_privmsg_ctcp(const CL_String &prefix, const CL_String &receiver, const CL_String &command, const CL_String &data)
{
#if defined(netsession_implemented)
	if (command == "CTALK")
	{
		std::vector<CL_TempString> args = CL_StringHelp::split_text(data, " ");
		if (args.size() < 2) return;
		CL_String hostname = args[0];
		CL_String port = args[1];

		CL_String from = prefix;
		CL_String::size_type pos = from.find_first_of('!');
		if (pos != CL_String::npos) from = from.substr(0, pos);

		// todo: walk list of hosts we are already connected and avoid connect if we already are.
		CL_NetComputer computer = netsession.connect_async(hostname, port);
		computer_to_nick[computer] = from;
	}
#endif
}

void IRCConnection::on_join(const CL_String &joinnick, const CL_String &channel)
{
#if defined(netsession_implemented)
	if (joinnick.substr(0, nick.length()) == nick)
	{
		send_privmsg_ctcp(channel, "CTALK", our_hostname + " " + ctalk_port);
	}
#endif
}

#if defined(netsession_implemented)
void IRCConnection::on_computer_connected(CL_NetComputer &computer)
{
	CL_NetPacket packet;
	packet.output.write_int32(0);
	packet.output.write_string(nick);
	computer.send("nickpacket", packet);
}

void IRCConnection::on_computer_reconnected(CL_NetComputer &computer)
{
	on_computer_connected(computer);
}

void IRCConnection::on_computer_disconnected(CL_NetComputer &computer)
{
	std::map<CL_NetComputer, CL_String>::iterator it;
	it = computer_to_nick.find(computer);
	if (it != computer_to_nick.end())
	{
		// Hmm crap. We lost connection to a guy (or he might be firewalled). Ask him to reconnect:
		send_privmsg_ctcp(it->second, "CTALK", our_hostname + " " + ctalk_port);
		computer_to_nick.erase(it);
	}
}

void IRCConnection::on_nickpacket_receive(CL_NetPacket &packet, CL_NetComputer &computer)
{
	int type = packet.input.read_int32();
	CL_String computer_nick = packet.input.read_string();

	if (type == 0)
	{
		CL_NetPacket packet;
		packet.output.write_int32(1);
		packet.output.write_string(nick);
		computer.send("nickpacket", packet);
	}

	computer_to_nick[computer] = computer_nick;
}

void IRCConnection::on_typepacket_receive(CL_NetPacket &packet, CL_NetComputer &computer)
{
	std::map<CL_NetComputer, CL_String>::iterator it;
	it = computer_to_nick.find(computer);
	if (it == computer_to_nick.end()) return;

	CL_String from = it->second;
	CL_String receiver = packet.input.read_string();
	CL_String text = packet.input.read_string();
	sig_typepacket(from, receiver, text);
}
#endif
