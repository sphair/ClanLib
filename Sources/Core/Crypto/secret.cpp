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
**    Mark Page
*/

#include "Core/precomp.h"
#include "API/Core/Crypto/secret.h"
#include "API/Core/System/databuffer.h"
#include "secret_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Secret Construction:

CL_Secret::CL_Secret()
: impl(new CL_Secret_Impl())
{
}

CL_Secret::CL_Secret(unsigned int new_key_length)
: impl(new CL_Secret_Impl())
{
	impl->create(new_key_length);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Secret Attributes:

unsigned int CL_Secret::get_size() const
{
	return impl->get_size();
}

unsigned char *CL_Secret::get_data() const
{
	return impl->get_data();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Secret Operations:

/////////////////////////////////////////////////////////////////////////////
// CL_Secret Implementation:
