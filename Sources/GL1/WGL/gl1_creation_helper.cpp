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

#include "GL1/precomp.h"
#include "gl1_creation_helper.h"
#include "../gl1_window_description_impl.h"
#include "API/GL1/opengl1_window_description.h"
#include <commctrl.h>

namespace clan
{

GL1CreationHelper::GL1CreationHelper(HWND window, HDC hdc)
: window(window), hdc(hdc), query_window(0), query_dc(0), query_context(0)
{
	WINDOWINFO window_info;
	memset(&window_info, 0, sizeof(WINDOWINFO));
	window_info.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(window, &window_info);

	query_window = CreateWindowEx(
		0,
		WC_STATIC,
		TEXT(""),
		WS_CHILD,
		window_info.rcWindow.left,
		window_info.rcWindow.top,
		window_info.rcWindow.right - window_info.rcWindow.left,
		window_info.rcWindow.bottom - window_info.rcWindow.top,
		window, 0, GetModuleHandle(0), 0);
	if (query_window == 0)
		throw Exception("Unable to create OpenGL creation query window");
	query_dc = GetDC(query_window);
	if (query_dc == 0)
	{
		DestroyWindow(query_window);
		throw Exception("Unable to retrieve OpenGL creation query device context");
	}

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;

	int pixelformat = ChoosePixelFormat(query_dc, &pfd);
	SetPixelFormat(query_dc, pixelformat, &pfd);

	query_context = wglCreateContext(query_dc);
	if (query_context == 0)
	{
		DeleteDC(query_dc);
		DestroyWindow(query_window);
		throw Exception("Unable to create OpenGL context for creation query window");
	}
}

GL1CreationHelper::~GL1CreationHelper()
{
	wglDeleteContext(query_context);
	DeleteDC(query_dc);
	DestroyWindow(query_window);
}

void GL1CreationHelper::set_multisampling_pixel_format(const GL1WindowDescription &gldesc)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
	pfd.iPixelType = PFD_TYPE_RGBA;
	if (gldesc.get_doublebuffer()) pfd.dwFlags |= PFD_DOUBLEBUFFER;
	if (gldesc.get_stereo()) pfd.dwFlags |= PFD_STEREO;
	pfd.cColorBits = gldesc.get_buffer_size();
	pfd.cRedBits = gldesc.get_red_size();
	pfd.cGreenBits = gldesc.get_green_size();
	pfd.cBlueBits = gldesc.get_blue_size();
	pfd.cAlphaBits = gldesc.get_alpha_size();
	pfd.cDepthBits = gldesc.get_depth_size();
	pfd.cStencilBits = gldesc.get_stencil_size();
	if (gldesc.is_layered())
	{
		pfd.cAlphaBits = 8;
		pfd.dwFlags |= PFD_DOUBLEBUFFER_DONTCARE; // | PFD_DRAW_TO_BITMAP
	}

	if (gldesc.get_multisampling() < 1)
	{
		int pixelformat = ChoosePixelFormat(hdc, &pfd);
		SetPixelFormat(hdc, pixelformat, &pfd);
	}
	else
	{
		int pixelformat = ChoosePixelFormat(hdc, &pfd);

		set_active();
		ptr_wglChoosePixelFormatEXT wglChoosePixelFormatEXT = (ptr_wglChoosePixelFormatEXT) wglGetProcAddress("wglChoosePixelFormatEXT");
		if (wglChoosePixelFormatEXT == 0)
			wglChoosePixelFormatEXT = (ptr_wglChoosePixelFormatEXT) wglGetProcAddress("wglChoosePixelFormatARB");
		reset_active();

		if (wglChoosePixelFormatEXT)
		{
			std::vector<FLOAT> float_attributes;
			std::vector<int> int_attributes;

			int_attributes.push_back(WGL_DRAW_TO_WINDOW);
			int_attributes.push_back(GL_TRUE);
			int_attributes.push_back(WGL_ACCELERATION);
			int_attributes.push_back(WGL_FULL_ACCELERATION);

			if (gldesc.get_doublebuffer())
			{
				int_attributes.push_back(WGL_DOUBLE_BUFFER);
				int_attributes.push_back(GL_TRUE);
			}
			if (gldesc.get_stereo())
			{
				int_attributes.push_back(WGL_STEREO);
				int_attributes.push_back(GL_TRUE);
			}

			int_attributes.push_back(WGL_COLOR_BITS);
			int_attributes.push_back(gldesc.get_red_size() + gldesc.get_green_size() + gldesc.get_blue_size());

			int_attributes.push_back(WGL_ALPHA_BITS);
			int_attributes.push_back(gldesc.get_alpha_size());

			int_attributes.push_back(WGL_DEPTH_BITS);
			int_attributes.push_back(gldesc.get_depth_size());

			int_attributes.push_back(WGL_STENCIL_BITS);
			int_attributes.push_back(gldesc.get_stencil_size());

			int_attributes.push_back(WGL_SAMPLE_BUFFERS);
			int_attributes.push_back(GL_TRUE);
			int_attributes.push_back(WGL_SAMPLES);
			int_attributes.push_back(gldesc.get_multisampling());

			float_attributes.push_back(0.0f);
			float_attributes.push_back(0.0f);
			int_attributes.push_back(0);
			int_attributes.push_back(0);
			int new_pixelformat = pixelformat;
			UINT num_formats = 0;
			BOOL result = wglChoosePixelFormatEXT(hdc, &int_attributes[0], &float_attributes[0], 1, &new_pixelformat, &num_formats);
			if (result == TRUE && num_formats > 0)
				pixelformat = new_pixelformat;
		}

		SetPixelFormat(hdc, pixelformat, &pfd);
	}
}

HGLRC GL1CreationHelper::create_opengl3_context(HGLRC share_context, int major_version, int minor_version)
{
	set_active();
	ptr_wglCreateContextAttribsARB wglCreateContextAttribsARB = (ptr_wglCreateContextAttribsARB) wglGetProcAddress("wglCreateContextAttribsARB");
	reset_active();

	HGLRC opengl3_context = 0;
	if (wglCreateContextAttribsARB)
	{
		std::vector<int> int_attributes;

		int_attributes.push_back(WGL_CONTEXT_MAJOR_VERSION_ARB);
		int_attributes.push_back(major_version);
		int_attributes.push_back(WGL_CONTEXT_MINOR_VERSION_ARB);
		int_attributes.push_back(minor_version);
		//int_attributes.push_back(WGL_CONTEXT_LAYER_PLANE_ARB);
		//int_attributes.push_back(layer_plane);
		//int_attributes.push_back(WGL_CONTEXT_FLAGS_ARB);
		//int_attributes.push_back(flags);
		//int_attributes.push_back(WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB);
		//int_attributes.push_back(forward_compatible_bit);
		int_attributes.push_back(0);

		opengl3_context = wglCreateContextAttribsARB(hdc, share_context, &int_attributes[0]);
	}
	return opengl3_context;
}

HGLRC GL1CreationHelper::create_opengl2_context(HGLRC share_context)
{
	HGLRC opengl2_context = wglCreateContext(hdc);
	if (opengl2_context)
		wglShareLists(share_context, opengl2_context);
	return opengl2_context;
}

void GL1CreationHelper::set_active()
{
	GL1::set_active(0);
	wglMakeCurrent(query_dc, query_context);
}

void GL1CreationHelper::reset_active()
{
	wglMakeCurrent(0, 0);
}

}
