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

//: <p>Graphical User Interface API. Provides a themeable and very flexible GUI
//: framework.</p>
//! Global=GUI

#ifndef header_gui
#define header_gui

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning( disable : 4786)
#endif

// Framework
#include "GUI/component.h"
#include "GUI/component_manager.h"
#include "GUI/component_type.h"
#include "GUI/component_move_handler.h"
#include "GUI/component_resize_handler.h"
#include "GUI/component_style.h"
#include "GUI/gui_manager.h"
#include "GUI/setupgui.h"
#include "GUI/stylemanager.h"
#include "GUI/layout.h"
#include "GUI/deck.h"

// Components
#include "GUI/button.h"
#include "GUI/checkbox.h"
#include "GUI/frame.h"
#include "GUI/image.h"
#include "GUI/inputbox.h"
#include "GUI/richedit.h"
#include "GUI/richedit_paragraph.h"
#include "GUI/label.h"
#include "GUI/listbox.h"
#include "GUI/listitem.h"
#include "GUI/progressbar.h"
#include "GUI/radiobutton.h"
#include "GUI/radiogroup.h"
#include "GUI/scrollbar.h"
#include "GUI/treeview.h"
#include "GUI/treeitem.h"
#include "GUI/treenode.h"
#include "GUI/window.h"
#include "GUI/menu.h"
#include "GUI/menu_node.h"
#include "GUI/menu_item.h"

// Dialogs
#include "GUI/messagebox.h"
#include "GUI/filedialog.h"
#include "GUI/inputdialog.h"

#if defined (_MSC_VER)
#if !defined (CL_GUI_EXPORT)		
// Test CL_GUI_EXPORT to prevent recursive linker refs
// see menu.cpp, menu_generic.cpp, menu_node_generic.h 
// for the offending #includes
#if !defined (_DEBUG)
#pragma comment(lib, "clanGUI-static-mt.lib")
#else
#pragma comment(lib, "clanGUI-static-mt-debug.lib")
#endif
#endif
#endif

#endif
