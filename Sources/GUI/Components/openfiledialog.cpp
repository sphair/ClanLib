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
**    Kenneth Gangstoe
*/

#include "GUI/precomp.h"
#include "API/GUI/Components/openfiledialog.h"
#include "filedialog_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OpenFileDialog Construction:

CL_OpenFileDialog::CL_OpenFileDialog(CL_GUIComponent *owner)
: impl(new CL_FileDialog_Impl)
{
	impl->owner = owner;
}

CL_OpenFileDialog::~CL_OpenFileDialog()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenFileDialog Attributes:

CL_String CL_OpenFileDialog::get_filename() const
{
	return impl->get_filename();
}

std::vector<CL_String> CL_OpenFileDialog::get_filenames() const
{
	return impl->filenames;
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenFileDialog Operations:

void CL_OpenFileDialog::set_multi_select(bool multi_select)
{
	impl->multi_select = multi_select;
}

void CL_OpenFileDialog::set_filename(const CL_String &filename)
{
	impl->set_filename(filename);
}

void CL_OpenFileDialog::add_filter(const CL_String &filter_description, const CL_String &filter_extension, bool is_default)
{
	impl->add_filter(filter_description, filter_extension, is_default);
}

void CL_OpenFileDialog::clear_filters()
{
	impl->clear_filters();
}

void CL_OpenFileDialog::set_filter_index(int filter_index)
{
	impl->set_filter_index(filter_index);
}

void CL_OpenFileDialog::set_initial_directory(const CL_String &path)
{
	impl->initial_directory = path;
}

void CL_OpenFileDialog::set_title(const CL_String &title)
{
	impl->title = title;
}

bool CL_OpenFileDialog::show()
{
	return impl->show(false);
}
