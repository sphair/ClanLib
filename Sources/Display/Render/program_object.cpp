/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#include "Display/precomp.h"
#include "API/Core/IOData/file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Display/Render/shader_object.h"
#include "API/Display/Render/program_object.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/TargetProviders/program_object_provider.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Resources/xml_resource_node.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/XML/dom_element.h"

namespace clan
{
	class ProgramObject_Impl
	{
	public:
		ProgramObject_Impl() : provider(nullptr)
		{
		}

		~ProgramObject_Impl()
		{
			if (provider)
				delete provider;
		}

		ProgramObjectProvider *provider;
	};

	ProgramObject::ProgramObject()
	{
	}

	ProgramObject::ProgramObject(GraphicContext &gc)
		: impl(std::make_shared<ProgramObject_Impl>())
	{
		GraphicContextProvider *gc_provider = gc.get_provider();

		impl->provider = gc_provider->alloc_program_object();
	}

	ProgramObject::ProgramObject(GraphicContextProvider *gc_provider)
		: impl(std::make_shared<ProgramObject_Impl>())
	{
		impl->provider = gc_provider->alloc_program_object();
	}

	ProgramObject::ProgramObject(ProgramObjectProvider *provider)
		: impl(std::make_shared<ProgramObject_Impl>())
	{
		impl->provider = provider;
	}

	ProgramObject ProgramObject::load(GraphicContext &gc, const std::string &vertex_filename, const std::string &fragment_filename, const FileSystem &fs)
	{
		ProgramObject program_object(gc);

		ShaderObject vertex_shader = ShaderObject::load_and_compile(gc, shadertype_vertex, vertex_filename, fs);
		program_object.attach(vertex_shader);

		ShaderObject fragment_shader = ShaderObject::load_and_compile(gc, shadertype_fragment, fragment_filename, fs);
		program_object.attach(fragment_shader);

		return program_object;
	}

	ProgramObject ProgramObject::load(GraphicContext &gc, const std::string &vertex_filename, const std::string &geometry_filename, const std::string &fragment_filename, const FileSystem &fs)
	{
		ProgramObject program_object(gc);

		ShaderObject vertex_shader = ShaderObject::load_and_compile(gc, shadertype_vertex, vertex_filename, fs);
		program_object.attach(vertex_shader);

		ShaderObject geometry_shader = ShaderObject::load_and_compile(gc, shadertype_geometry, geometry_filename, fs);
		program_object.attach(geometry_shader);

		ShaderObject fragment_shader = ShaderObject::load_and_compile(gc, shadertype_fragment, fragment_filename, fs);
		program_object.attach(fragment_shader);

		return program_object;
	}

	ProgramObject ProgramObject::load(GraphicContext &gc, const std::string &vertex_fullname, const std::string &fragment_fullname)
	{
		std::string path_vertex = PathHelp::get_fullpath(vertex_fullname, PathHelp::path_type_file);
		std::string filename_vertex = PathHelp::get_filename(vertex_fullname, PathHelp::path_type_file);
		FileSystem vfs_vertex(path_vertex);
		std::string path_fragment = PathHelp::get_fullpath(fragment_fullname, PathHelp::path_type_file);
		std::string filename_fragment = PathHelp::get_filename(fragment_fullname, PathHelp::path_type_file);
		FileSystem vfs_fragment(path_fragment);

		ProgramObject program_object(gc);

		ShaderObject vertex_shader = ShaderObject::load_and_compile(gc, shadertype_vertex, filename_vertex, vfs_vertex);
		program_object.attach(vertex_shader);

		ShaderObject fragment_shader = ShaderObject::load_and_compile(gc, shadertype_fragment, filename_fragment, vfs_fragment);
		program_object.attach(fragment_shader);

		return program_object;
	}

	ProgramObject ProgramObject::load(GraphicContext &gc, const std::string &vertex_fullname, const std::string &geometry_fullname, const std::string &fragment_fullname)
	{
		std::string path_vertex = PathHelp::get_fullpath(vertex_fullname, PathHelp::path_type_file);
		std::string filename_vertex = PathHelp::get_filename(vertex_fullname, PathHelp::path_type_file);
		FileSystem vfs_vertex(path_vertex);
		std::string path_geometry = PathHelp::get_fullpath(geometry_fullname, PathHelp::path_type_file);
		std::string filename_geometry = PathHelp::get_filename(geometry_fullname, PathHelp::path_type_file);
		FileSystem vfs_geometry(path_geometry);
		std::string path_fragment = PathHelp::get_fullpath(fragment_fullname, PathHelp::path_type_file);
		std::string filename_fragment = PathHelp::get_filename(fragment_fullname, PathHelp::path_type_file);
		FileSystem vfs_fragment(path_fragment);

		ProgramObject program_object(gc);

		ShaderObject vertex_shader = ShaderObject::load_and_compile(gc, shadertype_vertex, filename_vertex, vfs_vertex);
		program_object.attach(vertex_shader);

		ShaderObject geometry_shader = ShaderObject::load_and_compile(gc, shadertype_geometry, filename_geometry, vfs_geometry);
		program_object.attach(geometry_shader);

		ShaderObject fragment_shader = ShaderObject::load_and_compile(gc, shadertype_fragment, filename_fragment, vfs_fragment);
		program_object.attach(fragment_shader);

		return program_object;
	}

