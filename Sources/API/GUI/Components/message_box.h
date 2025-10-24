/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

/// \addtogroup clanGUI_Components clanGUI Components
/// \{

#pragma once

#include "../api_gui.h"
#include "../gui_component.h"
#include "../../Core/Signals/callback_v0.h"
#include "../../Display/Render/graphic_context.h"
#include "window.h"

enum CL_MessageBoxResult
{
	cl_mb_result_yes,
	cl_mb_result_no,
	cl_mb_result_cancel,
	cl_mb_result_ok,
};

enum CL_MessageBoxButtons
{
	cl_mb_buttons_yes_no,
	cl_mb_buttons_yes_no_cancel,
	cl_mb_buttons_ok,
	cl_mb_buttons_ok_cancel
};

enum CL_MessageBoxIcon
{
	cl_mb_icon_none,
	cl_mb_icon_warning,
	cl_mb_icon_info,
	cl_mb_icon_error,
	cl_mb_icon_question,
};

CL_MessageBoxResult cl_message_box(
	CL_GUIComponent *owner,
	CL_String title,
	CL_String detail_text,
	CL_MessageBoxButtons buttons,
	CL_MessageBoxIcon icon);

CL_MessageBoxResult cl_message_box(
	CL_GUIManager *gui_manager,
	CL_String title,
	CL_String detail_text,
	CL_MessageBoxButtons buttons,
	CL_MessageBoxIcon icon);

/// \}
