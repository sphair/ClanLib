/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
#include "API/Core/CSS/css_document.h"
#include "API/Core/CSS/css_selector.h"
#include "API/Core/CSS/css_ruleset.h"
#include "css_document_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_CSSDocument Construction:

CL_CSSDocument::CL_CSSDocument()
: impl(new CL_CSSDocument_Impl)
{
}

CL_CSSDocument::~CL_CSSDocument()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_CSSDocument Attributes:

std::vector<CL_CSSImport> &CL_CSSDocument::get_imports()
{
	return impl->imports;
}

std::vector<CL_CSSRuleSet> &CL_CSSDocument::get_rulesets()
{
	return impl->rulesets;
}

/////////////////////////////////////////////////////////////////////////////
// CL_CSSDocument Operations:

std::vector<CL_CSSRuleSet> CL_CSSDocument::select(const CL_StringRef &element)
{
	std::map<CL_String, std::vector<CL_CSSRuleSet> >::iterator it_cache;
	it_cache = impl->select_cache.find(element);
	if (it_cache != impl->select_cache.end())
		return it_cache->second;


	std::vector<CL_CSSRuleSet> sets;
	std::vector<int> specificities;

	std::vector<CL_StringRef> path_elements;
	CL_StringRef::size_type pos = 0;
	while (true)
	{
		CL_StringRef::size_type next_whitespace = element.find(cl_text(' '), pos);
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
		std::vector<CL_CSSSelector> &selectors = ruleset.get_selectors();
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

	impl->select_cache[element] = sets;

	return sets;
}

void CL_CSSDocument::add_import(CL_CSSImport import)
{
	impl->imports.push_back(import);
}

void CL_CSSDocument::add_ruleset(CL_CSSRuleSet ruleset)
{
	impl->rulesets.push_back(ruleset);
}

void CL_CSSDocument::remove_import(CL_CSSImport import)
{
	std::vector<CL_CSSImport>::size_type i, size;
	size = impl->imports.size();
	for (i=0; i<size; i++)
	{
		if (impl->imports[i] == import)
		{
			impl->imports.erase(impl->imports.begin() + i);
			return;
		}
	}
}

void CL_CSSDocument::remove_ruleset(CL_CSSRuleSet ruleset)
{
	std::vector<CL_CSSRuleSet>::size_type i, size;
	size = impl->rulesets.size();
	for (i=0; i<size; i++)
	{
		if (impl->rulesets[i] == ruleset)
		{
			impl->rulesets.erase(impl->rulesets.begin() + i);
			return;
		}
	}
}

void CL_CSSDocument::load(const CL_String &filename, const CL_VirtualDirectory &directory)
{
	impl->load(filename, directory);
}

void CL_CSSDocument::load(const CL_String &fullname)
{
	CL_String path = CL_PathHelp::get_fullpath(fullname, CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(fullname, CL_PathHelp::path_type_file);
	CL_IODevice file = CL_File(path + filename, CL_File::open_existing);
	impl->load(path, file);
}

void CL_CSSDocument::save(CL_IODevice &output)
{
	impl->save(output);
}

/////////////////////////////////////////////////////////////////////////////
// CL_CSSDocument Implementation:
