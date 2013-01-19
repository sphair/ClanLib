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

#pragma once

#include "../api_csslayout.h"
#include "../../Core/IOData/virtual_directory.h"
#include <memory>

namespace clan
{

class IODevice;
class DomElement;
class CSSProperty;
class CSSSelectNode;
class CSSPropertyValue;
class CSSSelectResult;
class CSSDocument_Impl;

enum CSSSheetOrigin
{
	user_agent_sheet_origin = 0,
	user_sheet_origin = 1,
	author_sheet_origin = 2
};

class CL_API_CSSLAYOUT CSSDocument
{
public:
	CSSDocument();
	~CSSDocument();

	static std::string get_default_html_sheet();

	void add_default_html_sheet();
	void add_sheet(CSSSheetOrigin origin, const std::string &filename, const VirtualDirectory &dir = VirtualDirectory());
	void add_sheet(CSSSheetOrigin origin, IODevice &iodevice, const std::string &base_uri);

	CSSSelectResult select(const DomElement &node, const std::string &pseudo_element = std::string());
	CSSSelectResult select(CSSSelectNode *node, const std::string &pseudo_element = std::string());

private:
	std::shared_ptr<CSSDocument_Impl> impl;
};

}
