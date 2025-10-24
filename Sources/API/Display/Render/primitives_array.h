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
**    Harry Storbacka
**    Mark Page
**    Kenneth Gangstoe
**
*/

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include "../api_display.h"
#include "../../Core/System/sharedptr.h"
#include "../../Core/Math/vec3.h"

class CL_GraphicContext;
class CL_Colorf;
class CL_Colorf;
class CL_GraphicContext;
class CL_VertexArrayBuffer;
class CL_PrimitivesArray_Impl;

/// \brief Primitives array description.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
enum CL_VertexAttributeDataType
{
	cl_type_unsigned_byte,
	cl_type_unsigned_short,
	cl_type_unsigned_int,
	cl_type_byte,
	cl_type_short,
	cl_type_int,
	cl_type_float
};

/// \brief Standard Attribute Index
///
/// These can be used within CL_Primitives array when you are using CL_StandardProgram.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
enum CL_StandardAttributeIndex
{
	cl_attrib_position = 0,
	cl_attrib_color = 1,
	cl_attrib_texture_position = 2,
	cl_attrib_texture_index = 3,
	cl_attrib_normal = 4	// The normal vector (Currently only used by the GL1 target)
};

/// \brief Primitives array description.
///
/// See "CL_StandardAttributeIndex" for the index values if using CL_StandardProgram with CL_PrimitivesArray
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_PrimitivesArray
{
/// \name Construction
/// \{
public:
	/// \brief Construct a null instance
	CL_PrimitivesArray();

	/// \brief Constructs a PrimitivesArray
	///
	/// \param gc = Graphic Context
	CL_PrimitivesArray(CL_GraphicContext &gc);

	~CL_PrimitivesArray();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;


/// \}
/// \name Operations
/// \{
public:

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec1ub
	void set_attribute(int index, const CL_Vec1ub &value);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec1b
	void set_attribute(int index, const CL_Vec1b &value);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec1us
	void set_attribute(int index, const CL_Vec1us &value);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec1s
	void set_attribute(int index, const CL_Vec1s &value);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec1ui
	void set_attribute(int index, const CL_Vec1ui &value);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec1i
	void set_attribute(int index, const CL_Vec1i &value);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec1f
	void set_attribute(int index, const CL_Vec1f &value);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec2ub
	void set_attribute(int index, const CL_Vec2ub &value);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec2b
	void set_attribute(int index, const CL_Vec2b &value);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec2us
	void set_attribute(int index, const CL_Vec2us &value);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec2s
	void set_attribute(int index, const CL_Vec2s &value);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec2ui
	void set_attribute(int index, const CL_Vec2ui &value);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec2i
	void set_attribute(int index, const CL_Vec2i &value);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec2f
	void set_attribute(int index, const CL_Vec2f &value);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec3ub
	void set_attribute(int index, const CL_Vec3ub &value);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec3b
	void set_attribute(int index, const CL_Vec3b &value);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec3us
	void set_attribute(int index, const CL_Vec3us &value);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec3s
	void set_attribute(int index, const CL_Vec3s &value);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec3ui
	void set_attribute(int index, const CL_Vec3ui &value);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec3i
	void set_attribute(int index, const CL_Vec3i &value);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec3f
	void set_attribute(int index, const CL_Vec3f &value);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec4ub
	/// \param normalize = bool
	void set_attribute(int index, const CL_Vec4ub &value, bool normalize = false);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec4b
	/// \param normalize = bool
	void set_attribute(int index, const CL_Vec4b &value, bool normalize = false);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec4us
	/// \param normalize = bool
	void set_attribute(int index, const CL_Vec4us &value, bool normalize = false);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec4s
	/// \param normalize = bool
	void set_attribute(int index, const CL_Vec4s &value, bool normalize = false);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec4ui
	/// \param normalize = bool
	void set_attribute(int index, const CL_Vec4ui &value, bool normalize = false);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec4i
	/// \param normalize = bool
	void set_attribute(int index, const CL_Vec4i &value, bool normalize = false);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Vec4f
	void set_attribute(int index, const CL_Vec4f &value);

	/// \brief Set attribute
	///
	/// \param index = value
	/// \param value = Colorf
	void set_attribute(int index, const CL_Colorf &value);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param buffer = Vertex Array Buffer
	/// \param size = value
	/// \param type = Vertex Attribute Data Type
	/// \param offset = void
	/// \param stride = value
	/// \param normalize = bool
	void set_attributes(int index, CL_VertexArrayBuffer &buffer, int size, CL_VertexAttributeDataType type, void *offset = 0, int stride = 0, bool normalize = false);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec1ub
	/// \param stride = value
	void set_attributes(int index, const CL_Vec1ub * const values, int stride = 0);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec1b
	/// \param stride = value
	void set_attributes(int index, const CL_Vec1b * const values, int stride = 0);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec1us
	/// \param stride = value
	void set_attributes(int index, const CL_Vec1us * const values, int stride = 0);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec1s
	/// \param stride = value
	void set_attributes(int index, const CL_Vec1s * const values, int stride = 0);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec1ui
	/// \param stride = value
	void set_attributes(int index, const CL_Vec1ui * const values, int stride = 0);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec1i
	/// \param stride = value
	void set_attributes(int index, const CL_Vec1i * const values, int stride = 0);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec1f
	/// \param stride = value
	void set_attributes(int index, const CL_Vec1f * const values, int stride = 0);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec2ub
	/// \param stride = value
	void set_attributes(int index, const CL_Vec2ub * const values, int stride = 0);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec2b
	/// \param stride = value
	void set_attributes(int index, const CL_Vec2b * const values, int stride = 0);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec2us
	/// \param stride = value
	void set_attributes(int index, const CL_Vec2us * const values, int stride = 0);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec2s
	/// \param stride = value
	void set_attributes(int index, const CL_Vec2s * const values, int stride = 0);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec2ui
	/// \param stride = value
	void set_attributes(int index, const CL_Vec2ui * const values, int stride = 0);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec2i
	/// \param stride = value
	void set_attributes(int index, const CL_Vec2i * const values, int stride = 0);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec2f
	/// \param stride = value
	void set_attributes(int index, const CL_Vec2f * const values, int stride = 0);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec3ub
	/// \param stride = value
	void set_attributes(int index, const CL_Vec3ub * const values, int stride = 0);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec3b
	/// \param stride = value
	void set_attributes(int index, const CL_Vec3b * const values, int stride = 0);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec3us
	/// \param stride = value
	void set_attributes(int index, const CL_Vec3us * const values, int stride = 0);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec3s
	/// \param stride = value
	void set_attributes(int index, const CL_Vec3s * const values, int stride = 0);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec3ui
	/// \param stride = value
	void set_attributes(int index, const CL_Vec3ui * const values, int stride = 0);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec3i
	/// \param stride = value
	void set_attributes(int index, const CL_Vec3i * const values, int stride = 0);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec3f
	/// \param stride = value
	void set_attributes(int index, const CL_Vec3f * const values, int stride = 0);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec4ub
	/// \param stride = value
	/// \param normalize = bool
	void set_attributes(int index, const CL_Vec4ub * const values, int stride = 0, bool normalize = false);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec4b
	/// \param stride = value
	/// \param normalize = bool
	void set_attributes(int index, const CL_Vec4b * const values, int stride = 0, bool normalize = false);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec4us
	/// \param stride = value
	/// \param normalize = bool
	void set_attributes(int index, const CL_Vec4us * const values, int stride = 0, bool normalize = false);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec4s
	/// \param stride = value
	/// \param normalize = bool
	void set_attributes(int index, const CL_Vec4s * const values, int stride = 0, bool normalize = false);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec4ui
	/// \param stride = value
	/// \param normalize = bool
	void set_attributes(int index, const CL_Vec4ui * const values, int stride = 0, bool normalize = false);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec4i
	/// \param stride = value
	/// \param normalize = bool
	void set_attributes(int index, const CL_Vec4i * const values, int stride = 0, bool normalize = false);

	/// \brief Set attributes
	///
	/// \param index = value
	/// \param values = Vec4f
	/// \param stride = value
	void set_attributes(int index, const CL_Vec4f * const values, int stride = 0);

/// \}
/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_PrimitivesArray_Impl> impl;

	friend class CL_GraphicContext;
	friend class CL_PrimitivesArrayBuilder;
/// \}
};

/// \}
