/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Display/Render/shader_object.h"
#include "API/Display/TargetProviders/shader_object_provider.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"

namespace clan
{
	class ShaderObject_Impl
	{
	public:
		ShaderObject_Impl() : provider(nullptr)
		{
		}

		~ShaderObject_Impl()
		{
			if (provider)
				delete provider;
		}

		ShaderObjectProvider *provider;
	};

	ShaderObject::ShaderObject()
	{
	}

	ShaderObject::ShaderObject(GraphicContext &gc, ShaderType type, const void *source, int source_size)
		: impl(std::make_shared<ShaderObject_Impl>())
	{
		GraphicContextProvider *gc_provider = gc.get_provider();
		impl->provider = gc_provider->alloc_shader_object();
		impl->provider->create(type, source, source_size);
	}

	ShaderObject::ShaderObject(GraphicContext &gc, ShaderType type, const std::string &source)
		: impl(std::make_shared<ShaderObject_Impl>())
	{
		GraphicContextProvider *gc_provider = gc.get_provider();
		impl->provider = gc_provider->alloc_shader_object();
		impl->provider->create(type, source);
	}


	ShaderObject::ShaderObject(GraphicContext &gc, ShaderType type, const std::vector<std::string> &sources)
		: impl(std::make_shared<ShaderObject_Impl>())
	{
		GraphicContextProvider *gc_provider = gc.get_provider();
		impl->provider = gc_provider->alloc_shader_object();
		impl->provider->create(type, sources);
	}

	ShaderObject::ShaderObject(GraphicContextProvider *gc_provider, ShaderType type, const std::string &source)
		: impl(std::make_shared<ShaderObject_Impl>())
	{
		impl->provider = gc_provider->alloc_shader_object();
		impl->provider->create(type, source);
	}

	ShaderObject::ShaderObject(GraphicContextProvider *gc_provider, ShaderType type, const std::vector<std::string> &sources)
		: impl(std::make_shared<ShaderObject_Impl>())
	{
		impl->provider = gc_provider->alloc_shader_object();
		impl->provider->create(type, sources);
	}

	ShaderObject ShaderObject::load(GraphicContext &gc, ShaderType shader_type, const std::string &filename, const FileSystem &fs)
	{
		IODevice file = fs.open_file(filename);
		return ShaderObject::load(gc, shader_type, file);
	}

	ShaderObject ShaderObject::load(GraphicContext &gc, ShaderType shader_type, IODevice &file)
	{
		int size = file.get_size();
		std::string source(size, 0);
		file.read(&source[0], size);

		return ShaderObject(gc, shader_type, source);
	}

	ShaderObject ShaderObject::load(GraphicContext &gc, ShaderType shader_type, const std::string &fullname)
	{
		std::string path = PathHelp::get_fullpath(fullname, PathHelp::path_type_file);
		std::string filename = PathHelp::get_filename(fullname, PathHelp::path_type_file);
		FileSystem vfs(path);
		return ShaderObject::load(gc, shader_type, filename, vfs);
	}

	ShaderObject ShaderObject::load_and_compile(GraphicContext &gc, ShaderType shader_type, const std::string &filename, const FileSystem &fs)
	{
		ShaderObject shader_object = ShaderObject::load(gc, shader_type, filename, fs);

		if (!shader_object.compile())
			throw Exception(string_format("Unable to compile shader program %1: %2", filename, shader_object.get_info_log()));

		return shader_object;
	}

	ShaderObject ShaderObject::load_and_compile(GraphicContext &gc, ShaderType shader_type, IODevice &file)
	{
		ShaderObject shader_object = ShaderObject::load(gc, shader_type, file);

		if (!shader_object.compile())
			throw Exception(string_format("Unable to compile shader program : %1", shader_object.get_info_log()));

		return shader_object;
	}

	ShaderObject ShaderObject::load_and_compile(GraphicContext &gc, ShaderType shader_type, const std::string &fullname)
	{
		std::string path = PathHelp::get_fullpath(fullname, PathHelp::path_type_file);
		std::string filename = PathHelp::get_filename(fullname, PathHelp::path_type_file);
		FileSystem vfs(path);
		return ShaderObject::load_and_compile(gc, shader_type, filename, vfs);
	}

	ShaderObject::~ShaderObject()
	{
	}

	unsigned int ShaderObject::get_handle() const
	{
		return impl->provider->get_handle();
	}

	ShaderType ShaderObject::get_shader_type() const
	{
		return impl->provider->get_shader_type();
	}

	std::string ShaderObject::get_info_log() const
	{
		return impl->provider->get_info_log();
	}

	std::string ShaderObject::get_shader_source() const
	{
		return impl->provider->get_shader_source();
	}

	void ShaderObject::throw_if_null() const
	{
		if (!impl)
			throw Exception("ShaderObject is null");
	}

	ShaderObjectProvider *ShaderObject::get_provider() const
	{
		return impl->provider;
	}

	bool ShaderObject::operator==(const ShaderObject &other) const
	{
		return impl == other.impl;
	}

	bool ShaderObject::compile()
	{
		impl->provider->compile();
		return impl->provider->get_compile_status();
	}
}
