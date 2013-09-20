
/*! \page Sockets Sockets

The clanNetwork library provides four simple classes for basic TCP and UDP communication over the Internet:

<ul>
<li>\ref clan::TCPListen </li>
<li>\ref clan::TCPConnection</li>
<li>\ref clan::UDPSocket</li>
<li>\ref clan::SocketName </li>
</ul>

<h2>Internet Protocol Basics</h2>

Although you can find much better information elsewhere on how the Internet works and how IP packet routing operates, we will give a small summary here of
its operation from the viewpoint of an application. 

Typically applications communicate over the Internet or on a Local Area Network (LAN) using either the Transmission Control Protocol (TCP) or the User Datagram
Protocol (UDP). The TCP protocol is used for connection based communication between two parties (like a phone call), while the UDP protocol is used to send
individual messages without an actual connection (like sending a SMS). With both protocols you connect or send messages to a specific IP address. But because
there may be many different programs running on a machine, the destination is also identified by a port. The combination of both is called a socket name and
is represented by the class clan::SocketName in clanNetwork. For example, the Google search engine web server runs on the server www.google.com, port 80 (the HTTP protocol port). 

The \ref clan::SocketName class in clanNetwork is designed so it can either hold a machine DNS name or an IP address of the server. This is because finding
the DNS name for an IP address (or vice versa) may take time and therefore is only done when absolutely neccessary. However, it is easy enough to convert
from one type to the other: 

\code

clan::SocketName dnsname("www.google.com", "80");
clan::String ipaddress = dnsname.lookup_ipv4();
 
clan::SocketName ipname(ipaddress, "80");
clan::String dnsname = ipname.lookup_hostname();
\endcode

Just remember that these two functions may block for a while when contacting the DNS server, so use them wisely. 

<h2>TCP Communication</h2>

Connecting to an existing server using TCP is fairly simple. If we want to fetch the HTML page for www.google.com, we might do this: 

\code

clan::SocketName google_server("www.google.com", "80");
clan::TCPConnection connection(google_server);
 
std::string http_request = 
	"GET / HTTP/1.1\r\n"
	"Host: www.google.com\r\n"
	"Connection: close\r\n"
	"\r\n";
connection.write(http_request.data(), http_request.length());
 
while(true)
{
	bool got_event = connection.get_read_event().wait(15000);
	if (!got_event)
		throw clan::Exception("timed out");
 
	char buffer[16*1024];
	int received = connection.read(buffer, 16*1024-1, false);
	if (received == 0)
		break;
	buffer[received] = 0;
	clan::Console::write(buffer);
}

connection.disconnect_graceful();
\endcode

To create your own server that allow clients to connect to a port, you use the \ref clan::TCPListen class: 

\code

clan::SocketName listen_port("5555");
clan::TCPListen listen(listen_port);
 
// Wait 60 seconds for someone to connect:
bool someone_connected = listen.get_accept_event().wait(60*1000);
if (someone_connected)
{
	clan::TCPConnection connection = listen.accept();
	clan::SocketName remote_name = connection.get_remote_name();
	clan::Console::write_line("%1 connected", remote_name.get_address());
	clan::String8 messsage = "Hello There!";
	connection.write(message.data(), message.length());
	connection.disconnect_graceful();
}
else
{
	clan::Console::write_line("Nobody connected for 60 seconds");
}
\endcode

<h2>UDP Communication</h2>

Talking via UDP is slightly different because there is no connection. Instead, you communicate by sending single message packets to an IP address and port: 

\code

clan::SocketName our_port("5555");
clan::SocketName destination("example.net", "5678");
 
clan::UDPSocket udp;
udp.bind(our_port);
udp.send(message.data(), message.length(), destination);
\endcode

If the UDP socket is not bound to a port, a random available port will be chosen when the first message is sent. The bind() command is therefore only needed for servers. 

Because UDP communication is connectionless, anyone can send messages to our socket and we retrieve the messages like this: 

\code
// Wait 60 seconds for a message:
bool data_available = udp.get_read_event().wait(60*1000);
if (data_available)
{
	bool got_event = connection.get_read_event().wait(15000);
	if (!got_event)
		throw clan::Exception("timed out");

	char buffer[16*1024];
	clan::SocketName sender_name;
	int received = udp.receive(buffer, 16*1024, sender_name);

	clan::Console::write_line("%1 sent us %2 bytes of data!", sender_name.get_address(), received);
}

\endcode

*/
