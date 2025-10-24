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
	static std::vector<CL_String> split_text(
		const CL_StringRef &text,
		const CL_StringRef &split_string,
		bool skip_empty = true);

	/// \brief Wide char to utf-8
	///
	/// \param value = Wide char value
	/// \return The UTF-8 String
	static CL_String8 wchar_to_utf8(wchar_t value);

	/// \brief Trim
	///
	/// \param text = String Ref
	///
	/// \return Temp String
	static CL_String trim(const CL_StringRef &text);

	/// \brief Compare
	///
	/// \param a = String Ref8
	/// \param b = String Ref8
	/// \param case_insensitive = bool
	///
	/// \return int
	static int compare(const CL_StringRef8 &a, const CL_StringRef8 &b, bool case_insensitive = false);

	/// \brief Compare
	///
	/// \param a = String Ref16
	/// \param b = String Ref16
	/// \param case_insensitive = bool
	///
	/// \return int
	static int compare(const CL_StringRef16 &a, const CL_StringRef16 &b, bool case_insensitive = false);

	/// \brief Text to upper
	///
	/// \param s = String Ref
	///
	/// \return Temp String
	static CL_String text_to_upper(const CL_StringRef &s);

	/// \brief Local8 to upper
	///
	/// \param s = String Ref8
	///
	/// \return Temp String8
	static CL_String8 local8_to_upper(const CL_StringRef8 &s);

	/// \brief Ucs2 to upper
	///
	/// \param s = String Ref16
	///
	/// \return Temp String16
	static CL_String16 ucs2_to_upper(const CL_StringRef16 &s);

	/// \brief Text to lower
	///
	/// \param s = String Ref
	///
	/// \return Temp String
	static CL_String text_to_lower(const CL_StringRef &s);

	/// \brief Local8 to lower
	///
	/// \param s = String Ref8
	///
	/// \return Temp String8
	static CL_String8 local8_to_lower(const CL_StringRef8 &s);

	/// \brief Ucs2 to lower
	///
	/// \param s = String Ref16
	///
	/// \return Temp String16
	static CL_String16 ucs2_to_lower(const CL_StringRef16 &s);

	/// \brief Float to text
	///
	/// \param value = value
	/// \param num_decimal_places = value
	///
	/// \return Temp String
	static CL_String float_to_text(float value, int num_decimal_places=6);

	/// \brief Float to local8
	///
	/// \param value = value
	/// \param num_decimal_places = value
	///
	/// \return Temp String8
	static CL_String8 float_to_local8(float value, int num_decimal_places=6);

	/// \brief Float to ucs2
	///
	/// \param value = value
	/// \param num_decimal_places = value
	///
	/// \return Temp String16
	static CL_String16 float_to_ucs2(float value, int num_decimal_places=6);

	/// \brief Text to float
	///
	/// \param value = String Ref
	///
	/// \return float
	static float text_to_float(const CL_StringRef &value);

	/// \brief Local8 to float
	///
	/// \param value = String Ref8
	///
	/// \return float
	static float local8_to_float(const CL_StringRef8 &value);

	/// \brief Ucs2 to float
	///
	/// \param value = String Ref16
	///
	/// \return float
	static float ucs2_to_float(const CL_StringRef16 &value);

	/// \brief Double to text
	///
	/// \param value = value
	/// \param num_decimal_places = value
	///
	/// \return Temp String
	static CL_String double_to_text(double value, int num_decimal_places=6);

	/// \brief Double to local8
	///
	/// \param value = value
	/// \param num_decimal_places = value
	///
	/// \return Temp String8
	static CL_String8 double_to_local8(double value, int num_decimal_places=6);

	/// \brief Double to ucs2
	///
	/// \param value = value
	/// \param num_decimal_places = value
	///
	/// \return Temp String16
	static CL_String16 double_to_ucs2(double value, int num_decimal_places=6);

	/// \brief Text to double
	///
	/// \param value = String Ref
	///
	/// \return double
	static double text_to_double(const CL_StringRef &value);

	/// \brief Local8 to double
	///
	/// \param value = String Ref8
	///
	/// \return double
	static double local8_to_double(const CL_StringRef8 &value);

	/// \brief Ucs2 to double
	///
	/// \param value = String Ref16
	///
	/// \return double
	static double ucs2_to_double(const CL_StringRef16 &value);

	/// \brief Int to text
	///
	/// \param value = value
	///
	/// \return Temp String
	static CL_String int_to_text(int value);

	/// \brief Int to local8
	///
	/// \param value = value
	///
	/// \return Temp String8
	static CL_String8 int_to_local8(int value);

	/// \brief Int to ucs2
	///
	/// \param value = value
	///
	/// \return Temp String16
	static CL_String16 int_to_ucs2(int value);

	/// \brief Text to int
	///
	/// \param value = String Ref
	/// \param base = value
	///
	/// \return int
	static int text_to_int(const CL_StringRef &value, int base = 10);

	/// \brief Local8 to int
	///
	/// \param value = String Ref8
	/// \param base = value
	///
	/// \return int
	static int local8_to_int(const CL_StringRef8 &value, int base = 10);

	/// \brief Ucs2 to int
	///
	/// \param value = String Ref16
	/// \param base = value
	///
	/// \return int
	static int ucs2_to_int(const CL_StringRef16 &value, int base = 10);

	/// \brief Uint to text
	///
	/// \param value = value
	///
	/// \return Temp String
	static CL_String uint_to_text(unsigned int value);

	/// \brief Uint to local8
	///
	/// \param value = value
	///
	/// \return Temp String8
	static CL_String8 uint_to_local8(unsigned int value);

	/// \brief Uint to ucs2
	///
	/// \param value = value
	///
	/// \return Temp String16
	static CL_String16 uint_to_ucs2(unsigned int value);

	static unsigned int text_to_uint(const CL_StringRef &value, int base = 10);

	static unsigned int local8_to_uint(const CL_StringRef8 &value, int base = 10);

	static unsigned int ucs2_to_uint(const CL_StringRef16 &value, int base = 10);

	/// \brief Bool to text
	///
	/// \param value = bool
	///
	/// \return Temp String
	static CL_String bool_to_text(bool value);

	/// \brief Bool to local8
	///
	/// \param value = bool
	///
	/// \return Temp String8
	static CL_String8 bool_to_local8(bool value);

	/// \brief Bool to ucs2
	///
	/// \param value = bool
	///
	/// \return Temp String16
	static CL_String16 bool_to_ucs2(bool value);

	/// \brief Text to bool
	///
	/// \param value = String Ref
	///
	/// \return bool
	static bool text_to_bool(const CL_StringRef &value);

	/// \brief Local8 to bool
	///
	/// \param value = String Ref8
	///
	/// \return bool
	static bool local8_to_bool(const CL_StringRef8 &value);

	/// \brief Ucs2 to bool
	///
	/// \param value = String Ref16
	///
	/// \return bool
	static bool ucs2_to_bool(const CL_StringRef16 &value);

	/// \brief Text to cp437
	///
	/// \param text = String Ref
	///
	/// \return Temp String8
	static CL_String8 text_to_cp437(const CL_StringRef &text);

	/// \brief Ucs2 to cp437
	///
	/// \param text = String Ref16
	///
	/// \return Temp String8
	static CL_String8 ucs2_to_cp437(const CL_StringRef16 &text);

	/// \brief Cp437 to text
	///
	/// \param cp437 = String Ref8
	///
	/// \return Temp String
	static CL_String cp437_to_text(const CL_StringRef8 &cp437);

	/// \brief Cp437 to ucs2
	///
	/// \param cp437 = String Ref8
	///
	/// \return Temp String16
	static CL_String16 cp437_to_ucs2(const CL_StringRef8 &cp437);

	/// \brief Text to local8
	///
	/// \param text = String Ref
	///
	/// \return Temp String8
	static CL_String8 text_to_local8(const CL_StringRef &text);

	/// \brief Text to utf8
	///
	/// \param text = String Ref
	///
	/// \return Temp String8
	static CL_String8 text_to_utf8(const CL_StringRef &text);

	/// \brief Ucs2 to latin1
	///
	/// \param ucs2 = String Ref16
	///
	/// \return Temp String8
	static CL_String8 ucs2_to_latin1(const CL_StringRef16 &ucs2);

	/// \brief Ucs2 to latin9
	///
	/// \param ucs2 = String Ref16
	///
	/// \return Temp String8
	static CL_String8 ucs2_to_latin9(const CL_StringRef16 &ucs2);

	/// \brief Ucs2 to local8
	///
	/// \param ucs2 = String Ref16
	///
	/// \return Temp String8
	static CL_String8 ucs2_to_local8(const CL_StringRef16 &ucs2);

	/// \brief Ucs2 to utf8
	///
	/// \param ucs2 = String Ref16
	///
	/// \return Temp String8
	static CL_String8 ucs2_to_utf8(const CL_StringRef16 &ucs2);

	/// \brief Local8 to text
	///
	/// \param local8 = String Ref8
	///
	/// \return Temp String
	static CL_String local8_to_text(const CL_StringRef8 &local8);

	/// \brief Ucs2 to text
	///
	/// \param ucs2 = String Ref16
	///
	/// \return Temp String
	static CL_String ucs2_to_text(const CL_StringRef16 &ucs2);

	/// \brief Latin1 to ucs2
	///
	/// \param latin1 = String Ref8
	///
	/// \return Temp String16
	static CL_String16 latin1_to_ucs2(const CL_StringRef8 &latin1);

	/// \brief Latin9 to ucs2
	///
	/// \param latin9 = String Ref8
	///
	/// \return Temp String16
	static CL_String16 latin9_to_ucs2(const CL_StringRef8 &latin9);

	/// \brief Local8 to ucs2
	///
	/// \param local8 = String Ref8
	///
	/// \return Temp String16
	static CL_String16 local8_to_ucs2(const CL_StringRef8 &local8);

	/// \brief Utf8 to ucs2
	///
	/// \param utf8 = String Ref8
	///
	/// \return Temp String16
	static CL_String16 utf8_to_ucs2(const CL_StringRef8 &utf8);

	/// \brief Utf8 to text
	///
	/// \param utf8 = String Ref8
	///
	/// \return Temp String
	static CL_String utf8_to_text(const CL_StringRef8 &utf8);

	enum BOMType
	{
		bom_none,
		bom_utf32_be,
		bom_utf32_le,
		bom_utf16_be,
		bom_utf16_le,
		bom_utf8
	};

	/// \brief Detect bom
	///
	/// \param data = void
	/// \param size_type = String
	///
	/// \return BOMType
	static BOMType detect_bom(const void *data, CL_String::size_type length);

#ifdef __cplusplus_cli
	/// \brief Clr to text
	///
	/// \param String = System
	///
	/// \return Temp String
	static CL_String clr_to_text(const System::String ^str)
	{
		return ucs2_to_utf8(clr_to_ucs2(str));
	}

	static System::String ^text_to_clr(const CL_StringRef &str)
	{
		return ucs2_to_clr(utf8_to_ucs2(str));
	}

	/// \brief Clr to ucs2
	///
	/// \param String = System
	///
	/// \return Temp String16
	static CL_String16 clr_to_ucs2(const System::String ^str)
	{
		cli::pin_ptr<const wchar_t> chars = PtrToStringChars(str);
		return CL_String16(chars);
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
