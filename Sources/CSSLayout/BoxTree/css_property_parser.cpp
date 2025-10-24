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
**    Magnus Norddahl
*/

#include "CSSLayout/precomp.h"
#include "css_property_parser.h"
#include "css_box_length.h"

CL_CSSToken CL_CSSPropertyParser::next_token(size_t &pos, const std::vector<CL_CSSToken> &tokens, bool skip_whitespace)
{
	CL_CSSToken token;
	do
	{
		if (pos != tokens.size())
		{
			token = tokens[pos];
			pos++;
		}
		else
		{
			token = CL_CSSToken();
		}
	} while (token.type == CL_CSSToken::type_whitespace);
	return token;
}

bool CL_CSSPropertyParser::is_length(const CL_CSSToken &token)
{
	if (token.type == CL_CSSToken::type_dimension)
		return true;
	else if (token.type == CL_CSSToken::type_number && token.value == "0")
		return true;
	else
		return false;
}

bool CL_CSSPropertyParser::parse_length(const CL_CSSToken &token, CL_CSSBoxLength &out_length)
{
	if (token.type == CL_CSSToken::type_dimension)
	{
		if (token.dimension == "px")
		{
			out_length = CL_CSSBoxLength(CL_StringHelp::text_to_float(token.value), CL_CSSBoxLength::type_px);
			return true;
		}
		else if (token.dimension == "em")
		{
			out_length = CL_CSSBoxLength(CL_StringHelp::text_to_float(token.value), CL_CSSBoxLength::type_em);
			return true;
		}
		else if (token.dimension == "pt")
		{
			out_length = CL_CSSBoxLength(CL_StringHelp::text_to_float(token.value), CL_CSSBoxLength::type_pt);
			return true;
		}
		else if (token.dimension == "mm")
		{
			out_length = CL_CSSBoxLength(CL_StringHelp::text_to_float(token.value), CL_CSSBoxLength::type_mm);
			return true;
		}
		else if (token.dimension == "cm")
		{
			out_length = CL_CSSBoxLength(CL_StringHelp::text_to_float(token.value), CL_CSSBoxLength::type_cm);
			return true;
		}
		else if (token.dimension == "in")
		{
			out_length = CL_CSSBoxLength(CL_StringHelp::text_to_float(token.value), CL_CSSBoxLength::type_in);
			return true;
		}
		else if (token.dimension == "pc")
		{
			out_length = CL_CSSBoxLength(CL_StringHelp::text_to_float(token.value), CL_CSSBoxLength::type_pc);
			return true;
		}
		else if (token.dimension == "ex")
		{
			out_length = CL_CSSBoxLength(CL_StringHelp::text_to_float(token.value), CL_CSSBoxLength::type_ex);
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (token.type == CL_CSSToken::type_number && token.value == "0")
	{
		out_length = CL_CSSBoxLength(0.0f, CL_CSSBoxLength::type_px);
		return true;
	}
	else
	{
		return false;
	}
}

bool CL_CSSPropertyParser::parse_integer(const CL_String &value, int &out_int)
{
	if (value.empty())
		return false;
	for (size_t i = 0; i < value.length(); i++)
	{
		if (value[i] < '0' || value[i] > '9')
			return false;
	}
	out_int = CL_StringHelp::text_to_int(value);
	return true;
}

bool CL_CSSPropertyParser::parse_color(const std::vector<CL_CSSToken> &tokens, size_t &in_out_pos, CL_Colorf &out_color)
{
	size_t pos = in_out_pos;
	CL_CSSToken token = next_token(pos, tokens);
	if (token.type == CL_CSSToken::type_ident)
	{
		for (int i = 0; colors[i].name != 0; i++)
		{
			if (token.value == colors[i].name)
			{
				out_color = CL_Colorf(
					(colors[i].color>>16)/255.0f,
					((colors[i].color>>8)&0xff)/255.0f,
					(colors[i].color&0xff)/255.0f,
					1.0f);

				in_out_pos = pos;
				return true;
			}
		}
	}
	else if (token.type == CL_CSSToken::type_function && token.value == "rgb")
	{
		int color[3] = { 0, 0, 0 };
		for (int channel = 0; channel < 3; channel++)
		{
			token = next_token(pos, tokens);
			if (token.type == CL_CSSToken::type_number)
			{
				int value = CL_StringHelp::text_to_int(token.value);
				value = cl_min(255, value);
				value = cl_max(0, value);
				color[channel] = value;
			}
			else if (token.type == CL_CSSToken::type_percentage)
			{
				float value = CL_StringHelp::text_to_float(token.value)/100.0f;
				value = cl_min(1.0f, value);
				value = cl_max(0.0f, value);
				color[channel] = (int)(value*255.0f);
			}
			else
			{
				return false;
			}

			if (channel < 2)
			{
				token = next_token(pos, tokens);
				if (token.type != CL_CSSToken::type_delim || token.value != ",")
					return false;
			}
		}
		token = next_token(pos, tokens);
		if (token.type == CL_CSSToken::type_bracket_end)
		{
			out_color = CL_Colorf(color[0]/255.0f, color[1]/255.0f, color[2]/255.0f, 1.0f);
			in_out_pos = pos;
			return true;
		}
	}
	else if (token.type == CL_CSSToken::type_function && token.value == "rgba")
	{
		int color[4] = { 0, 0, 0, 0 };
		for (int channel = 0; channel < 4; channel++)
		{
			token = next_token(pos, tokens);
			if (token.type == CL_CSSToken::type_number)
			{
				int value = CL_StringHelp::text_to_int(token.value);
				value = cl_min(255, value);
				value = cl_max(0, value);
				color[channel] = value;
			}
			else if (token.type == CL_CSSToken::type_percentage)
			{
				float value = CL_StringHelp::text_to_float(token.value)/100.0f;
				value = cl_min(1.0f, value);
				value = cl_max(0.0f, value);
				color[channel] = (int)(value*255.0f);
			}
			else
			{
				return false;
			}

			if (channel < 3)
			{
				token = next_token(pos, tokens);
				if (token.type != CL_CSSToken::type_delim || token.value != ",")
					return false;
			}
		}
		token = next_token(pos, tokens);
		if (token.type == CL_CSSToken::type_bracket_end)
		{
			out_color = CL_Colorf(color[0]/255.0f, color[1]/255.0f, color[2]/255.0f, color[3]/255.0f);
			in_out_pos = pos;
			return true;
		}
	}
	else if (token.type == CL_CSSToken::type_hash)
	{
		if (token.value.length() == 3)
		{
			float channels[3] = { 0.0f, 0.0f, 0.0f };
			for (int c = 0; c < 3; c++)
			{
				int v = 0;
				if (token.value[c] >= '0' && token.value[c] <= '9')
					v = token.value[c]-'0';
				else if (token.value[c] >= 'a' && token.value[c] <= 'f')
					v = token.value[c]-'a'+10;
				else if (token.value[c] >= 'A' && token.value[c] <= 'F')
					v = token.value[c]-'A'+10;
				else
					return false;
				v = (v << 4) + v;
				channels[c] = v/255.0f;
			}
			out_color = CL_Colorf(channels[0], channels[1], channels[2]);
			in_out_pos = pos;
			return true;
		}
		else if (token.value.length() == 4)
		{
			float channels[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			for (int c = 0; c < 4; c++)
			{
				int v = 0;
				if (token.value[c] >= '0' && token.value[c] <= '9')
					v = token.value[c]-'0';
				else if (token.value[c] >= 'a' && token.value[c] <= 'f')
					v = token.value[c]-'a'+10;
				else if (token.value[c] >= 'A' && token.value[c] <= 'F')
					v = token.value[c]-'A'+10;
				else
					return false;
				v = (v << 4) + v;
				channels[c] = v/255.0f;
			}
			out_color = CL_Colorf(channels[0], channels[1], channels[2], channels[3]);
			in_out_pos = pos;
			return true;
		}
		else if (token.value.length() == 6)
		{
			float channels[3] = { 0.0f, 0.0f, 0.0f };
			for (int c = 0; c < 3; c++)
			{
				int v = 0;
				if (token.value[c*2] >= '0' && token.value[c*2] <= '9')
					v = token.value[c*2]-'0';
				else if (token.value[c*2] >= 'a' && token.value[c*2] <= 'f')
					v = token.value[c*2]-'a'+10;
				else if (token.value[c*2] >= 'A' && token.value[c*2] <= 'F')
					v = token.value[c*2]-'A'+10;
				else
					return false;

				v <<= 4;

				if (token.value[c*2+1] >= '0' && token.value[c*2+1] <= '9')
					v += token.value[c*2+1]-'0';
				else if (token.value[c*2+1] >= 'a' && token.value[c*2+1] <= 'f')
					v += token.value[c*2+1]-'a'+10;
				else if (token.value[c*2+1] >= 'A' && token.value[c*2+1] <= 'F')
					v += token.value[c*2+1]-'A'+10;
				else
					return false;

				channels[c] = v/255.0f;
			}
			out_color = CL_Colorf(channels[0], channels[1], channels[2]);
			in_out_pos = pos;
			return true;
		}
		else if (token.value.length() == 8)
		{
			float channels[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			for (int c = 0; c < 4; c++)
			{
				int v = 0;
				if (token.value[c*2] >= '0' && token.value[c*2] <= '9')
					v = token.value[c*2]-'0';
				else if (token.value[c*2] >= 'a' && token.value[c*2] <= 'f')
					v = token.value[c*2]-'a'+10;
				else if (token.value[c*2] >= 'A' && token.value[c*2] <= 'F')
					v = token.value[c*2]-'A'+10;
				else
					return false;

				v <<= 4;

				if (token.value[c*2+1] >= '0' && token.value[c*2+1] <= '9')
					v += token.value[c*2+1]-'0';
				else if (token.value[c*2+1] >= 'a' && token.value[c*2+1] <= 'f')
					v += token.value[c*2+1]-'a'+10;
				else if (token.value[c*2+1] >= 'A' && token.value[c*2+1] <= 'F')
					v += token.value[c*2+1]-'A'+10;
				else
					return false;

				channels[c] = v/255.0f;
			}
			out_color = CL_Colorf(channels[0], channels[1], channels[2], channels[3]);
			in_out_pos = pos;
			return true;
		}
	}

	return false;
}

CL_CSSPropertyParser::ColorType CL_CSSPropertyParser::colors[] =
{
	"maroon", 0x800000,
	"red",    0xff0000,
	"orange", 0xffa500,
	"yellow", 0xffff00,
	"olive",  0x808000,
	"purple", 0x800080,
	"fuchia", 0xff00ff,
	"white",  0xffffff,
	"lime",   0x00ff00,
	"green",  0x008000,
	"navy",   0x000080,
	"blue",   0x0000ff,
	"aqua",   0x00ffff,
	"teal",   0x008080,
	"black",  0x000000,
	"silver", 0xc0c0c0,
	"gray",   0x808080,
	0, 0
};
