/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
*/

/// \brief <p>UI Put Text Here</p>
//! Global=UI

#pragma once

#ifdef WIN32
#pragma warning( disable : 4786)
#endif

#ifdef __cplusplus_cli
#pragma managed(push, off)
#endif

#include "UI/Events/activation_change_event.h"
#include "UI/Events/close_event.h"
#include "UI/Events/event.h"
#include "UI/Events/focus_change_event.h"
#include "UI/Events/key_event.h"
#include "UI/Events/pointer_event.h"
#include "UI/Events/resize_event.h"
#include "UI/StandardViews/button_view.h"
#include "UI/StandardViews/image_view.h"
#include "UI/StandardViews/label_view.h"
#include "UI/StandardViews/progress_view.h"
#include "UI/StandardViews/scroll_view.h"
#include "UI/StandardViews/scrollbar_view.h"
#include "UI/StandardViews/span_layout_view.h"
#include "UI/StandardViews/spinner_view.h"
#include "UI/StandardViews/text_field_view.h"
#include "UI/StandardViews/text_view.h"
#include "UI/StandardViews/window_view.h"
#include "UI/StandardViews/texture_view.h"
#include "UI/StandardViews/popup_view.h"
#include "UI/Image/image_source.h"
#include "UI/Style/box_geometry.h"
#include "UI/Style/style.h"
#include "UI/Style/style_property_parser.h"
#include "UI/Style/style_token.h"
#include "UI/Style/style_tokenizer.h"
#include "UI/SystemDialogs/open_file_dialog.h"
#include "UI/SystemDialogs/save_file_dialog.h"
#include "UI/SystemDialogs/folder_browse_dialog.h"
#include "UI/View/focus_policy.h"
#include "UI/View/view.h"
#include "UI/ViewController/view_controller.h"
#include "UI/UIThread/ui_thread.h"

#ifdef __cplusplus_cli
#pragma managed(pop)
#endif

#if defined(_MSC_VER)
	#if !defined(_MT)
		#error Your application is set to link with the single-threaded version of the run-time library. Go to project settings, in the C++ section, and change it to multi-threaded.
	#endif
	#if !defined(_DEBUG)
		#if defined(DLL)
			#pragma comment(lib, "clanUI-dll.lib")
		#elif defined(_DLL)
			#pragma comment(lib, "clanUI-static-mtdll.lib")
		#else
			#pragma comment(lib, "clanUI-static-mt.lib")
		#endif
	#else
		#if defined(DLL)
			#pragma comment(lib, "clanUI-dll-debug.lib")
		#elif defined(_DLL)
			#pragma comment(lib, "clanUI-static-mtdll-debug.lib")
		#else
			#pragma comment(lib, "clanUI-static-mt-debug.lib")
		#endif
	#endif
#endif
