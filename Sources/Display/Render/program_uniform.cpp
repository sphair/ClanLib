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
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include "API/Display/Render/program_uniform.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// ProgramUniform_Impl:

class ProgramUniform_Impl
{
//! Construction:
public:
	ProgramUniform_Impl() : type(0), size(0), location(0)
	{
	}
	
	~ProgramUniform_Impl()
	{
	}

//! Attributes: 
public:
	std::string name;

	int type;

	int size;

	int location;
};

/////////////////////////////////////////////////////////////////////////////
// ProgramUniform Construction:

ProgramUniform::ProgramUniform()
{
}

ProgramUniform::ProgramUniform(const std::string &name, int size, int type, int location)
: impl(new ProgramUniform_Impl)
{
	impl->name = name;
	impl->size = size;
	impl->type = type;
	impl->location = location;
}

ProgramUniform::~ProgramUniform()
{
}

/////////////////////////////////////////////////////////////////////////////
// ProgramUniform Attributes:

void ProgramUniform::throw_if_null() const
{
	if (!impl)
		throw Exception("ProgramUniform is null");
}

std::string ProgramUniform::get_name() const
{
	return impl->name;
}

int ProgramUniform::get_size() const
{
	return impl->size;
}

int ProgramUniform::get_type() const
{
	return impl->type;
}

int ProgramUniform::get_location() const
{
	return impl->location;
}

/////////////////////////////////////////////////////////////////////////////
// ProgramUniform Operations:

/////////////////////////////////////////////////////////////////////////////
// ProgramUniform Implementation:

}
