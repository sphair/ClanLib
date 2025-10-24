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

/// \addtogroup clanDisplay_2D clanDisplay 2D
/// \{

#pragma once

#include "../../Core/System/sharedptr.h"
#include "../../Core/Math/rect.h"
#include "../../Core/Math/size.h"
#include "color.h"

class CL_Font;
class CL_GraphicContext;
class CL_Point;
class CL_Size;
class CL_SpanLayout_Impl;
class CL_Image;
class CL_SpanComponent;

/// \brief Span Align
///
/// \xmlonly !group=Display/2D! !header=display.h! \endxmlonly
enum CL_SpanAlign
{
	cl_left,
	cl_right,
	cl_center,
	cl_justify
};

/// \brief Span component class
///
/// \xmlonly !group=Display/2D! !header=display.h! \endxmlonly
class CL_SpanComponent
{
public:
	virtual ~CL_SpanComponent() { }

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
class CL_SpanComponentBinder : public CL_SpanComponent
{
public:

	/// \brief Constructs a SpanComponentBinder
	///
	/// \param component = T
	CL_SpanComponentBinder(T *component)
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

/// \brief Span layout class
///
/// \xmlonly !group=Display/2D! !header=display.h! \endxmlonly
class CL_SpanLayout
{
/// \name Construction
/// \{
public:
	CL_SpanLayout();
	~CL_SpanLayout();
/// \}

	struct HitTestResult
	{
		HitTestResult() : object_id(-1), offset(0) {}

		enum Type
		{
			no_objects_available,
			outside_top,
			outside_left,
			outside_right,
			outside_bottom,
			inside
		} type;

		int object_id;
		int offset;
	};

/// \name Operations
/// \{
public:
	/// \brief Clear
	void clear();

	/// \brief Add text
	///
	/// \param text = String
	/// \param font = Font
	/// \param color = Colorf
	/// \param id = value
	void add_text(const CL_String &text, const CL_Font &font, const CL_Colorf &color = CL_Colorf::white, int id = -1);

	/// \brief Add image
	///
	/// \param image = Image
	/// \param baseline_offset = value
	/// \param id = value
	void add_image(const CL_Image &image, int baseline_offset = 0, int id = -1);

	template<typename T>

	/// \brief Add component
	///
	/// \param component = T
	/// \param baseline_offset = value
	/// \param id = value
	void add_component(T *component, int baseline_offset = 0, int id = -1)
	{
		add_component_helper(new CL_SpanComponentBinder<T>(component), baseline_offset, id);
	}

	/// \brief Layout
	///
	/// \param gc = Graphic Context
	/// \param max_width = value
	void layout(CL_GraphicContext &gc, int max_width);

	/// \brief Set position
	///
	/// \param pos = Point
	void set_position(const CL_Point &pos);

	/// \brief Get Size
	///
	/// \return size
	CL_Size get_size() const;

	/// \brief Get Rect By Id
	///
	/// \return rect
	std::vector<CL_Rect> get_rect_by_id(int id) const;


	/// \brief Hit test
	///
	/// \param gc = Graphic Context
	/// \param pos = Point
	///
	/// \return Hit Test Result
	HitTestResult hit_test(CL_GraphicContext &gc, const CL_Point &pos);

	/// \brief Draw layout
	///
	/// \param gc = Graphic Context
	void draw_layout(CL_GraphicContext &gc);

	/// \brief Set component geometry
	void set_component_geometry();

	/// \brief Find preferred size
	///
	/// \param gc = Graphic Context
	///
	/// \return Size
	CL_Size find_preferred_size(CL_GraphicContext &gc);

	/// \brief Set selection range
	///
	/// \param size_type = String
	/// \param size_type = String
	void set_selection_range(CL_String::size_type start, CL_String::size_type end);

	/// \brief Set selection colors
	///
	/// \param foreground = Colorf
	/// \param background = Colorf
	void set_selection_colors(const CL_Colorf &foreground, const CL_Colorf &background);

	/// \brief Get Combined text
	///
	/// \return combined_text
	CL_String get_combined_text() const;

	/// \brief Sets the text alignment
	///
	/// Alignment is applied when layout() is called
	///
	/// \param align = The alignment
	void set_align(CL_SpanAlign align);
/// \}

/// \name Implementation
/// \{
private:

	/// \brief Add component helper
	///
	/// \param component = Span Component
	/// \param baseline_offset = value
	/// \param id = value
	void add_component_helper(CL_SpanComponent *component, int baseline_offset, int id);

	CL_SharedPtr<CL_SpanLayout_Impl> impl;
/// \}
};
/// \}
