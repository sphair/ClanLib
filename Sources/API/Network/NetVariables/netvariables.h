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

//! clanNetwork="NetObjects"
//! header=network.h

#ifndef header_netvariables
#define header_netvariables

#ifdef CL_API_DLL
#ifdef CL_NETWORK_EXPORT
#define CL_API_NETWORK __declspec(dllexport)
#else
#define CL_API_NETWORK __declspec(dllimport)
#endif
#else
#define CL_API_NETWORK
#endif

#if _MSC_VER > 1000
#pragma once
#endif

class CL_NetVariables_Generic;
class CL_InputSource;
class CL_OutputSource;

//: Easy serialization of a data structure.
//- !group=Network/NetObjects!
//- !header=network.h!
//- <p>The netvariables class is used to read a set of variables from an object,
//- and write them structured, in network byte order, to an output source.</p>
//-
//- <p>The Netvariables interface can also read the data again, and produce 'diff'
//- packages where it only write variables that have changed since last write.</p>
class CL_API_NETWORK CL_NetVariables
{
//! Construction:
public:
	//: Construct a netvariables object.
	CL_NetVariables();

	//: Net Variables Destructor
	virtual ~CL_NetVariables();

//! Attributes:

	//: Returns true if the variables have changed since last save.
	bool is_different();

//! Operations:

	//: Add booleans to the data structure.
	void add_bool(bool *var, int array = 1);

	//: Add integers to the data structure.
	void add_int(int *var, int array = 1);

	//: Add shorts to the data structure.
	void add_short(short *var, int array = 1);

	//: Add floats to the data structure.
	void add_float(float *var, int array = 1);

	//: Add doubles to the data structure.
	void add_double(double *var, int array = 1);

	//: Add structures to the data structure.
	void add_vars(CL_NetVariables *variables, int array = 1);

	//: Write all variables in the data structure to the output source.
	void save_all(class CL_OutputSource *msg);

	//: Read all variables in the data structure from the input source.
	void load_all(class CL_InputSource *msg);

	//: Write all variables that have changed since last save.
	void save_diff(class CL_OutputSource *msg);

	//: Read a diff produced with save_diff from the input source.
	void load_diff(class CL_InputSource *msg);

//! Implementation:
public:
	CL_NetVariables_Generic *impl;
};

#endif
