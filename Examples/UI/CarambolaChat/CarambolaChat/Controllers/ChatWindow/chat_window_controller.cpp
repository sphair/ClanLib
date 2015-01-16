
#include "precomp.h"
#include "chat_window_controller.h"
#include "Controllers/Workspace/workspace_view_controller.h"
#include "Controllers/Chat/chat_view_controller.h"
#include "Models/app_model.h"
#include "Models/IRCSession/irc_session.h"

using namespace clan;

ChatWindowViewController::ChatWindowViewController()
{
	DisplayWindowDescription desc;
	desc.set_title("");
	desc.set_allow_resize(true);
	desc.set_size(Size(1536, 864), false);
	desc.set_type(WindowType::custom);
	desc.set_extend_frame(8, 42, 8, 8);

	view = std::make_shared<WindowView>(desc);
	view->box_style.set_layout_vbox();

	workspace = std::make_shared<WorkspaceViewController>();
	add_child_controller(workspace);

	slots.connect(AppModel::instance()->cb_irc_session_created, this, &ChatWindowViewController::irc_session_created);

	workspace->add_page("Freenode", std::make_shared<ChatViewController>(nullptr, IRCEntity()));
	workspace->add_page("#clanlib", std::make_shared<ChatViewController>(nullptr, IRCEntity::from_text("#clanlib")));

	window_view()->show(WindowShowType::show_default);
}

void ChatWindowViewController::irc_session_created(IRCSession *session)
{
	slots.connect(session->cb_joined, [=](const IRCChannel &channel) { irc_channel_joined(session, channel); });

	workspace->add_page(session->get_connection_name(), std::make_shared<ChatViewController>(session, IRCEntity()));
}

void ChatWindowViewController::irc_channel_joined(IRCSession *session, const IRCChannel &channel)
{
	workspace->add_page(channel.get_name(), std::make_shared<ChatViewController>(session, channel));
}
