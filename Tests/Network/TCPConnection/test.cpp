
#include <ClanLib/core.h>
#include <ClanLib/network.h>
#include <array>

using namespace clan;

void test1();
void test2();
void send_request(TCPConnection &connection);
DataBuffer receive_response(TCPConnection &connection);

int main(int, char**)
{
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

	uint64_t start_time = System::get_time();
	Console::write_line("Time start: %1", start_time);
	Console::write_line("");
	for (int i = 0; i < 10; i++)
	{
		send_request(connection);
		receive_response(connection);
		Console::write_line("");
	}
	uint64_t end_time = System::get_time();
	Console::write_line("Time end: %1", end_time);
	Console::write_line("Delta time: %1", (end_time - start_time));

	Console::write_line("");
	Console::write_line("--- End of test1 ---");
	Console::write_line("");
}

void send_request(TCPConnection &connection)
{
	std::string request =
		"HEAD /index.html HTTP/1.1\r\n"
		"Host: www.clanlib.org\r\n"
		"\r\n";

	NetworkConditionVariable wait_condition;
	std::mutex mutex;
	std::unique_lock<std::mutex> lock(mutex);

	size_t pos = 0;
	while (pos < request.length())
	{
		int bytes_written;
		while (true)
		{
			bytes_written = connection.write(request.data() + pos, request.length() - pos);
			if (bytes_written != -1)
				break;

			NetworkEvent *events[] = { &connection };
			wait_condition.wait(lock, 1, events);
		}
		pos += bytes_written;
	}
}

DataBuffer receive_response(TCPConnection &connection)
{
	std::array<char, 16000> response_data;

	NetworkConditionVariable wait_condition;
	std::mutex mutex;
	std::unique_lock<std::mutex> lock(mutex);

	size_t pos = 0;
	while (pos < response_data.size())
	{
		size_t last_pos = pos > 0 ? pos - 4 : pos;

		int bytes_read;
		while (true)
		{
			bytes_read = connection.read(response_data.data() + pos, response_data.size() - pos);
			if (bytes_read != -1)
				break;

			NetworkEvent *events[] = { &connection };
			wait_condition.wait(lock, 1, events);
		}
		pos += bytes_read;

		if (bytes_read == 0)
			break;

		bool end_header_found = false;
		for (size_t i = last_pos; i + 3 < pos; i++)
		{
			if (response_data[i] == '\r' && response_data[i + 1] == '\n' && response_data[i + 2] == '\r' && response_data[i + 3] == '\n')
			{
				end_header_found = true;
				break;
			}
		}

		if (end_header_found)
			break;
	}

	for (auto &line : StringHelp::split_text(std::string(response_data.data(), pos), "\r\n"))
	{
		Console::write_line(StringHelp::local8_to_text(line));
	}

	return DataBuffer();
}
