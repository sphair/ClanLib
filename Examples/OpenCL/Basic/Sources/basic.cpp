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

#include "precomp.h"
#include "basic.h"

#include <CL/opencl.h>
//#include <CL/cl.hpp>

#pragma comment(lib, "C:\\Program Files (x86)\\AMD APP\\lib\\x86\\OpenCL.lib")

// The start of the Application
int Basic::start(const std::vector<CL_String> &args)
{
	quit = false;

	// Set the window
	CL_DisplayWindowDescription desc;
	desc.set_title("ClanLib OpenCL Basic Example");
	desc.set_size(CL_Size(640, 480), true);
	desc.set_allow_resize(true);

	CL_DisplayWindow window(desc);

	// Connect the Window close event
	CL_Slot slot_quit = window.sig_window_close().connect(this, &Basic::on_window_close);

	// Connect a keyboard handler to on_key_up()
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &Basic::on_input_up);

	// Get the graphic context
	CL_GraphicContext gc = window.get_gc();

	cl_int error;
	cl_platform_id platform;
	cl_uint platforms;
	
	error = clGetPlatformIDs(1, &platform, &platforms);
	if (error != CL_SUCCESS)
		throw CL_Exception("clGetPlatformIDs failed");

	cl_device_id device;
	cl_uint devices;
	error = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, &devices);
	if (error != CL_SUCCESS)
		throw CL_Exception("clGetDeviceIDs failed");
	
	cl_context_properties properties[]={CL_CONTEXT_PLATFORM, (cl_context_properties)platform, 0};
	cl_context context;
	context = clCreateContext(properties, 1, &device, NULL, NULL, &error);
	if (error != CL_SUCCESS)
		throw CL_Exception("clCreateContext failed");

	cl_command_queue cq;
	cq = clCreateCommandQueue(context, device, 0, &error);
	if (error != CL_SUCCESS)
		throw CL_Exception("clCreateCommandQueue failed");

	static const char source_code[] = 
		""
		"__kernel void simple_test(__global float *vals)"
		"{"
		"	const uint i = get_global_id(0);"
		""
		"vals[i] = 1.23;"
		"}"
		"";


	const char *src[] = {source_code};

	cl_program prog;
	prog = clCreateProgramWithSource(context, 1, src, NULL, &error);
	if (error != CL_SUCCESS)
		throw CL_Exception("clCreateProgramWithSource failed");

	error = clBuildProgram(prog, 0, NULL, "", NULL, NULL);
	if (error != CL_SUCCESS)
		throw CL_Exception("clBuildProgram failed");

	cl_kernel k_simple_test;
	k_simple_test = clCreateKernel(prog, "simple_test", &error);
	if (error != CL_SUCCESS)
		throw CL_Exception("clCreateKernel failed");

	static const int buffer_length = 64;
	cl_mem ref_buffer;
	ref_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float) *  buffer_length, NULL, &error);
	if (error != CL_SUCCESS)
		throw CL_Exception("clCreateBuffer failed");

	error = clSetKernelArg(k_simple_test, 0, sizeof(ref_buffer), &ref_buffer);
	if (error != CL_SUCCESS)
		throw CL_Exception("clSetKernelArg failed");

	const size_t worksize = buffer_length;
	error = clEnqueueNDRangeKernel(cq, k_simple_test, 1, NULL, &worksize, NULL, 0, NULL, NULL);
	if (error != CL_SUCCESS)
		throw CL_Exception("clEnqueueNDRangeKernel failed");

	error = clFinish(cq);
	if (error != CL_SUCCESS)
		throw CL_Exception("clFinish failed");

	float ram_buffer[buffer_length];
	error=clEnqueueReadBuffer(cq, ref_buffer, CL_TRUE, 0, sizeof(float) * buffer_length, ram_buffer, 0, NULL, NULL);
	if (error != CL_SUCCESS)
		throw CL_Exception("clEnqueueReadBuffer failed");

	error=clFinish(cq);
	if (error != CL_SUCCESS)
		throw CL_Exception("clFinish failed");

	// Clean up
	clReleaseKernel(k_simple_test);
	clReleaseProgram(prog);
	clReleaseCommandQueue(cq);
	clReleaseContext(context);
	clReleaseMemObject(ref_buffer);

	// Test output
	for (int cnt=0;cnt < buffer_length; cnt++)
	{
		if (ram_buffer[cnt] != 1.23f)
			throw CL_Exception("It did not work!");
	}

	CL_Font font(gc, "tahoma", 24);

	// Run until someone presses escape
	while (!quit)
	{
		gc.clear(CL_Colorf(0.0f,0.0f,0.2f));
		font.draw_text(gc, 32, 32, "Success");
		window.flip(1);
		CL_KeepAlive::process(0);
	}

	return 0;
}

// A key was pressed
void Basic::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}
}

// The window was closed
void Basic::on_window_close()
{
	quit = true;
}


