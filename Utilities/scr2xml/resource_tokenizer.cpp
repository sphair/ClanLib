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
**    (if your name is missing here, please add it)
*/

#include "resource_tokenizer.h"
#include <ClanLib/Core/System/clanstring.h>

ResourceTokenizer::ResourceTokenizer(std::vector<char>& buffer)
	: config_file_data(buffer)
{
	cur_pos = 0;
	cur_line = 1;
	in_remark = false;
	total_size = buffer.size();
}

ResourceTokenizer::~ResourceTokenizer()
{
}

bool ResourceTokenizer::is_operator(char c)
{
	return
		c == ',' ||
		c == '=' ||
		c == '(' ||
		c == ')' ||
		c == ';' ||
		c == '{' ||
		c == '}';
}

bool ResourceTokenizer::is_space(char c)
{
	return
		c == ' ' ||
		c == '\t' ||
		c == 13 ||
		c == 10;
}

bool ResourceTokenizer::is_remark(char c, int position)
{
	if (in_remark)
	{
		if (c == '\n') 
		{	
			in_remark = false;
		}
		return true;
	}
	else
	{
		if (c == '/')
		{
			if (
				position+1 < total_size &&
				config_file_data[position+1] == '/')
			{
				in_remark = true;
				return true;
			}
		}
		else if (c == '#')
		{
			in_remark = true;
			return true;
		}
	}
	return false;
}
	
std::string ResourceTokenizer::get_next_token()
{
	while (
		cur_pos < total_size && 
		(
			is_remark(config_file_data[cur_pos], cur_pos) ||
			is_space(config_file_data[cur_pos])
		))
	{
		if (config_file_data[cur_pos] == '\n') cur_line++;
		cur_pos++;
	}
	if (cur_pos == total_size) return "";

	if (is_operator(config_file_data[cur_pos]))
	{
		char temp[2];
		temp[0] = config_file_data[cur_pos];
		temp[1] = 0;
		std::string ret(temp);
		cur_pos++;

		return ret;
	}
	else
	{
		int temp_pos = cur_pos;
		if (config_file_data[temp_pos] == '"')
		{
			int num_chars = 0;
			temp_pos++;
			while (temp_pos < total_size)
			{
				if (config_file_data[temp_pos] == '"')
				{
					if (temp_pos+1>=total_size || config_file_data[temp_pos+1] != '"') break;
					else
					{
						temp_pos++;
					}
				}
				temp_pos++;
				num_chars++;
			}
			if (temp_pos == total_size)
			{
				write_error("Missing '""' following '""'-begin");
				return "";
			}

			char *temp = new char[num_chars+1];
			temp[num_chars] = 0;
			int ofs = 1;
			for (int i=0;i<num_chars;i++)
			{
				temp[i] = config_file_data[cur_pos+ofs+i];
				if (config_file_data[cur_pos+ofs+i] == '"') 
				{
					ofs++;
				}
			}
			cur_pos = temp_pos+1;

			std::string ret(temp);
			delete [] temp;

			return ret;
		}
		else
		{
			while (temp_pos < total_size &&
				!is_space(config_file_data[temp_pos]) &&
				!is_operator(config_file_data[temp_pos]) &&
				!is_remark(config_file_data[temp_pos], cur_pos))
			{
				temp_pos++;
			}
			int tok_size = temp_pos-cur_pos;
			char *temp = new char[tok_size+1];
			temp[tok_size] = 0;
			memcpy(temp, &config_file_data[cur_pos], tok_size);
			cur_pos += tok_size;

			std::string ret(temp);
			delete [] temp;

			return ret;
		}
	}
}

std::string ResourceTokenizer::write_error(std::string err_msg)
{
	std::string err;
	
	err = CL_String::format("Error occured during read of config file (line %1): %2", cur_line, err_msg.c_str());

	return err;
}
