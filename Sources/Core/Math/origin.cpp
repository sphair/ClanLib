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

#include "Core/precomp.h"

#include "API/Core/Math/origin.h"

CL_Point calc_origin(CL_Origin origin, const CL_Size &size)
{
	switch(origin)
	{
	case origin_top_left:
	default:
		return CL_Point(0, 0);
		break;
	case origin_top_center:
		return CL_Point(size.width / 2, 0);
		break;
	case origin_top_right:
		return CL_Point(size.width, 0);
		break;
	case origin_center_left:
		return CL_Point(0, size.height / 2);
		break;
	case origin_center:
		return CL_Point(size.width / 2, size.height / 2);
		break;
	case origin_center_right:
		return CL_Point(size.width, size.height / 2);
		break;
	case origin_bottom_left:
		return CL_Point(0, size.height);
		break;
	case origin_bottom_center:
		return CL_Point(size.width / 2, size.height);
		break;
	case origin_bottom_right:
		return CL_Point(size.width, size.height);
		break;
	}
}

CL_Pointf calc_origin(CL_Origin origin, const CL_Sizef &size)
{
	switch(origin)
	{
	case origin_top_left:
	default:
		return CL_Pointf(0, 0);
		break;
	case origin_top_center:
		return CL_Pointf(size.width / 2, 0);
		break;
	case origin_top_right:
		return CL_Pointf(size.width, 0);
		break;
	case origin_center_left:
		return CL_Pointf(0, size.height / 2);
		break;
	case origin_center:
		return CL_Pointf(size.width / 2, size.height / 2);
		break;
	case origin_center_right:
		return CL_Pointf(size.width, size.height / 2);
		break;
	case origin_bottom_left:
		return CL_Pointf(0, size.height);
		break;
	case origin_bottom_center:
		return CL_Pointf(size.width / 2, size.height);
		break;
	case origin_bottom_right:
		return CL_Pointf(size.width, size.height);
		break;
	}
}
