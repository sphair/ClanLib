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

#include "precomp.h"
#include "component_type_generic.h"
#include "API/Core/System/error.h"
/////////////////////////////////////////////////////////////////////////////
// CL_ComponentType construction:

CL_ComponentType::CL_ComponentType() : impl(new CL_ComponentType_Generic)
{
}

CL_ComponentType::~CL_ComponentType()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ComponentType attributes:

std::map<std::string, CL_ComponentType *> CL_ComponentType::component_types;

const std::map<std::string, CL_ComponentType::SOptionType> &CL_ComponentType::get_options() const
{
	return impl->options;
}

bool CL_ComponentType::has_option(const std::string &option_name) const
{
	return impl->options.find(option_name) != impl->options.end();
}

CL_ComponentType::EDatatype CL_ComponentType::get_data_type(const std::string &option_name) const
{
	std::map<std::string, SOptionType>::iterator it = impl->options.find(option_name);
	if (it == impl->options.end()) return STRING; // throw error?
	return it->second.type;
}

std::string CL_ComponentType::get_default_value(const std::string &option_name) const
{
	std::map<std::string, SOptionType>::iterator it = impl->options.find(option_name);
	if (it == impl->options.end()) return std::string(); // throw error?
	return it->second.default_value;
}

const std::string &CL_ComponentType::get_class_name() const
{
	return impl->class_name;
}

CL_ComponentType::EDatatype CL_ComponentType::get_data_type(const std::string &type, const std::string &option_name)
{
	std::map<std::string, CL_ComponentType *>::const_iterator it(component_types.find(type));
	if (it == component_types.end()) cl_throw_error(std::string("Could not find component type ") + type);

	return it->second->get_data_type(option_name);
}

std::string CL_ComponentType::get_default_value(const std::string &type, const std::string &option_name)
{
	std::map<std::string, CL_ComponentType *>::const_iterator it(component_types.find(type));
	if (it == component_types.end()) cl_throw_error(std::string("Could not find component type ") + type);

	return it->second->get_default_value(option_name);
}

const std::string &CL_ComponentType::get_class_name(const std::string &type)
{
	std::map<std::string, CL_ComponentType *>::const_iterator it(component_types.find(type));
	if (it == component_types.end()) cl_throw_error(std::string("Could not find component type ") + type);

	return it->second->get_class_name();
}

/////////////////////////////////////////////////////////////////////////////
// CL_ComponentType operations:

CL_Component *CL_ComponentType::create_component(
	const std::string &type,
	CL_Component *parent,
	CL_StyleManager *style)
{
	std::map<std::string, CL_ComponentType *>::const_iterator it(component_types.find(type));
	if (it == component_types.end()) cl_throw_error(std::string("Failed to create component of type ") + type);
	
	return (*it).second->create_component(parent, style);
}

void CL_ComponentType::set_option(const std::string &name, EDatatype datatype, const std::string &value)
{
	impl->options[name].type = datatype;
	impl->options[name].default_value = value;
}

void CL_ComponentType::set_class_name(const std::string &class_name)
{
	impl->class_name = class_name;
}

/////////////////////////////////////////////////////////////////////////////
//  CL_ComponentType implementation:
