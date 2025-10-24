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

#ifndef RESOURCE_TOKENIZER_H_
#define RESOURCE_TOKENIZER_H_

#include <vector>
#include <string>

class ResourceTokenizer
{
public:
	ResourceTokenizer(std::vector<char>& buffer);
	~ResourceTokenizer();

	bool is_space(char c);
	bool is_operator(char c);
	bool is_remark(char c, int position);
	std::string get_next_token();
	std::string write_error(std::string err_msg);

private:
	int cur_pos, cur_line;
	int total_size;
	std::vector<char>& config_file_data;
	bool in_remark;
};

#endif
