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
**    Harry Storbacka
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

#include "Display/display_precomp.h"
#include "API/Display/Collision/outline_provider_file.h"
#include "API/Display/Collision/outline_circle.h"
#include "outline_provider_file_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OutlineProviderFile Construction:

CL_OutlineProviderFile::CL_OutlineProviderFile(const std::string &filename, CL_InputSourceProvider *provider)
{
	impl = new CL_OutlineProviderFile_Generic( filename, provider );
}

CL_OutlineProviderFile::~CL_OutlineProviderFile()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_OutlineProviderFile Attributes:

std::vector<CL_Contour> CL_OutlineProviderFile::get_contours()
{
	return impl->contours;
}

int CL_OutlineProviderFile::get_width()
{
	return impl->width;
}

int CL_OutlineProviderFile::get_height()
{
	return impl->height;
}

