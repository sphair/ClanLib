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
**    Magnus Norddahl
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component_description.h"
#include "API/Core/System/timer.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/GUI/Components/listview_column_data.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Font/font.h"
#include "API/Display/Window/keys.h"


/////////////////////////////////////////////////////////////////////////////
// CL_ListView_Impl Class:

class CL_ListViewColumnData_Impl
{
public:
	CL_ListViewColumnData_Impl()
	{
	}

	CL_String text;
	CL_String column_id;
};

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewColumnData Construction:

CL_ListViewColumnData::CL_ListViewColumnData()
: impl(new CL_ListViewColumnData_Impl)
{
}

CL_ListViewColumnData::~CL_ListViewColumnData()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewColumnData Attributes:

CL_StringRef CL_ListViewColumnData::get_text() const
{
	return impl->text;
}

CL_StringRef CL_ListViewColumnData::get_column_id() const
{
	return impl->column_id;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewColumnData Operations:

void CL_ListViewColumnData::set_text(const CL_StringRef &text)
{
	impl->text = text;
}

void CL_ListViewColumnData::set_column_id(const CL_StringRef &column_id)
{
	impl->column_id = column_id;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewColumnData Implementation:

