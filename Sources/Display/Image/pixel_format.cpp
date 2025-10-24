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
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Display/Image/pixel_format.h"

/////////////////////////////////////////////////////////////////////////////
// CL_PixelFormat statics:

int CL_PixelFormat::get_mask_shift(unsigned int mask)
{
	if (mask == 0)
		return 0;

	for (int shift = 0; shift < 32; shift++)
	{
		if (mask & 1)
			return shift;
		else
			mask >>= 1;
	}
	return 0;
}

int CL_PixelFormat::get_mask_bits(unsigned int mask)
{
	if (mask == 0)
		return 0;

	const int shift = get_mask_shift(mask);
	mask >>= shift;

	int bits;
	for (bits = 0; bits < (32 - shift); bits++)
	{
		if (!(mask & 1))
			return bits;
		else
			mask >>= 1;
	}
	return bits;
}

unsigned int CL_PixelFormat::get_bitmask(int bits, int shift)
{
	unsigned int mask = 0;
	for (int i = 0; i < bits; i++)
		mask |= 1 << i;
	return mask << shift;
}
