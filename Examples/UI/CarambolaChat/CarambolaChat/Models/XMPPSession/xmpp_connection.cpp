
#include "precomp.h"
#include "xmpp_connection.h"
#include "TLSConnection/setup_nss.h"
#include "TLSConnection/tls_connection.h"

XMPPConnection::XMPPConnection()
: buffer(16*1024), pos(0), size(0), eof(false)
{
}

XMPPConnection::~XMPPConnection()
{
	disconnect_abortive();
}

void XMPPConnection::connect(const CL_SocketName &new_server, const CL_String &new_username, const CL_String &new_password, const CL_String &new_domain)
{
	disconnect_abortive();
	stop_event.reset();
	queues.reset();
	shutdown_graceful.set(0);
	pos = 0;
	size = 0;
	eof = false;
	server = new_server;
	username = new_username;
	password = new_password;
	domain = new_domain;
	
	thread_worker.start(this, &XMPPConnection::worker_main);
}

void XMPPConnection::disconnect_graceful()
{
	shutdown_graceful.set(1);
	stop_event.set();
	thread_worker.join();
}

void XMPPConnection::disconnect_abortive()
{
	shutdown_graceful.set(0);
	stop_event.set();
	thread_worker.join();
}

void XMPPConnection::send_stanza(const XMPPStanza &stanza)
{
	queues.push_send(stanza);
}

CL_Callback_v1<const XMPPStanza &> &XMPPConnection::func_stanza_received()
{
	return cb_stanza_received;
}

CL_Callback_v1<const CL_String &> &XMPPConnection::func_disconnected()
{
	return cb_disconnected;
}

void XMPPConnection::process()
{
	while (true)
	{
		XMPPStanza stanza = queues.pop_received();
		if (stanza.empty())
			break;

		if (!cb_stanza_received.is_null())
			cb_stanza_received.invoke(stanza);
	}

	CL_String reason;
	bool was_disconnected = queues.pop_disconnected(reason);
	if (was_disconnected && !cb_disconnected.is_null())
		cb_disconnected.invoke(reason);
}

void XMPPConnection::worker_main()
{
	try
	{
		//CL_SetupNSS setup_nss("C:\\Users\\mbn\\AppData\\Roaming\\Mozilla\\Firefox\\Profiles\\4hljaxwk.default");
		CL_SetupNSS setup_nss("");

		CL_TCPConnection connection(server);
		connection.set_nodelay(true);
		connection.set_keep_alive(true, 60*1000, 60*1000);

		device = connection;
		read_event = connection.get_read_event();

		write_stream_start(domain);
		read_stream_start();
		read_features();
		write_start_tls();
		read_proceed();

		TLSConnection encrypted(connection);
		encrypted.handshake(server.get_address(), false);

		device = encrypted;
		read_event = encrypted.get_read_event();

		write_stream_start(domain);
		read_stream_start();
		read_features();

		auth_plain(username, password);

		write_stream_start(domain);
		stream_start = read_stream_start();

		CL_Event write_event = encrypted.get_write_event();

		std::vector<CL_Event> wait_events;
		bool continue_loop = true;
		while (continue_loop)
		{
			wait_events.clear();
			wait_events.push_back(read_event);
			wait_events.push_back(queues.send_event);

			bool empty_buffers = (shutdown_graceful.get() != 0);
			if (!empty_buffers)
				wait_events.push_back(stop_event);

			int wakeup_reason = CL_Event::wait(wait_events, empty_buffers ? 5000 : -1);
			if (wakeup_reason == 0) // read_event flagged
			{
				continue_loop = read_connection_data(connection);
			}
			else if (wakeup_reason == 1)
			{
				continue_loop = write_connection_data(connection);
			}
			else if (wakeup_reason == 2 && wait_events.size() == 3) // stop_event flagged
			{
				continue_loop = (shutdown_graceful.get() != 0);
			}
			else // timed out
			{
				continue_loop = false;
			}
		}

		if (shutdown_graceful.get())
			encrypted.disconnect_graceful();
		queues.set_disconnected("Connection closed by server");
		set_wakeup_event();
	}
	catch (CL_Exception &e)
	{
		queues.set_disconnected(e.message);
		set_wakeup_event();
	}

	read_event = CL_Event();
	device = CL_IODevice();
}

