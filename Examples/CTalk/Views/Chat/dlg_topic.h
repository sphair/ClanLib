
#pragma once

class ChatView;

class DlgTopic : public CL_Window
{
public:
	DlgTopic(ChatView *chatview);

private:
	void on_ok();
	void on_cancel();
	void on_close();

	static CL_GUITopLevelDescription get_description();

	ChatView *chatview;
	CL_GUILayoutCorners layout;
	CL_LineEdit *topic;
	CL_PushButton *ok;
	CL_PushButton *cancel;
};
