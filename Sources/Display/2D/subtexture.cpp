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
*/

#include "Display/precomp.h"
#include "API/Display/2D/subtexture.h"
#include "API/Display/Render/texture.h"
#include "API/Core/Math/rect.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Subtexture_Impl Class:

class CL_Subtexture_Impl
{
public:
	CL_Subtexture_Impl()
	{
	}

	~CL_Subtexture_Impl()
	{
	}

	CL_Texture texture;

	CL_Rect geometry;
};

/////////////////////////////////////////////////////////////////////////////
// CL_Subtexture Construction:

CL_Subtexture::CL_Subtexture()
{
}

CL_Subtexture::CL_Subtexture(CL_Texture texture, const CL_Rect &geometry)
: impl(new CL_Subtexture_Impl)
{
	impl->texture = texture;
	impl->geometry = geometry;
}

CL_Subtexture::~CL_Subtexture()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Subtexture Attributes:

void CL_Subtexture::throw_if_null() const
{
	if (impl.is_null())
		throw CL_Exception("CL_Subtexture is null");
}

CL_Texture CL_Subtexture::get_texture()
{
	return impl->texture;
}

CL_Rect CL_Subtexture::get_geometry() const
{
	return impl->geometry;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Subtexture Operations:

/////////////////////////////////////////////////////////////////////////////
// CL_Subtexture Implementation:
