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

#ifndef header_component_manager
#define header_component_manager

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
#include "component_type.h"
#include "../Core/System/error.h"
#include "../Core/System/sharedptr.h"

#include <string>
#include <map>

class CL_StyleManager;
class CL_ResourceManager;
class CL_InputSourceProvider;
class CL_DomDocument;
class CL_DomElement;
class CL_ComponentType;
class CL_ComponentManager_Generic;

//: Manages components loaded from a gui definition file.
//- !group=GUI/Framework!
//- !header=gui.h!
//- <p>The component manager is responsible for loading a set of components from a gui definition
//- file (for an example of such a file, have a look at the login_view in the CTalk example).
//- After a successful construction of a component manager, it is possible get pointers to
//- components defined in the definition file, thus making it easy to seperate component layout
//- from the user interface logic.</p>
class CL_API_GUI CL_ComponentManager
{
//! Construction:
public:
	//: Constructs a component manager.
	//param resource_id: Resource identifier used to load gui definition.
	//param res_manager: Resource manager for the resource identifier.
	//param style: Style manager to create components with. If set to null it will use the style from the parent component.
	//param parent: Parent component of top components in gui definition.
	//param filename: File to load gui definition from.
	//param input_provider: Input source provider to use. If set to null it will use the default file provider.
	//param delete_provider: If true the input source provider will be deleted after use.
	//param document: XML Document to load components from.
	//param element: Element to use as the top level components element.
	//- <p>If no resource manager or style is specified, it inherits style manager and
	//- resources from parent component.</p>
	CL_ComponentManager(
		const std::string &resource_id,
		CL_ResourceManager *res_manager,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	CL_ComponentManager(
		const std::string &filename,
		CL_Component *parent,
		CL_StyleManager *style = NULL,
		CL_InputSourceProvider *input_provider = NULL,
		bool delete_provider = false);

	CL_ComponentManager(
		const CL_DomDocument &document,
		const CL_DomElement &element,
		CL_Component *parent,
		CL_StyleManager *style = NULL); 

	CL_ComponentManager(const CL_ComponentManager &copy);
	
	//: Component manager destructor.
	virtual ~CL_ComponentManager();

//! Attributes:
public:
	//: Returns the component identified by the specified name in the gui definition file.
	//- <p>If the component is not found, a CL_Error exception is thrown.</p>
	CL_Component *get_component(const std::string &name) const;

	//: Get component identifed by the specified name in the gui definition file.
	//- <p>If the component is not of the correct type, a CL_Error exception is thrown.</p>
	template<class Comp>
	void get_component(const std::string &name, Comp **component) const
	{
		*component = dynamic_cast<Comp*>(get_component(name.c_str()));
		if (*component == 0) cl_throw_error("Wrong component type!");
	}

	CL_StyleManager *get_style_manager() const;

//! Operations:
public:

//! Implementation:
private:
	CL_SharedPtr<CL_ComponentManager_Generic> impl;
};

#endif
