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
**    Ingo Ruhnke
**    Dieter Buys
*/

#include "Core/precomp.h"
#include <iostream>
#include "API/Core/Math/rect.h"
#include "API/Core/Math/size.h"
#include "API/Core/Math/point.h"
#include "API/Core/System/error.h"
#include "API/Core/core_iostream.h"

std::ostream& operator<<(std::ostream& s, const CL_Rect& rect)
{
	s << "["
	  << rect.left   << ", "
	  << rect.top    << ", "
	  << rect.right  << ", "
	  << rect.bottom << "]";
	return s;
}

std::ostream& operator<<(std::ostream& s, const CL_Rectf& rect)
{
	s << "["
	  << rect.left   << ", "
	  << rect.top    << ", "
	  << rect.right  << ", "
	  << rect.bottom << "]";
	return s;
}

std::ostream& operator<<(std::ostream& s, const CL_Point& point)
{
	s << "["
	  << point.x   << ", "
	  << point.y << "]";
	return s;
}

std::ostream& operator<<(std::ostream& s, const CL_Pointf& point)
{
	s << "["
	  << point.x   << ", "
	  << point.y << "]";
	return s;
}

std::ostream& operator<<(std::ostream& s, const CL_Size& size)
{
	s << size.width << "x" << size.height;
	return s;
}

std::ostream& operator<<(std::ostream& s, const CL_Sizef& size)
{
	s << size.width << "x" << size.height;
	return s;
}

std::ostream& operator<<(std::ostream& s, const CL_Error& error)
{
	s << error.message;
	return s;
}

// EOF //

