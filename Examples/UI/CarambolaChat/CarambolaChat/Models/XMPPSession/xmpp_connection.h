
#pragma once

#include "xmpp_connection_queues.h"
#include "xmpp_token.h"

class XMPPConnection : public CL_KeepAliveObject
{
public:
	XMPPConnection();
	~XMPPConnection();

	void connect(const CL_SocketName &server, const CL_String &username, const CL_String &password, const CL_String &domain);
	void disconnect_graceful();
	void disconnect_abortive();
	void send_stanza(const XMPPStanza &stanza);

	// on_stanza_received(const XMPPStanza &stanza)
	CL_Callback_v1<const XMPPStanza &> &func_stanza_received();

	// on_disconnected(const CL_String &reason)
	CL_Callback_v1<const CL_String &> &func_disconnected();

	void process();

private:
	void worker_main();
	bool read_connection_data(CL_TCPConnection &connection);
	bool write_connection_data(CL_TCPConnection &connection);

	CL_SocketName server;
	CL_String username, password, domain;
	CL_Thread thread_worker;
	CL_Event stop_event;
	CL_InterlockedVariable shutdown_graceful;
	XMPPConnectionQueues queues;
	XMPPToken stream_start;

	CL_Callback_v1<const XMPPStanza &> cb_stanza_received;
	CL_Callback_v1<const CL_String &> cb_disconnected;

	// Worker thread:

	void write_stream_start(CL_String target_hostname);
	XMPPToken read_stream_start();
	void read_features();
	void write_start_tls();
	void read_proceed();
	void auth_plain(const CL_String &username, const CL_String &password);
	void write_stream_end();
	void read_stream_end();

	XMPPToken read_token();
	void write_token(const XMPPToken &token);
	bool is_eof();
	unsigned char get_char();
	unsigned char peek_char();
	void try_read_more();

	CL_IODevice device;
	CL_Event read_event;
	CL_DataBuffer buffer;
	int pos, size;
	bool eof;
};
