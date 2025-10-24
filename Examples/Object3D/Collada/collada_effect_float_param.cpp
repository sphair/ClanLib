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
**    Mark Page
*/

#include "precomp.h"
#include "collada_effect_float_param.h"

class CL_Collada_Effect_FloatParam_Impl
{
public:
	CL_Collada_Effect_FloatParam_Impl() {}

	float value;
};

CL_Collada_Effect_FloatParam::CL_Collada_Effect_FloatParam()
{
}

CL_Collada_Effect_FloatParam::CL_Collada_Effect_FloatParam(CL_DomElement &float_param_element) : impl(new CL_Collada_Effect_FloatParam_Impl())
{
	CL_DomElement float_element = float_param_element.named_item("float").to_element();
	if (float_element.is_null())
		throw CL_Exception("only float currently supported (not param) - fixme please");

	impl->value = CL_StringHelp::text_to_float(float_element.get_text());

}

bool CL_Collada_Effect_FloatParam::is_float_set()
{
	return true;
}

float CL_Collada_Effect_FloatParam::get_float()
{
	return impl->value;
}

bool CL_Collada_Effect_FloatParam::is_null()
{
	return impl.is_null();
}

