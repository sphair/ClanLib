/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    Thomas Gottschalk Larsen
*/

#include "Core/precomp.h"
#include "API/Core/XML/xpath_evaluator.h"
#include "API/Core/XML/xpath_exception.h"
#include "API/Core/XML/dom_node.h"
#include "xpath_evaluator_impl.h"
#include "xpath_token.h"

/////////////////////////////////////////////////////////////////////////////
// CL_XPathEvaluator Construction:

CL_XPathEvaluator::CL_XPathEvaluator()
: impl(new CL_XPathEvaluator_Impl)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_XPathEvaluator Operations:

CL_XPathObject CL_XPathEvaluator::evaluate(const CL_StringRef &expression, const CL_DomNode &context_node) const
{
	CL_XPathToken prev_token;
	std::vector<CL_DomNode> nodelist(1, context_node);
	CL_XPathEvaluateResult result = impl->evaluate(expression, nodelist, 0, prev_token);
	if (result.next_token.type != CL_XPathToken::type_none)
		throw CL_XPathException("Expected end of expression", expression, result.next_token);
	return result.result;
}
