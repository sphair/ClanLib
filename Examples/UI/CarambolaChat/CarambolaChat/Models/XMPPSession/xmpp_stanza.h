
#pragma once

#include "xmpp_token.h"

class XMPPStanza
{
public:
	XMPPToken stream_token;
	std::vector<XMPPToken> tokens;

	bool empty() const;

	bool is_element(int index, const CL_String &local_name, const CL_String &namespace_uri) const;
	bool is_text(int index) const;

	CL_String get_attribute(int index, const CL_String &local_name, const CL_String &namespace_uri) const;
	CL_String get_text(int index) const;

	int find_child(int index, const CL_String &local_name, const CL_String &namespace_uri) const;
	int find_parent(int index) const;
	int find_next_sibling(int index) const;

	CL_String to_string(int level = 0) const;

private:
	CL_String token_to_string(int index) const;
	CL_String get_namespace_uri(const CL_String &xmlns, int index) const;
	static CL_String get_prefix(const CL_String &tag);
	static CL_String get_local_name(const CL_String &tag);
};
