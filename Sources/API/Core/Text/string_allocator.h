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
#include "../System/sharedptr.h"
#include "string_types.h"

class CL_StringAllocator_Impl;

/// \brief Allocate strings in blocks.
///
///    <p>This class allows you to efficiently allocate strings. Instead
///    of using the default new operator, this class allocates string data in
///    blocks. The temporary strings returned by the class remain valid as long
///    as the CL_StringAllocator stays valid, or until clear() is called.</p> 
/// \xmlonly !group=Core/Text! !header=core.h! \endxmlonly
class CL_API_CORE CL_StringAllocator
{
/// \name Construction
/// \{

public:
	CL_StringAllocator();

	/// \brief Constructs a StringAllocator
	///
	/// \param source = String Allocator
	CL_StringAllocator(const CL_StringAllocator &source);

	~CL_StringAllocator();

/// \}
/// \name Operations
/// \{

public:
	CL_StringAllocator &operator =(const CL_StringAllocator &source);

	/// \brief Alloc
	///
	/// \param str = String Ref8
	///
	/// \return String Ref8
	CL_StringRef8 alloc(const CL_StringRef8 &str);

	/// \brief Alloc
	///
	/// \param str = char
	///
	/// \return String Ref8
	CL_StringRef8 alloc(const char *str);

	CL_StringRef8 alloc(
		const char *str,
		CL_StringRef8::size_type length);

	/// \brief Alloc
	///
	/// \param str = String Ref16
	///
	/// \return String Ref16
	CL_StringRef16 alloc(const CL_StringRef16 &str);

	/// \brief Alloc
	///
	/// \param str = wchar_t
	///
	/// \return String Ref16
	CL_StringRef16 alloc(const wchar_t *str);

	CL_StringRef16 alloc(
		const wchar_t *str,
		CL_StringRef16::size_type length);

	/// \brief Alloc
	///
	/// \param length = value
	///
	/// \return String Ref
	CL_StringRef alloc(unsigned int length);

	/// \brief Alloc8
	///
	/// \param length = value
	///
	/// \return String Ref8
	CL_StringRef8 alloc8(unsigned int length);

	/// \brief Alloc16
	///
	/// \param length = value
	///
	/// \return String Ref16
	CL_StringRef16 alloc16(unsigned int length);

	/// \brief Clear
	void clear();

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_StringAllocator_Impl> impl;
/// \}
};

/// \}
