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

#include "RegExp/precomp.h"
#include "API/RegExp/regexp_match.h"

/////////////////////////////////////////////////////////////////////////////
// CL_RegExpMatch_Impl Class:

class CL_RegExpMatch_Impl
{
//! Construction:
public:
	CL_RegExpMatch_Impl()
	: vector(0), size(0), allocated(0), captures_count(0), partial(false)
	{
	}

	~CL_RegExpMatch_Impl()
	{
		delete[] vector;
	}

//! Attributes:
public:
	int *vector;
	
	int size;
	
	int allocated;
	
	int captures_count;
	
	bool partial;
};

/////////////////////////////////////////////////////////////////////////////
// CL_RegExpMatch Construction:

CL_RegExpMatch::CL_RegExpMatch()
: impl(new CL_RegExpMatch_Impl)
{
}

CL_RegExpMatch::~CL_RegExpMatch()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_RegExpMatch Attributes:

const int *CL_RegExpMatch::get_vector() const
{
	return impl->vector;
}
	
int *CL_RegExpMatch::get_vector()
{
	return impl->vector;
}
	
int CL_RegExpMatch::get_vector_size() const
{
	return impl->size;
}

CL_StringRef8 CL_RegExpMatch::get_capture(const char *subject, int capture) const
{
	return CL_StringRef8(
		subject + get_capture_pos(capture),
		(CL_StringRef8::size_type) get_capture_length(capture),
		false);
}

CL_StringRef8 CL_RegExpMatch::get_capture(const CL_StringRef8 &subject, int capture) const
{
	return get_capture(subject.data(), capture);
}

int CL_RegExpMatch::get_capture_pos(int capture) const
{
	if (capture < 0 || capture >= impl->captures_count)
		return -1;
	return impl->vector[capture*2];
}
	
int CL_RegExpMatch::get_capture_length(int capture) const
{
	if (capture < 0 || capture >= impl->captures_count)
		return 0;
	return impl->vector[capture*2+1] - impl->vector[capture*2];
}
	
int CL_RegExpMatch::get_capture_end(int capture) const
{
	if (capture < 0 || capture >= impl->captures_count)
		return 0;
	return impl->vector[capture*2+1];
}
	
int CL_RegExpMatch::get_captures_count() const
{
	return impl->captures_count;
}

bool CL_RegExpMatch::is_partial() const
{
	return impl->partial;
}
	
bool CL_RegExpMatch::is_match() const
{
	return impl->captures_count > 0;
}

/////////////////////////////////////////////////////////////////////////////	
// CL_RegExpMatch Operations:

CL_RegExpMatch &CL_RegExpMatch::operator =(const CL_RegExpMatch &other)
{
	impl = other.impl;
	return *this;
}

void CL_RegExpMatch::set_vector_size(int new_size)
{
	impl->captures_count = 0;
	impl->partial = false;
	if (new_size > impl->allocated)
	{
		delete[] impl->vector;
		impl->vector = new int[new_size];
		impl->allocated = new_size;
	}
	impl->size = new_size;
}
	
void CL_RegExpMatch::set_captures_count(int count)
{
	impl->captures_count = count;
}
	
void CL_RegExpMatch::set_partial_match(bool new_partial)
{
	impl->partial = new_partial;
}

/////////////////////////////////////////////////////////////////////////////
// CL_RegExpMatch Implementation:
