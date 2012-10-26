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

#include "css_ruleset.h"
#include <map>

namespace clan
{

class IODevice;
class VirtualFileSystem;

class CSSDocument_Impl
{
public:
	std::vector<CSSRuleSet> rulesets;
	std::map<std::string, std::vector<CSSProperty> > select_cache;

	void load(const std::string &filename, const VirtualDirectory &directory);
	void load(const std::string &path, IODevice &input);

	int load_import(unsigned char *data, int pos, int length, const VirtualDirectory &directory, const std::string &last_path);
	int load_import(unsigned char *data, int pos, int length, const std::string &path);

	int load_ruleset(unsigned char *data, int pos, int length);
	int load_keyword(const char *keyword, unsigned char *data, int pos, int length);
	int load_until(const char *chars, unsigned char *data, int pos, int length);

	void whitespace_comments(unsigned char *data, unsigned int length);

	void save(IODevice &output);

	static unsigned int style_load_until(const std::string::value_type *chars, const std::string &style_text, unsigned int pos);
};

}
