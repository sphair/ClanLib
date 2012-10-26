
#include <ClanLib/core.h>
#include <ClanLib/network.h>

#include <ctime>

void test();

int main(int, char**)
{
	CL_SetupCore setup_core;
	CL_SetupNetwork setup_network;
	try
	{
		test();

	}
	catch (CL_Exception e)
	{
		CL_Console::write_line(e.message);
		CL_Console::wait_for_key();
	}
	return 0;
}


void test()
{
	CL_SocketName socket_name("encrypted.google.com", "443");
	//CL_SocketName socket_name("www.facebook.com", "443");
	
	CL_TCPConnection tcp_connection(socket_name);
	tcp_connection.set_nodelay(true);
	CL_TLSConnection tls_connection(tcp_connection);

	CL_String remote_hostname = "encrypted.google.com";
	CL_String request = cl_format(
		"GET / HTTP/1.1\r\n"
		"Host: %1\r\n"
		"Connection: close\r\n"
		"\r\n", remote_hostname);

	tls_connection.send(request.data(), request.length(), true);

	CL_DataBuffer response(16*1024);
	while(true)
	{
		bool wakeup_reason = tcp_connection.get_read_event().wait(2000);	// Wait for data
		if (!wakeup_reason)
			break;

		int receieved = tls_connection.receive(response.get_data(), response.get_size(), false);
		if (receieved == 0)
			break;
		//CL_Console::write(CL_String(cl_format("%1\r\n", receieved)));
		CL_Console::write(CL_String((char *) response.get_data(), receieved));
	}

}

