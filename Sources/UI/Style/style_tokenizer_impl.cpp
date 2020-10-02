/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#include "UI/precomp.h"
#include "style_tokenizer_impl.h"
#include "API/Core/Math/cl_math.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/Text/string_help.h"
#include "API/UI/Style/style_token.h"

namespace clan
{
	StyleTokenizer_Impl::StyleTokenizer_Impl(const std::string &text)
	: doc(text), pos(0)
	{
	}

	void StyleTokenizer_Impl::peek(StyleToken &out_token)
	{
		size_t cur_pos = pos;
		read(out_token);
		pos = cur_pos;
	}

	void StyleTokenizer_Impl::read(StyleToken &token)
	{
		token.type = StyleTokenType::null;
		token.dimension.clear();
		token.value.clear();
		if (pos >= doc.length())
			return;
		switch (doc[pos])
		{
		case '@':
			read_atkeyword(token);
			break;
		case '#':
			read_hash(token);
			break;
		case '<':
			read_cdo(token);
			break;
		case '-':
			read_cdc(token);
			break;
		case ':':
			token.type = StyleTokenType::colon;
			pos++;
			break;
		case ';':
			token.type = StyleTokenType::semi_colon;
			pos++;
			break;
		case '{':
			token.type = StyleTokenType::curly_brace_begin;
			pos++;
			break;
		case '}':
			token.type = StyleTokenType::curly_brace_end;
			pos++;
			break;
		case '(':
			token.type = StyleTokenType::bracket_begin;
			pos++;
			break;
		case ')':
			token.type = StyleTokenType::bracket_end;
			pos++;
			break;
		case '[':
			token.type = StyleTokenType::square_bracket_begin;
			pos++;
			break;
		case ']':
			token.type = StyleTokenType::square_bracket_end;
			pos++;
			break;
		case '/':
			read_comment(token);
			break;
		case 'u':
		case 'U':
			read_uri(token);
			// read_unicode_range(token);
			break;
		case ' ':
		case '\t':
		case '\r':
		case '\n':
		case '\f':
			read_whitespace(token);
			break;
		case '~':
			read_includes(token);
			break;
		case '|':
			read_dashmatch(token);
			break;
		case '.':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			read_number_type(token);
			break;
		}
		if (token.type != StyleTokenType::null)
			return;

		read_function(token);
		if (token.type != StyleTokenType::null)
			return;

		pos = read_ident(pos, token.value);
		if (!token.value.empty())
		{
			token.type = StyleTokenType::ident;
			return;
		}

		if (doc[pos] == '"')
		{
			size_t end_pos = read_string(pos+1, token.value, '"');
			if (end_pos != pos+1)
			{
				token.type = StyleTokenType::string;
				pos = end_pos;
				return;
			}
			else
			{
				token.type = StyleTokenType::invalid;
				pos = read_invalid(pos+1);
				return;
			}
		}
		else if (doc[pos] == '\'')
		{
			size_t end_pos = read_string(pos+1, token.value, '\'');
			if (end_pos != pos+1)
			{
				token.type = StyleTokenType::string;
				pos = end_pos;
				return;
			}
			else
			{
				token.type = StyleTokenType::invalid;
				pos = read_invalid(pos+1);
				return;
			}
		}
		else
		{
			token.type = StyleTokenType::delim;
			token.value.append(1, doc[pos]);
			pos++;
		}
	}

	void StyleTokenizer_Impl::read_number_type(StyleToken &token)
	{
		bool dot_encountered = false;
		size_t end_pos = pos;
		while (end_pos < doc.length())
		{
			if (doc[end_pos] == '.' && !dot_encountered)
			{
				dot_encountered = true;
			}
			else if (doc[end_pos] < '0' || doc[end_pos] > '9')
			{
				break;
			}
			end_pos++;
		}

		if (!(pos+1 == end_pos && dot_encountered))
		{
			token.value = doc.substr(pos, end_pos-pos);
			if (end_pos < doc.length() && doc[end_pos] == '%')
			{
				token.type = StyleTokenType::percentage;
				pos = end_pos+1;
			}
			else
			{
				end_pos = read_ident(end_pos, token.dimension);
				if (!token.dimension.empty())
					token.type = StyleTokenType::dimension;
				else
					token.type = StyleTokenType::number;
				pos = end_pos;
			}
		}
	}

