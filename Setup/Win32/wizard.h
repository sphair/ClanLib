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
*/

#ifndef header_wizard
#define header_wizard

#include "page_welcome.h"
#include "page_target.h"
#include "page_system.h"
#include "page_system2.h"
#include "page_finished.h"

class Workspace;

class Wizard
{
private:
	Workspace create_workspace();

public:
	Wizard();

	INT_PTR exec();

	BOOL finish();

	PageWelcome page_welcome;
	
	PageTarget page_target;

	PageSystem page_system;

	PageSystem2 page_system2;

	PageFinished page_finished;

	PROPSHEETHEADER propsheetheader;

	HPROPSHEETPAGE pages[6];
};

#endif
