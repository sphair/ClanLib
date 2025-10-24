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
**    Mark Page
*/

/// \addtogroup clanGL1_Display clanGL1 Display
/// \{

#pragma once

#include "api_gl1.h"
#include "opengl1_defines.h"
#include "../Core/System/sharedptr.h"
#include "../Display/Render/graphic_context.h"
#include "material.h"
#include "light_source.h"
#include "texture_unit.h"
#include "light_model.h"

class CL_RenderWindowProvider;
class CL_ShaderObjectProvider;
class CL_FrameBufferProvider;
class CL_RenderBufferProvider;
class CL_GL1FrameBufferProvider;
class CL_GraphicContext_GL1_Impl;

/// \brief GL1 Graphic Context
///
/// \xmlonly !group=GL1/Display! !header=gl1.h! \endxmlonly
class CL_API_GL1 CL_GraphicContext_GL1 : public CL_GraphicContext
{
//!Construction
public:
	// \brief Create a null instance
	CL_GraphicContext_GL1() {}

	/// \brief Create a GL1 specific graphics context
	CL_GraphicContext_GL1(CL_GraphicContext &gc);

	~CL_GraphicContext_GL1();

//!Attributes
public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Get Max texture coords
	///
	/// \return max_texture_coords
	int get_max_texture_coords();

	/// \brief Get Light count
	///
	/// \return light_count
	int get_light_count();

	/// \brief Get the opengl version
	///
	/// \param version_major = On Return: Major
	/// \param version_minor = On Return: Minor
	/// \param version_release = On Return: Release
	void get_opengl_version(int &version_major, int &version_minor, int &version_release);

//!Operations
public:

	/// \brief Push texture matrix
	///
	/// \param unit_index = value
	void push_texture_matrix(int unit_index);

	/// \brief Set texture matrix
	///
	/// \param unit_index = value
	/// \param matrix = Mat4f
	void set_texture_matrix(int unit_index, const CL_Mat4f &matrix);

	/// \brief Pop texture matrix
	///
	/// \param unit_index = value
	void pop_texture_matrix(int unit_index);

	/// \brief Set light model
	///
	/// \param light_model = Light Model_ GL1
	void set_light_model(const CL_LightModel_GL1 &light_model);

	/// \brief Reset light model
	void reset_light_model();

	/// \brief Set material
	///
	/// \param material = Material_ GL1
	void set_material(const CL_Material_GL1 &material);

	/// \brief Set front material
	///
	/// \param material = Material_ GL1
	void set_front_material(const CL_Material_GL1 &material);

	/// \brief Set back material
	///
	/// \param material = Material_ GL1
	void set_back_material(const CL_Material_GL1 &material);

	/// \brief Reset front material
	void reset_front_material();

	/// \brief Reset back material
	void reset_back_material();

	/// \brief Set material
	///
	/// \param face = GLenum
	/// \param material = Material_ GL1
	void set_material(GLenum face, const CL_Material_GL1 &material);

	/// \brief Reset material
	void reset_material();

	/// \brief Set color material
	///
	/// \param color = Color Material_ GL1
	void set_color_material(CL_ColorMaterial_GL1 color);

	/// \brief Set color material front
	///
	/// \param color = Color Material_ GL1
	void set_color_material_front(CL_ColorMaterial_GL1 color);

	/// \brief Set color material back
	///
	/// \param color = Color Material_ GL1
	void set_color_material_back(CL_ColorMaterial_GL1 color);

	/// \brief Reset color material
	void reset_color_material();

	/// \brief Set color material
	///
	/// \param face = GLenum
	/// \param color = Color Material_ GL1
	void set_color_material(GLenum face, CL_ColorMaterial_GL1 color);

	/// \brief Set light
	///
	/// \param light_index = value
	/// \param light = Light Source_ GL1
	void set_light(int light_index, const CL_LightSource_GL1 &light);

	/// \brief Reset light
	///
	/// \param light_index = value
	void reset_light(int light_index);

	/// \brief Set texture unit
	///
	/// \param unit_index = value
	/// \param unit = Texture Unit_ GL1
	void set_texture_unit(int unit_index, const CL_TextureUnit_GL1 &unit);

	/// \brief Sets the thread's OpenGL context to the one used by the graphic context
	///
	/// If the CL_FrameBuffer is active, then the OpenGL context will be the OpenGL PBuffer
	void set_active();

//!Implementation
private:
	CL_SharedPtr<CL_GraphicContext_GL1_Impl> impl;
};

/// \}
