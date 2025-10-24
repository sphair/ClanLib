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

#ifndef header_stylemanager
#define header_stylemanager

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

#include <string>

class CL_DomElement;
class CL_Component;
class CL_ResourceManager;
class CL_StyleManager_Generic;

//: Manages the overall style of the system.
//- !group=GUI/Framework!
//- !header=gui.h!
//- <p>The style manager is responsible of attaching CL_ComponentStyle objects to
//- CL_Component objects.</p>
//- <p>When a component is constructed, it will contact its style manager's
//- connect_styles() function. The style manager will then construct and attach
//- component styles to the component.</p>
//- <p>The style manager also works as a 'component factory' for the component
//- manager. By using create_component(), it is possible to create a component
//- based on a name and component options describing it.</p>
class CL_API_GUI CL_StyleManager
{
//! Construction:
public:
	//: Construct a style manager using the specified resources.
	CL_StyleManager(CL_ResourceManager *resources);

	//: Destructor.
	virtual ~CL_StyleManager();

//! Attributes:
public:
	//: Returns style manager's resources.
	CL_ResourceManager *get_resources();

//! Overrideables:
public:
	//: Create a component based on a type name.
	virtual CL_Component *create_component(
		const std::string &type,
		CL_Component *parent);

	//: Connect component styles to component.
	//: The 'type' parameter indicates what type the component is.
	virtual void connect_styles(
		const std::string &type,
		CL_Component *owner)=0;

//! Implementation:
private:
	// Disallow copy contruction of style managers.
	CL_StyleManager(const CL_StyleManager &copy) { return; }
	
	CL_StyleManager_Generic *impl;
};

#endif
