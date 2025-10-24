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

#include "Core/precomp.h"
#include "API/Core/Math/sha1.h"
#include "API/Core/System/databuffer.h"
#include "sha1_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SHA1 Construction:

CL_SHA1::CL_SHA1()
: impl(new CL_SHA1_Impl)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SHA1 Attributes:

CL_String8 CL_SHA1::get_hash(bool uppercase)
{
	return impl->get_hash(uppercase);
}

void CL_SHA1::get_hash(unsigned char out_hash[20])
{
	impl->get_hash(out_hash);
}

/////////////////////////////////////////////////////////////////////////////
// CL_SHA1 Operations:

void CL_SHA1::reset()
{
	impl->reset();
}

void CL_SHA1::add(const void *data, int size)
{
	impl->add(data, size);
}

void CL_SHA1::add(const CL_DataBuffer &data)
{
	add(data.get_data(), data.get_size());
}

void CL_SHA1::calculate()
{
	impl->calculate();
}

/////////////////////////////////////////////////////////////////////////////
// CL_SHA1 Implementation:
