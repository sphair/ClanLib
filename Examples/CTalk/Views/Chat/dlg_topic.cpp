
#include "precomp.h"
#include "dlg_topic.h"
#include "chatview.h"
#include "Framework/mainframe.h"
#include "Document/irc_session.h"

DlgTopic::DlgTopic(ChatView *chatview)
: CL_Window(chatview->get_mainframe(), get_description()), chatview(chatview)
{
	set_layout(layout);
	create_components("dlg_topic.gui");
	topic = dynamic_cast<CL_LineEdit*>(get_named_item("topic"));
	ok = dynamic_cast<CL_PushButton*>(get_named_item("ok"));
	cancel = dynamic_cast<CL_PushButton*>(get_named_item("cancel"));
	ok->func_clicked().set(this, &DlgTopic::on_ok);
	cancel->func_clicked().set(this, &DlgTopic::on_cancel);
	ok->set_default(true);
	cancel->set_cancel(true);
	topic->set_focus();
	func_close().set(this, &DlgTopic::on_close);
	CL_Rect owner_geometry = get_owner_component()->get_window_geometry();
	CL_Point p = owner_geometry.get_center();
	CL_Size s = layout.get_preferred_size();
	p.x -= s.width/2;
	p.y -= s.height;
	set_geometry(CL_Rect(p,s));
	set_visible(true, true);

	IRCJoinedChannel status = chatview->get_session()->get_channel_status(chatview->get_filter());
	topic->set_text(status.topic.get_text());
}

CL_GUITopLevelDescription DlgTopic::get_description()
{
	CL_GUITopLevelDescription desc;
	desc.set_title("Set Channel Topic");
	desc.set_allow_resize(false);
	desc.set_visible(false);
	desc.show_minimize_button(false);
	desc.show_maximize_button(false);
	desc.show_sysmenu(false);
	return desc;
}

bool DlgTopic::on_close()
{
	exit_with_code(0);
	return true;
}

void DlgTopic::on_ok()
{
	chatview->get_session()->set_topic(chatview->get_filter(), IRCText::from_text(topic->get_text()));
	exit_with_code(1);
}

void DlgTopic::on_cancel()
{
	exit_with_code(0);
}