	void StyleTokenizer_Impl::read_atkeyword(StyleToken &token)
	{
		if (pos+2 <= doc.length() && doc[pos] == '@')
		{
			pos = read_ident(pos+1, token.value);
			if (!token.value.empty())
				token.type = StyleTokenType::atkeyword;
		}
	}

	void StyleTokenizer_Impl::read_hash(StyleToken &token)
	{
		if (pos+2 <= doc.length() && doc[pos] == '#')
		{
			pos = read_name(pos+1, token.value);
			if (!token.value.empty())
				token.type = StyleTokenType::hash;
		}
	}

	size_t StyleTokenizer_Impl::read_ident(size_t p, std::string &out_ident)
	{
		out_ident.clear();
		std::string::value_type c = 0;
		size_t len = 0;
		if (p+2 <= doc.length() && doc[p] == '-')
		{
			len = read_nmstart(p+1, c);
			if (len > 0)
			{
				out_ident.append(1, '-');
				len++;
			}
		}
		else
		{
			len = read_nmstart(p, c);
		}
		if (len > 0)
		{
			out_ident.append(1, c);
			size_t end_pos = p+len;
			while (end_pos < doc.length())
			{
				len = read_nmchar(end_pos, c);
				if (len == 0)
					break;
				out_ident.append(1, c);
				end_pos += len;
			}
			p = end_pos;
		}
		return p;
	}

	size_t StyleTokenizer_Impl::read_name(size_t p, std::string &out_ident)
	{
		out_ident.clear();
		std::string::value_type c;
		size_t len = read_nmchar(p, c);
		if (len > 0)
		{
			out_ident.append(1, c);
			size_t end_pos = p+len;
			while (end_pos < doc.length())
			{
				len = read_nmchar(end_pos, c);
				if (len == 0)
					break;
				out_ident.append(1, c);
				end_pos += len;
			}
			p = end_pos;
		}
		return p;
	}

	void StyleTokenizer_Impl::read_cdo(StyleToken &token)
	{
		if (pos+4 <= doc.length() && doc[pos] == '<' && doc[pos+1] == '!' && doc[pos+2] == '-' && doc[pos+3] == '-')
		{
			token.type = StyleTokenType::cdo;
			token.value.clear();
			pos += 4;
		}
	}

	void StyleTokenizer_Impl::read_cdc(StyleToken &token)
	{
		if (pos+3 <= doc.length() && doc[pos] == '-' && doc[pos+1] == '-' && doc[pos+2] == '>')
		{
			token.type = StyleTokenType::cdc;
			token.value.clear();
			pos += 3;
		}
	}

	void StyleTokenizer_Impl::read_comment(StyleToken &token)
	{
		if (pos+4 <= doc.length() && doc[pos] == '/' && doc[pos+1] == '*')
		{
			size_t end_pos = pos+3;
			while (end_pos < doc.length() && !(doc[end_pos] == '/' && doc[end_pos-1] == '*'))
				end_pos++;
			if (end_pos < doc.length())
			{
				token.type = StyleTokenType::comment;
				pos = end_pos+1;
			}
		}
	}

	void StyleTokenizer_Impl::read_uri(StyleToken &token)
	{
		if (pos+5 <= doc.length() &&
			(doc[pos] == 'u' || doc[pos] == 'U') &&
			(doc[pos+1] == 'r' || doc[pos+1] == 'R') &&
			(doc[pos+2] == 'l' || doc[pos+2] == 'L') &&
			doc[pos+3] == '(')
		{
			size_t end_pos = pos+4;
			while (end_pos < doc.length() && is_whitespace(doc[end_pos]))
				end_pos++;
			if (end_pos < doc.length())
			{
				switch (doc[end_pos])
				{
				case '"': // string1
					end_pos = read_string(end_pos+1, token.value, '"');
					break;
				case '\'': // string2
					end_pos = read_string(end_pos+1, token.value, '\'');
					break;
				default: // ([!#$%&*-~]|{nonascii}|{escape})*
					end_pos = read_uri_nonquoted_string(end_pos, token.value);
					break;
				case ')':
					token.value.clear();
					break;
				}
			}
			while (end_pos < doc.length() && is_whitespace(doc[end_pos]))
				end_pos++;

			if (end_pos < doc.length() && doc[end_pos] == ')')
			{
				token.type = StyleTokenType::uri;
				pos = end_pos+1;
			}
		}
	}

