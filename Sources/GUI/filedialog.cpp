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

#include "precomp.h"
#include "API/GUI/filedialog.h"
#include "API/GUI/stylemanager.h"
#include "API/Core/XML/dom_element.h"
#include "filedialog_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_FileDialog::CL_FileDialog(
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Window(parent, style), impl(NULL)
{
	impl = new CL_FileDialog_Generic(this, "", "", "*");
}

CL_FileDialog::CL_FileDialog(
	const std::string &title,
	const std::string &file,
	const std::string &filter,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Window(parent, style), impl(NULL)
{
	impl = new CL_FileDialog_Generic(this, title, file, filter);
}

CL_FileDialog::~CL_FileDialog()
{
	delete impl;
}

const std::string CL_FileDialog::open(
	CL_Component *parent)
{
	CL_FileDialog filedialog("Open File", "", "*.*", parent);
	filedialog.run();

	return filedialog.get_file();
}

const std::string CL_FileDialog::open(
	const std::string &file,
	const std::string &filter,
	CL_Component *parent)
{
	CL_FileDialog filedialog("Open File", file, filter, parent);
	filedialog.run();

	return filedialog.get_file();
}

const std::string CL_FileDialog::save(
	CL_Component *parent)
{
	CL_FileDialog filedialog("Save File", "", "*.*", parent);
	filedialog.run();

	return filedialog.get_file();
}

const std::string CL_FileDialog::save(
	const std::string &file,
	const std::string &filter,
	CL_Component *parent)
{
	CL_FileDialog filedialog("Save File", file, filter, parent);
	filedialog.run();

	return filedialog.get_file();
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:
const CL_FileDialog::ButtonPressed &CL_FileDialog::get_pressed_button(void) const
{
	return impl->get_pressed_button();
} 

const std::string &CL_FileDialog::get_file() const
{
	return impl->get_file();
}

const std::string &CL_FileDialog::get_filter() const
{
	return impl->get_filter();
}

const std::string &CL_FileDialog::get_dir() const
{
	return impl->get_dir();
}

const std::string &CL_FileDialog::get_path() const
{
	return impl->get_path();
}

bool CL_FileDialog::is_hidden_files_visible() const
{
	return impl->is_hidden_files_visible();
}

/////////////////////////////////////////////////////////////////////////////
// Operations:
void CL_FileDialog::set_behavior(Behavior behavior)
{
	impl->set_behavior(behavior);
}

void CL_FileDialog::set_file(const std::string &filename)
{
	impl->set_file(filename);
}

void CL_FileDialog::set_dir(const std::string &dir)
{
	impl->set_dir(dir);
}

void CL_FileDialog::set_filter(const std::string &filter)
{
	impl->set_filter(filter);
}

void CL_FileDialog::show_hidden_files(bool enable)
{
	impl->show_hidden_files(enable);
}

void CL_FileDialog::refresh()
{
	impl->refresh();
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CL_Signal_v1<const std::string &> &CL_FileDialog::sig_file_highlighted()
{
	return impl->sig_file_highlighted;
}

CL_Signal_v1<const std::string &> &CL_FileDialog::sig_file_selected()
{
	return impl->sig_file_selected;
}

CL_Signal_v1<const std::string &> &CL_FileDialog::sig_dir_entered()
{
	return impl->sig_dir_entered;
}
