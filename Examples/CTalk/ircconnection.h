
#ifndef file_ircconnection
#define file_ircconnection

class IRCConnection : public CL_IRCConnection
{
//! Construction:
public:
	IRCConnection(const std::string &server, const std::string &port = "6667");

	~IRCConnection();

//! Attributes:
public:
	const std::string &get_nick() const { return nick; }

	const std::string &get_our_hostname() const { return our_hostname; }

	//: sig_typepacket(from, receiver, text)
	CL_Signal_v3<const std::string &, const std::string &, const std::string &> sig_typepacket;

//! Operations:
public:
	void set_nick(const std::string &new_nick) { nick = new_nick; }

	void send_type_packet(const std::string &receiver, const std::string &text);

//! Implementation:
private:
	void on_ping(const std::string &daemon1, const std::string &daemon2);

	void on_numeric_reply(const std::string &prefix, int command, const std::vector<std::string> &params);

	void on_privmsg_ctcp(const std::string &prefix, const std::string &receiver, const std::string &command, const std::string &data);

	void on_join(const std::string &nick, const std::string &channel);

	void on_computer_connected(CL_NetComputer &computer);

	void on_computer_reconnected(CL_NetComputer &computer);

	void on_computer_disconnected(CL_NetComputer &computer);

	void on_nickpacket_receive(CL_NetPacket &packet, CL_NetComputer &computer);

	void on_typepacket_receive(CL_NetPacket &packet, CL_NetComputer &computer);

	CL_SlotContainer slots;

	std::string nick;

	CL_NetSession netsession;

	std::string our_hostname, ctalk_port;

	std::map<CL_NetComputer, std::string> computer_to_nick;
};

#endif