	void StyleTokenizer_Impl::read_function(StyleToken &token)
	{
		size_t end_pos = read_ident(pos, token.value);
		if (end_pos != pos)
		{
			if (end_pos+1 <= doc.length() && doc[end_pos] == '(')
			{
				pos = end_pos+1;
				token.type = StyleTokenType::function;
			}
		}
	}

	void StyleTokenizer_Impl::read_whitespace(StyleToken &token)
	{
		if (pos+1 <= doc.length() && is_whitespace(doc[pos]))
		{
			token.type = StyleTokenType::whitespace;
			token.value.clear();
			size_t end_pos = pos+1;
			while (end_pos < doc.length() && is_whitespace(doc[end_pos]))
				end_pos++;
			pos = end_pos;
		}
	}

	void StyleTokenizer_Impl::read_includes(StyleToken &token)
	{
		if (pos+2 <= doc.length() && doc[pos] == '~' && doc[pos+1] == '=')
		{
			token.type = StyleTokenType::includes;
			token.value.clear();
			pos += 2;
		}
	}

	void StyleTokenizer_Impl::read_dashmatch(StyleToken &token)
	{
		if (pos+2 <= doc.length() && doc[pos] == '|' && doc[pos+1] == '=')
		{
			token.type = StyleTokenType::dashmatch;
			token.value.clear();
			pos += 2;
		}
	}

	size_t StyleTokenizer_Impl::read_nmstart(size_t p, std::string::value_type &out_c)
	{
		out_c = '?';
		if ((p < doc.length()) && (doc[p] == '_' ||
			(doc[p] >= 'a' && doc[p] <= 'z') ||
			(doc[p] >= 'A' && doc[p] <= 'Z') ||
			((unsigned char)doc[p]) > 177))
		{
			out_c = doc[p];
			return 1;
		}
		else if (p+1 < doc.length() && doc[p] == '\\')
		{
			if ((doc[p+1] >= '0' && doc[p+1] <= '9') ||
				(doc[p+1] >= 'a' && doc[p+1] <= 'f') ||
				(doc[p+1] >= 'A' && doc[p+1] <= 'F'))
			{
				unsigned int unicode_char = 0;
				size_t p1 = p+1;
				size_t p2 = p1;
				while (p2 < doc.length() && p2-p1 < 6)
				{
					if (doc[p+1] >= '0' && doc[p+1] <= '9')
					{
						unicode_char <<= 4;
						unicode_char += (unsigned int)(doc[p+1]-'0');
					}
					else if (doc[p+1] >= 'a' && doc[p+1] <= 'f')
					{
						unicode_char <<= 4;
						unicode_char += (unsigned int)(doc[p+1]-'a')+10;
					}
					else if (doc[p+1] >= 'A' && doc[p+1] <= 'F')
					{
						unicode_char <<= 4;
						unicode_char += (unsigned int)(doc[p+1]-'A')+10;
					}
					else
					{
						break;
					}
					p2++;
				}

				if (p2 + 1 < doc.length() && doc[p2] == '\r' && doc[p2+1] == '\n')
				{
					p2+=2;
				}
				else if (p2 < doc.length() && (doc[p2] == '\r' || doc[p2] == '\n' || doc[p2] == '\t' || doc[p2] == '\f'))
				{
					p2++;
				}

				// To do: return the char as utf-8
				out_c = (std::string::value_type)unicode_char;
				return p2-p1;
			}
			else if (doc[p+1] != '\r' &&
				doc[p+1] != '\n' &&
				doc[p+1] != '\t' &&
				doc[p+1] != '\f')
			{
				out_c = doc[p+1];
				return 2;
			}
		}

		return 0;
	}

	size_t StyleTokenizer_Impl::read_nmchar(size_t p, std::string::value_type &out_c)
	{
		if (p+1 <= doc.length())
		{
			out_c = doc[p];
			if (doc[p] >= '0' && doc[p] <= '9')
				return 1;
			else if (doc[p] == '-')
				return 1;
			else
				return read_nmstart(p, out_c);
		}
		else
		{
			return 0;
		}
	}

