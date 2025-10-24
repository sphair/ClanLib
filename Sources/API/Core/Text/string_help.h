/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

/// \addtogroup clanCore_Text clanCore Text
/// \{


#pragma once


#include "../api_core.h"
#include "string_types.h"
#include <vector>

#ifdef __cplusplus_cli
#pragma managed(push, on)
#include <vcclr.h>
#endif

/// \brief String helper functions.
///
/// \xmlonly !group=Core/Text! !header=core.h! \endxmlonly
class CL_API_CORE CL_StringHelp
{
/// \name Operations
/// \{

public:
	static std::vector<CL_TempString> split_text(
		const CL_StringRef &text,
		const CL_StringRef &split_string,
		bool skip_empty = true);

	static CL_TempString trim(const CL_StringRef &text);

	static int compare(const CL_StringRef8 &a, const CL_StringRef8 &b, bool case_insensitive = false);

	static int compare(const CL_StringRef16 &a, const CL_StringRef16 &b, bool case_insensitive = false);

	static CL_TempString text_to_upper(const CL_StringRef &s);

	static CL_TempString8 local8_to_upper(const CL_StringRef8 &s);

	static CL_TempString16 ucs2_to_upper(const CL_StringRef16 &s);

	static CL_TempString text_to_lower(const CL_StringRef &s);

	static CL_TempString8 local8_to_lower(const CL_StringRef8 &s);

	static CL_TempString16 ucs2_to_lower(const CL_StringRef16 &s);

	static CL_TempString float_to_text(float value, int num_decimal_places=6);

	static CL_TempString8 float_to_local8(float value, int num_decimal_places=6);

	static CL_TempString16 float_to_ucs2(float value, int num_decimal_places=6);

	static float text_to_float(const CL_StringRef &value);

	static float local8_to_float(const CL_StringRef8 &value);

	static float ucs2_to_float(const CL_StringRef16 &value);

	static CL_TempString double_to_text(double value, int num_decimal_places=6);

	static CL_TempString8 double_to_local8(double value, int num_decimal_places=6);

	static CL_TempString16 double_to_ucs2(double value, int num_decimal_places=6);

	static double text_to_double(const CL_StringRef &value);

	static double local8_to_double(const CL_StringRef8 &value);

	static double ucs2_to_double(const CL_StringRef16 &value);

	static CL_TempString int_to_text(int value);

	static CL_TempString8 int_to_local8(int value);

	static CL_TempString16 int_to_ucs2(int value);

	static int text_to_int(const CL_StringRef &value, int base = 10);

	static int local8_to_int(const CL_StringRef8 &value, int base = 10);

	static int ucs2_to_int(const CL_StringRef16 &value, int base = 10);

	static CL_TempString uint_to_text(unsigned int value);

	static CL_TempString8 uint_to_local8(unsigned int value);

	static CL_TempString16 uint_to_ucs2(unsigned int value);

	static unsigned int text_to_uint(const CL_StringRef &value, int base = 10);

	static unsigned int local8_to_uint(const CL_StringRef8 &value, int base = 10);

	static unsigned int ucs2_to_uint(const CL_StringRef16 &value, int base = 10);

	static CL_TempString bool_to_text(bool value);

	static CL_TempString8 bool_to_local8(bool value);

	static CL_TempString16 bool_to_ucs2(bool value);

	static bool text_to_bool(const CL_StringRef &value);

	static bool local8_to_bool(const CL_StringRef8 &value);

	static bool ucs2_to_bool(const CL_StringRef16 &value);

	static CL_TempString8 text_to_cp437(const CL_StringRef &text);

	static CL_TempString8 ucs2_to_cp437(const CL_StringRef16 &text);

	static CL_TempString cp437_to_text(const CL_StringRef8 &cp437);

	static CL_TempString16 cp437_to_ucs2(const CL_StringRef8 &cp437);

	static CL_TempString8 text_to_local8(const CL_StringRef &text);

	static CL_TempString8 text_to_utf8(const CL_StringRef &text);

	static CL_TempString8 ucs2_to_latin1(const CL_StringRef16 &ucs2);

	static CL_TempString8 ucs2_to_latin9(const CL_StringRef16 &ucs2);

	static CL_TempString8 ucs2_to_local8(const CL_StringRef16 &ucs2);

	static CL_TempString8 ucs2_to_utf8(const CL_StringRef16 &ucs2);

	static CL_TempString local8_to_text(const CL_StringRef8 &local8);

	static CL_TempString ucs2_to_text(const CL_StringRef16 &ucs2);

	static CL_TempString16 latin1_to_ucs2(const CL_StringRef8 &latin1);

	static CL_TempString16 latin9_to_ucs2(const CL_StringRef8 &latin9);

	static CL_TempString16 local8_to_ucs2(const CL_StringRef8 &local8);

	static CL_TempString16 utf8_to_ucs2(const CL_StringRef8 &utf8);

	static CL_TempString utf8_to_text(const CL_StringRef8 &utf8);

#ifdef __cplusplus_cli
#ifdef UNICODE
	static CL_TempString clr_to_text(const System::String ^str)
	{
		return clr_to_ucs2(str);
	}

	static System::String ^text_to_clr(const CL_StringRef &str)
	{
		return ucs2_to_clr(str);
	}
#else
	static CL_TempString clr_to_text(const System::String ^str)
	{
		return ucs2_to_text(clr_to_ucs2(str));
	}

	static System::String ^text_to_clr(const CL_StringRef &str)
	{
		return ucs2_to_clr(local8_to_ucs2(str));
	}
#endif
	static CL_TempString16 clr_to_ucs2(const System::String ^str)
	{
		cli::pin_ptr<const wchar_t> chars = PtrToStringChars(str);
		return CL_TempString16(chars);
	}

	static System::String ^ucs2_to_clr(const CL_StringRef16 &str)
	{
		return gcnew System::String(str.c_str());
	}
#endif


/// \}
/// \name Implementation
/// \{

private:
	static const char trailing_bytes_for_utf8[256];

	static const unsigned char bitmask_leadbyte_for_utf8[6];
/// \}
};

#ifdef __cplusplus_cli
#pragma managed(pop)
#endif


/// \}
