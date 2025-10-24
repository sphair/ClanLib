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
*/

#include "GDI/precomp.h"
#include "gdi_program_object_provider.h"
#include "API/Display/Render/shader_object.h"
#include "API/Display/Render/program_attribute.h"
#include "API/Display/Render/program_uniform.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GDIProgramObjectProvider Construction:

CL_GDIProgramObjectProvider::CL_GDIProgramObjectProvider()
{
}

CL_GDIProgramObjectProvider::~CL_GDIProgramObjectProvider()
{
}

void CL_GDIProgramObjectProvider::destroy()
{
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GDIProgramObjectProvider Attributes:

unsigned int CL_GDIProgramObjectProvider::get_handle() const
{
	return 0;
}

bool CL_GDIProgramObjectProvider::get_link_status() const
{
	return false;
}

bool CL_GDIProgramObjectProvider::get_validate_status() const
{
	return false;
}

CL_String CL_GDIProgramObjectProvider::get_info_log() const
{
	return CL_String();
}

int CL_GDIProgramObjectProvider::get_attribute_count() const
{
	return 0;
}

std::vector<CL_ShaderObject> CL_GDIProgramObjectProvider::get_shaders() const
{
	return std::vector<CL_ShaderObject>();
}

std::vector<CL_ProgramAttribute> CL_GDIProgramObjectProvider::get_attributes() const
{
	return std::vector<CL_ProgramAttribute>();
}

int CL_GDIProgramObjectProvider::get_attribute_location(const CL_StringRef &name) const
{
	return 0;
}

int CL_GDIProgramObjectProvider::get_uniform_count() const
{
	return 0;
}

std::vector<CL_ProgramUniform> CL_GDIProgramObjectProvider::get_uniforms() const
{
	return std::vector<CL_ProgramUniform>();
}

int CL_GDIProgramObjectProvider::get_uniform_location(const CL_StringRef &name) const
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GDIProgramObjectProvider Operations:

void CL_GDIProgramObjectProvider::attach(const CL_ShaderObject &obj)
{
}

void CL_GDIProgramObjectProvider::detach(const CL_ShaderObject &obj)
{
}

void CL_GDIProgramObjectProvider::bind_attribute_location(int index, const CL_StringRef &name)
{
}

void CL_GDIProgramObjectProvider::link()
{
}

void CL_GDIProgramObjectProvider::validate()
{
}

void CL_GDIProgramObjectProvider::set_uniform1i(const CL_StringRef &name, int)
{
}

void CL_GDIProgramObjectProvider::set_uniform2i(const CL_StringRef &name, int, int)
{
}

void CL_GDIProgramObjectProvider::set_uniform3i(const CL_StringRef &name, int, int, int)
{
}

void CL_GDIProgramObjectProvider::set_uniform4i(const CL_StringRef &name, int, int, int, int)
{
}

void CL_GDIProgramObjectProvider::set_uniformiv(const CL_StringRef &name, int size, int count, int *data)
{
}

void CL_GDIProgramObjectProvider::set_uniform1f(const CL_StringRef &name, float)
{
}

void CL_GDIProgramObjectProvider::set_uniform2f(const CL_StringRef &name, float, float)
{
}

void CL_GDIProgramObjectProvider::set_uniform3f(const CL_StringRef &name, float, float, float)
{
}

void CL_GDIProgramObjectProvider::set_uniform4f(const CL_StringRef &name, float, float, float, float)
{
}

void CL_GDIProgramObjectProvider::set_uniformfv(const CL_StringRef &name, int size, int count, float *data)
{
}

void CL_GDIProgramObjectProvider::set_uniform_matrix(const CL_StringRef &name, int size, int count, bool transpose, float *data)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GDIProgramObjectProvider Implementation:
