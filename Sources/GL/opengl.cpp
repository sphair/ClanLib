/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "Display/display_precomp.h"
#include "API/GL/opengl.h"
#include "API/GL/opengl_wrap.h"
#include "API/Core/System/error.h"
#include "API/Core/IOData/cl_endian.h"
#include "API/core.h"
#include <cstring>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include <map>

#ifndef WIN32
#ifdef __APPLE__
#include <AGL/agl.h>
#include <Carbon/Carbon.h>
#else
#define GLX_GLXEXT_PROTOTYPES
#include <GL/glx.h>
#endif
#endif

std::vector<std::string> CL_OpenGL::m_ignored_gl_extension;

CL_ProcAddress *CL_OpenGL::get_proc_address(const std::string& function_name)
{
#ifdef WIN32
	return (void (*)())wglGetProcAddress(function_name.c_str());
#else
#ifdef __APPLE__
	// Mac OS X don't have an OpenGL extension fetch function. Isn't that silly?

	static CFBundleRef cl_gBundleRefOpenGL = 0;
	if (cl_gBundleRefOpenGL == 0)
	{
		cl_gBundleRefOpenGL = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.opengl"));
		if (cl_gBundleRefOpenGL == 0)
			throw CL_Error("Unable to find com.apple.opengl bundle");
	}

	return (CL_ProcAddress *) CFBundleGetFunctionPointerForName(
		cl_gBundleRefOpenGL,
		CFStringCreateWithCStringNoCopy(
			0,
			function_name.c_str(),
			CFStringGetSystemEncoding(),
			0));
#else
#  ifdef HAVE_GLX_GETPROCADDRESSARB
	return glXGetProcAddressARB((GLubyte*)function_name.c_str());
#  else
	return glXGetProcAddress((GLubyte*)function_name.c_str());
#  endif
#endif
#endif
}

CL_GLFunctions *CL_OpenGL::functions = 0;

bool CL_OpenGL::from_opengl_pixelformat(CLenum format, CLenum type, CL_PixelFormat &pf)
{
	int bits_per_component;
	if (type == CL_UNSIGNED_BYTE || type == CL_BYTE)
		bits_per_component = 8;
	else if (type == CL_SHORT || type == CL_UNSIGNED_SHORT)
		bits_per_component = 16;
	else if (type == CL_INT || type == CL_UNSIGNED_INT || type == CL_FLOAT)
		bits_per_component = 32;
	else
		return false;

	bool big = CL_Endian::is_system_big();

	if (format == CL_COLOR_INDEX)
	{
		pf.set_type(pixelformat_index);
		pf.set_depth(bits_per_component);
	}
	else if (format == CL_RED)
	{
		pf.set_depth(bits_per_component);
		pf.set_red_mask(CL_PixelFormat::get_bitmask(bits_per_component, 0));
	}
	else if (format == CL_GREEN)
	{
		pf.set_depth(bits_per_component);
		pf.set_green_mask(CL_PixelFormat::get_bitmask(bits_per_component, 0));
	}
	else if (format == CL_BLUE)
	{
		pf.set_depth(bits_per_component);
		pf.set_blue_mask(CL_PixelFormat::get_bitmask(bits_per_component, 0));
	}
	else if (format == CL_ALPHA)
	{
		pf.set_depth(bits_per_component);
		pf.set_alpha_mask(CL_PixelFormat::get_bitmask(bits_per_component, 0));
	}
	else if (format == CL_RGB)
	{
		if (bits_per_component * 3 > 32)
			return false;
		pf.set_depth(bits_per_component * 3);
		if (!big)
		{
			pf.set_red_mask(CL_PixelFormat::get_bitmask(bits_per_component, 0));
			pf.set_green_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component));
			pf.set_blue_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component * 2));
		}
		else
		{
			pf.set_red_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component * 2));
			pf.set_green_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component));
			pf.set_blue_mask(CL_PixelFormat::get_bitmask(bits_per_component, 0));
		}
	}
	else if (format == CL_RGBA)
	{
		if (bits_per_component * 4 > 32)
			return false;
		pf.set_depth(bits_per_component * 4);
		if (!big)
		{
			pf.set_red_mask(CL_PixelFormat::get_bitmask(bits_per_component, 0));
			pf.set_green_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component));
			pf.set_blue_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component * 2));
			pf.set_alpha_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component * 3));
		}
		else
		{
			pf.set_red_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component * 3));
			pf.set_green_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component * 2));
			pf.set_blue_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component));
			pf.set_alpha_mask(CL_PixelFormat::get_bitmask(bits_per_component, 0));
		}
	}
	else if (format == CL_BGR)
	{
		if (bits_per_component * 3 > 32)
			return false;
		pf.set_depth(bits_per_component * 3);
		if (!big)
		{
			pf.set_blue_mask(CL_PixelFormat::get_bitmask(bits_per_component, 0));
			pf.set_green_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component));
			pf.set_red_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component * 2));
		}
		else
		{
			pf.set_blue_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component * 2));
			pf.set_green_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component));
			pf.set_red_mask(CL_PixelFormat::get_bitmask(bits_per_component, 0));
		}
	}
	else if (format == CL_BGRA)
	{
		if (bits_per_component * 4 > 32)
			return false;
		pf.set_depth(bits_per_component * 4);
		if (!big)
		{
			pf.set_blue_mask(CL_PixelFormat::get_bitmask(bits_per_component, 0));
			pf.set_green_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component));
			pf.set_red_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component * 2));
			pf.set_alpha_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component * 3));
		}
		else
		{
			pf.set_blue_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component * 3));
			pf.set_green_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component * 2));
			pf.set_red_mask(CL_PixelFormat::get_bitmask(bits_per_component, bits_per_component));
			pf.set_alpha_mask(CL_PixelFormat::get_bitmask(bits_per_component, 0));
		}
	}
	else
		return false;

	return true;
}

bool CL_OpenGL::to_opengl_pixelformat(const CL_PixelFormat &pf, CLenum &format, CLenum &type)
{
	//Using CL_ABGR requires a GL extension to use it, we'll do a one time test to see if it's available or not
	static const bool bHasEXT_abgr = extension_exists("GL_EXT_abgr");

	// indexed modes and colorkey requires special conversion to alpha and
	// therefore no OpenGL mode has a direct conversion format
	if (pf.has_colorkey() || pf.get_type() == pixelformat_index)
		return false;

	bool big = CL_Endian::is_system_big();

	if (pf.get_type() == pixelformat_index)
	{
		format = CL_COLOR_INDEX;
		if (pf.get_depth() == 8)
			type = CL_UNSIGNED_BYTE;
		else if (pf.get_depth() == 16)
			type = CL_UNSIGNED_SHORT;
		else if (pf.get_depth() == 32)
			type = CL_UNSIGNED_INT;
		else
			return false;
	}
	else if (pf.get_type() == pixelformat_rgba)
	{
		const unsigned int rm = pf.get_red_mask(),
			gm = pf.get_green_mask(),
			bm = pf.get_blue_mask(),
			am = pf.get_alpha_mask();

		const int rms = pf.get_mask_shift(rm),
			gms = pf.get_mask_shift(gm),
			bms = pf.get_mask_shift(bm),
			ams = pf.get_mask_shift(am);

		const int rmb = pf.get_mask_bits(rm),
			gmb = pf.get_mask_bits(gm),
			bmb = pf.get_mask_bits(bm),
			amb = pf.get_mask_bits(am);

		typedef std::map<int, CLenum> BitsToTypeMap;
		static BitsToTypeMap bits_to_type;
		if (bits_to_type.empty())
		{
			// initialize it only once
			bits_to_type[8] = CL_UNSIGNED_BYTE;
			bits_to_type[16] = CL_UNSIGNED_SHORT;
			bits_to_type[32] = CL_UNSIGNED_INT;
		}

		if (rm && gm && bm && am)
		{
			
			// the bit number must be 8, 16 or 32 for component
			// they all must have the same number of bits
			if ((!(rmb == 8 || rmb == 16 || rmb == 32)) ||
				(rmb != gmb || rmb != bmb || rmb != amb))
				return false;

			const int bits = rmb; // they all have the same bits

			if (pf.get_depth() != bits * 4)
				return false;

			type = bits_to_type[bits];

			if (!big && rms == 0*bits && gms == 1*bits && bms == 2*bits && ams == 3*bits)
				format = CL_RGBA;
			else if (!big && ams == 0*bits && bms == 1*bits && gms == 2*bits && rms == 3*bits && bHasEXT_abgr)
				format = CL_ABGR;
			else if (big && rms == 3*bits && gms == 2*bits && bms == 1*bits && ams == 0*bits)
				format = CL_RGBA;
			else if (big && ams == 3*bits && bms == 2*bits && gms == 1*bits && rms == 0*bits)
				format = CL_ABGR;
			else
				return false;
		}
		else if (rm && gm && bm && !am)
		{
			// the bit number must be 8, 16 or 32 for component
			// they all must have the same number of bits
			if ((!(rmb == 8 || rmb == 16 || rmb == 32)) ||
				(rmb != gmb || rmb != bmb))
				return false;

			const int bits = rmb; // they all have the same bits

			if (pf.get_depth() != bits * 3)
				return false;

			type = bits_to_type[bits];

			if (!big && rms == 0*bits && gms == 1*bits && bms == 2*bits)
				format = CL_RGB;
			else if (!big && bms == 0*bits && gms == 1*bits && rms == 2*bits)
				format = CL_BGR;
			else if (big && rms == 2*bits && gms == 1*bits && bms == 0*bits)
				format = CL_RGB;
			else if (big && bms == 2*bits && gms == 1*bits && rms == 0*bits)
				format = CL_BGR;
			else
				return false;
		}
		else if (rm && !gm && !bm && !am)
		{
			format = CL_RED;
			if (rms != 0 || !bits_to_type.count(rmb))
				return false;
			if (pf.get_depth() != rmb)
				return false;
			type = bits_to_type[rmb];
		}
		else if (!rm && gm && !bm && !am)
		{
			format = CL_GREEN;
			if (gms != 0 || !bits_to_type.count(gmb))
				return false;
			if (pf.get_depth() != gmb)
				return false;
			type = bits_to_type[gmb];
		}
		else if (!rm && !gm && bm && !am)
		{
			format = CL_BLUE;
			if (bms != 0 || !bits_to_type.count(bmb))
				return false;
			if (pf.get_depth() != bmb)
				return false;
			type = bits_to_type[bmb];
		}
		else if (!rm && !gm && !bm && am)
		{
			format = CL_ALPHA;
			if (ams != 0 || !bits_to_type.count(amb))
				return false;
			if (pf.get_depth() != amb)
				return false;
			type = bits_to_type[amb];
		}
		else
			return false;
	}
	else
		return false;

	return true;
}

bool CL_OpenGL::extension_exists( const char * extension_name )
{
	/*
	** Search for extName in the extensions string.  Use of strstr()
	** is not sufficient because extension names can be prefixes of
	** other extension names.  Could use strtok() but the constant
	** string returned by glGetString can be in read-only memory.
	*/
	char *p = (char *) glGetString(GL_EXTENSIONS);

	if (!p)
	{
		cl_assert(!"setup_binds() hasn't been run, load something onto a GL surface first before trying to call this.");
		return false;
	}

	//is it on our ignore list?

	for (unsigned int i=0; i < m_ignored_gl_extension.size(); i++)
	{
		if (m_ignored_gl_extension.at(i) == extension_name) return false;
	}
	char *end;
	int extNameLen;

	extNameLen = strlen(extension_name);
	end = p + strlen(p);

	while (p < end) 
	{
		int n = strcspn(p, " ");
		if ((extNameLen == n) && (strncmp(extension_name, p, n) == 0)) 
		{
			return true;
		}
		p += (n + 1);
	}

	return false;
}


void CL_OpenGL::ignore_extension(const std::string &extension_name)
{

#ifdef DEBUG
	char *p = (char *) glGetString(GL_EXTENSIONS);

	if (p)
	{
		cl_assert(!"ignore_extension should be used earlier,  before OGL has been initialized.");
	}

#endif
	
	m_ignored_gl_extension.push_back(extension_name);
}