bool XMPPConnection::read_connection_data(CL_TCPConnection &connection)
{
	bool eof = is_eof();
	while (pos != size)
	{
		if (peek_char() != '<')
		{
			unsigned char c = get_char();
			if (c != ' ' && c != '\t' && c != '\r' && c != '\n')
				throw CL_Exception("Text not allowed at stream level");
		}
		else
		{
			XMPPStanza stanza;
			XMPPToken token = read_token();
			stanza.tokens.push_back(token);
			int level = (token.type == XMPPToken::type_begin) ? 1 : 0;
			while (level > 0)
			{
				XMPPToken token = read_token();
				stanza.tokens.push_back(token);
				if (token.type == XMPPToken::type_begin)
					level++;
				else if (token.type == XMPPToken::type_end)
					level--;
			}
			stanza.stream_token = stream_start;
			queues.push_received(stanza);
			set_wakeup_event();
		}
	}
	return !eof;
}

bool XMPPConnection::write_connection_data(CL_TCPConnection &connection)
{
	XMPPStanza stanza = queues.pop_send();
	for (size_t i = 0; i < stanza.tokens.size(); i++)
		write_token(stanza.tokens[i]);
	return true;
}

XMPPToken XMPPConnection::read_token()
{
	XMPPToken token;

	unsigned char c = get_char();
	if (c == '<')
	{
		c = get_char();
		if (c == '/')
		{
			token.type = XMPPToken::type_end;
		}
		else
		{
			token.type = XMPPToken::type_begin;
			token.name.append(1, c);
		}

		while (true)
		{
			c = get_char();
			if (c == ' ' || c == '/' || c == '>')
				break;
			token.name.append(1, c);
		}

		while (true)
		{
			while (c == ' ')
				c = get_char();
			if (c == '/' || c == '>')
				break;

			CL_String name;
			name.append(1, c);
			while (true)
			{
				c = get_char();
				if (c == ' ' || c == '=')
					break;
				name.append(1, c);
			}

			while (c == ' ')
				c = get_char();

			if (c != '=')
				throw CL_Exception("Malformed XMPP tag");
			c = get_char();

			while (c == ' ')
				c = get_char();

			CL_String value;
			if (c == '"' || c == '\'')
			{
				unsigned char type = c;
				while (true)
				{
					// To do: support escapes
					c = get_char();
					if (c == type)
						break;
					value.append(1, c);
				}
				c = get_char();
			}
			else
			{
				value.append(1, c);
				while (true)
				{
					c = get_char();
					if (c == ' ' || c == '/' || c == '>')
						break;
					value.append(1, c);
				}
			}

			token.attributes.push_back(XMPPAttribute(name, value));
		}

		if (c == '/')
		{
			c = get_char();
			if (c != '>' || token.type == XMPPToken::type_end)
				throw CL_Exception("Malformed XMPP tag");
			token.type = XMPPToken::type_single;
		}
		else if (c != '>')
		{
			throw CL_Exception("Malformed XMPP tag");
		}
	}
	else
	{
		token.type = XMPPToken::type_text;
		token.value.append(1, c);
		while (peek_char() != '<')
			token.value.append(1, get_char());
	}

	return token;
}

bool XMPPConnection::is_eof()
{
	if (!eof && pos == size)
		try_read_more();
	return eof;
}

unsigned char XMPPConnection::peek_char()
{
	if (is_eof())
		throw CL_Exception("XMPP connection closed by remote host");
	return (unsigned char)buffer.get_data()[pos];
}

unsigned char XMPPConnection::get_char()
{
	unsigned char c = peek_char();
	pos++;
	return c;
}

void XMPPConnection::try_read_more()
{
	if (!eof && pos == size)
	{
		if (!read_event.wait(15000))
			throw CL_Exception("read_token timed out");
		int received = device.receive(buffer.get_data(), buffer.get_size(), false);
		eof = (received == 0);
		pos = 0;
		size = received;
	}
}

void XMPPConnection::write_token(const XMPPToken &token)
{
	if (token.type == XMPPToken::type_begin || token.type == XMPPToken::type_single)
	{
		CL_String str = cl_format("<%1", token.name);
		device.write(str.data(), str.length(), true);
		for (size_t i = 0; i < token.attributes.size(); i++)
		{
			CL_String str = cl_format(" %1='%2'", token.attributes[i].name, token.attributes[i].value);
			device.write(str.data(), str.length(), true);
		}
		if (token.type == XMPPToken::type_single)
			device.write("/>", 2, true);
		else
			device.write(">", 1, true);
	}
	else if (token.type == XMPPToken::type_end)
	{
		CL_String str = cl_format("</%1>", token.name);
		device.write(str.data(), str.length(), true);
	}
	else if (token.type == XMPPToken::type_text)
	{
		device.write(token.value.data(), token.value.length(), true);
	}
}

void XMPPConnection::read_features()
{
/*
   <stream:features>
     <starttls xmlns='urn:ietf:params:xml:ns:xmpp-tls'>
       <required/>
     </starttls>
     <mechanisms xmlns='urn:ietf:params:xml:ns:xmpp-sasl'>
       <mechanism>DIGEST-MD5</mechanism>
       <mechanism>PLAIN</mechanism>
     </mechanisms>
   </stream:features>
*/
	XMPPToken token = read_token();
	if (token.type != XMPPToken::type_begin || token.name != "stream:features")
		throw CL_Exception("Expected <features> tag");

	int level = 1;
	while (level > 0)
	{
		XMPPToken token = read_token();
		if (token.type == XMPPToken::type_begin)
			level++;
		else if (token.type == XMPPToken::type_end)
			level--;
	}
}

void XMPPConnection::write_start_tls()
{
	XMPPToken token;
	token.type = XMPPToken::type_single;
	token.name = "starttls";
	token.attributes.push_back(XMPPAttribute("xmlns", "urn:ietf:params:xml:ns:xmpp-tls"));
	write_token(token);
}

void XMPPConnection::read_proceed()
{
	XMPPToken token = read_token();
	if (token.type != XMPPToken::type_single || token.name != "proceed")
		throw CL_Exception("Expected <proceed> tag");
}

void XMPPConnection::write_stream_start(CL_String target_hostname)
{
	XMPPToken token;
	token.type = XMPPToken::type_begin;
	token.name = "stream:stream";
	token.attributes.push_back(XMPPAttribute("xmlns", "jabber:client"));
	token.attributes.push_back(XMPPAttribute("xmlns:stream", "http://etherx.jabber.org/streams"));
	token.attributes.push_back(XMPPAttribute("to", target_hostname));
	token.attributes.push_back(XMPPAttribute("version", "1.0"));
	token.attributes.push_back(XMPPAttribute("xml:lang", "en"));
	write_token(token);
}

void XMPPConnection::write_stream_end()
{
	XMPPToken token;
	token.type = XMPPToken::type_end;
	token.name = "stream:stream";
	write_token(token);
}

XMPPToken XMPPConnection::read_stream_start()
{
	XMPPToken token = read_token();
	if (token.type != XMPPToken::type_begin || token.name != "stream:stream")
		throw CL_Exception("Expected <stream> tag");
	return token;
}

void XMPPConnection::read_stream_end()
{
	try
	{
		XMPPToken token = read_token();
		if (token.type != XMPPToken::type_end || token.name != "stream:stream")
			throw CL_Exception("Expected </stream> tag");
	}
	catch (CL_Exception e)
	{
		// It would appear that the Google XMPP server hangs up prematurely? (or this client disconnects incorrectly)
	}
}

void XMPPConnection::auth_plain(const CL_String &username, const CL_String &password)
{
	XMPPToken token;
	token.type = XMPPToken::type_begin;
	token.name = "auth";
	token.attributes.push_back(XMPPAttribute("xmlns", "urn:ietf:params:xml:ns:xmpp-sasl"));
	token.attributes.push_back(XMPPAttribute("mechanism", "PLAIN"));
	write_token(token);
	token.type = XMPPToken::type_text;
	token.value = CL_Base64Encoder::encode(cl_format(CL_String("%1\0%2\0%3", 8), "", username, password, 32));
	write_token(token);
	token.type = XMPPToken::type_end;
	write_token(token);

	token = read_token();
	if (token.type != XMPPToken::type_single || token.name != "success") // namespace must be urn:ietf:params:xml:ns:xmpp-sasl
		throw CL_Exception(cl_format("Expected <success> tag; got <%1>", token.name));
}
