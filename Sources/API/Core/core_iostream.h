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
**    Ingo Ruhnke
*/

#pragma once

#include <iosfwd>
#include <iostream>

class CL_Rect;
class CL_Rectf;
class CL_Rectd;
class CL_Point;
class CL_Pointf;
class CL_Pointd;
class CL_Size;
class CL_Sizef;
class CL_Sized;

CL_API_CORE std::ostream& operator<<(std::ostream& s, const CL_Rect& rect);
CL_API_CORE std::ostream& operator<<(std::ostream& s, const CL_Rectf& rect);
CL_API_CORE std::ostream& operator<<(std::ostream& s, const CL_Rectd& rect);
CL_API_CORE std::ostream& operator<<(std::ostream& s, const CL_Point& point);
CL_API_CORE std::ostream& operator<<(std::ostream& s, const CL_Pointf& point);
CL_API_CORE std::ostream& operator<<(std::ostream& s, const CL_Pointd& point);
CL_API_CORE std::ostream& operator<<(std::ostream& s, const CL_Size& size);
CL_API_CORE std::ostream& operator<<(std::ostream& s, const CL_Sizef& size);

template<typename T>
CL_API_CORE std::ostream& operator<<(std::ostream& s, const CL_Vec1<T>& vec)
{
	s << vec.x;
	return s;
}

template<typename T>
CL_API_CORE std::ostream& operator<<(std::ostream& s, const CL_Vec2<T>& vec)
{
	s << "["
	  << vec.x   << ", "
	  << vec.y << "]";
	return s;
}

template<typename T>
CL_API_CORE std::ostream& operator<<(std::ostream& s, const CL_Vec3<T>& vec)
{
	s << "["
	  << vec.x << ", "
	  << vec.y << ", "
	  << vec.z << "]";
	return s;
}

template<typename T>
CL_API_CORE std::ostream& operator<<(std::ostream& s, const CL_Vec4<T>& vec)
{
	s << "["
	  << vec.x << ", "
	  << vec.y << ", "
	  << vec.z << ", "
	  << vec.w << "]";
	return s;
}

