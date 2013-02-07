/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

/// \brief <p>ClanLib GameIDE library.</p>
//! Global=GameIDE

#pragma once

#ifdef __cplusplus_cli
#pragma managed(push, off)
#endif

#include "GameIDE/BuildSystem/build_operation.h"
#include "GameIDE/BuildSystem/build_system.h"
#include "GameIDE/BuildSystem/build_tool.h"
#include "GameIDE/FileItemType/file_item_type.h"
#include "GameIDE/FileItemType/file_item_type_factory.h"
#include "GameIDE/MainWindow/main_window.h"
#include "GameIDE/SolutionModel/Project/project.h"
#include "GameIDE/SolutionModel/Project/project_item.h"
#include "GameIDE/SolutionModel/Solution/project_reference.h"
#include "GameIDE/SolutionModel/Solution/solution.h"
#include "GameIDE/SolutionModel/UserOptions/opened_item.h"
#include "GameIDE/SolutionModel/UserOptions/project_options.h"
#include "GameIDE/SolutionModel/UserOptions/user_options.h"
#include "GameIDE/SolutionModel/solution_model.h"
#include "GameIDE/TextEditor/text_editor_file_item_type.h"
#include "GameIDE/UIController/document_editor.h"
#include "GameIDE/UIController/document_editor_type.h"
#include "GameIDE/UIController/ui_controller.h"
#include "GameIDE/UIController/ui_controller_listener.h"
#include "GameIDE/Workspace/dockable_component.h"

#ifdef __cplusplus_cli
#pragma managed(pop)
#endif

#if defined(_MSC_VER)
	#if !defined(_MT)
		#error Your application is set to link with the single-threaded version of the run-time library. Go to project settings, in the C++ section, and change it to multi-threaded.
	#endif
	#if !defined(_DEBUG)
		#if defined(DLL)
			#pragma comment(lib, "clanGameIDE-dll.lib")
		#elif defined(_DLL)
			#pragma comment(lib, "clanGameIDE-static-mtdll.lib")
		#else
			#pragma comment(lib, "clanGameIDE-static-mt.lib")
		#endif
	#else
		#if defined(DLL)
			#pragma comment(lib, "clanGameIDE-dll-debug.lib")
		#elif defined(_DLL)
			#pragma comment(lib, "clanGameIDE-static-mtdll-debug.lib")
		#else
			#pragma comment(lib, "clanGameIDE-static-mt-debug.lib")
		#endif
	#endif
#endif
