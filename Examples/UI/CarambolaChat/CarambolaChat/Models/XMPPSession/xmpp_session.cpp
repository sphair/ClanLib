
#include "precomp.h"
#include "xmpp_session.h"
#include "xmpp_entity.h"

XMPPSession::XMPPSession()
{
	connection.func_stanza_received().set(this, &XMPPSession::on_stanza_received);
	connection.func_disconnected().set(this, &XMPPSession::on_disconnected);
}

XMPPSession::~XMPPSession()
{
}

void XMPPSession::set_connection_name(const CL_String &name)
{
	connection_name = name;
}

void XMPPSession::connect(const CL_String &server, const CL_String &port, const CL_String &username, const CL_String &password, const CL_String &domain)
{
	connection.connect(CL_SocketName(server, port), username, password, domain);
}

void XMPPSession::disconnect_graceful()
{
	connection.disconnect_graceful();
}

void XMPPSession::disconnect_abortive()
{
	connection.disconnect_abortive();
}

void XMPPSession::send_message(const CL_String &name, const CL_String &text)
{
	// message types: chat, error, groupchat, headline, normal (no type or unrecognized = normal)
	/*
		<message from='me@gmail.com' to='you@gmail.com' xml:lang='en' type='normal'>
		  <subject>blah</subject>
		  <body>blahblah</body>
		  <thread>threadid</thread> <-- aka instance messaging session
		</message>
	*/

	XMPPStanza stanza;

	XMPPToken token;
	token.type = XMPPToken::type_begin;
	token.name = "message";
	token.attributes.push_back(XMPPAttribute("from", our_jid));
	token.attributes.push_back(XMPPAttribute("to", get_jid_for_name(name)));
	token.attributes.push_back(XMPPAttribute("xml:lang", "en"));
	token.attributes.push_back(XMPPAttribute("type", "normal"));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_begin;
	token.name = "body";
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_text;
	token.value = text;
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "body";
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "message";
	stanza.tokens.push_back(token);

	send_stanza(stanza);
}

void XMPPSession::send_chat_message(const CL_String &jid, const CL_String &text)
{
	XMPPStanza stanza;

	XMPPToken token;
	token.type = XMPPToken::type_begin;
	token.name = "message";
	token.attributes.push_back(XMPPAttribute("from", our_jid));
	token.attributes.push_back(XMPPAttribute("to", jid));
	token.attributes.push_back(XMPPAttribute("xml:lang", "en"));
	token.attributes.push_back(XMPPAttribute("type", "chat"));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_begin;
	token.name = "body";
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_text;
	token.value = text;
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "body";
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "message";
	stanza.tokens.push_back(token);

	send_stanza(stanza);
}

void XMPPSession::on_stanza_received(const XMPPStanza &stanza)
{
	cb_debug_text.invoke(stanza.to_string(), true);

	if (stanza.is_element(0, "features", "http://etherx.jabber.org/streams"))
	{
		on_features_received(stanza);
	}
	else if (stanza.is_element(0, "error", "http://etherx.jabber.org/streams"))
	{
		on_stream_error_received(stanza);
	}
	else if (stanza.is_element(0, "iq", "jabber:client"))
	{
		on_iq_received(stanza);
	}
	else if (stanza.is_element(0, "presence", "jabber:client"))
	{
		on_presence_received(stanza);
	}
	else if (stanza.is_element(0, "message", "jabber:client"))
	{
		on_message_received(stanza);
	}
}

void XMPPSession::on_features_received(const XMPPStanza &stanza)
{
	features = stanza;
	send_iq_bind("bind-request");
}

void XMPPSession::on_stream_error_received(const XMPPStanza &stanza)
{
}

void XMPPSession::on_iq_received(const XMPPStanza &stanza)
{
	CL_String type = stanza.get_attribute(0, "type", "jabber:client");
	CL_String id = stanza.get_attribute(0, "id", "jabber:client");
	if (type == "set")
	{
		on_iq_set_received(id, stanza);
	}
	else if (type == "get")
	{
		on_iq_get_received(id, stanza);
	}
	else if (type == "result")
	{
		on_iq_result_received(id, stanza);
	}
	else if (type == "error")
	{
		on_iq_error_received(id, stanza);
	}
}

void XMPPSession::on_iq_set_received(const CL_String &id, const XMPPStanza &stanza)
{
	CL_String from = stanza.get_attribute(0, "from", "japper:client");

	int query_index = stanza.find_child(0, "query", "jabber:iq:roster");
	if (query_index != -1 && stanza.tokens[query_index].type == XMPPToken::type_begin)
	{
		parse_roster(query_index, stanza);

		XMPPStanza stanza2;
		XMPPToken token;
		token.type = XMPPToken::type_single;
		token.name = "iq";
		token.attributes.push_back(XMPPAttribute("type", "result"));
		token.attributes.push_back(XMPPAttribute("id", id));
		token.attributes.push_back(XMPPAttribute("from", our_jid));
		token.attributes.push_back(XMPPAttribute("to", from));
		stanza2.tokens.push_back(token);
		send_stanza(stanza2);
	}
	else
	{
		send_iq_error(id, from);
	}
}

void XMPPSession::on_iq_get_received(const CL_String &id, const XMPPStanza &stanza)
{
	CL_String from = stanza.get_attribute(0, "from", "japper:client");
	int query_index = stanza.find_child(0, "query", "http://jabber.org/protocol/disco#info");
	if (query_index != -1)
	{
		send_iq_disco_response(id, from);
	}
	else
	{
		send_iq_error(id, from);
	}
}

void XMPPSession::on_iq_result_received(const CL_String &id, const XMPPStanza &stanza)
{
	if (id == "bind-request")
	{
		int bind_index = stanza.find_child(0, "bind", "urn:ietf:params:xml:ns:xmpp-bind");
		int jid_index = stanza.find_child(bind_index, "jid", "urn:ietf:params:xml:ns:xmpp-bind");
		if (jid_index != -1 && stanza.is_text(jid_index+1))
		{
			our_jid = stanza.get_text(jid_index+1);
			bool supports_session = (features.find_child(0, "session", "urn:ietf:params:xml:ns:xmpp-session") != -1);
			if (supports_session)
				send_iq_session_request("session-request");
			else
				send_iq_roster_request("roster-request");
		}
	}
	else if (id == "session-request")
	{
		send_iq_roster_request("roster-request");
	}
	else if (id == "roster-request")
	{
		int query_index = stanza.find_child(0, "query", "jabber:iq:roster");
		if (query_index != -1 && stanza.tokens[query_index].type == XMPPToken::type_begin)
			parse_roster(query_index, stanza);
		send_presence_announce(show_chat, "Carambola Chat");
	}
}

void XMPPSession::on_iq_error_received(const CL_String &id, const XMPPStanza &stanza)
{
}

void XMPPSession::on_presence_received(const XMPPStanza &stanza)
{
	CL_String type = stanza.get_attribute(0, "type", "jabber:client");
	if (type.empty())
	{
		on_presence_announce_received(stanza);
	}
	else if (type == "unavailable")
	{
		on_presence_unavailable_received(stanza);
	}
	else if (type == "subscribe")
	{
		on_presence_subscribe_received(stanza);
	}
	else if (type == "subscribed")
	{
		on_presence_subscribed_received(stanza);
	}
	else if (type == "unsubscribe")
	{
		on_presence_unsubscribe_received(stanza);
	}
	else if (type == "unsubscribed")
	{
		on_presence_unsubscribed_received(stanza);
	}
	else if (type == "error")
	{
		on_presence_error_received(stanza);
	}
}

void XMPPSession::on_presence_announce_received(const XMPPStanza &stanza)
{
	CL_String from = stanza.get_attribute(0, "from", "jabber:client");

	CL_String::size_type p = from.find('/');
	CL_String from_jid = (p == CL_String::npos) ? from : from.substr(0, p);
	for (size_t i = 0; i < roster.size(); i++)
	{
		if (is_equal_jid(roster[i].jid, from_jid))
		{
			int show_index = stanza.find_child(0, "show", "jabber:client");
			CL_String show_type;
			if (show_index != -1 && stanza.is_text(show_index+1))
				show_type = stanza.get_text(show_index+1);

			if (show_type == "away")
				roster[i].show_type = show_away;
			else if (show_type == "chat")
				roster[i].show_type = show_chat;
			else if (show_type == "dnd")
				roster[i].show_type = show_dnd;
			else if (show_type == "xa")
				roster[i].show_type = show_extended_away;
			else
				roster[i].show_type = show_chat;

			int status_index = stanza.find_child(0, "status", "jabber:client");
			if (status_index != -1 && stanza.is_text(status_index+1))
				roster[i].show_text = stanza.get_text(status_index+1);
			cb_roster_updated.invoke();
			break;
		}
	}
}

void XMPPSession::on_presence_unavailable_received(const XMPPStanza &stanza)
{
	CL_String from = stanza.get_attribute(0, "from", "jabber:client");

	CL_String::size_type p = from.find('/');
	CL_String from_jid = (p == CL_String::npos) ? from : from.substr(0, p);
	for (size_t i = 0; i < roster.size(); i++)
	{
		if (is_equal_jid(roster[i].jid, from_jid))
		{
			roster[i].show_type = show_offline;
			roster[i].show_text = "";
			cb_roster_updated.invoke();
			break;
		}
	}
}

void XMPPSession::on_presence_subscribe_received(const XMPPStanza &stanza)
{
}

void XMPPSession::on_presence_subscribed_received(const XMPPStanza &stanza)
{
}

void XMPPSession::on_presence_unsubscribe_received(const XMPPStanza &stanza)
{
}

void XMPPSession::on_presence_unsubscribed_received(const XMPPStanza &stanza)
{
}

void XMPPSession::on_presence_error_received(const XMPPStanza &stanza)
{
}

void XMPPSession::on_message_received(const XMPPStanza &stanza)
{
	// message types: chat, error, groupchat, headline, normal (no type or unrecognized = normal)
	CL_String type = stanza.get_attribute(0, "type", "jabber:client");
	CL_String from = stanza.get_attribute(0, "from", "jabber:client");

	if (type == "error")
	{
		on_message_error_received(stanza, from);
	}
	else
	{
		CL_String to = stanza.get_attribute(0, "to", "jabber:client");
		int body_index = stanza.find_child(0, "body", "jabber:client");
		CL_String body;
		if (body_index != -1 && stanza.is_text(body_index + 1))
		{
			body = stanza.get_text(body_index + 1);
			if (type == "chat")
			{
				cb_chat_message_received.invoke(from, body);
			}
			else
			{
				int bad_request_index = stanza.find_child(0, "bad-request", "urn:ietf:params:xml:ns:xmpp-stanzas");
				cb_normal_message_received.invoke(from, body);
			}
		}
	}
}

void XMPPSession::on_message_error_received(const XMPPStanza &stanza, CL_String from)
{
	int error_index = stanza.find_child(0, "error", "jabber:client");
	CL_String error_condition = get_error_condition(stanza, error_index);
	int text_index = stanza.find_child(error_index, "text", "urn:ietf:params:xml:ns:xmpp-stanzas");
	if (text_index != -1)
	{
		CL_String reason = stanza.get_text(text_index+1);
		cb_error_message_received.invoke(from, cl_format("%1 (%2)", reason, error_condition));
	}
	else
	{
		cb_error_message_received.invoke(from, error_condition);
	}
}
void XMPPSession::on_disconnected(const CL_String &reason)
{
	cb_error_text.invoke(reason.empty() ? "Disconnected" : cl_format("Disconnected (%1)", reason));
}

void XMPPSession::send_stanza(const XMPPStanza &stanza)
{
	cb_debug_text.invoke(stanza.to_string(), false);
	connection.send_stanza(stanza);
}

void XMPPSession::send_iq_bind(const CL_String &id)
{
	XMPPStanza stanza;

	XMPPToken token;
	token.type = XMPPToken::type_begin;
	token.name = "iq";
	token.attributes.push_back(XMPPAttribute("type", "set"));
	token.attributes.push_back(XMPPAttribute("id", id));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_single;
	token.name = "bind";
	token.attributes.push_back(XMPPAttribute("xmlns", "urn:ietf:params:xml:ns:xmpp-bind"));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "iq";
	stanza.tokens.push_back(token);

	send_stanza(stanza);
}

void XMPPSession::send_iq_error(const CL_String &id, const CL_String &to)
{
	XMPPStanza stanza;

	XMPPToken token;
	token.type = XMPPToken::type_begin;
	token.name = "iq";
	token.attributes.push_back(XMPPAttribute("type", "error"));
	token.attributes.push_back(XMPPAttribute("id", id));
	token.attributes.push_back(XMPPAttribute("to", to));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_begin;
	token.name = "error";
	token.attributes.push_back(XMPPAttribute("type", "cancel"));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_single;
	token.name = "feature-not-implemented";
	token.attributes.push_back(XMPPAttribute("xmlns", "urn:ietf:params:xml:ns:xmpp-stanzas"));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "error";
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "iq";
	stanza.tokens.push_back(token);

	send_stanza(stanza);
}

void XMPPSession::send_iq_disco_response(const CL_String &id, const CL_String &to)
{
	XMPPStanza stanza;

	XMPPToken token;
	token.type = XMPPToken::type_begin;
	token.name = "iq";
	token.attributes.push_back(XMPPAttribute("type", "result"));
	token.attributes.push_back(XMPPAttribute("id", id));
	token.attributes.push_back(XMPPAttribute("to", to));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_begin;
	token.name = "query";
	token.attributes.push_back(XMPPAttribute("xmlns", "http://jabber.org/protocol/disco#info"));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_single;
	token.name = "feature";
	token.attributes.push_back(XMPPAttribute("var", "http://jabber.org/protocol/chatstates"));
	token.attributes.push_back(XMPPAttribute("var", "urn:xmpp:jingle:1"));
	token.attributes.push_back(XMPPAttribute("var", "urn:xmpp:jingle:transports:raw-udp:1"));
	token.attributes.push_back(XMPPAttribute("var", "urn:xmpp:jingle:apps:rtp:1"));
	token.attributes.push_back(XMPPAttribute("var", "urn:xmpp:jingle:apps:rtp:audio"));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "query";
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "iq";
	stanza.tokens.push_back(token);

	send_stanza(stanza);
}

void XMPPSession::send_iq_session_request(const CL_String &id)
{
	XMPPStanza stanza;

	XMPPToken token;
	token.type = XMPPToken::type_begin;
	token.name = "iq";
	token.attributes.push_back(XMPPAttribute("from", our_jid));
	token.attributes.push_back(XMPPAttribute("type", "set"));
	token.attributes.push_back(XMPPAttribute("id", id));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_single;
	token.name = "session";
	token.attributes.push_back(XMPPAttribute("xmlns", "urn:ietf:params:xml:ns:xmpp-session"));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "iq";
	stanza.tokens.push_back(token);

	send_stanza(stanza);
}

void XMPPSession::send_iq_roster_request(const CL_String &id)
{
	XMPPStanza stanza;

	XMPPToken token;
	token.type = XMPPToken::type_begin;
	token.name = "iq";
	token.attributes.push_back(XMPPAttribute("from", our_jid));
	token.attributes.push_back(XMPPAttribute("type", "get"));
	token.attributes.push_back(XMPPAttribute("id", id));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_single;
	token.name = "query";
	token.attributes.push_back(XMPPAttribute("xmlns", "jabber:iq:roster"));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "iq";
	stanza.tokens.push_back(token);

	send_stanza(stanza);
}

void XMPPSession::send_presence_unavailable()
{
	XMPPStanza stanza;

	XMPPToken token;
	token.type = XMPPToken::type_single;
	token.name = "presence";
	token.attributes.push_back(XMPPAttribute("type", "unavailable"));
	stanza.tokens.push_back(token);

	send_stanza(stanza);
}

void XMPPSession::send_presence_announce(XMPPPresenceShowType type, const CL_String &text)
{
	XMPPStanza stanza;

	XMPPToken token;
	token.type = XMPPToken::type_begin;
	token.name = "presence";
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_begin;
	token.name = "show";
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_text;
	switch (type)
	{
	case show_away:
		token.value = "away";
		break;
	case show_chat:
		token.value = "chat";
		break;
	case show_dnd:
		token.value = "dnd";
		break;
	case show_extended_away:
		token.value = "xa";
		break;
	}
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "show";
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_begin;
	token.name = "status";
	token.attributes.push_back(XMPPAttribute("xml:lang", "en"));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_text;
	token.value = text;
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "status";
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "presence";
	stanza.tokens.push_back(token);

	send_stanza(stanza);
}

CL_String XMPPSession::get_jid_for_name(const CL_String &name) const
{
	for (size_t i = 0; i < roster.size(); i++)
	{
		if (roster[i].name == name)
		{
			return roster[i].jid;
		}
	}
	throw CL_Exception("Name not found in XMPP roster");
}

CL_String XMPPSession::get_name_for_jid(const CL_String &jid) const
{
	CL_String::size_type p = jid.find('/');
	CL_String jid2 = (p == CL_String::npos) ? jid : jid.substr(0, p);

	for (size_t i = 0; i < roster.size(); i++)
	{
		if (is_equal_jid(roster[i].jid, jid2))
		{
			return roster[i].name;
		}
	}
	return jid2;
}

void XMPPSession::set_nick_name(const CL_String &name)
{
	our_name = name;
}

XMPPEntity XMPPSession::get_nick() const
{
	CL_String name = our_name;
	if (name.empty())
	{
		CL_String::size_type p = our_jid.find('@');
		if (p == CL_String::npos)
			name = our_jid;
		else
			name = our_jid.substr(0, p);
	}

	return XMPPEntity(our_name, our_jid);
}

CL_String XMPPSession::get_error_condition(const XMPPStanza &stanza, int error_index)
{
	for (int i = 0; error_conditions[i].tag != 0; i++)
	{
		int index = stanza.find_child(error_index, error_conditions[i].tag, "urn:ietf:params:xml:ns:xmpp-stanzas");
		if (index != -1)
			return error_conditions[i].text;
	}
	return CL_String();
}

XMPPSession::StanzaErrorConditions XMPPSession::error_conditions[] =
{
	"bad-request", "Bad request",
	"conflict", "Conflict",
	"feature-not-implemented", "Feature not implemented",
	"forbidden", "Forbidden",
	"gone", "Gone",
	"internal-server-error", "Internal server error",
	"item-not-found", "Item not found",
	"jid-malformed", "JID malformed",
	"not-acceptable", "Not acceptable",
	"not-allowed", "Not allowed",
	"not-authorized", "Not authorized",
	"payment-required", "Payment required",
	"recipient-unavailable", "Recipient unavailable",
	"redirect", "Redirect",
	"registration-required", "Registration required",
	"remote-server-not-found", "Remote server not found",
	"remote-server-timeout", "Remote server timeout",
	"resource-constraint", "Resource constraint",
	"service-unavailable", "Resource unavailable",
	"subscription-required", "Subscription required",
	"undefined-condition", "Undefined condition",
	"unexpected-request", "Unexpected request",
	0, 0
};

void XMPPSession::send_jingle_session_initiate(const CL_String &id, const CL_String &to, const CL_String &sid)
{
	XMPPStanza stanza;

	XMPPToken token;
	token.type = XMPPToken::type_begin;
	token.name = "iq";
	token.attributes.push_back(XMPPAttribute("type", "set"));
	token.attributes.push_back(XMPPAttribute("id", id));
	token.attributes.push_back(XMPPAttribute("from", our_jid));
	token.attributes.push_back(XMPPAttribute("to", to));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_begin;
	token.name = "jingle";
	token.attributes.push_back(XMPPAttribute("xmlns", "urn:xmpp:jingle:1"));
	token.attributes.push_back(XMPPAttribute("action", "session-initiate"));
	token.attributes.push_back(XMPPAttribute("initiator", our_jid));
	token.attributes.push_back(XMPPAttribute("sid", sid));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_begin;
	token.name = "content";
	token.attributes.push_back(XMPPAttribute("creator", "initiator"));
	token.attributes.push_back(XMPPAttribute("name", "voice"));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_begin;
	token.name = "description";
	token.attributes.push_back(XMPPAttribute("xmlns", "urn:xmpp:jingle:apps:rtp:1"));
	token.attributes.push_back(XMPPAttribute("media", "audio"));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_single;
	token.name = "payload-type";
	token.attributes.push_back(XMPPAttribute("id", "96"));
	token.attributes.push_back(XMPPAttribute("name", "speed"));
	token.attributes.push_back(XMPPAttribute("clockrate", "16000"));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "description";
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_begin;
	token.name = "transport";
	token.attributes.push_back(XMPPAttribute("xmlns", "urn:xmpp:jingle:transports:raw-udp:1"));
	//token.attributes.push_back(XMPPAttribute("pwd", "asd88fgpdd777uzjYhagZg"));
	//token.attributes.push_back(XMPPAttribute("ufrag", "8hhy"));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_single;
	token.name = "candidate";
	token.attributes.push_back(XMPPAttribute("component", "1"));
	token.attributes.push_back(XMPPAttribute("foundation", "1"));
	token.attributes.push_back(XMPPAttribute("generation", "0"));
	token.attributes.push_back(XMPPAttribute("id", "connection1"));
	token.attributes.push_back(XMPPAttribute("ip", "10.0.1.1"));
	token.attributes.push_back(XMPPAttribute("network", "1"));
	token.attributes.push_back(XMPPAttribute("port", "8998"));
	token.attributes.push_back(XMPPAttribute("priority", "1337"));
	token.attributes.push_back(XMPPAttribute("protocol", "udp"));
	token.attributes.push_back(XMPPAttribute("type", "host"));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "transport";
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "content";
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "jingle";
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "iq";
	stanza.tokens.push_back(token);

	send_stanza(stanza);
}

void XMPPSession::send_jingle_session_accept(const CL_String &id, const CL_String &to, const CL_String &sid)
{
	XMPPStanza stanza;

	XMPPToken token;
	token.type = XMPPToken::type_begin;
	token.name = "iq";
	token.attributes.push_back(XMPPAttribute("type", "set"));
	token.attributes.push_back(XMPPAttribute("id", id));
	token.attributes.push_back(XMPPAttribute("from", our_jid));
	token.attributes.push_back(XMPPAttribute("to", to));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_begin;
	token.name = "jingle";
	token.attributes.push_back(XMPPAttribute("xmlns", "urn:xmpp:jingle:1"));
	token.attributes.push_back(XMPPAttribute("action", "session-accept"));
	token.attributes.push_back(XMPPAttribute("responder", our_jid));
	token.attributes.push_back(XMPPAttribute("sid", sid));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_begin;
	token.name = "content";
	token.attributes.push_back(XMPPAttribute("creator", "initiator"));
	token.attributes.push_back(XMPPAttribute("name", "voice"));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_begin;
	token.name = "description";
	token.attributes.push_back(XMPPAttribute("xmlns", "urn:xmpp:jingle:apps:rtp:1"));
	token.attributes.push_back(XMPPAttribute("media", "audio"));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_single;
	token.name = "payload-type";
	token.attributes.push_back(XMPPAttribute("id", "96"));
	token.attributes.push_back(XMPPAttribute("name", "speed"));
	token.attributes.push_back(XMPPAttribute("clockrate", "16000"));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "description";
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_begin;
	token.name = "transport";
	token.attributes.push_back(XMPPAttribute("xmlns", "urn:xmpp:jingle:transports:raw-udp:1"));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_single;
	token.name = "candidate";
	token.attributes.push_back(XMPPAttribute("component", "1"));
	token.attributes.push_back(XMPPAttribute("foundation", "1"));
	token.attributes.push_back(XMPPAttribute("generation", "0"));
	token.attributes.push_back(XMPPAttribute("id", "connection1"));
	token.attributes.push_back(XMPPAttribute("ip", "10.0.2.2"));
	token.attributes.push_back(XMPPAttribute("network", "0"));
	token.attributes.push_back(XMPPAttribute("port", "3478"));
	token.attributes.push_back(XMPPAttribute("priority", "1337"));
	token.attributes.push_back(XMPPAttribute("protocol", "udp"));
	token.attributes.push_back(XMPPAttribute("type", "host"));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "transport";
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "content";
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "jingle";
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "iq";
	stanza.tokens.push_back(token);

	send_stanza(stanza);
}

void XMPPSession::send_jingle_session_terminate(const CL_String &id, const CL_String &to, const CL_String &sid)
{
	XMPPStanza stanza;

	XMPPToken token;
	token.type = XMPPToken::type_begin;
	token.name = "iq";
	token.attributes.push_back(XMPPAttribute("type", "set"));
	token.attributes.push_back(XMPPAttribute("id", id));
	token.attributes.push_back(XMPPAttribute("from", our_jid));
	token.attributes.push_back(XMPPAttribute("to", to));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_begin;
	token.name = "jingle";
	token.attributes.push_back(XMPPAttribute("xmlns", "urn:xmpp:jingle:1"));
	token.attributes.push_back(XMPPAttribute("action", "session-terminate"));
	token.attributes.push_back(XMPPAttribute("sid", sid));
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_begin;
	token.name = "reason";
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_single;
	token.name = "success";
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "reason";
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "jingle";
	stanza.tokens.push_back(token);

	token = XMPPToken();
	token.type = XMPPToken::type_end;
	token.name = "iq";
	stanza.tokens.push_back(token);
}

void XMPPSession::send_jingle_session_ack(const CL_String &id, const CL_String &to)
{
	XMPPStanza stanza;

	XMPPToken token;
	token.type = XMPPToken::type_single;
	token.name = "iq";
	token.attributes.push_back(XMPPAttribute("type", "result"));
	token.attributes.push_back(XMPPAttribute("id", id));
	token.attributes.push_back(XMPPAttribute("from", our_jid));
	token.attributes.push_back(XMPPAttribute("to", to));
	stanza.tokens.push_back(token);

	send_stanza(stanza);
}

void XMPPSession::on_jingle_session_initiate(const CL_String &id, const XMPPStanza &stanza)
{
	CL_String from = stanza.get_attribute(0, "type", "jabber:client");
	int jingle_index = stanza.find_child(0, "jingle", "urn:xmpp:jingle:1");
	int content_index = stanza.find_child(jingle_index, "content", "urn:xmpp:jingle:1");
	int description_index = stanza.find_child(content_index, "description", "urn:xmpp:jingle:apps:rtp:1");
	int transport_index = stanza.find_child(description_index, "transport", "urn:xmpp:jingle:transports:ice-udp:1");
	CL_String sid = stanza.get_attribute(jingle_index, "sid", "urn:xmpp:jingle:1");

	send_jingle_session_ack(id, from);
	send_jingle_session_accept("jingle-accept", from, sid);
}

void XMPPSession::on_jingle_session_accept(const CL_String &id, const XMPPStanza &stanza)
{
	CL_String from = stanza.get_attribute(0, "type", "jabber:client");
	int jingle_index = stanza.find_child(0, "jingle", "urn:xmpp:jingle:1");
	int content_index = stanza.find_child(jingle_index, "content", "urn:xmpp:jingle:1");
	int description_index = stanza.find_child(content_index, "description", "urn:xmpp:jingle:apps:rtp:1");
	int transport_index = stanza.find_child(description_index, "transport", "urn:xmpp:jingle:transports:ice-udp:1");
	CL_String sid = stanza.get_attribute(jingle_index, "sid", "urn:xmpp:jingle:1");

	send_jingle_session_ack(id, from);
}

void XMPPSession::on_jingle_session_terminate(const CL_String &id, const XMPPStanza &stanza)
{
	CL_String from = stanza.get_attribute(0, "type", "jabber:client");
	int jingle_index = stanza.find_child(0, "jingle", "urn:xmpp:jingle:1");
	CL_String sid = stanza.get_attribute(jingle_index, "sid", "urn:xmpp:jingle:1");

	send_jingle_session_ack(id, from);
}

void XMPPSession::parse_roster(int query_index, const XMPPStanza &stanza)
{
	int index = query_index + 1;
	while (index != -1)
	{
		if (stanza.is_element(index, "item", "jabber:iq:roster"))
		{
			CL_String jid = stanza.get_attribute(index, "jid", "jabber:iq:roster");
			CL_String subscription_type = stanza.get_attribute(index, "subscription", "jabber:iq:roster");
			CL_String name = stanza.get_attribute(index, "name", "jabber:iq:roster");
			if (name.empty())
			{
				CL_String::size_type p = jid.find('@');
				if (p == CL_String::npos)
					name = jid;
				else
					name = jid.substr(0, p);
			}

			size_t index_roster;
			for (index_roster = 0; index_roster < roster.size(); index_roster++)
			{
				if (is_equal_jid(roster[index_roster].jid, jid))
					break;
			}

			if (subscription_type == "remove")
			{
				if (index_roster != roster.size())
					roster.erase(roster.begin()+index_roster);
			}
			else
			{
				XMPPRosterItem item(jid, subscription_type, name);
				if (index_roster != roster.size())
					roster[index_roster] = item;
				else
					roster.push_back(item);
			}
		}
		index = stanza.find_next_sibling(index);
	}
	cb_roster_updated.invoke();
}

bool XMPPSession::is_equal_jid(const CL_String &jid1, const CL_String &jid2)
{
	return CL_StringHelp::compare(jid1, jid2, true) == 0;
}
