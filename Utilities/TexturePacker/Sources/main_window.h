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

class Application;
class SpriteComponent;
class ResourceItem;
class TexturePacker;
class TextureGroupComponent;
class View;
class ViewWorkspace;

class MainWindow : public clan::GUIComponent
{
public:
	MainWindow(clan::GUIManager *gui);
	~MainWindow();

// Attributes:
public:

// Operations:
public:
	void add_view(View *view);
	void remove_view(View *view);

// Implementation:
private:
	clan::GUITopLevelDescription get_window_description();

	void create_components();

	void create_welcome_view();
	void create_resource_viewer_view();
	void create_texture_packer_view();
	void create_css_packer_view();
	void create_ico_creator_view();

	int find_view_index(View *view);

	bool on_close();
	void on_close2();
	void on_resized();
	void on_view_close(View *view);

	Application *application;

	std::vector<View *> views;
	ViewWorkspace *workspace;

	clan::MenuBar *menubar;
};
