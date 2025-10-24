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

//! clanGUI="Framework"
//! header=gui.h

#ifndef header_deck
#define header_deck

#ifdef CL_API_DLL
#ifdef CL_GUI_EXPORT
#define CL_API_GUI __declspec(dllexport)
#else
#define CL_API_GUI __declspec(dllimport)
#endif
#else
#define CL_API_GUI
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "component.h"
#include <map>

class CL_Deck_Generic;

//: Deck
//- !group=GUI/Framework!
//- !header=gui.h!
//- <p>Deck is a component manager that acts like a tab system.
//- It is used to group a list of components and show only one at a time.</p>
class CL_API_GUI CL_Deck
{
//! Construction:
public:
	//: Deck Constructor
	CL_Deck();

	//: Deck Destructor
	~CL_Deck();

//! Attributes:
public:
	//: Returns the component identified by the specified name.
	//- <p>If the component is not found, a CL_Error exception is thrown.</p>
	CL_Component *get_component(const std::string &name);

	//: Returns the current visible component.
	//- <p>If no component has yet been added, NULL will be returned.</p>
	CL_Component *get_visible();

	//: Returns the stored component map.
	std::map<std::string, CL_Component *> &get_map();

//! Operations:
public:
	//: Add a component.
	//- <p>If the component name exist, the previous added component will be replaced.</p>
	void add(const std::string &name, CL_Component *component);

	//: Swap visible component.
	//- <p>If the component is not found, a CL_Error exception is thrown.</p>
	void swap(const std::string &name);

	//: Remove a component.
	//- <p>If the component is not found, a CL_Error exception is thrown.</p>
	void remove(const std::string &name);

//! Implementation:
private:
	CL_Deck_Generic *impl;

};

#endif
