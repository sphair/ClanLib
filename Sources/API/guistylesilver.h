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

#ifndef header_guistylesilver
#define header_guistylesilver

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning( disable : 4786)
#endif

#include "GUIStyleSilver/stylemanager_silver.h"
#include "GUIStyleSilver/button_silver.h"
#include "GUIStyleSilver/checkbox_silver.h"
#include "GUIStyleSilver/frame_silver.h"
#include "GUIStyleSilver/image_silver.h"
#include "GUIStyleSilver/inputbox_silver.h"
#include "GUIStyleSilver/label_silver.h"
#include "GUIStyleSilver/listbox_silver.h"
#include "GUIStyleSilver/richedit_silver.h"
#include "GUIStyleSilver/progressbar_silver.h"
#include "GUIStyleSilver/radiobutton_silver.h"
#include "GUIStyleSilver/scrollbar_silver.h"
#include "GUIStyleSilver/window_silver.h"
#include "GUIStyleSilver/menu_silver.h"
#include "GUIStyleSilver/menu_node_silver.h"
#include "GUIStyleSilver/menu_item_silver.h"
#include "GUIStyleSilver/treeview_silver.h"
#include "GUIStyleSilver/treeitem_silver.h"

#if defined (_MSC_VER)
#if !defined (CL_GUISTYLESILVER_EXPORT)
// Test CL_GUISTYLESILVER_EXPORT to prevent recursive linker refs
// see notes in clanGUI/gui.h
#if !defined (_DEBUG)
#pragma comment(lib, "clanGUIStyleSilver-static-mt.lib")
#else
#pragma comment(lib, "clanGUIStyleSilver-static-mt-debug.lib")
#endif
#endif
#endif

#endif
