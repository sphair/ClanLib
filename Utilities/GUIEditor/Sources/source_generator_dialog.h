/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    Harry Storbacka
*/

#pragma once

class SourceGeneratorDialog : public CL_Window
{
//! Construction:
public:
	SourceGeneratorDialog(CL_GUIManager *gui_manager);
	virtual ~SourceGeneratorDialog() { }

//! Attributes:
public:

//! Operations:
public:

//! Events:
public:

//! Implementation:
private:
	void on_btn_generate();
	void on_btn_browse_cpp();
	void on_btn_browse_h();

	CL_GUITopLevelDescription get_startup_description();

	CL_GUILayoutCorners layout;

	CL_PushButton *btn_generate;
	CL_PushButton *btn_browse_h;
	CL_PushButton *btn_browse_cpp;
	CL_ComboBox *combo_h_filename;
	CL_ComboBox *combo_cpp_filename;
};