	bool StyleTokenizer_Impl::is_whitespace(std::string::value_type c)
	{
		switch (c)
		{
		case ' ':
		case '\t':
		case '\r':
		case '\n':
		case '\f':
			return true;
		default:
			return false;
		}
	}

	size_t StyleTokenizer_Impl::read_string(size_t p, std::string &out_str, std::string::value_type str_char) const
	{
		out_str.clear();
		size_t end_pos = p;
		while (end_pos < doc.length() && doc[end_pos] != str_char)
		{
			if (doc[end_pos] == '\\')
			{
				if (end_pos+2 < doc.length() && doc[end_pos+1] == '\r' && doc[end_pos+2] == '\n')
				{
					out_str.append(1, '\n');
					end_pos += 3;
				}
				else if (doc[end_pos+1] == '\n' || doc[end_pos+1] == '\f')
				{
					out_str.append(1, '\n');
					end_pos += 2;
				}
				else
				{
					// escape
					if ((doc[end_pos+1] >= 'a' && doc[end_pos+1] <= 'z') ||
						(doc[end_pos+1] >= 'A' && doc[end_pos+1] <= 'Z') ||
						(doc[end_pos+1] >= '0' && doc[end_pos+1] <= '9'))
					{
						size_t i;
						unsigned int character = 0;
						for (i = end_pos+1; i < doc.length(); i++)
						{
							int v = 0;
							if ((doc[i] >= 'a' && doc[i] <= 'z'))
							{
								v = 10+(doc[i]-'a');
							}
							else if (doc[i] >= 'A' && doc[i] <= 'Z')
							{
								v = 10+(doc[i]-'A');
							}
							else if (doc[i] >= '0' && doc[i] <= '9')
							{
								v = doc[i]-'0';
							}
							else if (doc[i] == ' ' || doc[i] == '\t')
							{
								i++;
								break;
							}
							else
							{
								break;
							}
							character <<= 4;
							character += v;
						}
						if (character == 0)
							character = 0xfffd;
						out_str.append(1, character);
						end_pos = i;
					}
					else
					{
						out_str.append(1, doc[end_pos+1]);
						end_pos+=2;
					}
				}
			}
			else if (end_pos+1 < doc.length() && doc[end_pos] == '\r' && doc[end_pos+1] == '\n')
			{
				out_str.clear();
				return p;
			}
			else if (doc[end_pos] == '\n')
			{
				out_str.clear();
				return p;
			}
			else
			{
				out_str.append(1, doc[end_pos]);
				end_pos++;
			}
		}
		if (end_pos == doc.length())
		{
			out_str.clear();
			return p;
		}
		return end_pos+1;
	}

	size_t StyleTokenizer_Impl::read_invalid(size_t p) const
	{
		size_t end_pos = p;
		while (end_pos < doc.length() && doc[end_pos] != '\n')
			end_pos++;
		size_t doc_length = doc.length();
		return min((end_pos+1), doc_length);
	}

	size_t StyleTokenizer_Impl::read_uri_nonquoted_string(size_t p, std::string &out_str) const
	{
		out_str.clear();
		size_t end_pos = p;
		while (end_pos < doc.length() && doc[end_pos] != ' ' && doc[end_pos] != '\t' && doc[end_pos] != ')')
		{
			if (doc[end_pos] == '\\')
			{
				if (end_pos+2 < doc.length() && doc[end_pos+1] == '\r' && doc[end_pos+2] == '\n')
				{
					out_str.append(1, '\n');
					end_pos += 3;
				}
				else if (doc[end_pos+1] == '\n')
				{
					out_str.append(1, '\n');
					end_pos += 2;
				}
				else
				{
					out_str.append(1, '?');
					// escape
					end_pos+=2;
				}
			}
			else if (end_pos+1 < doc.length() && doc[end_pos] == '\r' && doc[end_pos+1] == '\n')
			{
				out_str.clear();
				return p;
			}
			else if (doc[end_pos] == '\n')
			{
				out_str.clear();
				return p;
			}
			else
			{
				out_str.append(1, doc[end_pos]);
				end_pos++;
			}
		}
		if (end_pos == doc.length())
		{
			out_str.clear();
			return p;
		}
		return end_pos;
	}
}