	ProgramObject ProgramObject::load(GraphicContext &gc, IODevice &vertex_file, IODevice &fragment_file)
	{
		ProgramObject program_object(gc);

		ShaderObject vertex_shader = ShaderObject::load_and_compile(gc, shadertype_vertex, vertex_file);
		program_object.attach(vertex_shader);

		ShaderObject fragment_shader = ShaderObject::load_and_compile(gc, shadertype_fragment, fragment_file);
		program_object.attach(fragment_shader);

		return program_object;
	}

	ProgramObject ProgramObject::load(GraphicContext &gc, IODevice &vertex_file, IODevice &geometry_file, IODevice &fragment_file)
	{
		ProgramObject program_object(gc);

		ShaderObject vertex_shader = ShaderObject::load_and_compile(gc, shadertype_vertex, vertex_file);
		program_object.attach(vertex_shader);

		ShaderObject geometry_shader = ShaderObject::load_and_compile(gc, shadertype_geometry, geometry_file);
		program_object.attach(geometry_shader);

		ShaderObject fragment_shader = ShaderObject::load_and_compile(gc, shadertype_fragment, fragment_file);
		program_object.attach(fragment_shader);

		return program_object;
	}

	ProgramObject ProgramObject::load_and_link(GraphicContext &gc, const std::string &vertex_filename, const std::string &fragment_filename, const FileSystem &fs)
	{
		ProgramObject program_object = ProgramObject::load(gc, vertex_filename, fragment_filename, fs);

		if (!program_object.link())
			throw Exception(string_format("Unable to link program object: %1", program_object.get_info_log()));

		return program_object;
	}

	ProgramObject ProgramObject::load_and_link(GraphicContext &gc, const std::string &vertex_filename, const std::string &geometry_filename, const std::string &fragment_filename, const FileSystem &fs)
	{
		ProgramObject program_object = ProgramObject::load(gc, vertex_filename, geometry_filename, fragment_filename, fs);

		if (!program_object.link())
			throw Exception(string_format("Unable to link program object: %1", program_object.get_info_log()));

		return program_object;
	}

	ProgramObject ProgramObject::load_and_link(GraphicContext &gc, const std::string &vertex_fullname, const std::string &fragment_fullname)
	{
		ProgramObject program_object = ProgramObject::load(gc, vertex_fullname, fragment_fullname);

		if (!program_object.link())
			throw Exception(string_format("Unable to link program object: %1", program_object.get_info_log()));

		return program_object;
	}

	ProgramObject ProgramObject::load_and_link(GraphicContext &gc, const std::string &vertex_fullname, const std::string &geometry_fullname, const std::string &fragment_fullname)
	{
		ProgramObject program_object = ProgramObject::load(gc, vertex_fullname, geometry_fullname, fragment_fullname);

		if (!program_object.link())
			throw Exception(string_format("Unable to link program object: %1", program_object.get_info_log()));

		return program_object;
	}

	ProgramObject ProgramObject::load_and_link(GraphicContext &gc, IODevice &vertex_file, IODevice &fragment_file)
	{
		ProgramObject program_object = ProgramObject::load(gc, vertex_file, fragment_file);

		if (!program_object.link())
			throw Exception(string_format("Unable to link program object: %1", program_object.get_info_log()));

		return program_object;
	}

	ProgramObject ProgramObject::load_and_link(GraphicContext &gc, IODevice &vertex_file, IODevice &geometry_file, IODevice &fragment_file)
	{
		ProgramObject program_object = ProgramObject::load(gc, vertex_file, geometry_file, fragment_file);

		if (!program_object.link())
			throw Exception(string_format("Unable to link program object: %1", program_object.get_info_log()));

		return program_object;
	}

	ProgramObject::~ProgramObject()
	{
	}

	void ProgramObject::throw_if_null() const
	{
		if (!impl)
			throw Exception("ProgramObject is null");
	}

	unsigned int ProgramObject::get_handle() const
	{
		return impl->provider->get_handle();
	}

	ProgramObjectProvider *ProgramObject::get_provider() const
	{
		if (!impl)
			return nullptr;
		return impl->provider;
	}

	std::vector<ShaderObject> ProgramObject::get_shaders() const
	{
		return impl->provider->get_shaders();
	}

	std::string ProgramObject::get_info_log() const
	{
		return impl->provider->get_info_log();
	}

	int ProgramObject::get_attribute_location(const std::string &name) const
	{
		int value = impl->provider->get_attribute_location(name);
		//if (value <0)
		//	throw Exception(string_format("Attribute %1 not found", name));
		return value;
	}

	int ProgramObject::get_uniform_location(const std::string &name) const
	{
		int value = impl->provider->get_uniform_location(name);
		//if (value <0)
		//	throw Exception(string_format("Uniform %1 not found", name));
		return value;
	}

	int ProgramObject::get_uniform_buffer_size(const std::string &block_name) const
	{
		int block_index = impl->provider->get_uniform_buffer_index(block_name);
		//if (block_index == -1)
		//	throw Exception(string_format("Uniform block %1 not found", block_name));
		return impl->provider->get_uniform_buffer_size(block_index);
	}

	int ProgramObject::get_uniform_buffer_size(int block_index) const
	{
		return impl->provider->get_uniform_buffer_size(block_index);
	}

	int ProgramObject::get_uniform_buffer_index(const std::string &block_name) const
	{
		return impl->provider->get_uniform_buffer_index(block_name);
	}

	int ProgramObject::get_storage_buffer_index(const std::string &block_name) const
	{
		return impl->provider->get_storage_buffer_index(block_name);
	}

	bool ProgramObject::operator==(const ProgramObject &other) const
	{
		return impl == other.impl;
	}

	void ProgramObject::attach(const ShaderObject &obj)
	{
		if (obj.is_null())
		{
			throw Exception("cannot attach null shader object");
		}

		impl->provider->attach(obj);
	}

	void ProgramObject::detach(const ShaderObject &obj)
	{
		impl->provider->detach(obj);
	}

	void ProgramObject::bind_attribute_location(int index, const std::string &name)
	{
		impl->provider->bind_attribute_location(index, name);
	}

	void ProgramObject::bind_frag_data_location(int color_number, const std::string &name)
	{
		impl->provider->bind_frag_data_location(color_number, name);
	}

	bool ProgramObject::link()
	{
		impl->provider->link();
		return impl->provider->get_link_status();
	}

	bool ProgramObject::validate()
	{
		impl->provider->validate();
		return impl->provider->get_validate_status();
	}

	void ProgramObject::set_uniform1i(int location, int v1)
	{
		impl->provider->set_uniform1i(location, v1);
	}

	void ProgramObject::set_uniform2i(int location, int v1, int v2)
	{
		impl->provider->set_uniform2i(location, v1, v2);
	}

	void ProgramObject::set_uniform3i(int location, int v1, int v2, int v3)
	{
		impl->provider->set_uniform3i(location, v1, v2, v3);
	}

	void ProgramObject::set_uniform4i(int location, int v1, int v2, int v3, int v4)
	{
		impl->provider->set_uniform4i(location, v1, v2, v3, v4);
	}

	void ProgramObject::set_uniformiv(int location, int size, int count, const int *data)
	{
		impl->provider->set_uniformiv(location, size, count, data);
	}

	void ProgramObject::set_uniform1f(int location, float v1)
	{
		impl->provider->set_uniform1f(location, v1);
	}

	void ProgramObject::set_uniform2f(int location, float v1, float v2)
	{
		impl->provider->set_uniform2f(location, v1, v2);
	}

	void ProgramObject::set_uniform3f(int location, float v1, float v2, float v3)
	{
		impl->provider->set_uniform3f(location, v1, v2, v3);
	}

	void ProgramObject::set_uniform4f(int location, float v1, float v2, float v3, float v4)
	{
		impl->provider->set_uniform4f(location, v1, v2, v3, v4);
	}

	void ProgramObject::set_uniformfv(int location, int size, int count, const float *data)
	{
		impl->provider->set_uniformfv(location, size, count, data);
	}

	void ProgramObject::set_uniform_matrix(int location, int size, int count, bool transpose, const float *data)
	{
		impl->provider->set_uniform_matrix(location, size, count, transpose, data);
	}

	void ProgramObject::set_uniform_buffer_index(const std::string &name, int bind_index)
	{
		impl->provider->set_uniform_buffer_index(get_uniform_buffer_index(name), bind_index);
	}

	void ProgramObject::set_uniform_buffer_index(int block_index, int bind_index)
	{
		impl->provider->set_uniform_buffer_index(block_index, bind_index);
	}

	void ProgramObject::set_storage_buffer_index(const std::string &name, int bind_index)
	{
		impl->provider->set_storage_buffer_index(get_storage_buffer_index(name), bind_index);
	}

	void ProgramObject::set_storage_buffer_index(int block_index, int bind_index)
	{
		impl->provider->set_storage_buffer_index(block_index, bind_index);
	}
}
