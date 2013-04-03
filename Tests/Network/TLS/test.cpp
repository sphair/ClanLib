
#include <ClanLib/core.h>
#include <ClanLib/network.h>

#include <ctime>
using namespace clan;

void test();
void test2();

int main(int, char**)
{
	SetupCore setup_core;
	SetupNetwork setup_network;
	try
	{
		test2();

	}
	catch (Exception e)
	{
		Console::write_line(e.message);
		Console::wait_for_key();
	}
	return 0;
}


void test()
{
	SocketName socket_name("encrypted.google.com", "443");
	//CL_SocketName socket_name("www.facebook.com", "443");
	
	TCPConnection tcp_connection(socket_name);
	tcp_connection.set_nodelay(true);
	TLSConnection tls_connection(tcp_connection);

	std::string remote_hostname = "encrypted.google.com";
	std::string request = string_format(
		"GET / HTTP/1.1\r\n"
		"Host: %1\r\n"
		"Connection: close\r\n"
		"\r\n", remote_hostname);

	tls_connection.send(request.data(), request.length(), true);

	DataBuffer response(16*1024);
	while(true)
	{
		bool wakeup_reason = tcp_connection.get_read_event().wait(2000);	// Wait for data
		if (!wakeup_reason)
			break;

		int receieved = tls_connection.receive(response.get_data(), response.get_size(), false);
		if (receieved == 0)
			break;
		//CL_Console::write(CL_String(cl_format("%1\r\n", receieved)));
		Console::write(std::string((char *) response.get_data(), receieved));
	}

}

void test2()
{
	SocketName socket_name("encrypted.google.com", "443");

	// ConsoleWindow window("I have a secret, do you think you can keep it?");

	std::string remote_hostname = "encrypted.google.com";
	std::string request = string_format(
		"GET / HTTP/1.1\r\n"
		"Host: %1\r\n"
		"Connection: close\r\n"
		"\r\n", remote_hostname);

	TCPConnection connection(socket_name);
	TLSClient tls;

	tls.encrypt(request.data(), request.length());

	const int max_buffer_size = 16*1024;
	char buffer[max_buffer_size];
	int buffer_pos = 0;
	int buffer_size = 0;
	bool eof = false;
	while (true)
	{
		bool should_continue;
		do
		{
			should_continue = false;
			if (tls.get_encrypted_data_available() && connection.get_write_event().wait(0))
			{
				int bytes_sent = connection.write(tls.get_encrypted_data(), tls.get_encrypted_data_available(), false);
				tls.encrypted_data_consumed(bytes_sent);
				should_continue = true;
			}

			if (tls.get_decrypted_data_available())
			{
				Console::write(std::string((const char*)tls.get_decrypted_data(), tls.get_decrypted_data_available()));
				tls.decrypted_data_consumed(tls.get_decrypted_data_available());
				should_continue = true;
			}

			if (buffer_size < max_buffer_size && !eof && connection.get_read_event().wait(0))
			{
				int bytes_read = connection.read(buffer + buffer_size, max_buffer_size - buffer_size, false);
				buffer_size += bytes_read;
				if (bytes_read == 0)
					eof = true;
				else
					should_continue = true;
			}

			if (buffer_pos < buffer_size)
			{
				buffer_pos += tls.decrypt(buffer + buffer_pos, buffer_size - buffer_pos);
				if (buffer_pos == buffer_size)
				{
					buffer_pos = 0;
					buffer_size = 0;
				}
				should_continue = true;
			}

		} while(should_continue);

		if (eof)
			break;

		std::vector<Event> wait_events;
		if (!eof && buffer_size < max_buffer_size)
			wait_events.push_back(connection.get_read_event());
		if (tls.get_encrypted_data_available())
			wait_events.push_back(connection.get_write_event());

		Event::wait(wait_events);
	}

	// window.display_close_message();
}
