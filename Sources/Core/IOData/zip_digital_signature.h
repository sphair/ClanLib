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

#ifndef zip_digital_signature
#define zip_digital_signature

#include "API/Core/IOData/datatypes.h"

class CL_InputSource;
class CL_OutputSource;

class CL_Zip_DigitalSignature
{
//! Construction:
public:
	CL_Zip_DigitalSignature();
	
	~CL_Zip_DigitalSignature();
	
//! Attributes:
public:
	cl_int32 signature; // 0x05054b50
	
	cl_int16 size_of_data;
	
	std::string signature_data;
	
//! Operations:
public:
	void load(CL_InputSource *input);
	
	void save(CL_OutputSource *output);

//! Implementation:
private:
};

#endif
