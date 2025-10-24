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
*/

#pragma once

class CL_CSSReplacedComponent
{
public:
	virtual ~CL_CSSReplacedComponent() { }

	/// \brief Get Size
	///
	/// \return size
	virtual CL_Size get_size() const = 0;

	/// \brief Set geometry
	///
	/// \param geometry = Rect
	virtual void set_geometry(const CL_Rect &geometry) = 0;
};

/// \brief Span Component Binder (templated class)
///
/// \xmlonly !group=Display/2D! !header=display.h! \endxmlonly
template<typename T>
class CL_CSSReplacedComponentBinder : public CL_CSSReplacedComponent
{
public:

	/// \brief Constructs a CSSReplacedComponentBinder
	///
	/// \param component = T
	CL_CSSReplacedComponentBinder(T *component)
	: component(component)
	{
	}

	/// \brief Get Size
	///
	/// \return size
	CL_Size get_size() const
	{
		return component->get_size();
	}

	/// \brief Set geometry
	///
	/// \param geometry = Rect
	void set_geometry(const CL_Rect &geometry)
	{
		component->set_geometry(geometry);
	}

private:
	T *component;
};
