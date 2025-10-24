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

/// \addtogroup clanCore_Math clanCore Math
/// \{

#pragma once

#include "../api_core.h"
#include "../System/sharedptr.h"

class CL_DataBuffer;
class CL_SHA1_Impl;

/// \brief SHA-1 hash function class.
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_API_CORE CL_SHA1
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a SHA-1 hash generator.
	CL_SHA1();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the calculated hash.
	CL_String8 get_hash(bool uppercase = false);

	/// \brief Get hash
	///
	/// \param out_hash = where to write to
	void get_hash(unsigned char out_hash[20]);

/// \}
/// \name Operations
/// \{

public:
	/// \brief Resets the hash generator.
	void reset();

	/// \brief Adds data to be hashed.
	void add(const void *data, int size);

	/// \brief Add
	///
	/// \param data = Data Buffer
	void add(const CL_DataBuffer &data);

	/// \brief Finalize hash calculation.
	void calculate();

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_SHA1_Impl> impl;
/// \}
};

/// \}
