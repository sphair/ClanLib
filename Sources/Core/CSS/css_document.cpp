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

namespace clan
{

CSSDocument::CSSDocument()
: impl(new CSSDocument_Impl)
{
}

CSSDocument::~CSSDocument()
{
}

std::vector<CSSProperty> CSSDocument::select(const std::string &element)
{
	std::map<std::string, std::vector<CSSProperty> >::iterator it_cache;
	it_cache = impl->select_cache.find(element);
	if (it_cache != impl->select_cache.end())
		return it_cache->second;


	std::vector<CSSRuleSet> sets;
	std::vector<int> specificities;

	std::vector<std::string> path_elements;
	std::string::size_type pos = 0;
	while (true)
	{
		std::string::size_type next_whitespace = element.find(' ', pos);
		if (next_whitespace != std::string::npos)
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

	std::vector<CSSRuleSet>::size_type index_rulesets, size_rulesets;
	size_rulesets = impl->rulesets.size();
	for (index_rulesets = 0; index_rulesets < size_rulesets; index_rulesets++)
	{
		CSSRuleSet &ruleset = impl->rulesets[index_rulesets];
		std::vector<CSSSelector> &selectors = ruleset.selectors;
		bool match = false;
		int specificity = 0;

		std::vector<CSSSelector>::size_type index_selectors, size_selectors;
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

	std::vector<CSSProperty> properties;

	// Prioritize properties with the important flag:
	for (size_t i = 0; i < sets.size(); i++)
	{
		for (size_t j = 0; j < sets[i].properties.size(); j++)
		{
			if (sets[i].properties[j].get_priority() == CSSProperty::priority_important)
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
			if (sets[i].properties[j].get_priority() != CSSProperty::priority_important)
			{
				properties.push_back(sets[i].properties[j]);
			}
		}
	}

	impl->select_cache[element] = properties;
	return properties;
}

void CSSDocument::load(const std::string &filename, const VirtualDirectory &directory)
{
	impl->load(filename, directory);
}

void CSSDocument::load(const std::string &fullname)
{
	std::string path = PathHelp::get_fullpath(fullname, PathHelp::path_type_file);
	std::string filename = PathHelp::get_filename(fullname, PathHelp::path_type_file);
	IODevice file = File(path + filename);
	impl->load(path, file);
}

std::vector<CSSProperty> CSSDocument::parse_style_properties(const std::string &style_text)
{
	std::vector<CSSProperty> properties;
	std::string::size_type pos = 0;
	while (pos < style_text.length())
	{
		int name_start, name_end;
		name_start = pos;
		pos = CSSDocument_Impl::style_load_until(":;", style_text, pos);
		name_end = pos;
		pos++;

		std::string name_text = StringHelp::trim(style_text.substr(name_start, name_end-name_start));

		if (pos <= style_text.length() && style_text[pos-1] == ':')
		{
			if (name_text.empty())
				throw Exception(string_format("Unexpected ':' at position %1", pos));

			int value_start, value_end;
			value_start = pos;
			pos = CSSDocument_Impl::style_load_until("!;", style_text, pos);
			value_end = pos;

			std::string value_text = StringHelp::trim(style_text.substr(value_start, value_end-value_start));
			std::string priority_text;
			if (style_text[pos] == '!')
			{
				pos++;
				int priority_start, priority_end;
				priority_start = pos;
				pos = CSSDocument_Impl::style_load_until(";", style_text, pos);
				priority_end = pos;

				priority_text = StringHelp::trim(style_text.substr(priority_start, priority_end-priority_start));
			}

			CSSProperty property;
			property.set_name(name_text);
			property.set_value(value_text);
			if (StringHelp::compare(priority_text, "important", true) == 0)
				property.set_priority(CSSProperty::priority_important);
			else
				property.set_priority(CSSProperty::priority_normal);
			properties.push_back(property);
		}
		else if (pos > style_text.length() || style_text[pos-1] == ';')
		{
			if (!name_text.empty())
				throw Exception(string_format("Unexpected ';' at position %1", pos));
		}
		else
		{
			break;
		}
	}
	return properties;
}

}
