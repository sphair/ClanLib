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
**    Mark Page
*/
	
#include "Display/precomp.h"
#include "API/Display/Image/image_import_description.h"
#include "API/Display/Image/pixel_buffer.h"
#include "image_import_description_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ImageImportDescription construction:

CL_ImageImportDescription::CL_ImageImportDescription() 
: impl(new CL_ImageImportDescription_Impl)
{
}

CL_ImageImportDescription::~CL_ImageImportDescription()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ImageImportDescription attributes:
bool CL_ImageImportDescription::get_premultiply_alpha() const
{
	return impl->premultiply_alpha;
}

bool CL_ImageImportDescription::flip_vertical() const
{
	return impl->flip_vertical;
}


/////////////////////////////////////////////////////////////////////////////
// CL_ImageImportDescription operations:

void CL_ImageImportDescription::set_premultiply_alpha(bool enable)
{
	impl->premultiply_alpha = enable;
}

void CL_ImageImportDescription::set_flip_vertical(bool enable)
{
	impl->flip_vertical = enable;
}

CL_PixelBuffer CL_ImageImportDescription::process(CL_PixelBuffer &image) const
{
	if (impl->premultiply_alpha)
		image.premultiply_alpha();

	if (impl->flip_vertical)
		image.flip_vertical();

	if (!impl->func_process.is_null())
		image = impl->func_process.invoke(image);

	return image;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ImageImportDescription callbacks:
CL_Callback_1<CL_PixelBuffer, CL_PixelBuffer &> &CL_ImageImportDescription::func_process()
{
	return impl->func_process;
}
/////////////////////////////////////////////////////////////////////////////
// CL_ImageImportDescription implementation:

