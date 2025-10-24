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
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#include "Display/precomp.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Display/Render/shader_object.h"
#include "API/Display/Render/program_object.h"
#include "API/Display/Render/program_attribute.h"
#include "API/Display/Render/program_uniform.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/TargetProviders/program_object_provider.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Resources/resource.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/XML/dom_element.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ProgramObject_Impl Class:

class CL_ProgramObject_Impl
{
public:
	CL_ProgramObject_Impl() : provider(0)
	{
	}

	~CL_ProgramObject_Impl()
	{
		if (provider)
			provider->destroy();
	}

	CL_ProgramObjectProvider *provider;
};

/////////////////////////////////////////////////////////////////////////////
// CL_ProgramObject Construction:

CL_ProgramObject::CL_ProgramObject()
{
}

CL_ProgramObject::CL_ProgramObject(CL_GraphicContext &gc)
: impl(new CL_ProgramObject_Impl)
{
	CL_GraphicContextProvider *gc_provider = gc.get_provider();

	impl->provider = gc_provider->alloc_program_object();
}

CL_ProgramObject::CL_ProgramObject(CL_GraphicContextProvider *gc_provider)
: impl(new CL_ProgramObject_Impl)
{
	impl->provider = gc_provider->alloc_program_object();
}

CL_ProgramObject::CL_ProgramObject(CL_ProgramObjectProvider *provider)
: impl(new CL_ProgramObject_Impl)
{
	impl->provider = provider;
}

CL_ProgramObject CL_ProgramObject::load(
	CL_GraphicContext &gc,
	const CL_StringRef &resource_id,
	CL_ResourceManager *resources)
{
	CL_ProgramObject program_object(gc);

	CL_Resource resource = resources->get_resource(resource_id);

	CL_DomNode node = resource.get_element().get_first_child();

	while (!node.is_null())
	{
		if (node.is_element())
		{
			CL_DomElement element = node.to_element();
			if (element.get_tag_name() == "shader")
			{
				CL_ShaderObject shader = CL_ShaderObject::load(gc, element.get_attribute("name"), resources);
				program_object.attach(shader);
			}
			else if (element.get_tag_name() == "bind-attribute")
			{
				program_object.bind_attribute_location(
					CL_StringHelp::text_to_int(element.get_attribute("index")),
					element.get_attribute("name"));
			}
		}
		node = node.get_next_sibling();
	}
	
	if (!resource.get_element().get_attribute("shader").empty())
	{
		CL_ShaderObject shader = CL_ShaderObject::load(gc, resource.get_element().get_attribute("shader"), resources);
		program_object.attach(shader);
	}
	
	if (resource.get_element().get_attribute("link", "true") == "true")
		if(!program_object.link())
			throw CL_Exception(cl_format("Unable to link program object: %1", program_object.get_info_log()));

	return program_object;
}

CL_ProgramObject CL_ProgramObject::load(CL_GraphicContext &gc, const CL_StringRef &vertex_filename, const CL_StringRef &fragment_filename, const CL_VirtualDirectory &directory)
{
	CL_ProgramObject program_object(gc);

	CL_ShaderObject vertex_shader = CL_ShaderObject::load_and_compile(gc, cl_shadertype_vertex, vertex_filename, directory);
	program_object.attach(vertex_shader);

	CL_ShaderObject fragment_shader = CL_ShaderObject::load_and_compile(gc, cl_shadertype_fragment, fragment_filename, directory);
	program_object.attach(fragment_shader);

	return program_object;
}

CL_ProgramObject CL_ProgramObject::load(CL_GraphicContext &gc, const CL_StringRef &vertex_filename, const CL_StringRef &geometry_filename, const CL_StringRef &fragment_filename, const CL_VirtualDirectory &directory)
{
	CL_ProgramObject program_object(gc);

	CL_ShaderObject vertex_shader = CL_ShaderObject::load_and_compile(gc, cl_shadertype_vertex, vertex_filename, directory);
	program_object.attach(vertex_shader);

	CL_ShaderObject geometry_shader = CL_ShaderObject::load_and_compile(gc, cl_shadertype_geometry, geometry_filename, directory);
	program_object.attach(geometry_shader);

	CL_ShaderObject fragment_shader = CL_ShaderObject::load_and_compile(gc, cl_shadertype_fragment, fragment_filename, directory);
	program_object.attach(fragment_shader);

	return program_object;
}

