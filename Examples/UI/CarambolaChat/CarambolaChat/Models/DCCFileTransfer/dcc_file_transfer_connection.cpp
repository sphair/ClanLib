
#include "precomp.h"
#include "dcc_file_transfer_connection.h"

DCCFileTransferConnection::DCCFileTransferConnection(const clan::SocketName &socket_name)
: socket_name(socket_name), bytes_received(0), status(status_not_started)
{
}

DCCFileTransferConnection::~DCCFileTransferConnection()
{
	abort_event.set();
	thread.join();
}

void DCCFileTransferConnection::start(const std::string &output_filename)
{
	clan::MutexSection mutex_lock(&mutex);
	if (status != status_not_started)
		throw clan::Exception("DCC file transfer can only be started once");

	status = status_connecting;
	status_text = "Connecting..";
	filename = output_filename;
	mutex_lock.unlock();
	abort_event.reset();
	thread.start(this, &DCCFileTransferConnection::worker_main);
}

DCCFileTransferConnection::Status DCCFileTransferConnection::get_status()
{
	clan::MutexSection mutex_lock(&mutex);
	return status;
}

std::string DCCFileTransferConnection::get_status_text()
{
	clan::MutexSection mutex_lock(&mutex);
	return status_text;
}

unsigned int DCCFileTransferConnection::get_bytes_received()
{
	clan::MutexSection mutex_lock(&mutex);
	return bytes_received;
}

void DCCFileTransferConnection::abort()
{
	abort_event.set();
}

void DCCFileTransferConnection::worker_main()
{
	try
	{
		connection = clan::TCPConnection(socket_name);
		connection.set_big_endian_mode();
		connection.set_nodelay(true);
		clan::MutexSection mutex_lock(&mutex);
		status = status_receiving;
		status_text = "Downloading..";
		mutex_lock.unlock();

		clan::File outputfile(filename, clan::File::create_always, clan::File::access_write, clan::File::share_read);
		clan::DataBuffer buffer(1024*1024);
		int total_received_counter = 0;
		while (true)
		{
			int wakeup_reason = clan::Event::wait(abort_event, connection.get_read_event(), 60*1000);
			if (wakeup_reason == 1)
			{
				int received = connection.read(buffer.get_data(), buffer.get_size(), false);
				if (received == 0)
					break;
				outputfile.write(buffer.get_data(), received);
				total_received_counter += received;
				connection.write_int32(total_received_counter);

				mutex_lock.lock();
				bytes_received += received;
				mutex_lock.unlock();
			}
			else if (wakeup_reason == 0)
			{
				mutex_lock.lock();
				status = status_aborted;
				status_text = "Abort requested by user";
				return;
			}
			else
			{
				mutex_lock.lock();
				status = status_error;
				status_text = "Received no data for 60 seconds and gave up";
				return;
			}
		}

		mutex_lock.lock();
		status = status_finished_transfer;
		status_text = "Transfer complete";
		connection.disconnect_graceful();
	}
	catch (clan::Exception &e)
	{
		connection = clan::TCPConnection();
		clan::MutexSection mutex_lock(&mutex);
		status = status_error;
		status_text = e.message;
	}
}
