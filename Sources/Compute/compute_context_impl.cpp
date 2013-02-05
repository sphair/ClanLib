/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
*/

#include "Compute/precomp.h"
#include "API/Core/Text/logger.h"
#include "compute_context_impl.h"
#include "opencl_bindings.h"
#include "GL/gl3_graphic_context_provider.h"
#include "D3D/d3d_graphic_context_provider.h"
#include "D3D/d3d_display_window_provider.h"

namespace clan
{

ComputeContext_Impl::ComputeContext_Impl(GraphicContext &gc)
: bindings(0), handle(0), gc(gc)
{
	GL3GraphicContextProvider *gl_provider = dynamic_cast<GL3GraphicContextProvider *>(gc.get_provider());
	D3DGraphicContextProvider *d3d_provider = dynamic_cast<D3DGraphicContextProvider *>(gc.get_provider());

	std::vector<cl_context_properties> properties;

	properties.push_back(CL_CONTEXT_PLATFORM);
	size_t platform_handle_index = properties.size();
	properties.push_back(0);

	if (gl_provider)
	{
		OpenGL::set_active(gc);
		HGLRC opengl_context = wglGetCurrentContext();//gl_provider->get_opengl_context();
		HDC opengl_dc = wglGetCurrentDC();//gl_provider->get_opengl_dc();

		properties.push_back(CL_GL_CONTEXT_KHR);
		properties.push_back((intptr_t)opengl_context);
		properties.push_back(CL_WGL_HDC_KHR);
		properties.push_back((intptr_t)opengl_dc);
	}
	else if (d3d_provider)
	{
		ID3D11Device *device = d3d_provider->get_window()->get_device();
		properties.push_back(CL_CONTEXT_D3D11_DEVICE_KHR);
		properties.push_back((intptr_t)device);
	}
	else
	{
		throw Exception("Unsupported display target");
	}

	properties.push_back(0);
	properties.push_back(0);

	std::map<cl_platform_id, OpenCLPlatformBindings>::iterator it;
	for (it = OpenCLBindings::platform_bindings.begin(); it != OpenCLBindings::platform_bindings.end(); ++it)
	{
		properties[platform_handle_index] = (cl_context_properties)it->first;

		cl_int errorcode = 0;
		handle = it->second.CreateContextFromType(&properties[0], CL_DEVICE_TYPE_GPU, &ComputeContext_Impl::static_error_notify, this, &errorcode);
		if (handle != 0)
		{
			bindings = &it->second;
			break;
		}
	}

	if (handle == 0)
		throw Exception("clCreateContextFromType failed");
}

ComputeContext_Impl::~ComputeContext_Impl()
{
	if (bindings && handle)
		bindings->ReleaseContext(handle);
}

void ComputeContext_Impl::error_notify(const char *errinfo, const void *private_info, size_t private_length)
{
	cl_log_event("OpenCL", errinfo);
}

void ComputeContext_Impl::static_error_notify(const char *errinfo, const void *private_info, size_t private_length, void *user_data)
{
	ComputeContext_Impl *self = reinterpret_cast<ComputeContext_Impl*>(user_data);
	self->error_notify(errinfo, private_info, private_length);
}

}
