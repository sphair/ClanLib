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

#include "Core/precomp.h"
#include "zip_digital_signature.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/outputsource.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_DigitalSignature construction:

CL_Zip_DigitalSignature::CL_Zip_DigitalSignature()
{
	signature = 0x05054b50;
}

CL_Zip_DigitalSignature::~CL_Zip_DigitalSignature()
{
}
	
/////////////////////////////////////////////////////////////////////////////
// CL_Zip_DigitalSignature attributes:

	
/////////////////////////////////////////////////////////////////////////////
// CL_Zip_DigitalSignature operations:

void CL_Zip_DigitalSignature::load(CL_InputSource *input)
{
	signature = input->read_int32();
	if (signature != 0x05054b50)
	{
		throw CL_Error("Incorrect Digital Signature signature!");
	}

	size_of_data = input->read_short16();

	char *str = new char[size_of_data];
	try
	{
		input->read(str, size_of_data);
		signature_data = std::string(str, size_of_data);

		delete[] str;
	}
	catch (...)
	{
		delete[] str;
	}
}
	
void CL_Zip_DigitalSignature::save(CL_OutputSource *output)
{
	output->write_int32(signature);
	output->write_short16(size_of_data);
	output->write(signature_data.data(), size_of_data);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_DigitalSignature implementation:
