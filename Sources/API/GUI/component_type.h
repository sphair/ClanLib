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

#ifndef header_component_type
#define header_component_type

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

#include "../Core/System/clanstring.h"
#include <set>
#include <string>
#include <map>

class CL_Component;
class CL_StyleManager;
class CL_ComponentType_Generic;

//: Component Type.
//- !group=GUI/Framework!
//- !header=gui.h!
class CL_API_GUI CL_ComponentType
{
//! Construction:
public:
	//: Component Type Constructor
	CL_ComponentType();

	//: Component Type Destructor
	virtual ~CL_ComponentType();

//! Enums:
public:
	enum EDatatype
	{
		STRING,
		BOOL,
		NUMBER
	};

//! Attributes:
public:
	struct SOptionType
	{
		EDatatype type;

		std::string default_value;
	};

	//: List of known component types.
	static std::map<std::string, CL_ComponentType *> component_types;

	//: Options for component type.
	const std::map<std::string, SOptionType> &get_options() const;

	//: Has option
	bool has_option(const std::string &option_name) const;

	EDatatype get_data_type(const std::string &option_name) const;

	std::string get_default_value(const std::string &option_name) const;

	const std::string &get_class_name() const;

	static EDatatype get_data_type(const std::string &type, const std::string &option_name);

	static std::string get_default_value(const std::string &type, const std::string &option_name);

	static const std::string &get_class_name(const std::string &type);

//! Operations:
public:
	//: Sets an option's datatype and default value.
	void set_option(const std::string &name, EDatatype datatype, const std::string &value);

	//: Sets the class name (ie "CL_Button") of a component type.
	void set_class_name(const std::string &class_name);

	//: Creates a component of type 'component_type_name'.
	//- <p>If no such component type exists, a CL_Error is thrown. The method
	//- invokes the specific component type's
	//- virtual 'create_component' method with the passed arguments.</p>
	static CL_Component *create_component(
		const std::string &type,
		CL_Component *parent,
		CL_StyleManager *style);

	//: Create component.
	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style) = 0;

// !Implementation:
private:
	CL_ComponentType_Generic *impl;
};

#endif
