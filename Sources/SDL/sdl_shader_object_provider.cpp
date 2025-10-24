/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
**    Mark Page
*/

#include "SDL/precomp.h"
#include "sdl_shader_object_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SDLShaderObjectProvider Construction:

CL_SDLShaderObjectProvider::CL_SDLShaderObjectProvider()
{
}

CL_SDLShaderObjectProvider::~CL_SDLShaderObjectProvider()
{
}

void CL_SDLShaderObjectProvider::create(CL_ShaderType type, const CL_StringRef &source)
{
}

void CL_SDLShaderObjectProvider::create(CL_ShaderType type, const std::vector<CL_StringRef> &sources)
{
}

void CL_SDLShaderObjectProvider::destroy()
{
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SDLShaderObjectProvider Attributes:

unsigned int CL_SDLShaderObjectProvider::get_handle() const
{
	return 0;
}

bool CL_SDLShaderObjectProvider::get_compile_status() const
{
	return false;
}

CL_ShaderType CL_SDLShaderObjectProvider::get_shader_type() const
{
	return cl_shadertype_vertex;
}

CL_String CL_SDLShaderObjectProvider::get_info_log() const
{
	return CL_String();
}

CL_String CL_SDLShaderObjectProvider::get_shader_source() const
{
	return CL_String();
}

/////////////////////////////////////////////////////////////////////////////
// CL_SDLShaderObjectProvider Operations:

void CL_SDLShaderObjectProvider::compile()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SDLShaderObjectProvider Implementation:
