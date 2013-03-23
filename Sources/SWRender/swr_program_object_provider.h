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

namespace clan
{

class SoftwareProgram;

class SWRenderProgramObjectProvider : public ProgramObjectProvider
{
/// \name Construction
/// \{
public:
	SWRenderProgramObjectProvider();
	~SWRenderProgramObjectProvider();

/// \}

/// \name Attributes
/// \{
public:
	SoftwareProgram *get_program() const;
	bool is_sprite_program() const;
	unsigned int get_handle() const;
	bool get_link_status() const;
	bool get_validate_status() const;
	std::string get_info_log() const;
	int get_attribute_count() const;
	std::vector<ShaderObject> get_shaders() const;
	std::vector<ProgramAttribute> get_attributes() const;
	int get_attribute_location(const std::string &name) const;
	int get_uniform_count() const;
	std::vector<ProgramUniform> get_uniforms() const;
	int get_uniform_location(const std::string &name) const;

	const std::vector<int> &get_bind_locations() const { return bind_locations; }
	const std::vector<Vec4f> &get_attribute_defaults() const { return attribute_defaults; }
	std::vector<Vec4f> &get_current_attribute_values() { return current_attribute_values; }
	int get_uniform_buffer_size(int block_index) const;
	int get_uniform_buffer_index(const std::string &block_name) const;
	int get_storage_buffer_index(const std::string &name) const;
/// \}

/// \name Operations
/// \{
public:

	void set_uniform1i(int location, int);
	void set_uniform_buffer_index(int block_index, int bind_index);
	void set_storage_buffer_index(int buffer_index, int bind_unit_index);

	void set_program(SoftwareProgram *program);
	void set_sprite_program(bool is_sprite_program_flag);
	void attach(const ShaderObject &obj);
	void detach(const ShaderObject &obj);
	void bind_attribute_location(int index, const std::string &name);
	void bind_frag_data_location(int color_number, const std::string &name);
	void link();
	void validate();

	void set_uniform1i(const std::string &name, int);
	void set_uniform2i(const std::string &name, int, int);
	void set_uniform3i(const std::string &name, int, int, int);
	void set_uniform4i(const std::string &name, int, int, int, int);
	void set_uniformiv(const std::string &name, int size, int count, int *data);
	void set_uniform1f(const std::string &name, float);
	void set_uniform2f(const std::string &name, float, float);
	void set_uniform3f(const std::string &name, float, float, float);
	void set_uniform4f(const std::string &name, float, float, float, float);
	void set_uniformfv(const std::string &name, int size, int count, float *data);

	void set_uniform_matrix(const std::string &name, int size, int count, bool transpose, float *data);
/// \}

/// \name Implementation
/// \{
private:
	SoftwareProgram *current_program;
	bool sprite_program;
	std::vector<int> bind_locations;
	std::vector<Vec4f> attribute_defaults;
	std::vector<Vec4f> current_attribute_values;
/// \}
};

}

