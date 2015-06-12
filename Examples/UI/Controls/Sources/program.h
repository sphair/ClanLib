/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    Mark Page
*/

#pragma once

#include "controls.h"

class Form : public clan::WindowView
{
public:
	Form(const clan::DisplayWindowDescription &desc) : clan::WindowView(desc)
	{
		style()->set("flex-direction: column");
		style()->set("padding: 11px");
		style()->set("background: #efefef");

		headline->style()->set("font: bold 16px/20px Segoe UI, Tahoma, sans-serif; margin-bottom: 11px;");
		paragraph->style()->set("font: 13px/16px Segoe UI, Tahoma, sans-serif; margin-bottom: 11px;");
		datetime->style()->set("font: 13px/16px Segoe UI, Tahoma, sans-serif; margin-bottom: 11px; height: 200px; border: 1px solid #555; box-shadow: 0 0 2px rgba(0,0,0,0.1)");
		button_bar->style()->set("flex-direction: row");
		spacer->style()->set("flex: auto");
		button_ok->style()->set("font-size: 10px; margin-right: 5px");
		button_cancel->style()->set("font-size: 10px");
		spacer2->style()->set("flex: auto");
		statusbar->style()->set("font-size: 10px; margin: 5px -11px -11px -11px");

		add_subview(headline);
		add_subview(paragraph);
		add_subview(datetime);
		add_subview(button_bar);
		add_subview(spacer2);
		add_subview(statusbar);

		headline->add_text("Common Controls Layout Example", headline->style());
		paragraph->add_text("This is an example demonstrating how to make the Win32 common controls participate in layout.", paragraph->style());

		button_bar->add_subview(spacer);
		button_bar->add_subview(button_ok);
		button_bar->add_subview(button_cancel);

		button_ok->set_text("OK");
		button_cancel->set_text("Cancel");

		statusbar->set_simple();
		//statusbar->set_parts({ 500 });
		statusbar->set_text("READY FOR ACTION!");
	}

	std::shared_ptr<clan::SpanLayoutView> headline = std::make_shared<clan::SpanLayoutView>();
	std::shared_ptr<clan::SpanLayoutView> paragraph = std::make_shared<clan::SpanLayoutView>();
	std::shared_ptr<MonthCalControl> datetime = std::make_shared<MonthCalControl>();
	std::shared_ptr<clan::View> button_bar = std::make_shared<clan::View>();
	std::shared_ptr<clan::View> spacer = std::make_shared<clan::View>();
	std::shared_ptr<ButtonControl> button_ok = std::make_shared<ButtonControl>();
	std::shared_ptr<ButtonControl> button_cancel = std::make_shared<ButtonControl>();
	std::shared_ptr<clan::View> spacer2 = std::make_shared<clan::View>();
	std::shared_ptr<StatusBarControl> statusbar = std::make_shared<StatusBarControl>();
};

class Program : public clan::Application
{
public:
	Program();
	bool update() override;

	clan::UIThread ui_thread;
	std::shared_ptr<Form> form;
	clan::SlotContainer slots;
};