CL_ProgramObject CL_ProgramObject::load(CL_GraphicContext &gc, const CL_StringRef &vertex_fullname, const CL_StringRef &fragment_fullname)
{
	CL_String path_vertex = CL_PathHelp::get_fullpath(vertex_fullname, CL_PathHelp::path_type_file);
	CL_String filename_vertex = CL_PathHelp::get_filename(vertex_fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs_vertex(path_vertex);
	CL_String path_fragment = CL_PathHelp::get_fullpath(fragment_fullname, CL_PathHelp::path_type_file);
	CL_String filename_fragment = CL_PathHelp::get_filename(fragment_fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs_fragment(path_fragment);

	CL_ProgramObject program_object(gc);

	CL_ShaderObject vertex_shader = CL_ShaderObject::load_and_compile(gc, cl_shadertype_vertex, filename_vertex, vfs_vertex.get_root_directory());
	program_object.attach(vertex_shader);

	CL_ShaderObject fragment_shader = CL_ShaderObject::load_and_compile(gc, cl_shadertype_fragment, filename_fragment, vfs_fragment.get_root_directory());
	program_object.attach(fragment_shader);

	return program_object;
}

CL_ProgramObject CL_ProgramObject::load(CL_GraphicContext &gc, const CL_StringRef &vertex_fullname, const CL_StringRef &geometry_fullname, const CL_StringRef &fragment_fullname)
{
	CL_String path_vertex = CL_PathHelp::get_fullpath(vertex_fullname, CL_PathHelp::path_type_file);
	CL_String filename_vertex = CL_PathHelp::get_filename(vertex_fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs_vertex(path_vertex);
	CL_String path_geometry = CL_PathHelp::get_fullpath(geometry_fullname, CL_PathHelp::path_type_file);
	CL_String filename_geometry = CL_PathHelp::get_filename(geometry_fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs_geometry(path_geometry);
	CL_String path_fragment = CL_PathHelp::get_fullpath(fragment_fullname, CL_PathHelp::path_type_file);
	CL_String filename_fragment = CL_PathHelp::get_filename(fragment_fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs_fragment(path_fragment);

	CL_ProgramObject program_object(gc);

	CL_ShaderObject vertex_shader = CL_ShaderObject::load_and_compile(gc, cl_shadertype_vertex, filename_vertex, vfs_vertex.get_root_directory());
	program_object.attach(vertex_shader);

	CL_ShaderObject geometry_shader = CL_ShaderObject::load_and_compile(gc, cl_shadertype_geometry, filename_geometry, vfs_geometry.get_root_directory());
	program_object.attach(geometry_shader);

	CL_ShaderObject fragment_shader = CL_ShaderObject::load_and_compile(gc, cl_shadertype_fragment, filename_fragment, vfs_fragment.get_root_directory());
	program_object.attach(fragment_shader);

	return program_object;
}

CL_ProgramObject CL_ProgramObject::load(CL_GraphicContext &gc, CL_IODevice &vertex_file, CL_IODevice &fragment_file)
{
	CL_ProgramObject program_object(gc);

	CL_ShaderObject vertex_shader = CL_ShaderObject::load_and_compile(gc, cl_shadertype_vertex, vertex_file);
	program_object.attach(vertex_shader);

	CL_ShaderObject fragment_shader = CL_ShaderObject::load_and_compile(gc, cl_shadertype_fragment, fragment_file);
	program_object.attach(fragment_shader);

	return program_object;
}

CL_ProgramObject CL_ProgramObject::load(CL_GraphicContext &gc, CL_IODevice &vertex_file, CL_IODevice &geometry_file, CL_IODevice &fragment_file)
{
	CL_ProgramObject program_object(gc);

	CL_ShaderObject vertex_shader = CL_ShaderObject::load_and_compile(gc, cl_shadertype_vertex, vertex_file);
	program_object.attach(vertex_shader);

	CL_ShaderObject geometry_shader = CL_ShaderObject::load_and_compile(gc, cl_shadertype_geometry, geometry_file);
	program_object.attach(geometry_shader);

	CL_ShaderObject fragment_shader = CL_ShaderObject::load_and_compile(gc, cl_shadertype_fragment, fragment_file);
	program_object.attach(fragment_shader);

	return program_object;
}

CL_ProgramObject CL_ProgramObject::load(CL_GraphicContext &gc, const CL_StringRef &fragment_filename, const CL_VirtualDirectory &directory)
{
	CL_ProgramObject program_object(gc);

	CL_ShaderObject fragment_shader = CL_ShaderObject::load_and_compile(gc, cl_shadertype_fragment, fragment_filename, directory);
	program_object.attach(fragment_shader);

	return program_object;
}

CL_ProgramObject CL_ProgramObject::load(CL_GraphicContext &gc, const CL_StringRef &fragment_fullname)
{
	CL_String path_fragment = CL_PathHelp::get_fullpath(fragment_fullname, CL_PathHelp::path_type_file);
	CL_String filename_fragment = CL_PathHelp::get_filename(fragment_fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs_fragment(path_fragment);

	CL_ProgramObject program_object(gc);

	CL_ShaderObject fragment_shader = CL_ShaderObject::load_and_compile(gc, cl_shadertype_fragment, filename_fragment, vfs_fragment.get_root_directory());
	program_object.attach(fragment_shader);

	return program_object;
}

CL_ProgramObject CL_ProgramObject::load(CL_GraphicContext &gc, CL_IODevice &fragment_file)
{
	CL_ProgramObject program_object(gc);

	CL_ShaderObject fragment_shader = CL_ShaderObject::load_and_compile(gc, cl_shadertype_fragment, fragment_file);
	program_object.attach(fragment_shader);

	return program_object;
}


CL_ProgramObject CL_ProgramObject::load_and_link(CL_GraphicContext &gc, const CL_StringRef &vertex_filename, const CL_StringRef &fragment_filename, const CL_VirtualDirectory &directory)
{
	CL_ProgramObject program_object = CL_ProgramObject::load(gc, vertex_filename, fragment_filename, directory);

	if(!program_object.link())
		throw CL_Exception(cl_format("Unable to link program object: %1", program_object.get_info_log()));

	return program_object;
}

CL_ProgramObject CL_ProgramObject::load_and_link(CL_GraphicContext &gc, const CL_StringRef &vertex_filename, const CL_StringRef &geometry_filename, const CL_StringRef &fragment_filename, const CL_VirtualDirectory &directory)
{
	CL_ProgramObject program_object = CL_ProgramObject::load(gc, vertex_filename, geometry_filename, fragment_filename, directory);

	if(!program_object.link())
		throw CL_Exception(cl_format("Unable to link program object: %1", program_object.get_info_log()));

	return program_object;
}

CL_ProgramObject CL_ProgramObject::load_and_link(CL_GraphicContext &gc, const CL_StringRef &vertex_fullname, const CL_StringRef &fragment_fullname)
{
	CL_ProgramObject program_object = CL_ProgramObject::load(gc, vertex_fullname, fragment_fullname);

	if(!program_object.link())
		throw CL_Exception(cl_format("Unable to link program object: %1", program_object.get_info_log()));

	return program_object;
}

CL_ProgramObject CL_ProgramObject::load_and_link(CL_GraphicContext &gc, const CL_StringRef &vertex_fullname, const CL_StringRef &geometry_fullname, const CL_StringRef &fragment_fullname)
{
	CL_ProgramObject program_object = CL_ProgramObject::load(gc, vertex_fullname, geometry_fullname, fragment_fullname);

	if(!program_object.link())
		throw CL_Exception(cl_format("Unable to link program object: %1", program_object.get_info_log()));

	return program_object;
}

CL_ProgramObject CL_ProgramObject::load_and_link(CL_GraphicContext &gc, CL_IODevice &vertex_file, CL_IODevice &fragment_file)
{
	CL_ProgramObject program_object = CL_ProgramObject::load(gc, vertex_file, fragment_file);

	if(!program_object.link())
		throw CL_Exception(cl_format("Unable to link program object: %1", program_object.get_info_log()));

	return program_object;
}

CL_ProgramObject CL_ProgramObject::load_and_link(CL_GraphicContext &gc, CL_IODevice &vertex_file, CL_IODevice &geometry_file, CL_IODevice &fragment_file)
{
	CL_ProgramObject program_object = CL_ProgramObject::load(gc, vertex_file, geometry_file, fragment_file);

	if(!program_object.link())
		throw CL_Exception(cl_format("Unable to link program object: %1", program_object.get_info_log()));

	return program_object;
}
CL_ProgramObject CL_ProgramObject::load_and_link(CL_GraphicContext &gc, const CL_StringRef &fragment_filename, const CL_VirtualDirectory &directory)
{
	CL_ProgramObject program_object = CL_ProgramObject::load(gc, fragment_filename, directory);

	if(!program_object.link())
		throw CL_Exception(cl_format("Unable to link program object: %1", program_object.get_info_log()));

	return program_object;
}

CL_ProgramObject CL_ProgramObject::load_and_link(CL_GraphicContext &gc, const CL_StringRef &fragment_fullname)
{
	CL_ProgramObject program_object = CL_ProgramObject::load(gc, fragment_fullname);

	if(!program_object.link())
		throw CL_Exception(cl_format("Unable to link program object: %1", program_object.get_info_log()));

	return program_object;
}

CL_ProgramObject CL_ProgramObject::load_and_link(CL_GraphicContext &gc, CL_IODevice &fragment_file)
{
	CL_ProgramObject program_object = CL_ProgramObject::load(gc, fragment_file);

	if(!program_object.link())
		throw CL_Exception(cl_format("Unable to link program object: %1", program_object.get_info_log()));

	return program_object;
}

CL_ProgramObject::~CL_ProgramObject()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ProgramObject Attributes:

void CL_ProgramObject::throw_if_null() const
{
	if (impl.is_null())
		throw CL_Exception("CL_ProgramObject is null");
}

unsigned int CL_ProgramObject::get_handle() const
{
	return impl->provider->get_handle();
}

CL_ProgramObjectProvider *CL_ProgramObject::get_provider() const
{
	if (impl.is_null())
		return 0;
	return impl->provider;
}

std::vector<CL_ShaderObject> CL_ProgramObject::get_shaders() const
{
	return impl->provider->get_shaders();
}

CL_String CL_ProgramObject::get_info_log() const
{
	return impl->provider->get_info_log();
}

int CL_ProgramObject::get_attribute_count() const
{
	return impl->provider->get_attribute_count();
}

std::vector<CL_ProgramAttribute> CL_ProgramObject::get_attributes() const
{
	return impl->provider->get_attributes();
}

int CL_ProgramObject::get_attribute_location(const CL_StringRef &name) const
{
	return impl->provider->get_attribute_location(name);
}

int CL_ProgramObject::get_uniform_count() const
{
	return impl->provider->get_uniform_count();	
}

std::vector<CL_ProgramUniform> CL_ProgramObject::get_uniforms() const
{
	return impl->provider->get_uniforms();
}

int CL_ProgramObject::get_uniform_location(const CL_StringRef &name) const
{
	return impl->provider->get_uniform_location(name);
}

/////////////////////////////////////////////////////////////////////////////
// CL_ProgramObject Operations:

void CL_ProgramObject::attach(const CL_ShaderObject &obj)
{
	if (obj.is_null())
	{
		throw CL_Exception("cannot attach null shader object");
	}

	impl->provider->attach(obj);
}

void CL_ProgramObject::detach(const CL_ShaderObject &obj)
{
	impl->provider->attach(obj);
}

void CL_ProgramObject::bind_attribute_location(int index, const CL_StringRef &name)
{
	impl->provider->bind_attribute_location(index, name);
}

bool CL_ProgramObject::link()
{
	impl->provider->link();
	return impl->provider->get_link_status();
}

bool CL_ProgramObject::validate()
{
	impl->provider->validate();
	return impl->provider->get_validate_status();
}

void CL_ProgramObject::set_uniform1i(const CL_StringRef &name, int v1)
{
	impl->provider->set_uniform1i(name, v1);
}

void CL_ProgramObject::set_uniform2i(const CL_StringRef &name, int v1, int v2)
{
	impl->provider->set_uniform2i(name, v1, v2);
}

void CL_ProgramObject::set_uniform3i(const CL_StringRef &name, int v1, int v2, int v3)
{
	impl->provider->set_uniform3i(name, v1, v2, v3);
}

void CL_ProgramObject::set_uniform4i(const CL_StringRef &name, int v1, int v2, int v3, int v4)
{
	impl->provider->set_uniform4i(name, v1, v2, v3, v4);
}

void CL_ProgramObject::set_uniformiv(const CL_StringRef &name, int size, int count, int *data)
{
	impl->provider->set_uniformiv(name, size, count, data);
}

void CL_ProgramObject::set_uniform1f(const CL_StringRef &name, float v1)
{
	impl->provider->set_uniform1f(name, v1);
}

void CL_ProgramObject::set_uniform2f(const CL_StringRef &name, float v1, float v2)
{
	impl->provider->set_uniform2f(name, v1, v2);
}

void CL_ProgramObject::set_uniform3f(const CL_StringRef &name, float v1, float v2, float v3)
{
	impl->provider->set_uniform3f(name, v1, v2, v3);
}

void CL_ProgramObject::set_uniform4f(const CL_StringRef &name, float v1, float v2, float v3, float v4)
{
	impl->provider->set_uniform4f(name, v1, v2, v3, v4);
}

void CL_ProgramObject::set_uniformfv(const CL_StringRef &name, int size, int count, float *data)
{
	impl->provider->set_uniformfv(name, size, count, data);
}

void CL_ProgramObject::set_uniform_matrix(const CL_StringRef &name, int size, int count, bool transpose, float *data)
{
	impl->provider->set_uniform_matrix(name, size, count, transpose, data);
}

/////////////////////////////////////////////////////////////////////////////
// CL_ProgramObject Implementation:
