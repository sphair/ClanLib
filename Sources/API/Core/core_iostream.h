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

#ifndef header_core_iostream
#define header_core_iostream

#ifdef CL_API_DLL
#ifdef CL_CORE_EXPORT
#define CL_API_CORE __declspec(dllexport)
#else
#define CL_API_CORE __declspec(dllimport)
#endif
#else
#define CL_API_CORE
#endif

#include <iosfwd>


class CL_Rect;
class CL_Rectf;
class CL_Point;
class CL_Pointf;
class CL_Size;
class CL_Sizef;
class CL_Error;

CL_API_CORE std::ostream& operator<<(std::ostream& s, const CL_Rect& rect);
CL_API_CORE std::ostream& operator<<(std::ostream& s, const CL_Rectf& rect);
CL_API_CORE std::ostream& operator<<(std::ostream& s, const CL_Point& point);
CL_API_CORE std::ostream& operator<<(std::ostream& s, const CL_Pointf& point);
CL_API_CORE std::ostream& operator<<(std::ostream& s, const CL_Size& size);
CL_API_CORE std::ostream& operator<<(std::ostream& s, const CL_Sizef& size);
CL_API_CORE std::ostream& operator<<(std::ostream& s, const CL_Error& error);


#endif
