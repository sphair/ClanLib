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
**    Kenneth Gangstoe
**    Harry Storbacka
**    Mark Page
*/

/// \addtogroup clanCore_Math clanCore Math
/// \{

#pragma once

#include "../api_core.h"
#include "vec2.h"
#include "size.h"
#include "point.h"
#include "origin.h"
#include "cl_math.h"

/// \brief 2D (left,top,right,bottom) rectangle structure.
///
/// These line templates are defined for: int (CL_Rect), float (CL_Rectf), double (CL_Rectd)
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
template<typename Type>
class CL_API_CORE CL_Rectx
{
/// \name Construction
/// \{
public:
	/// \brief Constructs an rectangle.
	///
	/// Initialised to zero
	CL_Rectx() { left = right = top = bottom = 0; }

	/// \brief Constructs an rectangle.
	///
	/// \param s = Size
	CL_Rectx(const CL_Sizex<Type> &s) { left = 0; top = 0; right = s.width; bottom = s.height; }

	/// \brief Constructs an rectangle.
	///
	/// \param new_left Initial left position of rectangle.
	/// \param new_top Initial top position of rectangle.
	/// \param new_right Initial right position of rectangle.
	/// \param new_bottom Initial bottom position of rectangle.
	CL_Rectx(Type new_left, Type new_top, Type new_right, Type new_bottom)
	{ left = new_left; top = new_top; right = new_right; bottom = new_bottom; }

	/// \brief Constructs an rectangle.
	///
	/// \param p = Initial top-left position of rectangle.
	/// \param size Initial size of rectangle.
	CL_Rectx(const CL_Pointx<Type> &p, const CL_Sizex<Type> &size)
	{ left = p.x; top = p.y; right = left + size.width; bottom = top + size.height; }

	/// \brief Constructs an rectangle.
	///
	/// \param new_left Initial left position of rectangle.
	/// \param new_top Initial top position of rectangle.
	/// \param size Initial size of rectangle.
	CL_Rectx(Type new_left, Type new_top, const CL_Sizex<Type> &size)
	{ left = new_left; top = new_top; right = left + size.width; bottom = top + size.height; }

	/// \brief Constructs an rectangle.
	///
	/// \param rect Initial rectangle position and size.
	CL_Rectx(const CL_Rectx<int> &rect);

	/// \brief Constructs an rectangle.
	///
	/// \param rect Initial rectangle position and size.
	CL_Rectx(const CL_Rectx<float> &rect);

	/// \brief Constructs an rectangle.
	///
	/// \param rect Initial rectangle position and size.
	CL_Rectx(const CL_Rectx<double> &rect);

	/// \brief Rect == Rect operator.
	bool operator==(const CL_Rectx<Type> &r) const
	{ return (left == r.left && top == r.top && right == r.right && bottom == r.bottom); }

	/// \brief Rect != Rect operator.
	bool operator!=(const CL_Rectx<Type> &r) const
	{ return (left != r.left || top != r.top || right != r.right || bottom != r.bottom); }

/// \}
/// \name Attributes
/// \{
public:
	/// \brief X1-coordinate.
	Type left;

	/// \brief Y1-coordinate.
	Type top;

	/// \brief X2-coordinate.
	Type right;

	/// \brief Y2-coordinate.
	Type bottom;

	/// \brief Returns the width of the rectangle.
	Type get_width() const { return right - left; }

	/// \brief Returns the height of the rectangle.
	Type get_height() const { return bottom - top; }

	/// \brief Returns the size of the rectangle.
	CL_Sizex<Type> get_size() const { return CL_Sizex<Type>(right - left, bottom - top); }

	/// \brief Returns true if the rectangle contains the point.
	bool contains(const CL_Vec2<Type> &p) const
	{
		return ((p.x >= left && p.x <= right) || (p.x <= left && p.x >= right))
		    && ((p.y >= top && p.y <= bottom) || (p.y <= top && p.y >= bottom));
	}

	/// \brief Returns the top-left point of the rectangle
	CL_Pointx<Type> get_top_left() const
	{
		return CL_Pointx<Type>(left, top);
	}

	/// \brief Returns the top-right point of the rectangle
	CL_Pointx<Type> get_top_right() const
	{
		return CL_Pointx<Type>(right, top);
	}

	/// \brief Returns the bottom-right point of the rectangle
	CL_Pointx<Type> get_bottom_right() const
	{
		return CL_Pointx<Type>(right, bottom);
	}

	/// \brief Returns the bottom-left point of the rectangle
	CL_Pointx<Type> get_bottom_left() const
	{
		return CL_Pointx<Type>(left, bottom);
	}

