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
*/

#pragma once

#include "../api_core.h"

/// \brief Class for handling HTML urls.
class CL_API_CORE CL_HTMLUrl
{
public:
	CL_HTMLUrl();
	CL_HTMLUrl(const CL_String &url, const CL_HTMLUrl &base = CL_HTMLUrl());
	CL_String to_string() const;

	CL_String scheme;

	// http scheme:
	CL_String host;
	CL_String port;
	CL_String path;
	CL_String query;

	// data scheme:
	CL_String content_type;
	CL_String encoding;
	CL_String data;

private:
	CL_String read_scheme(CL_String &input, CL_String::size_type &pos);
};
