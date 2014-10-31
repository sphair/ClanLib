
#include "precomp.h"
#include "xmpp_stanza.h"

bool XMPPStanza::empty() const
{
	return tokens.empty();
}

bool XMPPStanza::is_element(int index, const CL_String &local_name, const CL_String &namespace_uri) const
{
	if (index < 0 || index >= (int)tokens.size())
	{
		return false;
	}
	else if (tokens[index].type == XMPPToken::type_begin || tokens[index].type == XMPPToken::type_end || tokens[index].type == XMPPToken::type_single)
	{
		CL_String tag_local_name = get_local_name(tokens[index].name);
		if (tag_local_name == local_name)
		{
			CL_String tag_prefix = get_prefix(tokens[index].name);
			CL_String xmlns = "xmlns";
			if (!tag_prefix.empty())
				xmlns += ":" + tag_prefix;
			return get_namespace_uri(xmlns, index) == namespace_uri;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool XMPPStanza::is_text(int index) const
{
	if (index < 0 || index >= (int)tokens.size())
	{
		return false;
	}
	else
	{
		return tokens[index].type == XMPPToken::type_text;
	}
}

CL_String XMPPStanza::get_attribute(int index, const CL_String &local_name, const CL_String &namespace_uri) const
{
	if (index < 0 || index >= (int)tokens.size())
	{
		return CL_String();
	}
	else
	{
		for (size_t i = 0; i < tokens[index].attributes.size(); i++)
		{
			CL_String tag_local_name = get_local_name(tokens[index].attributes[i].name);
			if (tag_local_name == local_name)
			{
				CL_String tag_prefix = get_prefix(tokens[index].attributes[i].name);
				CL_String xmlns = "xmlns";
				if (!tag_prefix.empty())
					xmlns += ":" + tag_prefix;
				if (get_namespace_uri(xmlns, index) == namespace_uri)
					return tokens[index].attributes[i].value;
			}
		}

		return CL_String();
	}
}

CL_String XMPPStanza::get_text(int index) const
{
	if (index < 0 || index >= (int)tokens.size())
	{
		return CL_String();
	}
	else
	{
		return tokens[index].value;
	}
}

int XMPPStanza::find_child(int index, const CL_String &local_name, const CL_String &namespace_uri) const
{
	if (index < 0 || index + 1 >= (int)tokens.size())
	{
		return -1;
	}
	else
	{
		index++;
		while (index != -1)
		{
			if (is_element(index, local_name, namespace_uri))
				return index;
			index = find_next_sibling(index);
		}
		return -1;
	}
}

int XMPPStanza::find_parent(int index) const
{
	int level = 1;
	while (level > 0)
	{
		index--;
		if (index == -1)
			break;
		if (tokens[index].type == XMPPToken::type_begin)
			level--;
		else if (tokens[index].type == XMPPToken::type_end)
			level++;
	}
	return index;
}

int XMPPStanza::find_next_sibling(int index) const
{
	int level = 0;
	while (index < (int)tokens.size())
	{
		if (tokens[index].type == XMPPToken::type_begin)
			level++;
		else if (tokens[index].type == XMPPToken::type_end)
			level--;
		index++;
		if (level == 0)
			break;
	}
	return (index == tokens.size()) ? -1 : index;
}

CL_String XMPPStanza::to_string(int level) const
{
	CL_String str;
	for (size_t i = 0; i < tokens.size(); i++)
	{
		if (i > 0)
			str += "\n";

		if (tokens[i].type == XMPPToken::type_end)
			level--;
		CL_String line = CL_String(level*4, ' ');
		line += token_to_string(i);
		if (i + 2 < tokens.size() && tokens[i].type == XMPPToken::type_begin && tokens[i+1].type == XMPPToken::type_text && tokens[i+2].type == XMPPToken::type_end)
		{
			line += token_to_string(i + 1);
			line += token_to_string(i + 2);
			i += 2;
		}
		else if (tokens[i].type == XMPPToken::type_begin)
			level++;
		str += line;
	}
	return str;
}

CL_String XMPPStanza::token_to_string(int index) const
{
	CL_String line;

	if (tokens[index].type == XMPPToken::type_text)
	{
		line += tokens[index].value;
	}
	else
	{
		line += "<";
		if (tokens[index].type == XMPPToken::type_end)
			line += "/";
		line += tokens[index].name;
		for (size_t i = 0; i < tokens[index].attributes.size(); i++)
		{
			line += " ";
			line += tokens[index].attributes[i].name;
			line += "='";
			line += tokens[index].attributes[i].value;
			line += "'";
		}
		if (tokens[index].type == XMPPToken::type_single)
			line += "/";
		line += ">";
	}

	return line;
}

CL_String XMPPStanza::get_namespace_uri(const CL_String &xmlns, int index) const
{
	while (index != -1)
	{
		for (size_t i = 0; i < tokens[index].attributes.size(); i++)
		{
			if (tokens[index].attributes[i].name == xmlns)
				return tokens[index].attributes[i].value;
		}
		index = find_parent(index);
	}

	if (index == -1)
	{
		for (size_t i = 0; i < stream_token.attributes.size(); i++)
		{
			if (stream_token.attributes[i].name == xmlns)
				return stream_token.attributes[i].value;
		}
	}

	return CL_String();
}

CL_String XMPPStanza::get_prefix(const CL_String &tag)
{
	CL_String::size_type p = tag.find(':');
	if (p == CL_String::npos)
		return CL_String();
	else
		return tag.substr(0, p);
}

CL_String XMPPStanza::get_local_name(const CL_String &tag)
{
	CL_String::size_type p = tag.find(':');
	if (p == CL_String::npos)
		return tag;
	else
		return tag.substr(p + 1);
}