	/// \brief Returns true if rectangle passed is overlapping or inside this rectangle.
	bool is_overlapped(const CL_Rectx<Type> &r) const
	{
		return (r.left < right && r.right > left && r.top < bottom && r.bottom > top);
	}

	/// \brief Returns true if rectangle passed is inside this rectangle
	bool is_inside(const CL_Rectx<Type> &r) const
	{
		return ((left <= r.left)
			&& (top <= r.top)
			&& (right >= r.right)
			&& (bottom >= r.bottom));
	}

	/// \brief Returns another CL_Rectx<Type> containing a rotated version of this one.
	///
	/// \param hotspot Point to rotate around.
	/// \param angle Angle to rotate.
	CL_Rectx<Type> get_rot_bounds(const CL_Vec2<Type> &hotspot, const CL_Angle &angle) const;

	/// \brief Returns another CL_Rectx<Type> containing a rotated version of this one.
	///
	/// \param origin Determines the hotspot point within the rectangle
	/// \param x Offsets applied negatively to the hotspot point
	/// \param y Offsets applied negatively to the hotspot point
	/// \param angle Angle
	CL_Rectx<Type> get_rot_bounds(CL_Origin origin, Type x, Type y, const CL_Angle &angle) const;

	/// \brief Returns the center point of the rectangle.
	CL_Pointx<Type> get_center() const
	{
		return CL_Pointx<Type>( (left + right)/2, ( top + bottom)/2 );
	}

/// \}
/// \name Operations
/// \{
public:
	/// \brief Sets the top-left point of the rectangle.
	///
	/// \return reference to this object
	CL_Rectx<Type> &set_top_left(const CL_Vec2<Type>& p)
	{
		left = p.x;
		top = p.y;
		return *this;
	}

	/// \brief Sets the top-right point of the rectangle.
	///
	/// \return reference to this object
	CL_Rectx<Type> &set_top_right(const CL_Vec2<Type>& p)
	{
		right = p.x;
		top = p.y;
		return *this;
	}

	/// \brief Sets the bottom-right point of the rectangle.
	///
	/// \return reference to this object
	CL_Rectx<Type> &set_bottom_right(const CL_Vec2<Type>& p)
	{
		right = p.x;
		bottom = p.y;
		return *this;
	}

	/// \brief Sets the bottom-left point of the rectangle.
	///
	/// \return reference to this object
	CL_Rectx<Type> &set_bottom_left(const CL_Vec2<Type>& p)
	{
		left = p.x;
		bottom = p.y;
		return *this;
	}

	/// \brief Sets the width of the rectangle.
	///
	/// \return reference to this object
	CL_Rectx<Type> &set_width(Type width)
	{
		right = left + width;
		return *this;
	}

	/// \brief Sets the height of the rectangle.
	///
	/// \return reference to this object
	CL_Rectx<Type> &set_height(Type height)
	{
		bottom = top + height;
		return *this;
	}

	/// \brief Shrink the rectangle
	///
	/// \return reference to this object
	CL_Rectx<Type> &shrink(const Type &left, const Type &top, const Type &right, const Type &bottom)
	{
		this->left += left; this->top += top; this->right -= right; this->bottom -= bottom;
		return *this;
	};

	/// \brief Shrink the rectangle
	///
	/// \return reference to this object
	CL_Rectx<Type> &shrink(const Type &left_right, const Type &top_bottom)
	{
		this->left += left_right; this->top += top_bottom; this->right -= left_right; this->bottom -= top_bottom;
		return *this;
	};

	/// \brief Shrink the rectangle
	///
	/// \return reference to this object
	CL_Rectx<Type> &shrink(const Type &shrink)
	{
		this->left += shrink; this->top += shrink; this->right -= shrink; this->bottom -= shrink;
		return *this;
	};

	/// \brief Expand the rectangle
	///
	/// \return reference to this object
	CL_Rectx<Type> &expand(const Type &left, const Type &top, const Type &right, const Type &bottom)
	{
		this->left -= left; this->top -= top; this->right += right; this->bottom += bottom;
		return *this;
	};

	/// \brief Expand the rectangle
	///
	/// \return reference to this object
	CL_Rectx<Type> &expand(const Type &left_and_right, const Type &top_and_bottom)
	{
		this->left -= left_and_right;
		this->right += left_and_right;
		this->top -= top_and_bottom;
		this->bottom += top_and_bottom;
		return *this;
	};

