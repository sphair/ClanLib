/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#ifndef header_file_dialog_generic
#define header_file_dialog_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>

#include "API/Core/System/clanstring.h"
#include "API/signals.h"
#include "API/GUI/window.h"
#include "API/GUI/label.h"
#include "API/GUI/inputbox.h"
#include "API/GUI/button.h"
#include "API/GUI/treeview.h"
#include "API/GUI/inputdialog.h"
#include "API/GUI/filedialog.h"

//class CL_FileDialog;
class CL_StyleManager;
class CL_TreeNode;

/////////////////////////////////////////////////////////////////////////////
// FileInfo

class FileInfo
{
public:
	FileInfo(std::string name, bool directory, bool readable, bool writable, bool hidden, int size);
	std::string name;
	bool directory;
	bool readable;
	bool writable;
	bool hidden;
	int size;
};

/////////////////////////////////////////////////////////////////////////////
// CL_FileDialog_Generic

class CL_FileDialog_Generic
{
//! Construction:
public:
	CL_FileDialog_Generic(
		CL_FileDialog *self,
		const std::string &title,
		const std::string &file,
		const std::string &filter);

	~CL_FileDialog_Generic();

//! Attributes:
public:
	const CL_FileDialog::ButtonPressed &get_pressed_button(void) const; 
	const std::string &get_file() const;
	const std::string &get_filter() const;
	const std::string &get_dir() const;
	const std::string &get_path() const;
	bool is_hidden_files_visible() const;

//! Operations:
public:
	void set_behavior(CL_FileDialog::Behavior behavior);
	void set_file(const std::string &filename, bool refresh = true);
	void set_dir(const std::string &dir, bool refresh = true);
	void set_filter(const std::string &filter, bool refresh = true);
	void show_hidden_files(bool enable);
	void refresh();

//! Callbacks:
public:
	void on_file_activated(const CL_TreeNode &node);
	void on_button_quit(bool ok);
	void on_button_parent();
	void on_button_createdir();
	void on_edit_dir();
	void on_edit_file();
	void on_edit_filter();
	void on_set_options(const CL_DomElement &options);

//! Signals:
public:
	CL_Signal_v1<const std::string &> sig_file_highlighted;
	CL_Signal_v1<const std::string &> sig_file_selected;
	CL_Signal_v1<const std::string &> sig_dir_entered;

//! Implementation:
private:
	void read_dir();

	static bool sort(const FileInfo &a, const FileInfo &b);

	// Behaviou
	CL_FileDialog::Behavior behavior;
	// Button pressed
	CL_FileDialog::ButtonPressed button; 

	CL_FileDialog *filedialog;

	bool show_hidden;

	CL_SlotContainer slots;

	CL_Label *label_dir;
	CL_InputBox *input_dir;
	CL_Button *button_parent;
	CL_Button *button_createdir;

	CL_TreeView *treeview_files;

	CL_Label *label_file;
	CL_InputBox *input_file;

	CL_Label *label_filter;
	CL_InputBox *input_filter;

	CL_Button *button_ok;
	CL_Button *button_cancel;

	std::string file;
	std::string filter;
	std::string dir;
	std::string path;

	std::vector<FileInfo> Directories;
	std::vector<FileInfo> Files;
};

#endif
