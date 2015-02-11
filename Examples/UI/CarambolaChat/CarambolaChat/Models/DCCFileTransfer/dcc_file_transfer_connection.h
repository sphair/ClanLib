
#pragma once

class DCCFileTransferConnection
{
public:
	DCCFileTransferConnection(const clan::SocketName &socket_name);
	~DCCFileTransferConnection();

	enum Status
	{
		status_not_started,
		status_connecting,
		status_receiving,
		status_finished_transfer,
		status_aborted,
		status_error
	};

	void start(const std::string &output_filename);
	Status get_status();
	std::string get_status_text();
	unsigned int get_bytes_received();
	void abort();

private:
	void worker_main();

	clan::SocketName socket_name;
	std::string filename;
	clan::TCPConnection connection;
	std::thread thread;
	std::mutex mutex;
	clan::NetworkConditionVariable change_event;
	bool abort_flag = false;
	unsigned int bytes_received;
	Status status;
	std::string status_text;
};
