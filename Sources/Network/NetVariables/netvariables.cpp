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

#include "API/Core/System/cl_assert.h"
#include "API/Network/NetVariables/netvariables.h"
#include "netvariables_generic.h"
#include <vector>

/////////////////////////////////////////////////////////////////////////////
// CL_NetVariables Construction:

CL_NetVariables::CL_NetVariables()
: impl(new CL_NetVariables_Generic)
{
}

CL_NetVariables::~CL_NetVariables()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetVariables Attributes:

bool CL_NetVariables::is_different()
{
	std::list<CL_NetVariable *>::iterator it;
	for (it = impl->vars.begin(); it != impl->vars.end(); it++)
	{
		CL_NetVariable *var = *it;
		if (var->is_different()) return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetVariables Operations:

void CL_NetVariables::add_bool(bool *var, int array)
{
	impl->vars.push_back(new CL_NetVariableT<bool>(var, array));
}

void CL_NetVariables::add_int(int *var, int array)
{
	impl->vars.push_back(new CL_NetVariableT<int>(var, array));
}

void CL_NetVariables::add_short(short *var, int array)
{
	impl->vars.push_back(new CL_NetVariableT<short>(var, array));
}

void CL_NetVariables::add_float(float *var, int array)
{
	impl->vars.push_back(new CL_NetVariableT<float>(var, array, false));
}

void CL_NetVariables::add_double(double *var, int array)
{
	impl->vars.push_back(new CL_NetVariableT<double>(var, array, false));
}

void CL_NetVariables::add_vars(CL_NetVariables *variables, int array)
{
	for (int i=0; i<array; i++)
	{
		std::list<CL_NetVariable *>::iterator it;
		for (it = variables[i].impl->vars.begin(); it != variables[i].impl->vars.end(); it++)
		{
			CL_NetVariable *var = *it;
			impl->vars.push_back(var->clone());
		}
	}
}

void CL_NetVariables::save_all(class CL_OutputSource *msg)
{
	std::list<CL_NetVariable *>::iterator it;
	for (it = impl->vars.begin(); it != impl->vars.end(); it++)
	{
		CL_NetVariable *var = *it;
		var->save(msg);
	}
}

void CL_NetVariables::load_all(class CL_InputSource *msg)
{
	std::list<CL_NetVariable *>::iterator it;
	for (it = impl->vars.begin(); it != impl->vars.end(); it++)
	{
		CL_NetVariable *var = *it;
		var->load(msg);
	}
}

void CL_NetVariables::save_diff(class CL_OutputSource *msg)
{
	std::vector<unsigned char> diff_bits;
	std::list<CL_NetVariable *>::iterator it;
	int bitpos;

	if (impl->vars.empty()) return;

	// Create diff bits.
	// For every 8 netvariables, we have a byte indicating which ones
	// have changed. If the bit is set, it means we saved the variable.

	diff_bits.resize((impl->vars.size()+7) / 8);
	for (it = impl->vars.begin(), bitpos = 0; it != impl->vars.end(); it++, bitpos++)
	{
		CL_NetVariable *var = *it;
		if (var->is_different()) diff_bits[bitpos/8] |= (1 << (bitpos % 8));
	}

	// Save the variables, and store diffbit for every eight variable.
	for (it = impl->vars.begin(), bitpos = 0; it != impl->vars.end(); it++, bitpos++)
	{
		if (bitpos % 8 == 0) msg->write_uchar8(diff_bits[bitpos/8]);

		CL_NetVariable *var = *it;
		if (var->is_different()) var->save(msg);
	}
}

void CL_NetVariables::load_diff(class CL_InputSource *msg)
{
	std::list<CL_NetVariable *>::iterator it;
	int bitpos;
	unsigned char diff_bits = 0;

	if (impl->vars.empty()) return;

	for (it = impl->vars.begin(), bitpos = 0; it != impl->vars.end(); it++, bitpos++)
	{
		if (bitpos % 8 == 0) diff_bits = msg->read_uchar8();

		CL_NetVariable *var = *it;
		if ((diff_bits & 1)) var->load(msg);
		diff_bits >>= 1;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetVariables Implementation:
