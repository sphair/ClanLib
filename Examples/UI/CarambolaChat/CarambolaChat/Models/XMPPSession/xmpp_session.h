
#pragma once

#include "xmpp_connection.h"
#include "xmpp_roster_item.h"

class XMPPEntity;

class XMPPSession
{
public:
	XMPPSession();
	~XMPPSession();

	void set_connection_name(const CL_String &connection_name);
	CL_String get_connection_name() const { return connection_name; }
	const std::vector<XMPPRosterItem> &get_roster() const { return roster; }
	CL_String get_jid_for_name(const CL_String &name) const;
	CL_String get_name_for_jid(const CL_String &jid) const;
	static bool is_equal_jid(const CL_String &jid1, const CL_String &jid2);

	void set_nick_name(const CL_String &name);
	XMPPEntity get_nick() const;

	void connect(const CL_String &server, const CL_String &port, const CL_String &username, const CL_String &password, const CL_String &domain);
	void disconnect_graceful();
	void disconnect_abortive();
	void send_message(const CL_String &name, const CL_String &text);
	void send_chat_message(const CL_String &jid, const CL_String &text);

	CL_Signal_v0 cb_roster_updated;
	CL_Signal_v2<const CL_String &, bool> cb_debug_text;
	CL_Signal_v1<const CL_String &> cb_error_text;
	CL_Signal_v2<const CL_String &, const CL_String &> cb_chat_message_received;
	CL_Signal_v2<const CL_String &, const CL_String &> cb_normal_message_received;
	CL_Signal_v2<const CL_String &, const CL_String &> cb_error_message_received;

private:
	void on_stanza_received(const XMPPStanza &stanza);
	void on_features_received(const XMPPStanza & stanza);
	void on_iq_received(const XMPPStanza &stanza);
	void on_iq_get_received(const CL_String &id, const XMPPStanza &stanza);
	void on_iq_set_received(const CL_String &id, const XMPPStanza &stanza);
	void on_iq_result_received(const CL_String &id, const XMPPStanza &stanza);

	void parse_roster(int query_index, const XMPPStanza &stanza);
	void on_iq_error_received(const CL_String &id, const XMPPStanza &stanza);
	void on_stream_error_received(const XMPPStanza &stanza);
	void on_presence_received(const XMPPStanza &stanza);
	void on_presence_announce_received(const XMPPStanza &stanza);
	void on_presence_unavailable_received(const XMPPStanza &stanza);
	void on_presence_subscribe_received(const XMPPStanza &stanza);
	void on_presence_subscribed_received(const XMPPStanza &stanza);
	void on_presence_unsubscribe_received(const XMPPStanza &stanza);
	void on_presence_unsubscribed_received(const XMPPStanza &stanza);
	void on_presence_error_received(const XMPPStanza &stanza);
	void on_message_received(const XMPPStanza &stanza);
	void on_message_error_received(const XMPPStanza &stanza, CL_String from);
	void on_disconnected(const CL_String &reason);
	void send_stanza(const XMPPStanza &stanza);
	void send_iq_error(const CL_String &id, const CL_String &to);
	void send_iq_disco_response(const CL_String &id, const CL_String &to);
	void send_iq_bind(const CL_String &id);
	void send_iq_session_request(const CL_String &id);
	void send_iq_roster_request(const CL_String &id);
	void send_presence_unavailable();
	void send_presence_announce(XMPPPresenceShowType type, const CL_String &text);
	CL_String get_error_condition(const XMPPStanza &stanza, int error_index);

	void send_jingle_session_initiate(const CL_String &id, const CL_String &to, const CL_String &sid);
	void send_jingle_session_accept(const CL_String &id, const CL_String &to, const CL_String &sid);
	void send_jingle_session_terminate(const CL_String &id, const CL_String &to, const CL_String &sid);
	void send_jingle_session_ack(const CL_String &id, const CL_String &to);
	void on_jingle_session_initiate(const CL_String &id, const XMPPStanza &stanza);
	void on_jingle_session_accept(const CL_String &id, const XMPPStanza &stanza);
	void on_jingle_session_terminate(const CL_String &id, const XMPPStanza &stanza);

	CL_String connection_name;
	XMPPConnection connection;

	XMPPStanza features;
	CL_String our_name;
	CL_String our_jid;
	std::vector<XMPPRosterItem> roster;

	struct StanzaErrorConditions
	{
		const char *tag;
		const char *text;
	};
	static StanzaErrorConditions error_conditions[];
};
