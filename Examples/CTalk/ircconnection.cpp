
#include "precomp.h"
#include "ircconnection.h"

/////////////////////////////////////////////////////////////////////////////
// IRCConnection construction:

IRCConnection::IRCConnection(const std::string &server, const std::string &port)
: CL_IRCConnection(server, port), netsession("CTalk"), ctalk_port("6001")
{
	slots.connect(sig_ping(), this, &IRCConnection::on_ping);
	slots.connect(sig_privmsg_ctcp(), this, &IRCConnection::on_privmsg_ctcp);
	slots.connect(sig_join(), this, &IRCConnection::on_join);
	slots.connect(sig_numeric_reply(), this, &IRCConnection::on_numeric_reply);
	slots.connect(netsession.sig_computer_connected(), this, &IRCConnection::on_computer_connected);
	slots.connect(netsession.sig_computer_reconnected(), this, &IRCConnection::on_computer_reconnected);
	slots.connect(netsession.sig_computer_disconnected(), this, &IRCConnection::on_computer_disconnected);
	slots.connect(netsession.sig_netpacket_receive("nickpacket"), this, &IRCConnection::on_nickpacket_receive);
	slots.connect(netsession.sig_netpacket_receive("typepacket"), this, &IRCConnection::on_typepacket_receive);
	netsession.start_listen(ctalk_port);
}

IRCConnection::~IRCConnection()
{
}

/////////////////////////////////////////////////////////////////////////////
// IRCConnection attributes:


/////////////////////////////////////////////////////////////////////////////
// IRCConnection operations:

void IRCConnection::send_type_packet(const std::string &receiver, const std::string &text)
{
	CL_NetPacket packet;
	packet.output.write_string(receiver);
	packet.output.write_string(text);
	netsession.get_all().send("typepacket", packet);
}

/////////////////////////////////////////////////////////////////////////////
// IRCConnection implementation:

void IRCConnection::on_ping(const std::string &daemon1, const std::string &daemon2)
{
	send_pong(daemon1, daemon2);
}

void IRCConnection::on_numeric_reply(const std::string &prefix, int command, const std::vector<std::string> &params)
{
	if (command == RPL_WELCOME)
	{
		std::vector<std::string> params;
		params.push_back(nick);
		send_command("", "USERHOST", params);
	}
	else if (command == RPL_USERHOST && our_hostname.empty() && params.size() >= 2)
	{
		std::string::size_type pos = params[1].find("=+");
		if (pos == std::string::npos) pos = params[1].find("=-");
		if (pos != std::string::npos)
		{
			our_hostname = params[1].substr(pos+2);
			pos = our_hostname.find("@");
			if (pos != std::string::npos)
			{
				our_hostname = CL_String::trim_spaces(our_hostname.substr(pos+1));
			}
		}
	}
}

void IRCConnection::on_privmsg_ctcp(const std::string &prefix, const std::string &receiver, const std::string &command, const std::string &data)
{
	if (command == "CTALK")
	{
		std::vector<std::string> args = CL_String::tokenize(data, " ");
		if (args.size() < 2) return;
		std::string hostname = args[0];
		std::string port = args[1];

		std::string from = prefix;
		std::string::size_type pos = from.find_first_of('!');
		if (pos != std::string::npos) from = from.substr(0, pos);

		// todo: walk list of hosts we are already connected and avoid connect if we already are.

		CL_NetComputer computer = netsession.connect_async(hostname, port);
		computer_to_nick[computer] = from;
	}
}

void IRCConnection::on_join(const std::string &joinnick, const std::string &channel)
{
	if (joinnick.substr(0, nick.length()) == nick)
	{
		send_privmsg_ctcp(channel, "CTALK", our_hostname + " " + ctalk_port);
	}
}

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
	std::map<CL_NetComputer, std::string>::iterator it;
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
	std::string computer_nick = packet.input.read_string();

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
	std::map<CL_NetComputer, std::string>::iterator it;
	it = computer_to_nick.find(computer);
	if (it == computer_to_nick.end()) return;

	std::string from = it->second;
	std::string receiver = packet.input.read_string();
	std::string text = packet.input.read_string();
	sig_typepacket(from, receiver, text);
}
