/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
**    Artem Khomenko
*/

#pragma once

class App : public clan::Application
{
public:
	App();
	bool update() override;

	clan::UIThread ui_thread;
	std::shared_ptr<clan::TopLevelWindow> window;
	clan::WindowManager window_manager;

private:
	std::shared_ptr<clan::LabelView> label1;
	std::shared_ptr<clan::LabelView> label2;
	std::shared_ptr<clan::LabelView> label3;
	std::shared_ptr<clan::LabelView> label4;
	std::shared_ptr<clan::ButtonView> button4;

	void on_button1_down();
	void on_button2_down();
	void on_button3_down();	
	void on_button4_down();
};
