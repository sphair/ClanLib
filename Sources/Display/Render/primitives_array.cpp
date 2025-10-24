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
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#include "Display/precomp.h"
#include "API/Display/Render/primitives_array.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Render/vertex_array_buffer.h"
#include "graphic_context_impl.h"
#include "primitives_array_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_PrimitivesArray Construction:

CL_PrimitivesArray::CL_PrimitivesArray()
{
}

CL_PrimitivesArray::CL_PrimitivesArray(CL_GraphicContext &gc)
: impl(gc.impl->create_prim_array(gc.impl))
{
}

CL_PrimitivesArray::~CL_PrimitivesArray()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_PrimitivesArray Attributes:

void CL_PrimitivesArray::throw_if_null() const
{
	if (impl.is_null())
		throw CL_Exception("is null");
}


/////////////////////////////////////////////////////////////////////////////
// CL_PrimitivesArray Operations:

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec1ub &value)
{
	CL_PrimitivesArrayData::VertexData data = { 1, cl_type_unsigned_byte, 0, 0, true, { 0 } };
	memcpy(data.value_ubyte, (unsigned char *) value, sizeof(unsigned char) * 1);
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec1b &value)
{
	CL_PrimitivesArrayData::VertexData data = { 1, cl_type_byte, 0, 0, true, { 0 } };
	memcpy(data.value_byte, (char *) value, sizeof(char) * 1);
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec1us &value)
{
	CL_PrimitivesArrayData::VertexData data = { 1, cl_type_unsigned_short, 0, 0, true, { 0 } };
	memcpy(data.value_ushort, (unsigned short *) value, sizeof(unsigned short) * 1);
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec1s &value)
{
	CL_PrimitivesArrayData::VertexData data = { 1, cl_type_short, 0, 0, true, { 0 } };
	memcpy(data.value_short, (short *) value, sizeof(short) * 1);
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec1ui &value)
{
	CL_PrimitivesArrayData::VertexData data = { 1, cl_type_unsigned_int, 0, 0, true, { 0 } };
	memcpy(data.value_uint, (unsigned int *) value, sizeof(unsigned int) * 1);
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec1i &value)
{
	CL_PrimitivesArrayData::VertexData data = { 1, cl_type_int, 0, 0, true, { 0 } };
	memcpy(data.value_int, (int *) value, sizeof(int) * 1);
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec1f &value)
{
	CL_PrimitivesArrayData::VertexData data = { 1, cl_type_float, 0, 0, true, { 0 } };
	memcpy(data.value_float, (float *) value, sizeof(float) * 1);
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec2ub &value)
{
	CL_PrimitivesArrayData::VertexData data = { 2, cl_type_unsigned_byte, 0, 0, true, { 0 } };
	memcpy(data.value_ubyte, (unsigned char *) value, sizeof(unsigned char) * 2);
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec2b &value)
{
	CL_PrimitivesArrayData::VertexData data = { 2, cl_type_byte, 0, 0, true, { 0 } };
	memcpy(data.value_byte, (char *) value, sizeof(char) * 2);
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec2us &value)
{
	CL_PrimitivesArrayData::VertexData data = { 2, cl_type_unsigned_short, 0, 0, true, { 0 } };
	memcpy(data.value_ushort, (unsigned short *) value, sizeof(unsigned short) * 2);
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec2s &value)
{
	CL_PrimitivesArrayData::VertexData data = { 2, cl_type_short, 0, 0, true, { 0 } };
	memcpy(data.value_short, (short *) value, sizeof(short) * 2);
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec2ui &value)
{
	CL_PrimitivesArrayData::VertexData data = { 2, cl_type_unsigned_int, 0, 0, true, { 0 } };
	memcpy(data.value_uint, (unsigned int *) value, sizeof(unsigned int) * 2);
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec2i &value)
{
	CL_PrimitivesArrayData::VertexData data = { 2, cl_type_int, 0, 0, true, { 0 } };
	memcpy(data.value_int, (int *) value, sizeof(int) * 2);
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec2f &value)
{
	CL_PrimitivesArrayData::VertexData data = { 2, cl_type_float, 0, 0, true, { 0 } };
	memcpy(data.value_float, (float *) value, sizeof(float) * 2);
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec3ub &value)
{
	CL_PrimitivesArrayData::VertexData data = { 3, cl_type_unsigned_byte, 0, 0, true, { 0 } };
	memcpy(data.value_ubyte, (unsigned char *) value, sizeof(unsigned char) * 3);
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec3b &value)
{
	CL_PrimitivesArrayData::VertexData data = { 3, cl_type_byte, 0, 0, true, { 0 } };
	memcpy(data.value_byte, (char *) value, sizeof(char) * 3);
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec3us &value)
{
	CL_PrimitivesArrayData::VertexData data = { 3, cl_type_unsigned_short, 0, 0, true, { 0 } };
	memcpy(data.value_ushort, (unsigned short *) value, sizeof(unsigned short) * 3);
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec3s &value)
{
	CL_PrimitivesArrayData::VertexData data = { 3, cl_type_short, 0, 0, true, { 0 } };
	memcpy(data.value_short, (short *) value, sizeof(short) * 3);
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec3ui &value)
{
	CL_PrimitivesArrayData::VertexData data = { 3, cl_type_unsigned_int, 0, 0, true, { 0 } };
	memcpy(data.value_uint, (unsigned int *) value, sizeof(unsigned int) * 3);
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec3i &value)
{
	CL_PrimitivesArrayData::VertexData data = { 3, cl_type_int, 0, 0, true, { 0 } };
	memcpy(data.value_int, (int *) value, sizeof(int) * 3);
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec3f &value)
{
	CL_PrimitivesArrayData::VertexData data = { 3, cl_type_float, 0, 0, true, { 0 } };
	memcpy(data.value_float, (float *) value, sizeof(float) * 3);
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec4ub &value, bool normalize)
{
	CL_PrimitivesArrayData::VertexData data = { 4, cl_type_unsigned_byte, 0, 0, true, { 0 } };
	memcpy(data.value_ubyte, (unsigned char *) value, sizeof(unsigned char) * 4);
	impl->set_attribute(index, data, normalize);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec4b &value, bool normalize)
{
	CL_PrimitivesArrayData::VertexData data = { 4, cl_type_byte, 0, 0, true, { 0 } };
	memcpy(data.value_byte, (char *) value, sizeof(char) * 4);
	impl->set_attribute(index, data, normalize);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec4us &value, bool normalize)
{
	CL_PrimitivesArrayData::VertexData data = { 4, cl_type_unsigned_short, 0, 0, true, { 0 } };
	memcpy(data.value_ushort, (unsigned short *) value, sizeof(unsigned short) * 4);
	impl->set_attribute(index, data, normalize);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec4s &value, bool normalize)
{
	CL_PrimitivesArrayData::VertexData data = { 4, cl_type_short, 0, 0, true, { 0 } };
	memcpy(data.value_short, (short *) value, sizeof(short) * 4);
	impl->set_attribute(index, data, normalize);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec4ui &value, bool normalize)
{
	CL_PrimitivesArrayData::VertexData data = { 4, cl_type_unsigned_int, 0, 0, true, { 0 } };
	memcpy(data.value_uint, (unsigned int *) value, sizeof(unsigned int) * 4);
	impl->set_attribute(index, data, normalize);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec4i &value, bool normalize)
{
	CL_PrimitivesArrayData::VertexData data = { 4, cl_type_int, 0, 0, true, { 0 } };
	memcpy(data.value_int, (int *) value, sizeof(int) * 4);
	impl->set_attribute(index, data, normalize);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Vec4f &value)
{
	CL_PrimitivesArrayData::VertexData data = { 4, cl_type_float, 0, 0, true, { 0 } };
	memcpy(data.value_float, (float *) value, sizeof(float) * 4);
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attribute(int index, const CL_Colorf &value)
{
	CL_PrimitivesArrayData::VertexData data = { 4, cl_type_float, 0, 0, true, { 0 } };
	memcpy(data.value_float, (float *) &value, sizeof(float) * 4);
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attributes(int index, CL_VertexArrayBuffer &buffer, int size, CL_VertexAttributeDataType type, void *offset, int stride, bool normalize)
{
	CL_PrimitivesArrayData::VertexData data = { size, type, stride, buffer.get_provider(), false, { (void *) offset } };
	impl->set_attribute(index, data, normalize);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec1ub * const values, int stride)
{
	CL_PrimitivesArrayData::VertexData data = { 1, cl_type_unsigned_byte, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec1b * const values, int stride)
{
	CL_PrimitivesArrayData::VertexData data = { 1, cl_type_byte, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec1us * const values, int stride)
{
	CL_PrimitivesArrayData::VertexData data = { 1, cl_type_unsigned_short, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec1s * const values, int stride)
{
	CL_PrimitivesArrayData::VertexData data = { 1, cl_type_short, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec1ui * const values, int stride)
{
	CL_PrimitivesArrayData::VertexData data = { 1, cl_type_unsigned_int, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec1i * const values, int stride)
{
	CL_PrimitivesArrayData::VertexData data = { 1, cl_type_int, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec1f * const values, int stride)
{
	CL_PrimitivesArrayData::VertexData data = { 1, cl_type_float, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec2ub * const values, int stride)
{
	CL_PrimitivesArrayData::VertexData data = { 2, cl_type_unsigned_byte, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec2b * const values, int stride)
{
	CL_PrimitivesArrayData::VertexData data = { 2, cl_type_byte, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec2us * const values, int stride)
{
	CL_PrimitivesArrayData::VertexData data = { 2, cl_type_unsigned_short, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec2s * const values, int stride)
{
	CL_PrimitivesArrayData::VertexData data = { 2, cl_type_short, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec2ui * const values, int stride)
{
	CL_PrimitivesArrayData::VertexData data = { 2, cl_type_unsigned_int, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec2i * const values, int stride)
{
	CL_PrimitivesArrayData::VertexData data = { 2, cl_type_int, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec2f * const values, int stride)
{
	CL_PrimitivesArrayData::VertexData data = { 2, cl_type_float, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec3ub * const values, int stride)
{
	CL_PrimitivesArrayData::VertexData data = { 3, cl_type_unsigned_byte, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec3b * const values, int stride)
{
	CL_PrimitivesArrayData::VertexData data = { 3, cl_type_byte, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec3us * const values, int stride)
{
	CL_PrimitivesArrayData::VertexData data = { 3, cl_type_unsigned_short, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec3s * const values, int stride)
{
	CL_PrimitivesArrayData::VertexData data = { 3, cl_type_short, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec3ui * const values, int stride)
{
	CL_PrimitivesArrayData::VertexData data = { 3, cl_type_unsigned_int, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec3i * const values, int stride)
{
	CL_PrimitivesArrayData::VertexData data = { 3, cl_type_int, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec3f * const values, int stride)
{
	CL_PrimitivesArrayData::VertexData data = { 3, cl_type_float, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec4ub * const values, int stride, bool normalize)
{
	CL_PrimitivesArrayData::VertexData data = { 4, cl_type_unsigned_byte, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data, normalize);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec4b * const values, int stride, bool normalize)
{
	CL_PrimitivesArrayData::VertexData data = { 4, cl_type_byte, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data, normalize);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec4us * const values, int stride, bool normalize)
{
	CL_PrimitivesArrayData::VertexData data = { 4, cl_type_unsigned_short, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data, normalize);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec4s * const values, int stride, bool normalize)
{
	CL_PrimitivesArrayData::VertexData data = { 4, cl_type_short, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data, normalize);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec4ui * const values, int stride, bool normalize)
{
	CL_PrimitivesArrayData::VertexData data = { 4, cl_type_unsigned_int, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data, normalize);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec4i * const values, int stride, bool normalize)
{
	CL_PrimitivesArrayData::VertexData data = { 4, cl_type_int, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data, normalize);
}

void CL_PrimitivesArray::set_attributes(int index, const CL_Vec4f * const values, int stride)
{
	CL_PrimitivesArrayData::VertexData data = { 4, cl_type_float, stride, 0, false, { (void *) values } };
	impl->set_attribute(index, data);
}

/////////////////////////////////////////////////////////////////////////////
// CL_PrimitivesArray Implementation:


