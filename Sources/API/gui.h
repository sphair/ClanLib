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
**    Magnus Norddahl
**    Kenneth Gangstoe
**    Harry Storbacka
*/

/// \brief <p>ClanLib GUI library.</p>
//! Global=GUI

#pragma once

#ifdef __cplusplus_cli
#pragma managed(push, off)
#endif

#include "GUI/setup_gui.h"
#include "GUI/accelerator_table.h"
#include "GUI/accelerator_key.h"
#include "GUI/gui_component.h"
#include "GUI/gui_component_description.h"
#include "GUI/gui_layout.h"
#include "GUI/gui_layout_corners.h"
#include "GUI/gui_message.h"
#include "GUI/gui_message_input.h"
#include "GUI/gui_message_resize.h"
#include "GUI/gui_message_focus_change.h"
#include "GUI/gui_message_activation_change.h"
#include "GUI/gui_message_close.h"
#include "GUI/gui_message_pointer.h"
#include "GUI/gui_manager.h"
#include "GUI/gui_window_manager.h"
#include "GUI/gui_window_manager_system.h"
#include "GUI/gui_window_manager_texture.h"
#include "GUI/gui_window_manager_texture_window.h"
#include "GUI/gui_window_manager_direct.h"
#include "GUI/gui_theme.h"
#include "GUI/gui_theme_default.h"
#include "GUI/gui_theme_part.h"
#include "GUI/gui_theme_part_property.h"
#include "GUI/Components/checkbox.h"
#include "GUI/Components/combobox.h"
#include "GUI/Components/folderbrowsedialog.h"
#include "GUI/Components/frame.h"
#include "GUI/Components/groupbox.h"
#include "GUI/Components/imageview.h"
#include "GUI/Components/label.h"
#include "GUI/Components/lineedit.h"
#include "GUI/Components/listview.h"
#include "GUI/Components/listview_header.h"
#include "GUI/Components/listview_column_data.h"
#include "GUI/Components/main_window.h"
#include "GUI/Components/menubar.h"
#include "GUI/Components/message_box.h"
#include "GUI/Components/openfiledialog.h"
#include "GUI/Components/popupmenu.h"
#include "GUI/Components/progressbar.h"
#include "GUI/Components/push_button.h"
#include "GUI/Components/radiobutton.h"
#include "GUI/Components/savefiledialog.h"
#include "GUI/Components/scrollbar.h"
#include "GUI/Components/spin.h"
#include "GUI/Components/statusbar.h"
#include "GUI/Components/tab.h"
#include "GUI/Components/tab_page.h"
#include "GUI/Components/textedit.h"
#include "GUI/Components/toolbar.h"
#include "GUI/Components/toolbar_item.h"
#include "GUI/Components/tooltip.h"
#include "GUI/Components/slider.h"
#include "GUI/Components/window.h"
#include "GUI/Providers/gui_theme_provider.h"
#include "GUI/Providers/gui_layout_provider.h"
#include "GUI/Providers/gui_window_manager_provider.h"

#ifdef __cplusplus_cli
#pragma managed(pop)
#endif

#if defined(_MSC_VER)
	#if !defined(_MT)
		#error Your application is set to link with the single-threaded version of the run-time library. Go to project settings, in the C++ section, and change it to multi-threaded.
	#endif
	#if !defined(_DEBUG)
		#if defined(CL_DLL)
			#pragma comment(lib, "clanGUI-dll.lib")
			#pragma comment(lib, "clanCSSLayout-dll.lib")
		#elif defined(_DLL)
			#pragma comment(lib, "clanGUI-static-mtdll.lib")
			#pragma comment(lib, "clanCSSLayout-static-mtdll.lib")
		#else
			#pragma comment(lib, "clanGUI-static-mt.lib")
			#pragma comment(lib, "clanCSSLayout-static-mt.lib")
		#endif
	#else
		#if defined(CL_DLL)
			#pragma comment(lib, "clanGUI-dll-debug.lib")
			#pragma comment(lib, "clanCSSLayout-dll-debug.lib")
		#elif defined(_DLL)
			#pragma comment(lib, "clanGUI-static-mtdll-debug.lib")
			#pragma comment(lib, "clanCSSLayout-static-mtdll-debug.lib")
		#else
			#pragma comment(lib, "clanGUI-static-mt-debug.lib")
			#pragma comment(lib, "clanCSSLayout-static-mt-debug.lib")
		#endif
	#endif
#endif
