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
**    Magnus Norddahl
*/

#pragma once

class CL_JPEGStartOfScanComponent;

class CL_JPEGStartOfScan
{
public:
	CL_JPEGStartOfScan() : start_dct_coefficient(0), end_dct_coefficient(0), preceding_point_transform(0), point_transform(0){ }

	std::vector<CL_JPEGStartOfScanComponent> components;
	cl_uchar start_dct_coefficient;     // Ss
	cl_uchar end_dct_coefficient;       // Se
	cl_uchar preceding_point_transform; // Ah
	cl_uchar point_transform;           // Al
};

class CL_JPEGStartOfScanComponent
{
public:
	CL_JPEGStartOfScanComponent() : component_selector(0), dc_table_selector(0), ac_table_selector(0) { }

	cl_uchar component_selector;        // Cs
	cl_uchar dc_table_selector;         // Td
	cl_uchar ac_table_selector;         // Ta
};
