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
*/

#pragma once


#include "API/Display/TargetProviders/program_object_provider.h"
#include "API/Core/Math/vec4.h"

class CL_SoftwareProgram;

class CL_SWRenderProgramObjectProvider : public CL_ProgramObjectProvider
{
/// \name Construction
/// \{
public:
	CL_SWRenderProgramObjectProvider();
	~CL_SWRenderProgramObjectProvider();
	void destroy();
/// \}

/// \name Attributes
/// \{
public:
	CL_SoftwareProgram *get_program() const;
	bool is_sprite_program() const;
	unsigned int get_handle() const;
	bool get_link_status() const;
	bool get_validate_status() const;
	CL_String get_info_log() const;
	int get_attribute_count() const;
	std::vector<CL_ShaderObject> get_shaders() const;
	std::vector<CL_ProgramAttribute> get_attributes() const;
	int get_attribute_location(const CL_StringRef &name) const;
	int get_uniform_count() const;
	std::vector<CL_ProgramUniform> get_uniforms() const;
	int get_uniform_location(const CL_StringRef &name) const;

	const std::vector<int> &get_bind_locations() const { return bind_locations; }
	const std::vector<CL_Vec4f> &get_attribute_defaults() const { return attribute_defaults; }
	std::vector<CL_Vec4f> &get_current_attribute_values() { return current_attribute_values; }
/// \}

/// \name Operations
/// \{
public:
	void set_program(CL_SoftwareProgram *program);
	void set_sprite_program(bool is_sprite_program_flag);
	void attach(const CL_ShaderObject &obj);
	void detach(const CL_ShaderObject &obj);
	void bind_attribute_location(int index, const CL_StringRef &name);
	void bind_frag_data_location(int color_number, const CL_StringRef &name);
	void link();
	void validate();

	void set_uniform1i(const CL_StringRef &name, int);
	void set_uniform2i(const CL_StringRef &name, int, int);
	void set_uniform3i(const CL_StringRef &name, int, int, int);
	void set_uniform4i(const CL_StringRef &name, int, int, int, int);
	void set_uniformiv(const CL_StringRef &name, int size, int count, int *data);
	void set_uniform1f(const CL_StringRef &name, float);
	void set_uniform2f(const CL_StringRef &name, float, float);
	void set_uniform3f(const CL_StringRef &name, float, float, float);
	void set_uniform4f(const CL_StringRef &name, float, float, float, float);
	void set_uniformfv(const CL_StringRef &name, int size, int count, float *data);

	void set_uniform_matrix(const CL_StringRef &name, int size, int count, bool transpose, float *data);
/// \}

/// \name Implementation
/// \{
private:
	CL_SoftwareProgram *current_program;
	bool sprite_program;
	std::vector<int> bind_locations;
	std::vector<CL_Vec4f> attribute_defaults;
	std::vector<CL_Vec4f> current_attribute_values;
/// \}
};


