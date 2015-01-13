
#include <ClanLib/core.h>
#include <ClanLib/network.h>
using namespace clan;

void test1();
void test2();
void send_request(TCPConnection &connection);
DataBuffer receive_response(TCPConnection &connection);
bool read_line(TCPConnection &connection, std::string &out_line);
bool read_lines(TCPConnection &connection, std::string &out_header_lines);

int main(int, char**)
{
	SetupCore setup_core;
	SetupNetwork setup_network;
	try
	{
		test1();
	}
	catch (Exception e)
	{
		Console::write_line(e.message);
	}
	return 0;
}

void test1()
{
	Console::write_line("");
	Console::write_line("--- Begin of test1 ---");
	Console::write_line("");

	SocketName socket_name("www.clanlib.org", "80");
	TCPConnection connection(socket_name);
	connection.set_nodelay(true);
	clan::uint64_t start_time = System::get_time();
	Console::write_line("Time start: %1", start_time);
	for (int i = 0; i < 10; i++)
	{
		send_request(connection);
		receive_response(connection);
	}
	clan::uint64_t end_time = System::get_time();
	Console::write_line("Time end: %1", end_time);
	Console::write_line("Delta time: %1", (end_time - start_time));

	Console::write_line("");
	Console::write_line("--- End of test1 ---");
	Console::write_line("");
}

void send_request(TCPConnection &connection)
{
	const char *request =
		"HEAD /index.html HTTP/1.1\r\n"
		"Host: www.clanlib.org\r\n"
		"\r\n";

	connection.write(request, (int) strlen(request));
}

DataBuffer receive_response(TCPConnection &connection)
{
	std::string response_line;
	read_line(connection, response_line);
	Console::write_line(StringHelp::local8_to_text(response_line));

	std::string header_lines;
	read_lines(connection, header_lines);

	return DataBuffer();
}

bool read_line(TCPConnection &connection, std::string &out_line)
{
	out_line.clear();
	while (out_line.length() < 1024)
	{
		char buffer[1024];

		bool wakeup_reason = connection.get_read_event().wait(15000);
		if (!wakeup_reason)
			throw Exception("read_line timed out");

		int bytes_read = connection.peek(buffer, 1024);
		if (bytes_read <= 0)
			break;
		std::string str(buffer, bytes_read, false);
		std::string::size_type pos = str.find("\r\n");
		if (pos == std::string::npos)
		{
			connection.receive(buffer, bytes_read);
			out_line += str;
		}
		else
		{
			connection.receive(buffer, pos + 2);
			out_line += str.substr(0, pos);
			return true;
		}
	}
	return false;
}

bool read_lines(TCPConnection &connection, std::string &out_header_lines)
{
	out_header_lines.clear();
	while (out_header_lines.length() < 32*1024)
	{
		char buffer[1024];
		int bytes_read = connection.peek(buffer, 1024);
		if (bytes_read <= 0)
			break;
		std::string str(buffer, bytes_read, false);
		std::string::size_type start_pos = out_header_lines.length();
		out_header_lines += str;
		std::string::size_type pos = out_header_lines.find("\r\n\r\n");
		if (pos == std::string::npos)
		{
			connection.receive(buffer, bytes_read);
		}
		else
		{
			connection.receive(buffer, pos+4-start_pos);
			out_header_lines.resize(pos+4);
			return true;
		}
	}
	return false;
}
