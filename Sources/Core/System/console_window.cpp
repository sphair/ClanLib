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
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

#include "Core/precomp.h"
#include "API/Core/System/console_window.h"
#include "console_window_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ConsoleWindow construction:

CL_ConsoleWindow::CL_ConsoleWindow(
	const CL_StringRef &title,
	int width,
	int height)
: impl(NULL)
{
	impl = new CL_ConsoleWindow_Generic(title, width, height);
}

CL_ConsoleWindow::~CL_ConsoleWindow()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ConsoleWindow operations:

void CL_ConsoleWindow::wait_for_key()
{
	impl->wait_for_key();
}

void CL_ConsoleWindow::display_close_message()
{
	impl->display_close_message();
}
