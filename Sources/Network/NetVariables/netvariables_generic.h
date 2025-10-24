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

#ifndef header_netvariables_generic
#define header_netvariables_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include <list>
#include <cstring>
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/outputsource.h"

class CL_NetVariable
{
public:
	virtual ~CL_NetVariable() { return; }
	virtual CL_NetVariable *clone()=0;
	virtual void save(CL_OutputSource *output)=0;
	virtual void load(CL_InputSource *input)=0;
	virtual bool is_different()=0;
};

class CL_NetVariables_Generic
{
public:
	std::list<CL_NetVariable *> vars;
};

template<class T>
class CL_NetVariableT : public CL_NetVariable
{
public:
	CL_NetVariableT(T *var, int array, bool do_endian_swap = true)
	: var(var), array(array), do_endian_swap(do_endian_swap)
	{
		old = new T[array];
		memcpy(old, var, sizeof(T)*array);
	}

	virtual ~CL_NetVariableT()
	{
		delete[] old;
	}

	virtual CL_NetVariable *clone()
	{
		return new CL_NetVariableT<T>(var, array, do_endian_swap);
	}

	virtual void save(CL_OutputSource *output)
	{
		// todo: if (do_endian_swap)
		output->write(var, sizeof(T)*array);
		memcpy(old, var, sizeof(T)*array);
	}

	virtual void load(CL_InputSource *input)
	{
		input->read(var, sizeof(T)*array);
		// todo: if (do_endian_swap)
	}

	virtual bool is_different()
	{
		for (int i=0; i<array; i++) if (var[i] != old[i]) return true;
		return false;
	}

private:
	T *var, *old;
	int array;
	bool do_endian_swap;
};

#endif
