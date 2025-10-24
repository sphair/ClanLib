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
*/

#include "Core/precomp.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/CSS/css_document.h"
#include "API/Core/CSS/css_property.h"
#include "css_selector.h"
#include "css_ruleset.h"
#include "css_document_impl.h"

CL_CSSDocument::CL_CSSDocument()
: impl(new CL_CSSDocument_Impl)
{
}

CL_CSSDocument::~CL_CSSDocument()
{
}

std::vector<CL_CSSProperty> CL_CSSDocument::select(const CL_StringRef &element)
{
	std::map<CL_String, std::vector<CL_CSSProperty> >::iterator it_cache;
	it_cache = impl->select_cache.find(element);
	if (it_cache != impl->select_cache.end())
		return it_cache->second;


	std::vector<CL_CSSRuleSet> sets;
	std::vector<int> specificities;

	std::vector<CL_StringRef> path_elements;
	CL_StringRef::size_type pos = 0;
	while (true)
	{
		CL_StringRef::size_type next_whitespace = element.find(' ', pos);
		if (next_whitespace != CL_StringRef::npos)
		{
			path_elements.push_back(element.substr(pos, next_whitespace - pos));
		}
		else
		{
			path_elements.push_back(element.substr(pos));
			break;
		}
		pos = next_whitespace + 1;
	}

	std::vector<CL_CSSRuleSet>::size_type index_rulesets, size_rulesets;
	size_rulesets = impl->rulesets.size();
	for (index_rulesets = 0; index_rulesets < size_rulesets; index_rulesets++)
	{
		CL_CSSRuleSet &ruleset = impl->rulesets[index_rulesets];
		std::vector<CL_CSSSelector> &selectors = ruleset.selectors;
		bool match = false;
		int specificity = 0;

		std::vector<CL_CSSSelector>::size_type index_selectors, size_selectors;
		size_selectors = selectors.size();
		for (index_selectors = 0; index_selectors < size_selectors; index_selectors++)
		{
			int s = 0;
			bool m = selectors[index_selectors].select(path_elements, s);
			if (m)
			{
				match = true;
				if (s > specificity)
					specificity = s;
			}
		}

		if (match)
		{
			std::vector<int>::size_type index, size;
			size = specificities.size();
			bool inserted = false;
			for (index = 0; index < size; index++)
			{
				if (specificity >= specificities[index])
				{
					specificities.insert(specificities.begin() + index, specificity);
					sets.insert(sets.begin() + index, ruleset);
					inserted = true;
					break;
				}
			}
			if (!inserted)
			{
				specificities.push_back(specificity);
				sets.push_back(ruleset);
			}
		}
	}

	std::vector<CL_CSSProperty> properties;

	// Prioritize properties with the important flag:
	for (size_t i = 0; i < sets.size(); i++)
	{
		for (size_t j = 0; j < sets[i].properties.size(); j++)
		{
			if (sets[i].properties[j].get_priority() == CL_CSSProperty::priority_important)
			{
				properties.push_back(sets[i].properties[j]);
			}
		}
	}

	// Add the remaining properties:
	for (size_t i = 0; i < sets.size(); i++)
	{
		for (size_t j = 0; j < sets[i].properties.size(); j++)
		{
			if (sets[i].properties[j].get_priority() != CL_CSSProperty::priority_important)
			{
				properties.push_back(sets[i].properties[j]);
			}
		}
	}

	impl->select_cache[element] = properties;
	return properties;
}

void CL_CSSDocument::load(const CL_String &filename, const CL_VirtualDirectory &directory)
{
	impl->load(filename, directory);
}

void CL_CSSDocument::load(const CL_String &fullname)
{
	CL_String path = CL_PathHelp::get_fullpath(fullname, CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(fullname, CL_PathHelp::path_type_file);
	CL_IODevice file = CL_File(path + filename);
	impl->load(path, file);
}

std::vector<CL_CSSProperty> CL_CSSDocument::parse_style_properties(const CL_StringRef &style_text)
{
	std::vector<CL_CSSProperty> properties;
	CL_StringRef::size_type pos = 0;
	while (pos < style_text.length())
	{
		int name_start, name_end;
		name_start = pos;
		pos = CL_CSSDocument_Impl::style_load_until(":;", style_text, pos);
		name_end = pos;
		pos++;

		CL_String name_text = CL_StringHelp::trim(style_text.substr(name_start, name_end-name_start));

		if (pos <= style_text.length() && style_text[pos-1] == ':')
		{
			if (name_text.empty())
				throw CL_Exception(cl_format("Unexpected ':' at position %1", pos));

			int value_start, value_end;
			value_start = pos;
			pos = CL_CSSDocument_Impl::style_load_until("!;", style_text, pos);
			value_end = pos;

			CL_String value_text = CL_StringHelp::trim(style_text.substr(value_start, value_end-value_start));
			CL_String priority_text;
			if (style_text[pos] == '!')
			{
				pos++;
				int priority_start, priority_end;
				priority_start = pos;
				pos = CL_CSSDocument_Impl::style_load_until(";", style_text, pos);
				priority_end = pos;

				priority_text = CL_StringHelp::trim(style_text.substr(priority_start, priority_end-priority_start));
			}

			CL_CSSProperty property;
			property.set_name(name_text);
			property.set_value(value_text);
			if (CL_StringHelp::compare(priority_text, "important", true) == 0)
				property.set_priority(CL_CSSProperty::priority_important);
			else
				property.set_priority(CL_CSSProperty::priority_normal);
			properties.push_back(property);
		}
		else if (pos > style_text.length() || style_text[pos-1] == ';')
		{
			if (!name_text.empty())
				throw CL_Exception(cl_format("Unexpected ';' at position %1", pos));
		}
		else
		{
			break;
		}
	}
	return properties;
}
