
#pragma once

#include "Document/irc_text.h"
#include "Document/irc_entity.h"

class ChatView;

class ChannelTopic : public CL_GUIComponent
{
public:
	ChannelTopic(ChatView *chatview);
	int get_topic_height(int width);

	void set_topic(const CL_String &timestamp, const IRCText &text, const IRCNick &author);

private:
	void on_resized();
	void on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect);
	void on_process_message(CL_GUIMessage &msg);
	void on_input_message(const CL_GUIMessage_Input &msg);
	void on_pointer_message(const CL_GUIMessage_Pointer &msg);

	ChatView *chatview;
	CL_GUIThemePart part_background;
	CL_SpanLayout span_layout;

	CL_String timestamp;
	IRCText text;
	IRCNick author;
};
