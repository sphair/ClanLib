
#pragma once

#include "Controllers/Workspace/workspace_page_view_controller.h"
#include "Controllers/EditConnection/edit_connection_controller.h"
#include "Models/IRCSession/irc_session.h"

class XMLSettings;
class NetworkListView;

class ConnectionsViewController : public WorkspacePageViewController
{
public:
	ConnectionsViewController();

private:
	void add_clicked();
	void connect_clicked(XMLSettings connection);
	void edit_clicked(XMLSettings connection);
	void remove_clicked(XMLSettings connection);

	void on_irc_session_created(IRCSession *session);
	void on_irc_session_destroyed(IRCSession *session);
#if defined(XMPP_SUPPORT)
	void on_xmpp_session_created(XMPPSession *session);
	void on_xmpp_session_destroyed(XMPPSession *session);
	void on_xmpp_session_error_text(const CL_String &text, XMPPSession *session);
	void on_xmpp_session_roster_updated(XMPPSession *session);
#endif
	void on_session_connect_status_changed(IRCSession::ConnectStatus status, IRCSession *session);

	std::shared_ptr<NetworkListView> networks;
	std::shared_ptr<EditConnectionController> edit_connection = std::make_shared<EditConnectionController>();
};
