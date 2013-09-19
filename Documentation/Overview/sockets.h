
/*! \page Sockets Sockets

MainDocs:Sockets and DNS

From ClanLib Game SDK
 

The clanNetwork library provides four simple classes for basic TCP and UDP communication over the Internet: 
• CL_TCPListen 
• CL_TCPConnection 
• CL_UDPSocket 
• CL_SocketName 

Internet Protocol Basics

Although you can find much better information elsewhere on how the Internet works and how IP packet routing operates, we will give a small summary here of its operation from the point of an application. 

Typically applications communicate over the Internet or on a Local Area Network (LAN) using either the Transmission Control Protocol (TCP) or the User Datagram Protocol (UDP). The TCP protocol is used for connection based communication between two parties (like a phone call), while the UDP protocol is used to send individual messages without an actual connection (like sending a SMS). With both protocols you connect or send messages to a specific IP address. But because there may be many different programs running on a machine, the destination is also identified by a port. The combination of both is called a socket name and is represented by the class CL_SocketName in clanNetwork. For example, the Google search engine web server runs on the server www.google.com, port 80 (the HTTP protocol port). 

The CL_SocketName class in clanNetwork is designed so it can either hold a machine DNS name or an IP address of the server. This is because finding the DNS name for an IP address (or vice versa) may take time and therefore is only done when absolutely neccessary. However, it is easy enough to convert from one type to the other: 


CL_SocketName dnsname("www.google.com", "80");
CL_String ipaddress = dnsname.lookup_ipv4();
 
CL_SocketName ipname(ipaddress, "80");
CL_String dnsname = ipname.lookup_hostname();

Just remember that these two functions may block for a while when contacting the DNS server, so use them wisely. 

TCP Communication

Connecting to an existing server using TCP is fairly simple. If we want to fetch the HTML page for www.google.com, we might do this: 


CL_SocketName google_server("www.google.com", "80");
CL_TCPConnection connection(google_server);
 
CL_String8 http_request = 
	"GET / HTTP/1.1\r\n"
	"Host: www.google.com\r\n"
	"Connection: close\r\n"
	"\r\n";
connection.write(http_request.data(), http_request.length());
 
while(true)
{
	bool got_event = connection.get_read_event().wait(15000);
	if (!got_event)
		throw CL_Exception("timed out");
 
	char buffer[16*1024];
	int received = connection.read(buffer, 16*1024-1, false);
	if (received == 0)
		break;
	buffer[received] = 0;
	CL_Console::write(buffer);
}
 
connection.disconnect_graceful();

To create your own server that allow clients to connect to a port, you use the CL_TCPListen class: 


CL_SocketName listen_port("5555");
CL_TCPListen listen(listen_port);
 
// Wait 60 seconds for someone to connect:
bool someone_connected = listen.get_accept_event().wait(60*1000);
if (someone_connected)
{
	CL_TCPConnection connection = listen.accept();
	CL_SocketName remote_name = connection.get_remote_name();
	CL_Console::write_line("%1 connected", remote_name.get_address());
	CL_String8 messsage = "Hello There!";
	connection.write(message.data(), message.length());
	connection.disconnect_graceful();
}
else
{
	CL_Console::write_line("Nobody connected for 60 seconds");
}

UDP Communication

Talking via UDP is slightly different because there is no connection. Instead, you communicate by sending single message packets to an IP address and port: 


CL_SocketName our_port("5555");
CL_SocketName destination("example.net", "5678");
 
CL_UDPSocket udp;
udp.bind(our_port);
udp.send(message.data(), message.length(), destination);

If the UDP socket is not bound to a port, a random available port will be chosen when the first message is sent. The bind() command is therefore only needed for servers. 

Because UDP communication is connectionless, anyone can send messages to our socket and we retrieve the messages like this: 


// Wait 60 seconds for a message:
bool data_available = udp.get_read_event().wait(60*1000);
if (data_available)
{
	bool got_event = connection.get_read_event().wait(15000);
	if (!got_event)
		throw CL_Exception("timed out");
	char buffer[16*1024];
	CL_SocketName sender_name;
	int received = udp.receive(buffer, 16*1024, sender_name);
	CL_Console::write_line("%1 sent us %2 bytes of data!", sender_name.get_address(), received);


*/
