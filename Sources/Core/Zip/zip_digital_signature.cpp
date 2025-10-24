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

#include "Core/precomp.h"
#include "zip_digital_signature.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/Text/string_help.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ZipDigitalSignature construction:

CL_ZipDigitalSignature::CL_ZipDigitalSignature()
{
	signature = 0x05054b50;
}

CL_ZipDigitalSignature::~CL_ZipDigitalSignature()
{
}
	
/////////////////////////////////////////////////////////////////////////////
// CL_ZipDigitalSignature attributes:

	
/////////////////////////////////////////////////////////////////////////////
// CL_ZipDigitalSignature operations:

void CL_ZipDigitalSignature::load(CL_IODevice &input)
{
	signature = input.read_int32();
	if (signature != 0x05054b50)
	{
		throw CL_Exception("Incorrect Digital Signature signature!");
	}

	size_of_data = input.read_int16();

	char *str = new char[size_of_data];
	try
	{
		input.read(str, size_of_data);
		signature_data = CL_StringHelp::local8_to_text(CL_StringRef8(str, size_of_data, false));

		delete[] str;
	}
	catch (...)
	{
		delete[] str;
	}
}
	
void CL_ZipDigitalSignature::save(CL_IODevice &output)
{
	CL_String8 str = CL_StringHelp::text_to_cp437(signature_data);
	size_of_data = str.length();
	output.write_int32(signature);
	output.write_int16(size_of_data);
	output.write(str.data(), size_of_data);
}

/////////////////////////////////////////////////////////////////////////////
// CL_ZipDigitalSignature implementation:
