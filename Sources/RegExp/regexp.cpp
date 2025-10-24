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

#include "RegExp/precomp.h"
#include "API/RegExp/regexp.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/System/exception.h"

#define PCRE_STATIC

#ifdef __APPLE__
#include "PCRE/pcre.h"
#else
#include <pcre.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_RegExp_Impl Class:

class CL_RegExp_Impl
{
//! Construction:
public:
	CL_RegExp_Impl()
	: code(0), extra(0)
	{
	}

	~CL_RegExp_Impl()
	{
		if (extra)
			pcre_free(extra);
		if (code)
			pcre_free(code);
	}

//! Attributes:
public:
	pcre *code;
	
	pcre_extra *extra;

//! Operations:
public:
	void compile(const char *expression, int compile_flags, bool study);
};

/////////////////////////////////////////////////////////////////////////////
// CL_RegExp Construction:

CL_RegExp::CL_RegExp(const char *expression, int compile_flags, bool study)
: impl(new CL_RegExp_Impl)
{
	impl->compile(expression, compile_flags, study);
}

CL_RegExp::CL_RegExp(const CL_StringRef8 &expression, int compile_flags, bool study)
: impl(new CL_RegExp_Impl)
{
	impl->compile(expression.c_str(), compile_flags, study);
}

CL_RegExp::~CL_RegExp()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_RegExp Attributes:

int CL_RegExp::get_string_number(const char *name) const
{
	return pcre_get_stringnumber(impl->code, name);
}
	
int CL_RegExp::get_string_number(const CL_StringRef8 &name) const
{
	return get_string_number(name.c_str());
}

/////////////////////////////////////////////////////////////////////////////
// CL_RegExp Operations:

CL_RegExpMatch CL_RegExp::search(
	const char *subject,
	int length,
	int start_offset,
	int search_flags) const
{
	CL_RegExpMatch match;
	search(subject, length, start_offset, search_flags, match);
	return match;
}

CL_RegExpMatch CL_RegExp::search(
	const CL_StringRef8 &subject,
	int start_offset,
	int search_flags) const
{
	CL_RegExpMatch match;
	search(subject.data(), subject.length(), start_offset, search_flags, match);
	return match;
}

void CL_RegExp::search(
	const char *subject,
	int length,
	int start_offset,
	int search_flags,
	CL_RegExpMatch &result) const
{
	int flags = 0;
	if (search_flags & search_anchored)
		flags |= PCRE_ANCHORED;
	if (search_flags & search_not_bol)
		flags |= PCRE_NOTBOL;
	if (search_flags & search_not_eol)
		flags |= PCRE_NOTEOL;
	if (search_flags & search_not_empty)
		flags |= PCRE_NOTEMPTY;
	if (search_flags & search_no_utf8_check)
		flags |= PCRE_NO_UTF8_CHECK;
	if (search_flags & search_partial)
		flags |= PCRE_PARTIAL;

	int captures_count = 0;
	int r = pcre_fullinfo(impl->code, impl->extra, PCRE_INFO_CAPTURECOUNT, &captures_count);
	if (r != 0)
		throw CL_Exception("Regular expression error: Unable to get captures count");
	result.set_vector_size((captures_count+1)*3);
		
	r = pcre_exec(
		impl->code,
		impl->extra,
		subject,
		length,
		start_offset,
		flags,
		result.get_vector(),
		result.get_vector_size());
	if (r == PCRE_ERROR_NOMATCH)
	{
		result.set_captures_count(0);
	}
	else if (r == PCRE_ERROR_PARTIAL)
	{
		result.set_partial_match(true);
	}
	else if (r > 0)
	{
		result.set_captures_count(r);
	}
	else
	{
		switch (r)
		{
		case PCRE_ERROR_NULL:
			throw CL_Exception("Regular expression error: NULL pointer passed");
		case PCRE_ERROR_BADOPTION:
			throw CL_Exception("Regular expression error: Unrecognized bit was set in the options argument");
		case PCRE_ERROR_BADMAGIC:
			throw CL_Exception("Regular expression error: Bad Magic! Accessing deleted CL_RegExp object?");
		case PCRE_ERROR_UNKNOWN_NODE:
			throw CL_Exception("Regular expression error: While running the pattern match, an unknown item was encountered in the compiled pattern");
		case PCRE_ERROR_NOMEMORY:
			throw CL_Exception("Regular expression error: Out of memory");
		case PCRE_ERROR_NOSUBSTRING:
			throw CL_Exception("Regular expression error: No sub string found");
		case PCRE_ERROR_MATCHLIMIT:
			throw CL_Exception("Regular expression error: Match recursion limit reached");
		case PCRE_ERROR_CALLOUT:
			throw CL_Exception("Regular expression error: Error signalled from callout handler");
		case PCRE_ERROR_BADUTF8:
			throw CL_Exception("Regular expression error: A string that contains an invalid UTF-8 byte sequence was passed as a subject");
		case PCRE_ERROR_BADUTF8_OFFSET:
			throw CL_Exception("Regular expression error: Start offset did not point to the beginning of a UTF-8 character");
		case PCRE_ERROR_BADPARTIAL:
			throw CL_Exception("Regular expression error: Expression includes items not supported by partial matching");
		case PCRE_ERROR_INTERNAL:
			throw CL_Exception("Regular expression error: An unexpected internal error has occurred.");
		case PCRE_ERROR_BADCOUNT:
			throw CL_Exception("Regular expression error: The value of the ovecsize argument is negative");
		default:
			throw CL_Exception("Regular expression error: Unknown error");
		}
	}
}

