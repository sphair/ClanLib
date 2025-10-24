
#include <ClanLib/core.h>
#include <ClanLib/network.h>

void test1();
void test2();
void send_request(CL_TCPConnection &connection);
CL_DataBuffer receive_response(CL_TCPConnection &connection);
bool read_line(CL_TCPConnection &connection, CL_String8 &out_line);
bool read_lines(CL_TCPConnection &connection, CL_String8 &out_header_lines);

int main(int, char**)
{
	CL_SetupCore setup_core;
	CL_SetupNetwork setup_network;
	try
	{
		test1();
		test2();
	}
	catch (CL_Exception e)
	{
		CL_Console::write_line(e.message);
	}
	return 0;
}

void test1()
{
	CL_Console::write_line("");
	CL_Console::write_line("--- Begin of test1 ---");
	CL_Console::write_line("");

	CL_SocketName socket_name("www.clanlib.org", "80");
	CL_TCPConnection connection(socket_name);
	connection.set_nodelay(true);
	unsigned int start_time = CL_System::get_time();
	CL_Console::write_line("Time start: %1", start_time);
	for (int i = 0; i < 10; i++)
	{
		send_request(connection);
		receive_response(connection);
	}
	unsigned int end_time = CL_System::get_time();
	CL_Console::write_line("Time end: %1", end_time);
	CL_Console::write_line("Delta time: %1", (end_time - start_time));

	CL_Console::write_line("");
	CL_Console::write_line("--- End of test1 ---");
	CL_Console::write_line("");
}

void test2()
{
	CL_Console::write_line("");
	CL_Console::write_line("--- Begin of test2 ---");
	CL_Console::write_line("");

	CL_SocketName socket_name("www.clanlib.org", "80");
	CL_HTTPClientConnection connection(socket_name);
	unsigned int start_time = CL_System::get_time();
	CL_Console::write_line("Time start: %1", start_time);

	for (int i = 0; i < 10; i++)
	{
		CL_String8 headers = "Host: www.clanlib.org";
		connection.send_get("/index.html", headers);

		CL_String8 status_text, response_headers;
		CL_DataBuffer response_data;
		int status_code = connection.receive_response(status_text, response_headers, response_data);
		CL_Console::write_line(status_text);
	}

	unsigned int end_time = CL_System::get_time();
	CL_Console::write_line("Time end: %1", end_time);
	CL_Console::write_line("Delta time: %1", (end_time - start_time));

	CL_Console::write_line("");
	CL_Console::write_line("--- End of test2 ---");
	CL_Console::write_line("");
}

void send_request(CL_TCPConnection &connection)
{
	const char *request =
		"HEAD /index.html HTTP/1.1\r\n"
		"Host: www.clanlib.org\r\n"
		"\r\n";

	connection.write(request, (int) strlen(request));
}

CL_DataBuffer receive_response(CL_TCPConnection &connection)
{
	CL_String8 response_line;
	read_line(connection, response_line);
	CL_Console::write_line(CL_StringHelp::local8_to_text(response_line));

	CL_String8 header_lines;
	read_lines(connection, header_lines);

	return CL_DataBuffer();
}

bool read_line(CL_TCPConnection &connection, CL_String8 &out_line)
{
	out_line.clear();
	while (out_line.length() < 1024)
	{
		char buffer[1024];

		bool wakeup_reason = connection.get_read_event().wait(15000);
		if (!wakeup_reason)
			throw CL_Exception("read_line timed out");

		int bytes_read = connection.peek(buffer, 1024);
		if (bytes_read <= 0)
			break;
		CL_StringRef8 str(buffer, bytes_read, false);
		CL_StringRef8::size_type pos = str.find("\r\n");
		if (pos == CL_StringRef8::npos)
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

bool read_lines(CL_TCPConnection &connection, CL_String8 &out_header_lines)
{
	out_header_lines.clear();
	while (out_header_lines.length() < 32*1024)
	{
		char buffer[1024];
		int bytes_read = connection.peek(buffer, 1024);
		if (bytes_read <= 0)
			break;
		CL_StringRef8 str(buffer, bytes_read, false);
		CL_StringRef8::size_type start_pos = out_header_lines.length();
		out_header_lines += str;
		CL_StringRef8::size_type pos = out_header_lines.find("\r\n\r\n");
		if (pos == CL_StringRef8::npos)
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
