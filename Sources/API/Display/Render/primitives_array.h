/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

/// \brief Primitives array description.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_PrimitivesArray
{
/// \name Construction
/// \{
public:
	CL_PrimitivesArray();

	CL_PrimitivesArray(CL_GraphicContext &gc);

	~CL_PrimitivesArray();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns true if the primitives array was created without specifying a graphic context.
	bool is_null() const;

/// \}
/// \name Operations
/// \{
public:
	void set_attribute(int index, const CL_Vec1ub &value);
	void set_attribute(int index, const CL_Vec1b &value);
	void set_attribute(int index, const CL_Vec1us &value);
	void set_attribute(int index, const CL_Vec1s &value);
	void set_attribute(int index, const CL_Vec1ui &value);
	void set_attribute(int index, const CL_Vec1i &value);
	void set_attribute(int index, const CL_Vec1f &value);
	void set_attribute(int index, const CL_Vec2ub &value);
	void set_attribute(int index, const CL_Vec2b &value);
	void set_attribute(int index, const CL_Vec2us &value);
	void set_attribute(int index, const CL_Vec2s &value);
	void set_attribute(int index, const CL_Vec2ui &value);
	void set_attribute(int index, const CL_Vec2i &value);
	void set_attribute(int index, const CL_Vec2f &value);
	void set_attribute(int index, const CL_Vec3ub &value);
	void set_attribute(int index, const CL_Vec3b &value);
	void set_attribute(int index, const CL_Vec3us &value);
	void set_attribute(int index, const CL_Vec3s &value);
	void set_attribute(int index, const CL_Vec3ui &value);
	void set_attribute(int index, const CL_Vec3i &value);
	void set_attribute(int index, const CL_Vec3f &value);
	void set_attribute(int index, const CL_Vec4ub &value, bool normalize = false);
	void set_attribute(int index, const CL_Vec4b &value, bool normalize = false);
	void set_attribute(int index, const CL_Vec4us &value, bool normalize = false);
	void set_attribute(int index, const CL_Vec4s &value, bool normalize = false);
	void set_attribute(int index, const CL_Vec4ui &value, bool normalize = false);
	void set_attribute(int index, const CL_Vec4i &value, bool normalize = false);
	void set_attribute(int index, const CL_Vec4f &value);
	void set_attribute(int index, const CL_Colorf &value);

	void set_attributes(int index, CL_VertexArrayBuffer &buffer, int size, CL_VertexAttributeDataType type, void *offset = 0, int stride = 0, bool normalize = false);
	void set_attributes(int index, const CL_Vec1ub * const values, int stride = 0);
	void set_attributes(int index, const CL_Vec1b * const values, int stride = 0);
	void set_attributes(int index, const CL_Vec1us * const values, int stride = 0);
	void set_attributes(int index, const CL_Vec1s * const values, int stride = 0);
	void set_attributes(int index, const CL_Vec1ui * const values, int stride = 0);
	void set_attributes(int index, const CL_Vec1i * const values, int stride = 0);
	void set_attributes(int index, const CL_Vec1f * const values, int stride = 0);
	void set_attributes(int index, const CL_Vec2ub * const values, int stride = 0);
	void set_attributes(int index, const CL_Vec2b * const values, int stride = 0);
	void set_attributes(int index, const CL_Vec2us * const values, int stride = 0);
	void set_attributes(int index, const CL_Vec2s * const values, int stride = 0);
	void set_attributes(int index, const CL_Vec2ui * const values, int stride = 0);
	void set_attributes(int index, const CL_Vec2i * const values, int stride = 0);
	void set_attributes(int index, const CL_Vec2f * const values, int stride = 0);
	void set_attributes(int index, const CL_Vec3ub * const values, int stride = 0);
	void set_attributes(int index, const CL_Vec3b * const values, int stride = 0);
	void set_attributes(int index, const CL_Vec3us * const values, int stride = 0);
	void set_attributes(int index, const CL_Vec3s * const values, int stride = 0);
	void set_attributes(int index, const CL_Vec3ui * const values, int stride = 0);
	void set_attributes(int index, const CL_Vec3i * const values, int stride = 0);
	void set_attributes(int index, const CL_Vec3f * const values, int stride = 0);
	void set_attributes(int index, const CL_Vec4ub * const values, int stride = 0, bool normalize = false);
	void set_attributes(int index, const CL_Vec4b * const values, int stride = 0, bool normalize = false);
	void set_attributes(int index, const CL_Vec4us * const values, int stride = 0, bool normalize = false);
	void set_attributes(int index, const CL_Vec4s * const values, int stride = 0, bool normalize = false);
	void set_attributes(int index, const CL_Vec4ui * const values, int stride = 0, bool normalize = false);
	void set_attributes(int index, const CL_Vec4i * const values, int stride = 0, bool normalize = false);
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
