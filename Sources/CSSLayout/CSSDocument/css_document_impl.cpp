/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

#include "CSSLayout/precomp.h"
#include "css_document_impl.h"
#include "css_ruleset_match.h"
#include "API/Core/IOData/html_url.h"

namespace clan
{

std::vector<CSSRulesetMatch> CSSDocument_Impl::select_rulesets(CSSSelectNode *node, const std::string &pseudo_element)
{
	std::vector<CSSRulesetMatch> matches;
	for (size_t i = 0; i < sheets.size(); i++)
	{
		std::vector<CSSRulesetMatch> sheet_matches = sheets[i]->select_rulesets(node, pseudo_element);
		matches.insert(matches.end(), sheet_matches.begin(), sheet_matches.end());
	}
	return matches;
}

}
