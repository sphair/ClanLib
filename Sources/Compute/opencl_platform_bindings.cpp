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
#include "opencl_platform_bindings.h"
#include "opencl_bindings.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Text/string_help.h"

namespace clan
{

void OpenCLPlatformBindings::bind(cl_platform_id platform)
{
	platform_id = platform;
	profile = get_info_string(CL_PLATFORM_PROFILE);
	version = get_info_string(CL_PLATFORM_VERSION);
	name = get_info_string(CL_PLATFORM_NAME);
	vendor = get_info_string(CL_PLATFORM_VENDOR);
	extensions = get_info_string(CL_PLATFORM_EXTENSIONS);

	// CL_PLATFORM_VERSION must have the following format: OpenCL<space><major_version.minor_version><space><platform-specific information>
	std::string::size_type version_dot = version.find_first_of('.');
	std::string::size_type version_space = version.find_first_of(' ', version_dot + 1);
	major_version = StringHelp::text_to_int(version.substr(7, version_dot - 7));
	minor_version = StringHelp::text_to_int(version.substr(version_dot + 1, version_space - version_dot - 1));

	GetDeviceIDs = get_function<FuncGetDeviceIDs*>(1, 0, "clGetDeviceIDs");
	GetDeviceInfo = get_function<FuncGetDeviceInfo*>(1, 0, "clGetDeviceInfo");
	CreateContext = get_function<FuncCreateContext*>(1, 0, "clCreateContext");
	CreateContextFromType = get_function<FuncCreateContextFromType*>(1, 0, "clCreateContextFromType");
	RetainContext = get_function<FuncRetainContext*>(1, 0, "clRetainContext");
	ReleaseContext = get_function<FuncReleaseContext*>(1, 0, "clReleaseContext");
	GetContextInfo = get_function<FuncGetContextInfo*>(1, 0, "clGetContextInfo");
	CreateCommandQueue = get_function<FuncCreateCommandQueue*>(1, 0, "clCreateCommandQueue");
	RetainCommandQueue = get_function<FuncRetainCommandQueue*>(1, 0, "clRetainCommandQueue");
	ReleaseCommandQueue = get_function<FuncReleaseCommandQueue*>(1, 0, "clReleaseCommandQueue");
	GetCommandQueueInfo = get_function<FuncGetCommandQueueInfo*>(1, 0, "clGetCommandQueueInfo");
	CreateBuffer = get_function<FuncCreateBuffer*>(1, 0, "clCreateBuffer");
	RetainMemObject = get_function<FuncRetainMemObject*>(1, 0, "clRetainMemObject");
	ReleaseMemObject = get_function<FuncReleaseMemObject*>(1, 0, "clReleaseMemObject");
	GetSupportedImageFormats = get_function<FuncGetSupportedImageFormats*>(1, 0, "clGetSupportedImageFormats");
	GetMemObjectInfo = get_function<FuncGetMemObjectInfo*>(1, 0, "clGetMemObjectInfo");
	GetImageInfo = get_function<FuncGetImageInfo*>(1, 0, "clGetImageInfo");
	CreateSampler = get_function<FuncCreateSampler*>(1, 0, "clCreateSampler");
	RetainSampler = get_function<FuncRetainSampler*>(1, 0, "clRetainSampler");
	ReleaseSampler = get_function<FuncReleaseSampler*>(1, 0, "clReleaseSampler");
	GetSamplerInfo = get_function<FuncGetSamplerInfo*>(1, 0, "clGetSamplerInfo");
	CreateProgramWithSource = get_function<FuncCreateProgramWithSource*>(1, 0, "clCreateProgramWithSource");
	CreateProgramWithBinary = get_function<FuncCreateProgramWithBinary*>(1, 0, "clCreateProgramWithBinary");
	RetainProgram = get_function<FuncRetainProgram*>(1, 0, "clRetainProgram");
	ReleaseProgram = get_function<FuncReleaseProgram*>(1, 0, "clReleaseProgram");
	BuildProgram = get_function<FuncBuildProgram*>(1, 0, "clBuildProgram");
	GetProgramInfo = get_function<FuncGetProgramInfo*>(1, 0, "clGetProgramInfo");
	GetProgramBuildInfo = get_function<FuncGetProgramBuildInfo*>(1, 0, "clGetProgramBuildInfo");
	CreateKernel = get_function<FuncCreateKernel*>(1, 0, "clCreateKernel");
	CreateKernelsInProgram = get_function<FuncCreateKernelsInProgram*>(1, 0, "clCreateKernelsInProgram");
	RetainKernel = get_function<FuncRetainKernel*>(1, 0, "clRetainKernel");
	ReleaseKernel = get_function<FuncReleaseKernel*>(1, 0, "clReleaseKernel");
	SetKernelArg = get_function<FuncSetKernelArg*>(1, 0, "clSetKernelArg");
	GetKernelInfo = get_function<FuncGetKernelInfo*>(1, 0, "clGetKernelInfo");
	GetKernelWorkGroupInfo = get_function<FuncGetKernelWorkGroupInfo*>(1, 0, "clGetKernelWorkGroupInfo");
	WaitForEvents = get_function<FuncWaitForEvents*>(1, 0, "clWaitForEvents");
	GetEventInfo = get_function<FuncGetEventInfo*>(1, 0, "clGetEventInfo");
	RetainEvent = get_function<FuncRetainEvent*>(1, 0, "clRetainEvent");
	ReleaseEvent = get_function<FuncReleaseEvent*>(1, 0, "clReleaseEvent");
	GetEventProfilingInfo = get_function<FuncGetEventProfilingInfo*>(1, 0, "clGetEventProfilingInfo");
	Flush = get_function<FuncFlush*>(1, 0, "clFlush");
	Finish = get_function<FuncFinish*>(1, 0, "clFinish");
	EnqueueReadBuffer = get_function<FuncEnqueueReadBuffer*>(1, 0, "clEnqueueReadBuffer");
	EnqueueWriteBuffer = get_function<FuncEnqueueWriteBuffer*>(1, 0, "clEnqueueWriteBuffer");
	EnqueueCopyBuffer = get_function<FuncEnqueueCopyBuffer*>(1, 0, "clEnqueueCopyBuffer");
	EnqueueReadImage = get_function<FuncEnqueueReadImage*>(1, 0, "clEnqueueReadImage");
	EnqueueWriteImage = get_function<FuncEnqueueWriteImage*>(1, 0, "clEnqueueWriteImage");
	EnqueueCopyImage = get_function<FuncEnqueueCopyImage*>(1, 0, "clEnqueueCopyImage");
	EnqueueCopyImageToBuffer = get_function<FuncEnqueueCopyImageToBuffer*>(1, 0, "clEnqueueCopyImageToBuffer");
	EnqueueCopyBufferToImage = get_function<FuncEnqueueCopyBufferToImage*>(1, 0, "clEnqueueCopyBufferToImage");
	EnqueueMapBuffer = get_function<FuncEnqueueMapBuffer*>(1, 0, "clEnqueueMapBuffer");
	EnqueueMapImage = get_function<FuncEnqueueMapImage*>(1, 0, "clEnqueueMapImage");
	EnqueueUnmapMemObject = get_function<FuncEnqueueUnmapMemObject*>(1, 0, "clEnqueueUnmapMemObject");
	EnqueueNDRangeKernel = get_function<FuncEnqueueNDRangeKernel*>(1, 0, "clEnqueueNDRangeKernel");
	EnqueueTask = get_function<FuncEnqueueTask*>(1, 0, "clEnqueueTask");
	EnqueueNativeKernel = get_function<FuncEnqueueNativeKernel*>(1, 0, "clEnqueueNativeKernel");
	GetExtensionFunctionAddress = get_function<FuncGetExtensionFunctionAddress*>(1, 0, "clGetExtensionFunctionAddress");

	CreateFromGLBuffer = get_function<FuncCreateFromGLBuffer*>(1, 0, "clCreateFromGLBuffer");
	CreateFromGLRenderbuffer = get_function<FuncCreateFromGLRenderbuffer*>(1, 0, "clCreateFromGLRenderbuffer");
	GetGLObjectInfo = get_function<FuncGetGLObjectInfo*>(1, 0, "clGetGLObjectInfo");
	GetGLTextureInfo = get_function<FuncGetGLTextureInfo*>(1, 0, "clGetGLTextureInfo");
	EnqueueAcquireGLObjects = get_function<FuncEnqueueAcquireGLObjects*>(1, 0, "clEnqueueAcquireGLObjects");
	EnqueueReleaseGLObjects = get_function<FuncEnqueueReleaseGLObjects*>(1, 0, "clEnqueueReleaseGLObjects");
	CreateFromGLTexture2D = get_function<FuncCreateFromGLTexture2D*>(1, 0, "clCreateFromGLTexture2D");
	CreateFromGLTexture3D = get_function<FuncCreateFromGLTexture3D*>(1, 0, "clCreateFromGLTexture3D");

	CreateSubBuffer = get_function<FuncCreateSubBuffer*>(1, 1, "clCreateSubBuffer");
	SetMemObjectDestructorCallback = get_function<FuncSetMemObjectDestructorCallback*>(1, 1, "clSetMemObjectDestructorCallback");
	CreateUserEvent = get_function<FuncCreateUserEvent*>(1, 1, "clCreateUserEvent");
	SetUserEventStatus = get_function<FuncSetUserEventStatus*>(1, 1, "clSetUserEventStatus");
	SetEventCallback = get_function<FuncSetEventCallback*>(1, 1, "clSetEventCallback");
	EnqueueReadBufferRect = get_function<FuncEnqueueReadBufferRect*>(1, 1, "clEnqueueReadBufferRect");
	EnqueueWriteBufferRect = get_function<FuncEnqueueWriteBufferRect*>(1, 1, "clEnqueueWriteBufferRect");
	EnqueueCopyBufferRect = get_function<FuncEnqueueCopyBufferRect*>(1, 1, "clEnqueueCopyBufferRect");

	CreateSubDevices = get_function<FuncCreateSubDevices*>(1, 2, "clCreateSubDevices");
	RetainDevice = get_function<FuncRetainDevice*>(1, 2, "clRetainDevice");
	ReleaseDevice = get_function<FuncReleaseDevice*>(1, 2, "clReleaseDevice");
	CreateImage = get_function<FuncCreateImage*>(1, 2, "clCreateImage");
	CreateProgramWithBuiltInKernels = get_function<FuncCreateProgramWithBuiltInKernels*>(1, 2, "clCreateProgramWithBuiltInKernels");
	CompileProgram = get_function<FuncCompileProgram*>(1, 2, "clCompileProgram");
	LinkProgram = get_function<FuncLinkProgram*>(1, 2, "clLinkProgram");
	UnloadPlatformCompiler = get_function<FuncUnloadPlatformCompiler*>(1, 2, "clUnloadPlatformCompiler");
	GetKernelArgInfo = get_function<FuncGetKernelArgInfo*>(1, 2, "clGetKernelArgInfo");
	EnqueueFillBuffer = get_function<FuncEnqueueFillBuffer*>(1, 2, "clEnqueueFillBuffer");
	EnqueueFillImage = get_function<FuncEnqueueFillImage*>(1, 2, "clEnqueueFillImage");
	EnqueueMigrateMemObjects = get_function<FuncEnqueueMigrateMemObjects*>(1, 2, "clEnqueueMigrateMemObjects");
	EnqueueMarkerWithWaitList = get_function<FuncEnqueueMarkerWithWaitList*>(1, 2, "clEnqueueMarkerWithWaitList");
	EnqueueBarrierWithWaitList = get_function<FuncEnqueueBarrierWithWaitList*>(1, 2, "clEnqueueBarrierWithWaitList");
	SetPrintfCallback = get_function<FuncSetPrintfCallback*>(1, 2, "clSetPrintfCallback");
	GetExtensionFunctionAddressForPlatform = get_function<FuncGetExtensionFunctionAddressForPlatform*>(1, 2, "clGetExtensionFunctionAddressForPlatform");

	CreateFromGLTexture = get_function<FuncCreateFromGLTexture*>(1, 2, "clCreateFromGLTexture");

#ifdef WIN32
	if (extensions.find("cl_khr_d3d11_sharing") != std::string::npos)
	{
		GetDeviceIDsFromD3D11KHR = get_extension_function<clGetDeviceIDsFromD3D11KHR_fn>("clGetDeviceIDsFromD3D11KHR");
		CreateFromD3D11BufferKHR = get_extension_function<clCreateFromD3D11BufferKHR_fn>("clCreateFromD3D11BufferKHR");
		CreateFromD3D11Texture2DKHR = get_extension_function<clCreateFromD3D11Texture2DKHR_fn>("clCreateFromD3D11Texture2DKHR");
		CreateFromD3D11Texture3DKHR = get_extension_function<clCreateFromD3D11Texture3DKHR_fn>("clCreateFromD3D11Texture3DKHR");
		EnqueueAcquireD3D11ObjectsKHR = get_extension_function<clEnqueueAcquireD3D11ObjectsKHR_fn>("clEnqueueAcquireD3D11ObjectsKHR");
		EnqueueReleaseD3D11ObjectsKHR = get_extension_function<clEnqueueReleaseD3D11ObjectsKHR_fn>("clEnqueueReleaseD3D11ObjectsKHR");
	}
	else if (extensions.find("cl_nv_d3d11_sharing") != std::string::npos)
	{
		GetDeviceIDsFromD3D11KHR = get_extension_function<clGetDeviceIDsFromD3D11KHR_fn>("clGetDeviceIDsFromD3D11NV");
		CreateFromD3D11BufferKHR = get_extension_function<clCreateFromD3D11BufferKHR_fn>("clCreateFromD3D11BufferNV");
		CreateFromD3D11Texture2DKHR = get_extension_function<clCreateFromD3D11Texture2DKHR_fn>("clCreateFromD3D11Texture2DNV");
		CreateFromD3D11Texture3DKHR = get_extension_function<clCreateFromD3D11Texture3DKHR_fn>("clCreateFromD3D11Texture3DNV");
		EnqueueAcquireD3D11ObjectsKHR = get_extension_function<clEnqueueAcquireD3D11ObjectsKHR_fn>("clEnqueueAcquireD3D11ObjectsNV");
		EnqueueReleaseD3D11ObjectsKHR = get_extension_function<clEnqueueReleaseD3D11ObjectsKHR_fn>("clEnqueueReleaseD3D11ObjectsNV");
	}
	else
	{
		GetDeviceIDsFromD3D11KHR = 0;
		CreateFromD3D11BufferKHR = 0;
		CreateFromD3D11Texture2DKHR = 0;
		CreateFromD3D11Texture3DKHR = 0;
		EnqueueAcquireD3D11ObjectsKHR = 0;
		EnqueueReleaseD3D11ObjectsKHR = 0;
	}
#endif
}

std::string OpenCLPlatformBindings::get_info_string(cl_platform_info param_name) const
{
	size_t length = 0;
	cl_int result = OpenCLBindings::GetPlatformInfo(platform_id, param_name, 0, 0, &length);
	if (result != CL_SUCCESS)
		throw Exception("clGetPlatformInfo failed");
	if (length == 0)
		return std::string();

	DataBuffer buffer(length);
	result = OpenCLBindings::GetPlatformInfo(platform_id, param_name, buffer.get_size(), buffer.get_data(), &length);
	return buffer.get_data();
}

template<typename FuncType>
FuncType OpenCLPlatformBindings::get_function(int min_major_version, int min_minor_version, const char *name)
{
#ifdef WIN32
	if (major_version > min_major_version || (major_version == min_major_version && minor_version >= min_minor_version))
		return reinterpret_cast<FuncType>(GetProcAddress(OpenCLBindings::handle, name));
	else
		return 0;
#else
	throw Exception("OpenCLPlatformBindings::get_function not implemented yet for this platform");
#endif
}

template<typename FuncType>
FuncType OpenCLPlatformBindings::get_extension_function(const char *name)
{
	if (GetExtensionFunctionAddressForPlatform)
		return reinterpret_cast<FuncType>(GetExtensionFunctionAddressForPlatform(platform_id, name));
	else
		return reinterpret_cast<FuncType>(GetExtensionFunctionAddress(name));
}

}
