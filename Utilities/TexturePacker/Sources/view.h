/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
*/

#pragma once

class MainWindow;

class View : public clan::GUIComponent
{
public:
	View(clan::GUIComponent *parent, MainWindow *mainwindow, const std::string &title);
	virtual ~View();

	MainWindow *get_mainwindow() const { return mainwindow; }

	const std::string &get_title() const { return title; }
	void set_title(const std::string &title) { this->title = title; }

	bool is_closable() const { return closable; }
	void set_closable(bool closable) { this->closable = closable; }

private:
	MainWindow *mainwindow;
	std::string title;

	bool closable;
};
