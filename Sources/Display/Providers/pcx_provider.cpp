/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "Display/display_precomp.h"
#include "API/Display/Providers/pcx_provider.h"
#include "API/Core/System/error.h"
#include "pcx_provider_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_PCXProvider construction:

CL_PCXProvider::CL_PCXProvider(
		const std::string &name,
		CL_InputSourceProvider *provider)
: CL_PixelBuffer(
	new CL_PCXProvider_Generic(
		name,
		provider))
{
}

void
CL_PCXProvider::save(
	CL_PixelBuffer buffer,
	const std::string &filename,
	CL_OutputSourceProvider *output_provider)
{
	throw CL_Error("PCXProvider doesn't support saving");
}

/* EOF */
