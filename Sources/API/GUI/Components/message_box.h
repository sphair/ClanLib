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
**    Harry Storbacka
*/


#pragma once

#include "../api_gui.h"
#include "../gui_component.h"
#include "../../Core/Signals/callback_v0.h"
#include "../../Display/Render/graphic_context.h"

namespace clan
{
/// \addtogroup clanGUI_Components clanGUI Components
/// \{

enum MessageBoxResult
{
	mb_result_yes,
	mb_result_no,
	mb_result_cancel,
	mb_result_ok,
};

enum MessageBoxButtons
{
	mb_buttons_yes_no,
	mb_buttons_yes_no_cancel,
	mb_buttons_ok,
	mb_buttons_ok_cancel
};

enum MessageBoxIcon
{
	mb_icon_none,
	mb_icon_warning,
	mb_icon_info,
	mb_icon_error,
	mb_icon_question,
};

MessageBoxResult message_box(
	GUIComponent *owner,
	std::string title,
	std::string detail_text,
	MessageBoxButtons buttons,
	MessageBoxIcon icon);

MessageBoxResult message_box(
	GUIManager *gui_manager,
	std::string title,
	std::string detail_text,
	MessageBoxButtons buttons,
	MessageBoxIcon icon);

}

/// \}
