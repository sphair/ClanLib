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
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#include "Display/precomp.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/Text/string_types.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Resources/resource.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/XML/dom_element.h"
#include "API/Display/Render/shader_object.h"
#include "API/Display/TargetProviders/shader_object_provider.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ShaderObject_Impl Class:

class CL_ShaderObject_Impl
{
public:
	CL_ShaderObject_Impl() : provider(0)
	{
	}

	~CL_ShaderObject_Impl()
	{
		if (provider)
			provider->destroy();
	}

	CL_ShaderObjectProvider *provider;
};

/////////////////////////////////////////////////////////////////////////////
// CL_ShaderObject Construction:

CL_ShaderObject::CL_ShaderObject()
{
	// empty
}

CL_ShaderObject::CL_ShaderObject(CL_GraphicContext &gc, CL_ShaderType type, const CL_StringRef &source)
: impl(new CL_ShaderObject_Impl)
{
	CL_GraphicContextProvider *gc_provider = gc.get_provider();
	impl->provider = gc_provider->alloc_shader_object();
	impl->provider->create(type, source);
}

CL_ShaderObject::CL_ShaderObject(CL_GraphicContext &gc, CL_ShaderType type, const std::vector<CL_StringRef> &sources)
: impl(new CL_ShaderObject_Impl)
{
	CL_GraphicContextProvider *gc_provider = gc.get_provider();
	impl->provider = gc_provider->alloc_shader_object();
	impl->provider->create(type, sources);
}

CL_ShaderObject::CL_ShaderObject(CL_GraphicContextProvider *gc_provider, CL_ShaderType type, const CL_StringRef &source)
: impl(new CL_ShaderObject_Impl)
{
	impl->provider = gc_provider->alloc_shader_object();
	impl->provider->create(type, source);
}

CL_ShaderObject::CL_ShaderObject(CL_GraphicContextProvider *gc_provider, CL_ShaderType type, const std::vector<CL_StringRef> &sources)
: impl(new CL_ShaderObject_Impl)
{
	impl->provider = gc_provider->alloc_shader_object();
	impl->provider->create(type, sources);
}

CL_ShaderObject CL_ShaderObject::load(CL_GraphicContext &gc, const CL_StringRef &resource_id, CL_ResourceManager *resources)
{
	CL_Resource resource = resources->get_resource(resource_id);
	CL_String filename = resource.get_element().get_attribute("file");
	CL_String type = resource.get_element().get_tag_name();
	
	CL_ShaderType shader_type;
	if (type == "fragment-shader")
		shader_type = cl_shadertype_fragment;
	else if (type == "vertex-shader")
		shader_type = cl_shadertype_vertex;
	else
		throw CL_Exception("CL_ShaderObject: Unknown shader type: " + type);

	CL_VirtualDirectory directory = resources->get_directory(resource);

	CL_IODevice file = directory.open_file(filename, CL_File::open_existing, CL_File::access_read, CL_File::share_read);
	int size = file.get_size();
	CL_String8 source(size, 0);
	file.read(&source[0], size);

	CL_ShaderObject shader_object(gc, shader_type, CL_StringHelp::local8_to_text(source));

	if (resource.get_element().get_attribute("compile", "true") == "true")
		if(!shader_object.compile())
			throw CL_Exception(cl_format("Unable to compile shader program %1: %2", resource_id, shader_object.get_info_log()));

	return shader_object;
}

CL_ShaderObject CL_ShaderObject::load(CL_GraphicContext &gc, CL_ShaderType shader_type, const CL_StringRef &filename, const CL_VirtualDirectory &directory)
{
	CL_IODevice file = directory.open_file_read(filename);
	return CL_ShaderObject::load(gc, shader_type, file);
}

CL_ShaderObject CL_ShaderObject::load(CL_GraphicContext &gc, CL_ShaderType shader_type, CL_IODevice &file)
{
	int size = file.get_size();
	CL_String8 source(size, 0);
	file.read(&source[0], size);

	return CL_ShaderObject(gc, shader_type, CL_StringHelp::local8_to_text(source));
}

CL_ShaderObject CL_ShaderObject::load(CL_GraphicContext &gc, CL_ShaderType shader_type, const CL_StringRef &fullname)
{
	CL_String path = CL_PathHelp::get_fullpath(fullname, CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs(path);
	return CL_ShaderObject::load(gc, shader_type, filename, vfs.get_root_directory());
}

CL_ShaderObject CL_ShaderObject::load_and_compile(CL_GraphicContext &gc, CL_ShaderType shader_type, const CL_StringRef &filename, const CL_VirtualDirectory &directory)
{
	CL_ShaderObject shader_object = CL_ShaderObject::load(gc, shader_type, filename, directory);

	if(!shader_object.compile())
		throw CL_Exception(cl_format("Unable to compile shader program %1: %2", filename, shader_object.get_info_log()));

	return shader_object;
}

CL_ShaderObject CL_ShaderObject::load_and_compile(CL_GraphicContext &gc, CL_ShaderType shader_type, CL_IODevice &file)
{
	CL_ShaderObject shader_object = CL_ShaderObject::load(gc, shader_type, file);

	if(!shader_object.compile())
		throw CL_Exception(cl_format("Unable to compile shader program : %1", shader_object.get_info_log()));

	return shader_object;
}

CL_ShaderObject CL_ShaderObject::load_and_compile(CL_GraphicContext &gc, CL_ShaderType shader_type, const CL_StringRef &fullname)
{
	CL_String path = CL_PathHelp::get_fullpath(fullname, CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs(path);
	return CL_ShaderObject::load_and_compile(gc, shader_type, filename, vfs.get_root_directory());
}

CL_ShaderObject::~CL_ShaderObject()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ShaderObject Attributes:

unsigned int CL_ShaderObject::get_handle() const
{
	return impl->provider->get_handle();
}

CL_ShaderType CL_ShaderObject::get_shader_type() const
{
	return impl->provider->get_shader_type();
}

CL_String CL_ShaderObject::get_info_log() const
{
	return impl->provider->get_info_log();
}

CL_String CL_ShaderObject::get_shader_source() const
{
	return impl->provider->get_shader_source();
}

void CL_ShaderObject::throw_if_null() const
{
	if (!impl)
		throw CL_Exception("CL_ShaderObject is null");
}

CL_ShaderObjectProvider *CL_ShaderObject::get_provider() const
{
	return impl->provider;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ShaderObject Operations:

bool CL_ShaderObject::operator==(const CL_ShaderObject &other) const
{
	return impl == other.impl;
}

bool CL_ShaderObject::compile()
{
	impl->provider->compile();
	return impl->provider->get_compile_status();
}

/////////////////////////////////////////////////////////////////////////////
// CL_ShaderObject Implementation:
