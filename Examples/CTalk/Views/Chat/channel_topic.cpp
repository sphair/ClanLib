
#include "precomp.h"
#include "channel_topic.h"
#include "chatview.h"
#include "dlg_topic.h"

ChannelTopic::ChannelTopic(ChatView *chatview)
: CL_GUIComponent(chatview), chatview(chatview)
{
	set_type_name("channeltopic");
	func_render().set(this, &ChannelTopic::on_render);
	func_resized().set(this, &ChannelTopic::on_resized);
	func_process_message().set(this, &ChannelTopic::on_process_message);
	part_background = CL_GUIThemePart(this);

	CL_Font font = part_background.get_font();
	span_layout.clear();
	span_layout.add_text("No topic set for this channel", font, CL_Colorf::gray);
	on_resized();
}

int ChannelTopic::get_topic_height(int width)
{
	CL_Rect contentbox = part_background.get_content_box(CL_Size(width, 100));
	CL_GraphicContext gc = get_gc();
	span_layout.layout(gc, contentbox.get_width());
	CL_Size s = span_layout.get_size();
	contentbox.bottom = contentbox.top+s.height;
	CL_Rect renderbox = part_background.get_render_box(contentbox);
	return renderbox.get_height();
}

void ChannelTopic::set_topic(const CL_String &new_timestamp, const IRCText &new_text, const IRCNick &new_author)
{
	timestamp = new_timestamp;
	text = new_text;
	author = new_author;
	CL_Font font = part_background.get_font();
	span_layout.clear();
	span_layout.add_text(text.get_text(), font, CL_Colorf::black);
	span_layout.add_text("  set by ", font, CL_Colorf::gray);
	span_layout.add_text(new_author.get_name(), font, CL_Colorf::black);
//	span_layout.add_text(" at ", font, CL_Colorf::gray);
//	span_layout.add_text(timestamp, font, CL_Colorf::gray);
	on_resized();
}

void ChannelTopic::on_resized()
{
	CL_Rect renderbox = get_size();
	CL_Rect contentbox = part_background.get_content_box(renderbox);
	CL_GraphicContext gc = get_gc();
	span_layout.layout(gc, contentbox.get_width());
	request_repaint();
}

void ChannelTopic::on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect)
{
	CL_Rect renderbox = get_size();
	CL_Rect contentbox = part_background.get_content_box(renderbox);
	part_background.render_box(gc, renderbox, clip_rect);
	span_layout.set_position(contentbox.get_top_left());
	span_layout.draw_layout(gc);
}

void ChannelTopic::on_process_message(CL_GUIMessage &msg)
{
	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
		on_input_message(CL_GUIMessage_Input(msg));
	else if (msg.is_type(CL_GUIMessage_Pointer::get_type_name()))
		on_pointer_message(CL_GUIMessage_Pointer(msg));
}

void ChannelTopic::on_input_message(const CL_GUIMessage_Input &msg)
{
	CL_InputEvent input_event = msg.get_event();
	if (input_event.type == CL_InputEvent::pressed && input_event.id == CL_MOUSE_LEFT)
	{
	}
	else if (input_event.type == CL_InputEvent::released && input_event.id == CL_MOUSE_LEFT)
	{
		DlgTopic dlg(chatview);
		dlg.exec();
	}
	else if (input_event.type == CL_InputEvent::pointer_moved)
	{
	}
}

void ChannelTopic::on_pointer_message(const CL_GUIMessage_Pointer &msg)
{
/*
	if (msg.get_pointer_type() == CL_GUIMessage_Pointer::pointer_enter)
	{
		set_cursor(cl_cursor_ibeam);
	}
	else
	{
		set_cursor(cl_cursor_arrow);
	}
*/
}