void CL_RegExp::search(
	const CL_StringRef8 &subject,
	int start_offset,
	int search_flags,
	CL_RegExpMatch &result) const
{
	search(subject.data(), subject.length(), start_offset, search_flags, result);
}

/////////////////////////////////////////////////////////////////////////////
// CL_RegExp Implementation:

void CL_RegExp_Impl::compile(const char *expression, int compile_flags, bool study)
{
	const char *error = 0;
	int error_offset = 0;
	const unsigned char *tableptr = 0;
	
	int flags = 0;
	if (compile_flags & CL_RegExp::compile_anchored)
		flags |= PCRE_ANCHORED;
	if (compile_flags & CL_RegExp::compile_auto_callout)
		flags |= PCRE_AUTO_CALLOUT;
	if (compile_flags & CL_RegExp::compile_caseless)
		flags |= PCRE_CASELESS;
	if (compile_flags & CL_RegExp::compile_dollar_endonly)
		flags |= PCRE_DOLLAR_ENDONLY;
	if (compile_flags & CL_RegExp::compile_dot_all)
		flags |= PCRE_DOTALL;
	if (compile_flags & CL_RegExp::compile_extended)
		flags |= PCRE_EXTENDED;
	if (compile_flags & CL_RegExp::compile_extra)
		flags |= PCRE_EXTRA;
	if (compile_flags & CL_RegExp::compile_multi_line)
		flags |= PCRE_MULTILINE;
	if (compile_flags & CL_RegExp::compile_no_auto_capture)
		flags |= PCRE_NO_AUTO_CAPTURE;
	if (compile_flags & CL_RegExp::compile_ungreedy)
		flags |= PCRE_UNGREEDY;
	if (compile_flags & CL_RegExp::compile_utf8)
		flags |= PCRE_UTF8;
	if (compile_flags & CL_RegExp::compile_no_utf8_check)
		flags |= PCRE_NO_UTF8_CHECK;

	code = pcre_compile(
		expression,
		flags,
		&error,
		&error_offset,
		tableptr);
	if (code == 0)
	{
		CL_StringFormat s("Error in regular expression: %1 (at position %2)");
		s.set_arg(1, error);
		s.set_arg(2, error_offset);
		throw CL_Exception(s.get_result());
	}
	
	if (study)
	{
		extra = pcre_study(
			code,
			0,
			&error);
		// extra can be null if there's no extra data gained by the study.
		if (extra == 0 && error != 0)
		{
			CL_StringFormat s("Error studying regular expression: %1");
			s.set_arg(1, error);
			pcre_free(code);
			code = 0;
			throw CL_Exception(s.get_result());
		}
	}
}
