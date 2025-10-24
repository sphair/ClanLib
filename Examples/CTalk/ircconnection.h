
#pragma once

class IRCConnection : public CL_IRCConnection
{
//! Construction:
public:
	IRCConnection(const CL_String &server, const CL_String &port = "6667");
	~IRCConnection();

//! Attributes:
public:
	const CL_String &get_nick() const { return nick; }
	const CL_String &get_our_hostname() const { return our_hostname; }

	//: sig_typepacket(from, receiver, text)
	CL_Signal_v3<const CL_String &, const CL_String &, const CL_String &> sig_typepacket;

//! Operations:
public:
	void set_nick(const CL_String &new_nick) { nick = new_nick; }
#if defined(netsession_implemented)
	void send_type_packet(const CL_String &receiver, const CL_String &text);
#endif

//! Implementation:
private:
	void on_ping(const CL_String &daemon1, const CL_String &daemon2);
	void on_nick(const CL_String &old_nick, const CL_String &new_nick);
	void on_numeric_reply(const CL_String &prefix, int command, const std::vector<CL_String> &params);
	void on_privmsg_ctcp(const CL_String &prefix, const CL_String &receiver, const CL_String &command, const CL_String &data);
	void on_join(const CL_String &nick, const CL_String &channel);

#if defined(netsession_implemented)
	void on_computer_connected(CL_NetComputer &computer);
	void on_computer_reconnected(CL_NetComputer &computer);
	void on_computer_disconnected(CL_NetComputer &computer);
	void on_nickpacket_receive(CL_NetPacket &packet, CL_NetComputer &computer);
	void on_typepacket_receive(CL_NetPacket &packet, CL_NetComputer &computer);
#endif

	CL_SlotContainer slots;

	CL_String nick;
	CL_String our_hostname;

#if defined(netsession_implemented)
	CL_String ctalk_port;
	CL_NetSession netsession;
	std::map<CL_NetComputer, CL_String> computer_to_nick;
#endif
};
