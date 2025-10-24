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

#include "Core/precomp.h"
#include "API/Core/Resources/resource_manager.h"
#include "resource_manager_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceManager construction:

CL_ResourceManager::CL_ResourceManager(
	const std::string &config_file,
	CL_InputSourceProvider *provider,
	bool delete_inputsource_provider)
: impl(new CL_ResourceManager_Generic)
{
	impl->self = impl;
	impl->load(config_file, provider, delete_inputsource_provider);
}

CL_ResourceManager::CL_ResourceManager(const CL_ResourceManager &copy)
: impl(copy.impl)
{
}

CL_ResourceManager::CL_ResourceManager()
: impl(new CL_ResourceManager_Generic)
{
	impl->self = impl;
}

CL_ResourceManager::~CL_ResourceManager()
{
}

CL_ResourceManager::CL_ResourceManager(const CL_SharedPtr<CL_ResourceManager_Generic> &impl)
: impl(impl)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceManager attributes:

bool CL_ResourceManager::exists(const std::string &res_id)
{
	return impl->exists(res_id);
}

CL_Resource &CL_ResourceManager::get_resource(const std::string &res_id, bool resolve_alias, int level)
{
	CL_Resource &res = impl->get_resource(res_id);
	
	if (resolve_alias && res.get_type() == "alias" && level < 20)
		return get_resource(res.get_element().get_attribute("link"), true, level+1);
	else
		return res;
}

std::vector<std::string> CL_ResourceManager::get_all_resources()
{
	return impl->get_all_resources();
}

std::vector<std::string> CL_ResourceManager::get_all_resources(const std::string &section_name)
{
	return impl->get_all_resources(section_name);
}

std::vector<std::string> CL_ResourceManager::get_all_sections()
{
	return impl->get_all_sections();
}

std::vector<std::string> CL_ResourceManager::get_sections(const std::string &section_name)
{
	return impl->get_sections(section_name);
}

std::vector<std::string> CL_ResourceManager::get_resources_of_type(const std::string &type_id)
{
	return impl->get_resources_of_type(type_id);
}

std::vector<std::string> CL_ResourceManager::get_resources_of_type(const std::string &type_id, const std::string &section_name)
{
	return impl->get_resources_of_type(type_id, section_name);
}

CL_InputSourceProvider *CL_ResourceManager::get_resource_provider() const
{
	return impl->get_resource_provider();
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceManager signals:

CL_Signal_v1<CL_Resource &> &CL_ResourceManager::sig_resource_added()
{
	return CL_ResourceManager_Generic::sig_resource_added;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceManager operations:

bool CL_ResourceManager::operator == (const CL_ResourceManager &other) const
{
	return impl == other.impl;
}

void CL_ResourceManager::add_resources(const CL_ResourceManager &additional_resources)
{
	impl->add_resources(additional_resources);
}

void CL_ResourceManager::remove_resources(const CL_ResourceManager &additional_resources)
{
	impl->remove_resources(additional_resources);
}

void CL_ResourceManager::load_all()
{
	impl->load_all();
}

void CL_ResourceManager::unload_all()
{
	impl->unload_all();
}

void CL_ResourceManager::load_section(const std::string &section_name)
{
	impl->load_section(section_name);
}

void CL_ResourceManager::unload_section(const std::string &section_name)
{
	impl->unload_section(section_name);
}