	/// \brief Expand the rectangle
	///
	/// \return reference to this object
	CL_Rectx<Type> &expand(const Type &expand)
	{
		this->left -= expand;
		this->right += expand;
		this->top -= expand;
		this->bottom += expand;
		return *this;
	};

	/// \brief Translate the rect
	///
	/// \return reference to this object
	CL_Rectx<Type> &translate(const CL_Vec2<Type> &p)
	{
		left += p.x; top += p.y; right += p.x; bottom += p.y;
		return *this;
	};

	/// \brief Translate the rect by another rect (only uses the left and top coords).
	///
	/// \return reference to this object
	CL_Rectx<Type> &translate(const CL_Rectx<Type> &p)
	{
		left += p.left; top += p.top; right += p.left; bottom += p.top;
		return *this;
	};

	/// \brief Translate the rect
	///
	/// \return reference to this object
	CL_Rectx<Type> &translate(Type x, Type y)
	{
		left += x; top += y; right += x; bottom += y;
		return *this;
	};

	/// \brief Sets the size of the rectangle, maintaining top/left position.
	///
	/// \return reference to this object
	CL_Rectx<Type> &set_size(const CL_Sizex<Type> &size)
	{
		right = left + size.width;
		bottom = top + size.height;
		return *this;
	}

	/// \brief Calculates the union of two rectangles. 
	/// 
	/// <p>Rect values become: max left, max top, min right, min bottom.</p>
	///
	/// \return reference to this object
	CL_Rectx<Type> &overlap(const CL_Rectx<Type> &rect)
	{
		CL_Rectx<Type> result;
		result.left   = cl_max(left, rect.left);
		result.right  = cl_min(right, rect.right);
		result.top    = cl_max(top, rect.top);
		result.bottom = cl_min(bottom, rect.bottom);
		*this = result;
		return *this;
	}

	/// \brief Calculates the bounding rectangle of the rectangles. 
	/// 
	/// <p>Rect values become: min left, min top, max right, max bottom.</p>
	/// 
	/// \return reference to this object
	CL_Rectx<Type> &bounding_rect(const CL_Rectx<Type> &rect)
	{
		CL_Rectx<Type> result;
		result.left   = cl_min(left, rect.left);
		result.right  = cl_max(right, rect.right);
		result.top    = cl_min(top, rect.top);
		result.bottom = cl_max(bottom, rect.bottom);
		*this = result;
		return *this;
	}

	/// \brief Normalize rectangle. Ensures that left is less than right and top is less than bottom.
	///
	/// \return reference to this object
	CL_Rectx<Type> &normalize()
	{
		if (left > right)
		{
			Type temp = right;
			right = left;
			left = temp;
		}

		if (top > bottom)
		{
			Type temp = bottom;
			bottom = top;
			top = temp;
		}
		return *this;
	}

	/// \brief Applies an origin and offset pair to this rectangle
	///
	/// \param origin The new origin to adjust to from default upper-left position
	/// \param x, y Offsets applied negatively to each corner of the rectangle
	///
	/// \return reference to this object
	CL_Rectx<Type> &apply_alignment(CL_Origin origin, Type x, Type y)
	{
		CL_Vec2<Type> offset = CL_Vec2<Type>::calc_origin(origin, get_size());
		offset.x -= x;
		offset.y -= y;

		left += offset.x;
		top += offset.y;
		right += offset.x;
		bottom += offset.y;
		return *this;
	}

	/// \brief Clip according to the specified clip rectangle.
	///
	/// \return reference to this object
	CL_Rectx<Type> &clip(const CL_Rectx<Type> &cr)
	{
		top = cl_max(top, cr.top);
		left = cl_max(left, cr.left);
		right = cl_min(right, cr.right);
		bottom = cl_min(bottom, cr.bottom);
		top = cl_min(top, bottom);
		left = cl_min(left, right);
		return *this;
	}
/// \}
};

template<>
inline CL_Rectx<int>::CL_Rectx(const CL_Rectx<float> &rect)
{ left = (int) (floor(rect.left + 0.5f)); top = (int) (floor(rect.top + 0.5f)); right = (int) (floor(rect.right + 0.5f)); bottom = (int) (floor(rect.bottom+0.5f)); }

template<>
inline CL_Rectx<int>::CL_Rectx(const CL_Rectx<double> &rect)
{ left = (int) (floor(rect.left + 0.5)); top = (int) (floor(rect.top + 0.5)); right = (int) (floor(rect.right + 0.5)); bottom = (int) (floor(rect.bottom + 0.5)); }

template<typename Type>
inline CL_Rectx<Type>::CL_Rectx(const CL_Rectx<int> &rect)
{ left = (Type) rect.left; top = (Type) rect.top; right = (Type) rect.right; bottom = (Type) rect.bottom; }

template<typename Type>
inline CL_Rectx<Type>::CL_Rectx(const CL_Rectx<float> &rect)
{ left = (Type) rect.left; top = (Type) rect.top; right = (Type) rect.right; bottom = (Type) rect.bottom; }

template<typename Type>
inline CL_Rectx<Type>::CL_Rectx(const CL_Rectx<double> &rect)
{ left = (Type) rect.left; top = (Type) rect.top; right = (Type) rect.right; bottom = (Type) rect.bottom; }

/// \brief 2D (left,top,right,bottom) rectangle structure - Integer
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Rect : public CL_Rectx<int>
{
public:
	CL_Rect() : CL_Rectx<int>() {}
	CL_Rect(const CL_Sizex<int> &s) : CL_Rectx<int>(s) {}
	CL_Rect(int new_left, int new_top, int new_right, int new_bottom) : CL_Rectx<int>(new_left, new_top, new_right, new_bottom) {}
	CL_Rect(const CL_Pointx<int> &p, const CL_Sizex<int> &size) : CL_Rectx<int>(p, size) {}
	CL_Rect(const CL_Rectx<int> &rect) : CL_Rectx<int>(rect) {}
	CL_Rect(const CL_Rectx<float> &rect) : CL_Rectx<int>(rect) {}
	CL_Rect(const CL_Rectx<double> &rect) : CL_Rectx<int>(rect) {}
	CL_Rect(int new_left, int new_top, const CL_Sizex<int> &size) : CL_Rectx<int>(new_left, new_top, size) {}
};

/// \brief 2D (left,top,right,bottom) rectangle structure - Float
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Rectf : public CL_Rectx<float>
{
public:
	CL_Rectf() : CL_Rectx<float>() {}
	CL_Rectf(const CL_Sizex<int> &s) : CL_Rectx<float>(s) {}
	CL_Rectf(const CL_Sizex<float> &s) : CL_Rectx<float>(s) {}
	CL_Rectf(float new_left, float new_top, float new_right, float new_bottom) : CL_Rectx<float>(new_left, new_top, new_right, new_bottom) {}
	CL_Rectf(const CL_Pointx<float> &p, const CL_Sizex<float> &size) : CL_Rectx<float>(p, size) {}
	CL_Rectf(const CL_Rectx<int> &rect) : CL_Rectx<float>(rect) {}
	CL_Rectf(const CL_Rectx<float> &rect) : CL_Rectx<float>(rect) {}
	CL_Rectf(const CL_Rectx<double> &rect) : CL_Rectx<float>(rect) {}
	CL_Rectf(float new_left, float new_top, const CL_Sizex<float> &size) : CL_Rectx<float>(new_left, new_top, size) {}
};

/// \brief 2D (left,top,right,bottom) rectangle structure - Double
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Rectd : public CL_Rectx<double>
{
public:
	CL_Rectd() : CL_Rectx<double>() {}
	CL_Rectd(const CL_Sizex<int> &s) : CL_Rectx<double>(s) {}
	CL_Rectd(const CL_Sizex<float> &s) : CL_Rectx<double>(s) {}
	CL_Rectd(const CL_Sizex<double> &s) : CL_Rectx<double>(s) {}
	CL_Rectd(double new_left, double new_top, double new_right, double new_bottom) : CL_Rectx<double>(new_left, new_top, new_right, new_bottom) {}
	CL_Rectd(const CL_Pointx<double> &p, const CL_Sizex<double> &size) : CL_Rectx<double>(p, size) {}
	CL_Rectd(const CL_Rectx<int> &rect) : CL_Rectx<double>(rect) {}
	CL_Rectd(const CL_Rectx<float> &rect) : CL_Rectx<double>(rect) {}
	CL_Rectd(const CL_Rectx<double> &rect) : CL_Rectx<double>(rect) {}
	CL_Rectd(double new_left, double new_top, const CL_Sizex<double> &size) : CL_Rectx<double>(new_left, new_top, size) {}
};

inline CL_Rect CL_RectPS(int x, int y, int width, int height)
{
	return CL_Rect(x, y, x+width, y+height);
}

inline CL_Rectf CL_RectfPS(float x, float y, float width, float height)
{
	return CL_Rectf(x, y, x+width, y+height);
}

inline CL_Rectd CL_RectdPS(double x, double y, double width, double height)
{
	return CL_Rectd(x, y, x+width, y+height);
}

/// \}
